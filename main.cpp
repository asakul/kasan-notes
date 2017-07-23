
#include "app/desktop/kasannotesapplication.h"
#include "log.h"
#include "exceptions.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"


int main(int argc, char** argv)
{
	KasanNotesApplication app(argc, argv);

	try
	{
		return app.run();
	}
	catch(const KasanNotesException& e)
	{
		QString const* qstringPtr = boost::get_error_info<error_message>(e);
		if(qstringPtr)
		{
			LOG(ERROR) << *qstringPtr;
		}

		std::string const* stringPtr = boost::get_error_info<errinfo_str>(e);
		if(stringPtr)
		{
			LOG(ERROR) << *stringPtr;
		}
	}
}
