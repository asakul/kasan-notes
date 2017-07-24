/**
 * @file attachment.cpp
 */

#include "attachment.h"

#include <QCryptographicHash>

Attachment::Attachment(const QString& mimeType, const QByteArray& data) : m_mimeType(mimeType)
{
	setData(data);
}

Attachment::~Attachment()
{
}

void Attachment::setMimeType(const QString& mimeType)
{
	m_mimeType = mimeType;
}

QString Attachment::mimeType() const
{
	return m_mimeType;
}

void Attachment::setData(const QByteArray& data)
{
	m_data = data;
	m_hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
}

QByteArray Attachment::data() const
{
	return m_data;
}

QByteArray Attachment::hash() const
{
	return m_hash;
}
