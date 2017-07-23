#include "evernotenote.h"
#include "core/backends/evernote/evernotebackend.h"

#include "exceptions.h"

static QString convertToEnml(const QString& content)
{
	QString result =
R"(<?xml version="1.0" encoding="UTF-8"?>""
<!DOCTYPE en-note SYSTEM "http://xml.evernote.com/pub/enml2.dtd">"
<en-note>)";
	result += content;
	result += "</en-note>";
	return result;
}

static QString convertFromEnml(const QString& enml)
{
	auto ennotePos = enml.indexOf("<en-note>");
	auto ennoteClosePos = enml.lastIndexOf("</en-note>");

	if((ennotePos == -1) || (ennoteClosePos == -1))
		BOOST_THROW_EXCEPTION(FormatError() << error_message("Malformed note"));

	ennotePos += strlen("<en-note>");

	return enml.mid(ennotePos, ennoteClosePos - ennotePos);
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
	m_note.title = title;
}

QString EvernoteNote::title() const
{
	return m_note.title;
}

void EvernoteNote::setContent(const boost::optional<QString>& content)
{
	if(content)
	{
		m_note.content = convertToEnml(content.value());
	}
}

boost::optional<QString> EvernoteNote::content() const
{
	return convertFromEnml(m_note.content.value());
}