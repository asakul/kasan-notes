#ifndef NOTE_H
#define NOTE_H

#include "attachment.h"

#include <cstdint>
#include <memory>

#include <boost/optional.hpp>

#include <QString>
#include <QMetaType>
#include <QMap>

class Notebook;

class Note
{
public:
	using Ptr = std::shared_ptr<Note>;
	using id_t = uint64_t;

	Note(const id_t& id, const QString& backendId);
	virtual ~Note();

	virtual id_t id() const;
	virtual QString backendId() const;

	virtual void setTitle(const QString& title);
	virtual QString title() const;

	virtual void setContent(const boost::optional<QString>& content);
	virtual boost::optional<QString> content() const;

	virtual void addAttachment(const Attachment::Ptr& attachment);
	virtual Attachment::Ptr attachmentByHash(const QByteArray& hash) const;
	virtual Attachment::Ptr attachmentByIndex(size_t index) const;
	virtual size_t attachmentsCount() const;
	virtual void removeAttachment(const Attachment::Ptr& attachment);

	virtual void setParent(const std::shared_ptr<Notebook>& parent);
	virtual std::weak_ptr<Notebook> parent() const;

private:
	id_t m_id;
	QString m_backendId;

	QString m_path;
	QString m_title;
	boost::optional<QString> m_content;
	std::weak_ptr<Notebook> m_parent;

	QMap<QByteArray, Attachment::Ptr> m_attachments;
};

Q_DECLARE_METATYPE(Note::Ptr);

#endif // NOTE_H
