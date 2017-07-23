/**
 * @file notestoragemodel.h
 */

#ifndef CORE_NOTESTORAGE_NOTESTORAGEMODEL_H_
#define CORE_NOTESTORAGE_NOTESTORAGEMODEL_H_

#include <QAbstractItemModel>

#include "notestorage.h"

class NoteStorageModel : public QAbstractItemModel
{
public:
	NoteStorageModel(const NoteStorage::Ptr& noteStorage);
	virtual ~NoteStorageModel();

	virtual QModelIndex index(int row, int column,
	                              const QModelIndex &parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
	void notesChanged();

private:
	NoteStorage::Ptr m_noteStorage;

	struct Category
	{
		QString name;
		QList<Category> subcategories;
		QList<Note::id_t> notes;

		Category& getByPath(QString path)
		{
			if(path.startsWith('/'))
				path = path.right(path.size() - 1);

			if(path.isEmpty())
				return *this;

			auto thisName = path.section('/', 0, 0);
			auto rest = path.section('/', 1);
			for(auto& cat : subcategories)
			{
				if(cat.name == thisName)
					return cat.getByPath(rest);
			}

			Category newcat;
			newcat.name = thisName;
			subcategories.append(newcat);
			return subcategories.back().getByPath(rest);
		}
	};

	Category m_rootCategory;
};

#endif /* CORE_NOTESTORAGE_NOTESTORAGEMODEL_H_ */
