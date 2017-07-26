/**
 * @file mainwindow.cpp
 */

#include "mainwindow.h"

#include "log.h"
#include "exceptions.h"

#include <QTimer>
#include <QSettings>

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
	}
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	QSettings settings;
	settings.setValue("mainwindow/splitter_geometry", m_ui.splitter->saveState());
}
