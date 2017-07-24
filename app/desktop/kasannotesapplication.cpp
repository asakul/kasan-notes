#include "kasannotesapplication.h"
#include "core/filelocator.h"
#include "log.h"
#include "core/backends/evernote/evernotebackend.h"
#include "core/backends/common/note.h"
#include "core/backends/common/notebook.h"
#include "exceptions.h"

#include <QQuickItem>
#include <QQmlError>
#include <QScreen>
#include <QTimer>


KasanNotesApplication::KasanNotesApplication(int& argc, char** argv) : QGuiApplication(argc, argv)
{
	setOrganizationDomain("kasan.ws");
	setOrganizationName("K.A.S.A.N.");
	setApplicationName("kasan.notes");

	registerMetatypes();
	m_mainWindowController.setView(&m_view);
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

	m_mainWindowController.setBackend(m_backend);

	if(m_backend->isAuthenticated())
	{
		authenticationCompleted();
	}

	m_backendThread = std::unique_ptr<QThread>(new QThread);
	m_backend->moveToThread(m_backendThread.get());
	m_backendThread->start();

	m_mainWindowController.forceNotesRefresh();

	return exec();
}

void KasanNotesApplication::authenticationCompleted()
{
	auto locator = createDefaultFileLocator();
	m_view.setFlags(Qt::Window);
	m_view.setResizeMode(QQuickView::SizeRootObjectToView);
	m_view.setSource(QUrl::fromLocalFile(locator->getUiFile("main")));
	m_view.setVisibility(QWindow::Maximized);

	auto root = m_view.rootObject();
	auto toolbar = root->findChild<QObject*>("toolbar");

	connect(toolbar, SIGNAL(addButtonClicked()), &m_mainWindowController, SLOT(addButtonClicked()));
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
