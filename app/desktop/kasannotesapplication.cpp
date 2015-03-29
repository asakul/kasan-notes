#include "kasannotesapplication.h"
#include "core/filelocator.h"
#include <QQuickItem>
#include <QQmlError>
#include <QScreen>
#include "log.h"
#include "core/backends/kasandb/kasandbauthenticator.h"
#include "core/backends/kasandb/kasandbbackend.h"
#include "core/backends/kasandb/bmxppacket.h"


KasanNotesApplication::KasanNotesApplication(int& argc, char** argv) : QGuiApplication(argc, argv)
{
	setOrganizationDomain("kasan.ws");
	setOrganizationName("K.A.S.A.N.");
	setApplicationName("kasan.notes");

	registerMetatypes();
}

KasanNotesApplication::~KasanNotesApplication()
{
}

int KasanNotesApplication::run()
{
	auto locator = createDefaultFileLocator();
	initLogging(locator->getAppDataFilename("kasan-notes.log"), true);

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

	auto backend = std::make_shared<KasanDbBackend>("127.0.0.1", 5139);
	QObject* root = m_view.rootObject();
	QObject::connect(root, SIGNAL(doLogin(QString,QString)), backend.get(), SLOT(authenticate(QString,QString)));
	m_backend = backend;
	connect(backend.get(), SIGNAL(authenticationCompleted()), this, SLOT(authenticationCompleted()));

	m_view.setFlags(Qt::Dialog);
	m_view.setVisibility(QWindow::Windowed);
	m_view.setPosition((m_view.screen()->size().width() - m_view.size().width()) / 2,
			(m_view.screen()->size().height() - m_view.size().height()) / 2);
	m_view.show();

	m_backendThread = std::unique_ptr<QThread>(new QThread);
	backend->moveToThread(m_backendThread.get());
	m_backendThread->start();

	return exec();
}

void KasanNotesApplication::authenticationCompleted()
{
	auto locator = createDefaultFileLocator();
	m_view.setFlags(Qt::Window);
	m_view.setResizeMode(QQuickView::SizeRootObjectToView);
	m_view.setSource(QUrl::fromLocalFile(locator->getUiFile("main")));
	m_view.setVisibility(QWindow::Maximized);
}

void KasanNotesApplication::registerMetatypes()
{
	qRegisterMetaType<BmxpPacket::Ptr>("BmxpPacket::Ptr");
}
