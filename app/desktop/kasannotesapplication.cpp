#include "kasannotesapplication.h"
#include "core/filelocator.h"
#include "log.h"
#include "core/backends/evernote/evernotebackend.h"
#include "core/backends/common/note.h"
#include "core/backends/common/notebook.h"

#include "exceptions.h"

#include <QTimer>
#include <QFile>


KasanNotesApplication::KasanNotesApplication(int& argc, char** argv) : QApplication(argc, argv)
{
	registerMetatypes();
}

KasanNotesApplication::~KasanNotesApplication()
{
}

int KasanNotesApplication::run()
{
	auto locator = createDefaultFileLocator();
	initLogging(locator->getAppDataFilename("kasan-notes.log"), true);

	/*
	m_view.setResizeMode(QQuickView::SizeViewToRootObject);
	m_view.setSource(QUrl::fromLocalFile(locator->getUiFile("login")));
	if(m_view.status() != QQuickView::Ready)
	{
		LOG(WARNING) << "Unable to load login form";
		auto errors = m_view.errors();
		for(const auto& error : errors)
		{
			LOG(WARNING) << error.line() << ": " << error.description();
		}
		return -1;
	}
	*/

	auto tokenAndUrl = readTokenAndUrlFromFile("dev-data/dev-token.txt");
	m_backend = EvernoteBackend::createFromDeveloperToken(tokenAndUrl.first, tokenAndUrl.second);

	m_mainWindow.setBackend(m_backend);

	if(m_backend->isAuthenticated())
	{
		authenticationCompleted();
		m_mainWindow.authenticationSuccessful();
	}

	m_backendThread = std::unique_ptr<QThread>(new QThread);
	m_backend->moveToThread(m_backendThread.get());
	m_backendThread->start();

	m_mainWindow.show();
	m_mainWindow.forceNotesRefresh();

	int rc = exec();
	m_backendThread->quit();
	m_backendThread->wait(1000);
	return rc;
}

void KasanNotesApplication::authenticationCompleted()
{

}

void KasanNotesApplication::registerMetatypes()
{
	qRegisterMetaType<Notebook::Ptr>("Notebook::Ptr");
	qRegisterMetaType<Note::Ptr>("Note::Ptr");
	qRegisterMetaType<QList<Note::Ptr>>("QList<Note::Ptr>");
}

QPair<QString,QUrl> KasanNotesApplication::readTokenAndUrlFromFile(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
		BOOST_THROW_EXCEPTION(FileNotFound() << error_message(tr("Unable to open file with token/url: %1").arg(path)));

	auto token = QString::fromUtf8(file.readLine()).trimmed();
	auto url = QUrl::fromUserInput(QString::fromUtf8(file.readLine()).trimmed());

	return qMakePair(token, url);
}
