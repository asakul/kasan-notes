#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include <QQuickView>

#include "core/backends/common/backend.h"
#include "core/backends/common/note.h"

#include "core/notestorage/notestorage.h"
#include "core/notestorage/notestoragemodel.h"

#include <memory>

class MainWindowController : public QObject
{
	Q_OBJECT
public:
	explicit MainWindowController(QObject *parent = 0);

	void setBackend(const Backend::Ptr& backend);
	void setView(QQuickView* view);

	void forceNotesRefresh();

signals:
	void requestNoteContent(const Note::Ptr& note);

public slots:
	void authenticationSuccessful();
	void addButtonClicked();
	void notelistClicked(const QVariant& index);

	void allNotes(const Notebook::Ptr& root);
	void noteUpdated(const Note::Ptr& note);

private:
	void setNoteAsCurrent(const Note::Ptr& note);

private:
	Backend::Ptr m_backend;
	QQuickView* m_view;
	QObject* m_currentNoteEditor;
	NoteStorage::Ptr m_noteStorage;
	NoteStorageModel m_noteStorageModel;
};

#endif // MAINWINDOWCONTROLLER_H
