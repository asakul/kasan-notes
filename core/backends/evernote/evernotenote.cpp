#include "evernotenote.h"
#include "core/backends/evernote/evernotebackend.h"

#include "exceptions.h"

static std::string convertToEnml(const QString& content)
{
	std::string result =
R"(<?xml version="1.0" encoding="UTF-8"?>""
<!DOCTYPE en-note SYSTEM "http://xml.evernote.com/pub/enml2.dtd">"
<en-note>)";
	result += content.toStdString();
	result += "</en-note>";
	return result;
}

static QString convertFromEnml(const std::string& enml)
{
	auto ennotePos = enml.find_first_of("<en-note>");
	auto ennoteClosePos = enml.find_last_of("</en-note>");

	if((ennotePos == std::string::npos) || (ennoteClosePos == std::string::npos))
		BOOST_THROW_EXCEPTION(FormatError() << error_message("Malformed note"));

	ennotePos += strlen("<en-note>");

	return QString::fromStdString(enml.substr(ennotePos, ennoteClosePos - ennotePos));
}

EvernoteNote::EvernoteNote(const std::weak_ptr<EvernoteBackend>& backend) : m_backend(backend)
{

}

EvernoteNote::~EvernoteNote()
{

}

Note::id_t EvernoteNote::id() const
{
	// TODO lookup by guid
	return 0;
}

QString EvernoteNote::backendId() const
{
	return "evernote";
}

void EvernoteNote::setPath(const QString& path)
{
	// TODO
}

QString EvernoteNote::path() const
{
	// TODO
	return QString();
}

void EvernoteNote::setTitle(const QString& title)
{
	m_note->title = title.toStdString();
}

QString EvernoteNote::title() const
{
	return QString::fromStdString(m_note->title);
}

void EvernoteNote::setContent(const QString& content)
{
	m_note->content = convertToEnml(content);
}

QString EvernoteNote::content() const
{
	return convertFromEnml(m_note->content);
}
