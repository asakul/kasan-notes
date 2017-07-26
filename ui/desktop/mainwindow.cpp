/**
 * @file mainwindow.cpp
 */

#include "mainwindow.h"

#include "log.h"
#include "exceptions.h"

#include <QTimer>
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QCryptographicHash>
#include <QMimeDatabase>

MainWindow::MainWindow() : m_noteStorage(std::make_shared<NoteStorage>()),
	m_model(m_noteStorage)
{
	m_ui.setupUi(this);
	m_ui.tv_notes->setModel(&m_model);

	QSettings settings;
	auto splitterGeometry = settings.value("mainwindow/splitter_geometry").toByteArray();
	if(splitterGeometry.isEmpty())
	{
		m_ui.splitter->setSizes({200, 800});
	}
	else
	{
		m_ui.splitter->restoreState(splitterGeometry);
	}

	restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
}

MainWindow::~MainWindow()
{
}

void MainWindow::setBackend(const Backend::Ptr& backend)
{
	m_backend = backend;
	connect(backend.get(), SIGNAL(allNotes(Notebook::Ptr)), this, SLOT(allNotes(Notebook::Ptr)));
	connect(this, SIGNAL(requestNoteContent(Note::Ptr)), backend.get(), SLOT(requestNoteContent(Note::Ptr)));
	connect(backend.get(), SIGNAL(noteUpdated(Note::Ptr)), this, SLOT(noteUpdated(Note::Ptr)));
	connect(this, SIGNAL(updateNote(Note::Ptr)), backend.get(), SLOT(updateNote(Note::Ptr)));
	connect(backend.get(), SIGNAL(noteSaved(Note::Ptr)), this, SLOT(noteSaved(Note::Ptr)));
}

void MainWindow::forceNotesRefresh()
{
	QTimer::singleShot(0, m_backend.get(), SLOT(requestAllNotes()));
}

void MainWindow::authenticationSuccessful()
{
}

void MainWindow::addButtonClicked()
{
}

void MainWindow::notelistClicked(const QModelIndex& index)
{
	try
	{
		LOG(DEBUG) << "MainWindowController: notelistClicked";
		auto note = index.data(NoteStorageModel::NoteRole).value<Note::Ptr>();
		if(note)
		{
			setNoteAsCurrent(note);
			emit requestNoteContent(note);
		}
	}
	catch(const KasanNotesException& e)
	{
		LOG(WARNING) << "Exception occured: " << e.what();
		const QString* maybeStr = boost::get_error_info<error_message>(e);
		if(maybeStr)
		{
			LOG(WARNING) << *maybeStr;
		}
	}
}

void MainWindow::saveCurrentNote()
{
	if(m_currentNote)
	{
		m_currentNote->setContent(m_ui.textEdit->toHtml());
		emit updateNote(m_currentNote);
	}
}

void MainWindow::noteSaved(const Note::Ptr& note)
{
	m_ui.statusbar->showMessage(tr("Note saved"), 5000);
}

void MainWindow::insertImage()
{
	if(!m_currentNote)
		return;

	auto filepath = QFileDialog::getOpenFileName(this, tr("Insert image"), QString(), QString(tr("Images (*.png *.jpg *.gif);;All files (*.*)")));
	if(!filepath.isNull())
	{
		QFile imageFile(filepath);
		if(!imageFile.open(QIODevice::ReadOnly))
		{
			BOOST_THROW_EXCEPTION(FileNotFound() << error_message(tr("Unable to open file")));
		}
		auto data = imageFile.readAll();
		QMimeDatabase db;
		auto mime = db.mimeTypeForFile(filepath);

		auto attachment = std::make_shared<Attachment>(mime.name(), data);

		m_currentNote->addAttachment(attachment);

		QImage img;
		img.loadFromData(data);
		auto resourceName = "attachment://" + QString::fromUtf8(attachment->hash().toHex());
		m_ui.textEdit->document()->addResource(QTextDocument::ImageResource, resourceName, img);
		m_ui.textEdit->textCursor().insertImage(resourceName);
	}
}

void MainWindow::allNotes(const Notebook::Ptr& root)
{
	LOG(DEBUG) << "MainWindow: incoming notes";
	m_noteStorage->replaceRootNotebook(root);

	m_model.notesChanged();
}

void MainWindow::noteUpdated(const Note::Ptr& note)
{
	auto currentIndex = m_ui.tv_notes->currentIndex();
	auto currentNote = currentIndex.data(NoteStorageModel::NoteRole).value<Note::Ptr>();
	if(currentNote == note)
	{
		setNoteAsCurrent(note);
	}
}

void MainWindow::setNoteAsCurrent(const Note::Ptr& note)
{
	QTextDocument* doc = m_ui.textEdit->document();

	for(int i = 0; i < note->attachmentsCount(); i++)
	{
		auto attachment = note->attachmentByIndex(i);
		if(attachment->mimeType().startsWith("image/"))
		{
			QImage img;
			img.loadFromData(attachment->data());
			QUrl url("attachment://" + QString::fromUtf8(attachment->hash().toHex()));
			LOG(DEBUG) << "Url: " << url.toString();
			doc->addResource(QTextDocument::ImageResource, url, img);
		}
	}

	LOG(DEBUG) << "Content: " << note->content().value_or("");
	if(note->content())
	{
		m_ui.textEdit->setText(note->content().get());
		m_currentNote = note;
	}
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	QSettings settings;
	settings.setValue("mainwindow/splitter_geometry", m_ui.splitter->saveState());
	settings.setValue("mainwindow/geometry", saveGeometry());
}
