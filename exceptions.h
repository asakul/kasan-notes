#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <QtCore/QString>
#include <string>
#include <boost/exception/all.hpp>

struct KasanNotesException : virtual public boost::exception, virtual public std::exception
{
	const char* what() const throw()
	{
		return boost::diagnostic_information_what(*this);
	}
};

struct FileNotFound : public KasanNotesException
{
};

struct ConnectionError : public KasanNotesException
{
};

struct ProtocolError : public KasanNotesException
{
};

struct FormatError : public KasanNotesException
{
};

typedef boost::error_info<struct errinfo_str_, std::string> errinfo_str;
typedef boost::error_info<struct error_message_, QString> error_message;


#endif /* EXCEPTIONS_H_ */
