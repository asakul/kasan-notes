#include "mainwindowcontroller.h"

#include "core/filelocator.h"

#include "log.h"
#include "../../exceptions.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>
#include <QQmlContext>
#include <QQuickTextDocument>

#include <QTimer>

MainWindowController::MainWindowController(QObject *parent) : QObject(parent),
	m_view(nullptr),
	m_currentNoteEditor(nullptr),
	m_noteStorage(std::make_shared<NoteStorage>()),
	m_noteStorageModel(m_noteStorage)
{

}

void MainWindowController::setBackend(const Backend::Ptr& backend)
{
	m_backend = backend;
	connect(backend.get(), SIGNAL(allNotes(Notebook::Ptr)), this, SLOT(allNotes(Notebook::Ptr)));
	connect(this, SIGNAL(requestNoteContent(Note::Ptr)), backend.get(), SLOT(requestNoteContent(Note::Ptr)));
	connect(backend.get(), SIGNAL(noteUpdated(Note::Ptr)), this, SLOT(noteUpdated(Note::Ptr)));
}

void MainWindowController::setView(QQuickView* view)
{
	m_view = view;
	QQmlContext *ctxt = m_view->rootContext();
	ctxt->setContextProperty("notesModel", &m_noteStorageModel);
}

void MainWindowController::forceNotesRefresh()
{
	QTimer::singleShot(0, m_backend.get(), SLOT(requestAllNotes()));
}

void MainWindowController::authenticationSuccessful()
{
	auto root = m_view->rootObject();
	auto notelist = root->findChild<QObject*>("notes");
	connect(notelist, SIGNAL(clicked(QVariant)), this, SLOT(notelistClicked(QVariant)));
}

void MainWindowController::addButtonClicked()
{

}

void MainWindowController::notelistClicked(const QVariant& index)
{
	try
	{
		LOG(DEBUG) << "MainWindowController: notelistClicked";
		auto note = index.toModelIndex().data(NoteStorageModel::NoteRole).value<Note::Ptr>();
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

void MainWindowController::allNotes(const Notebook::Ptr& root)
{
	LOG(DEBUG) << "MainWindowController: incoming notes";
	m_noteStorage->replaceRootNotebook(root);

	m_noteStorageModel.notesChanged();
}

void MainWindowController::noteUpdated(const Note::Ptr& note)
{
	LOG(DEBUG) << "Note updated: " << note->id();
	auto root = m_view->rootObject();
	auto notelist = root->findChild<QObject*>("notes");
	if(notelist)
	{
		auto currentIndex = QQmlProperty::read(notelist, "currentIndex").toModelIndex();
		auto currentNote = currentIndex.data(NoteStorageModel::NoteRole).value<Note::Ptr>();
		if(currentNote == note)
		{
			setNoteAsCurrent(note);
		}
	}
}

void MainWindowController::setNoteAsCurrent(const Note::Ptr& note)
{
	auto locator = createDefaultFileLocator();
	QQmlComponent component(m_view->engine(), QUrl::fromLocalFile(locator->getUiFile("KasanTextEditor")));
	m_currentNoteEditor = component.create();
	auto noteArea = m_view->rootObject()->findChild<QObject*>("NoteArea");
	QQmlProperty::write(m_currentNoteEditor, "parent", QVariant::fromValue<QObject*>(noteArea));
	QQmlEngine::setObjectOwnership(m_currentNoteEditor, QQmlEngine::CppOwnership);

	QVariant qmltextdoc;
	QMetaObject::invokeMethod(m_currentNoteEditor, "getTextDocument", Q_RETURN_ARG(QVariant, qmltextdoc));


	LOG(DEBUG) << "Content: " << note->content().value_or("");
	QMetaObject::invokeMethod(m_currentNoteEditor, "setText", Q_ARG(QVariant, note->content().value_or("")));

	QTextDocument* doc = qmltextdoc.value<QQuickTextDocument*>()->textDocument();
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
}
