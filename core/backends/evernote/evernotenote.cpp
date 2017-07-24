#include "evernotenote.h"
#include "core/backends/evernote/evernotebackend.h"

#include "exceptions.h"
#include "log.h"

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

EvernoteNote::EvernoteNote(const id_t& id, const std::weak_ptr<EvernoteBackend>& backend) : Note(id, "evernote"), m_backend(backend)
{

}

EvernoteNote::~EvernoteNote()
{

}

void EvernoteNote::setEnml(const QString& enml)
{
	m_note.content = enml;
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
	if(m_note.content.isSet())
		return convertFromEnml(m_note.content.value());
	else
		return boost::optional<QString>();
}
