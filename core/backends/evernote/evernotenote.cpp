#include "evernotenote.h"
#include "core/backends/evernote/evernotebackend.h"

#include "exceptions.h"
#include "log.h"

#include <QDomDocument>

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
	QDomDocument doc;
	doc.setContent(enml);

	auto ennote = doc.elementsByTagName("en-note").at(0).toElement();
	if(ennote.isNull())
		return QString();

	auto mediaTags = ennote.elementsByTagName("en-media");
	for(int i = 0; i < mediaTags.size(); i++)
	{
		auto mediaTag = mediaTags.at(i).toElement();

		if(mediaTag.attribute("type").startsWith("image/"))
		{
			mediaTag.setTagName("img");
			mediaTag.setAttribute("src", "attachment://" + mediaTag.attribute("hash"));
		}
	}

	return doc.toString();
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
