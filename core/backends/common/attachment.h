/**
 * @file attachment.h
 */

#ifndef CORE_BACKENDS_COMMON_ATTACHMENT_H_
#define CORE_BACKENDS_COMMON_ATTACHMENT_H_

#include <QByteArray>
#include <QString>

#include <memory>

class Attachment
{
public:
	using Ptr = std::shared_ptr<Attachment>;

	Attachment(const QString& mimeType, const QByteArray& data = QByteArray());
	virtual ~Attachment();

	virtual void setMimeType(const QString& mimeType);
	virtual QString mimeType() const;

	virtual void setData(const QByteArray& data);
	virtual QByteArray data() const;

	virtual QByteArray hash() const;

private:
	QString m_mimeType;
	QByteArray m_data;
	QByteArray m_hash;
};

#endif /* CORE_BACKENDS_COMMON_ATTACHMENT_H_ */
