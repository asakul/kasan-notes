/**
 * @file mainwindow.h
 */

#ifndef UI_DESKTOP_MAINWINDOW_H_
#define UI_DESKTOP_MAINWINDOW_H_

#include "core/backends/common/backend.h"
#include "core/notestorage/notestoragemodel.h"

#include <QMainWindow>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();

	void setBackend(const Backend::Ptr& backend);

	void forceNotesRefresh();

signals:
	void requestNoteContent(const Note::Ptr& note);
	void updateNote(const Note::Ptr& note);

public slots:
	void authenticationSuccessful();
	void addButtonClicked();
	void notelistClicked(const QModelIndex& index);

	void saveCurrentNote();
	void noteSaved(const Note::Ptr& note);

	void insertImage();


	void allNotes(const Notebook::Ptr& root);
	void noteUpdated(const Note::Ptr& note);

protected:
	virtual void closeEvent(QCloseEvent * event);

private:
	void setNoteAsCurrent(const Note::Ptr& note);

private:
	Backend::Ptr m_backend;
	NoteStorage::Ptr m_noteStorage;
	NoteStorageModel m_model;
	Note::Ptr m_currentNote;

	Ui::MainWindow m_ui;
};

#endif /* UI_DESKTOP_MAINWINDOW_H_ */
