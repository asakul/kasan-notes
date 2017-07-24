#include "mainwindowcontroller.h"

#include "core/filelocator.h"

#include "log.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>
#include <QQmlContext>

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
	connect(backend.get(), SIGNAL(allNotes(QList<Note::Ptr>)), this, SLOT(allNotes(QList<Note::Ptr>)));
}

void MainWindowController::setView(QQuickView* view)
{
	m_view = view;
	QQmlContext *ctxt = view->rootContext();
	ctxt->setContextProperty("notesModel", &m_noteStorageModel);
}

void MainWindowController::forceNotesRefresh()
{
	QTimer::singleShot(0, m_backend.get(), SLOT(requestAllNotes()));
}

void MainWindowController::addButtonClicked()
{
	auto locator = createDefaultFileLocator();
	QQmlComponent component(m_view->engine(), QUrl::fromLocalFile(locator->getUiFile("KasanTextEditor")));
	m_currentNoteEditor = component.create();
	auto noteArea = m_view->rootObject()->findChild<QObject*>("NoteArea");
	QQmlProperty::write(m_currentNoteEditor, "parent", QVariant::fromValue<QObject*>(noteArea));
	QQmlEngine::setObjectOwnership(m_currentNoteEditor, QQmlEngine::CppOwnership);
}

void MainWindowController::allNotes(const Notebook::Ptr& root)
{
	LOG(DEBUG) << "MainWindowController: incoming notes";
	m_noteStorage->replaceRootNotebook(root);

	m_noteStorageModel.notesChanged();
}

