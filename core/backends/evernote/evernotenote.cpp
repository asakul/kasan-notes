#include "evernotenote.h"
#include "core/backends/evernote/evernotebackend.h"

#include "exceptions.h"
#include "log.h"

#include <QDomDocument>
#include <QDomImplementation>

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

qevercloud::Optional<QString> EvernoteNote::enml() const
{
	return m_note.content;
}

void EvernoteNote::setContent(const boost::optional<QString>& content)
{
	if(content)
	{
		auto newContentAndHashes = convertToEnml(content.value());
		auto newContent = newContentAndHashes.first;
		auto usedHashes = newContentAndHashes.second;

		for(int i = 0; i < attachmentsCount(); i++)
		{
			auto attachment = attachmentByIndex(i);
			if(!usedHashes.contains(QString::fromUtf8(attachment->hash().toHex())))
			{
				LOG(DEBUG) << "Removing unused hash: " << attachment->hash();
				removeAttachment(attachment);
			}
		}

		m_note.content = newContent;
	}
}

boost::optional<QString> EvernoteNote::content() const
{
	if(m_note.content.isSet())
		return convertFromEnml(m_note.content.value());
	else
		return boost::optional<QString>();
}

QPair<QString, QSet<QString>> EvernoteNote::convertToEnml(const QString& content)
{
	QSet<QString> usedHashes;
	QDomDocument doc;
	doc.setContent(content);

	auto body = doc.elementsByTagName("body").at(0);

	QDomDocument newDoc(doc.implementation().createDocumentType("en-note", QString(), "http://xml.evernote.com/pub/enml2.dtd"));
	auto newbody = newDoc.importNode(body, true).toElement();
	newDoc.appendChild(newbody);
	newbody.setTagName("en-note");
	auto mediaTags = newbody.elementsByTagName("img");
	for(int i = 0; i < mediaTags.size(); i++)
	{
		auto mediaTag = mediaTags.at(i).toElement();
		auto src = mediaTag.attribute("src");
		if(src.startsWith("attachment://"))
		{
			auto hash = src.mid(13);
			mediaTag.setAttribute("hash", hash);
			mediaTag.removeAttribute("src");
			auto attach = attachmentByHash(QByteArray::fromHex(hash.toUtf8()));
			if(!attach)
			{
				LOG(WARNING) << "EvernoteNote: trying to save image without corresponding attachment";
				continue;
			}
			LOG(DEBUG) << "Found hash: " << hash;
			usedHashes.insert(hash);
			mediaTag.setAttribute("type", attach->mimeType());
			mediaTag.setTagName("en-media");
		}
	}

	return qMakePair(newDoc.toString(), usedHashes);
}

