#ifndef CORE_BACKENDS_COMMON_BACKEND_H_
#define CORE_BACKENDS_COMMON_BACKEND_H_

#include <memory>
#include <QObject>

class Backend : public QObject
{
	Q_OBJECT
public:
	typedef std::shared_ptr<Backend> Ptr;

	virtual ~Backend();

	virtual bool isAuthenticated() = 0;

signals:
};


#endif /* CORE_BACKENDS_COMMON_BACKEND_H_ */
