#include "mainwindowcontroller.h"

#include "core/filelocator.h"

#include "log.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QTimer>

MainWindowController::MainWindowController(QObject *parent) : QObject(parent),
	m_view(nullptr),
	m_currentNoteEditor(nullptr)
{

}

void MainWindowController::setBackend(const Backend::Ptr& backend)
{
	m_backend = backend;
}

void MainWindowController::setView(QQuickView* view)
{
	m_view = view;
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

void MainWindowController::allNotes(const QList<Note::Ptr>& notes)
{

}

