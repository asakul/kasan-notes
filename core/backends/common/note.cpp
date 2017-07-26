#include "note.h"


Note::Note(const id_t& id, const QString& backendId) : m_id(id), m_backendId(backendId)
{
}

Note::~Note()
{

}

Note::id_t Note::id() const
{
	return m_id;
}

QString Note::backendId() const
{
	return m_backendId;
}

void Note::setTitle(const QString& title)
{
	m_title = title;
}

QString Note::title() const
{
	return m_title;
}

void Note::setContent(const boost::optional<QString>& content)
{
	m_content = content;
}

boost::optional<QString> Note::content() const
{
	return m_content;
}

void Note::addAttachment(const Attachment::Ptr& attachment)
{
	m_attachments[attachment->hash()] = attachment;
}

Attachment::Ptr Note::attachmentByHash(const QByteArray& hash) const
{
	auto it = m_attachments.find(hash);
	if(it != m_attachments.end())
		return it.value();
	return Attachment::Ptr();
}

Attachment::Ptr Note::attachmentByIndex(size_t index) const
{
	return m_attachments.values()[index];
}

size_t Note::attachmentsCount() const
{
	return m_attachments.size();
}

void Note::setParent(const std::shared_ptr<Notebook>& parent)
{
	m_parent = parent;
}

std::weak_ptr<Notebook> Note::parent() const
{
	return m_parent;
}
