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
