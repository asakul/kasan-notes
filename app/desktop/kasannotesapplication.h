#ifndef APP_DESKTOP_KASANNOTESAPPLICATION_H_
#define APP_DESKTOP_KASANNOTESAPPLICATION_H_

#include <QApplication>
#include <QThread>
#include <memory>
#include "core/backends/common/backend.h"
#include "ui/desktop/mainwindow.h"

class KasanNotesApplication : public QApplication
{
	Q_OBJECT
public:
	KasanNotesApplication(int& argc, char** argv);
	virtual ~KasanNotesApplication();

	int run();

private slots:
	void authenticationCompleted();

private:
	void registerMetatypes();
    QPair<QString,QUrl> readTokenAndUrlFromFile(const QString& path);

private:
	Backend::Ptr m_backend;
	std::unique_ptr<QThread> m_backendThread;
	MainWindow m_mainWindow;
};

#endif /* APP_DESKTOP_KASANNOTESAPPLICATION_H_ */
