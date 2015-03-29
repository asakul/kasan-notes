#ifndef APP_DESKTOP_DESKTOPFILELOCATOR_H_
#define APP_DESKTOP_DESKTOPFILELOCATOR_H_

#include "core/filelocator.h"

class DesktopFileLocator : public FileLocator
{
public:
	DesktopFileLocator();
	virtual ~DesktopFileLocator();

	virtual QString getUiFile(const QString& filename);
	virtual QString getAppDataFilename(const QString& s);

};

#endif /* APP_DESKTOP_DESKTOPFILELOCATOR_H_ */
