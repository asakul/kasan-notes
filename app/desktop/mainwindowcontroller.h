#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include <QQuickView>

#include "core/backends/common/backend.h"

class MainWindowController : public QObject
{
	Q_OBJECT
public:
	explicit MainWindowController(QObject *parent = 0);

	void setBackend(const Backend::Ptr& backend);
	void setView(QQuickView* view);

signals:

public slots:
	void addButtonClicked();

private:
	Backend::Ptr m_backend;
	QQuickView* m_view;
	QObject* m_currentNoteEditor;
};

#endif // MAINWINDOWCONTROLLER_H
