#ifndef APP_DESKTOP_KASANNOTESAPPLICATION_H_
#define APP_DESKTOP_KASANNOTESAPPLICATION_H_

#include <QGuiApplication>
#include <QThread>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <memory>
#include "core/backends/common/backend.h"

class KasanNotesApplication : public QGuiApplication
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

private:
	Backend::Ptr m_backend;
	std::unique_ptr<QThread> m_backendThread;
	QQuickView m_view;
	QQmlApplicationEngine m_engine;
};

#endif /* APP_DESKTOP_KASANNOTESAPPLICATION_H_ */
