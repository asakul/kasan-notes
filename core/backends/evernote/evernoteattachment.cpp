/**
 * @file evernoteattachment.cpp
 */

#include "evernoteattachment.h"

EvernoteAttachment::EvernoteAttachment(const QString& mimeType,
		const QByteArray& data) : Attachment(mimeType, data)
{
}

EvernoteAttachment::~EvernoteAttachment()
{
}

void EvernoteAttachment::setGuid(const QString& guid)
{
	m_guid = guid;
}

QString EvernoteAttachment::guid() const
{
	return m_guid;
}
