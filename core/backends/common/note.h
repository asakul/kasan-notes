#ifndef NOTE_H
#define NOTE_H

#include <QString>

#include <cstdint>
#include <memory>

#include <boost/optional.hpp>
#include <QMetaType>

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

private:
	id_t m_id;
	QString m_backendId;

	QString m_path;
	QString m_title;
	boost::optional<QString> m_content;
};

Q_DECLARE_METATYPE(Note::Ptr);

#endif // NOTE_H
