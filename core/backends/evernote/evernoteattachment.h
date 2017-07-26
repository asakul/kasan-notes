/**
 * @file evernoteattachment.h
 */

#ifndef CORE_BACKENDS_EVERNOTE_EVERNOTEATTACHMENT_H_
#define CORE_BACKENDS_EVERNOTE_EVERNOTEATTACHMENT_H_

#include "core/backends/common/attachment.h"

class EvernoteAttachment : public Attachment
{
public:
	EvernoteAttachment(const QString& mimeType, const QByteArray& data = QByteArray());
	virtual ~EvernoteAttachment();

	void setGuid(const QString& guid);
	QString guid() const;

private:
	QString m_guid;

};

#endif /* CORE_BACKENDS_EVERNOTE_EVERNOTEATTACHMENT_H_ */
