#include "desktopfilelocator.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include "exceptions.h"

DesktopFileLocator::DesktopFileLocator()
{
}

DesktopFileLocator::~DesktopFileLocator()
{
}

QString DesktopFileLocator::getUiFile(const QString& filename)
{
	QFileInfo info("ui/desktop/" + filename + ".qml");
	if(!info.exists())
		BOOST_THROW_EXCEPTION(FileNotFound() << errinfo_str("Unable to locate UI file: " + info.absoluteFilePath().toStdString()));
	return info.absoluteFilePath();
}

QString DesktopFileLocator::getAppDataFilename(const QString& s)
{
	QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	return d.absoluteFilePath(s);
}

FileLocator::Ptr createDefaultFileLocator()
{
	return std::make_shared<DesktopFileLocator>();
}
