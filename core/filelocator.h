#ifndef CORE_FILELOCATOR_H_
#define CORE_FILELOCATOR_H_

#include <QString>
#include <memory>

class FileLocator
{
public:
	typedef std::shared_ptr<FileLocator> Ptr;
	virtual ~FileLocator() {}

	virtual QString getUiFile(const QString& filename) = 0;
	virtual QString getAppDataFilename(const QString& s) = 0;
};

FileLocator::Ptr createDefaultFileLocator();

#endif /* CORE_FILELOCATOR_H_ */
