#ifndef CORE_BACKENDS_COMMON_BACKEND_H_
#define CORE_BACKENDS_COMMON_BACKEND_H_

#include <memory>

class Backend
{
public:
	typedef std::shared_ptr<Backend> Ptr;

	virtual ~Backend() {}
};


#endif /* CORE_BACKENDS_COMMON_BACKEND_H_ */
