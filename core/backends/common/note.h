#ifndef NOTE_H
#define NOTE_H

#include <QString>

#include <cstdint>
#include <memory>

#include <boost/optional.hpp>

class Note
{
public:
	using Ptr = std::shared_ptr<Note>;
	using id_t = uint64_t;

	Note();
	virtual ~Note();

	virtual id_t id() const = 0;
	virtual QString backendId() const = 0;

	virtual void setPath(const QString& path) = 0;
	virtual QString path() const = 0;

	virtual void setTitle(const QString& title) = 0;
	virtual QString title() const = 0;

	virtual void setContent(const boost::optional<QString>& content) = 0;
	virtual boost::optional<QString> content() const = 0;
};

#endif // NOTE_H
