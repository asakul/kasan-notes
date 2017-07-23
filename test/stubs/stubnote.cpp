/**
 * @file stubnote.cpp
 */

#include "stubnote.h"

StubNote::StubNote(id_t id, const QString& backendId) : m_id(id), m_backendId(backendId)
{
}

StubNote::~StubNote()
{
}

StubNote::id_t StubNote::id() const
{
	return m_id;
}

QString StubNote::backendId() const
{
	return m_backendId;
}

void StubNote::setPath(const QString& path)
{
	m_path = path;
}

QString StubNote::path() const
{
	return m_path;
}

void StubNote::setTitle(const QString& title)
{
	m_title = title;
}

QString StubNote::title() const
{
	return m_title;
}

void StubNote::setContent(const boost::optional<QString>& content)
{
	m_content = content;
}

boost::optional<QString> StubNote::content() const
{
	return m_content;
}
