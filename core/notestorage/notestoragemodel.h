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
	enum
	{
		NoteRole = Qt::UserRole + 1,
		NotebookRole
	};

	NoteStorageModel(const NoteStorage::Ptr& noteStorage);
	virtual ~NoteStorageModel();

	virtual QModelIndex index(int row, int column,
	                              const QModelIndex &parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	virtual QHash<int, QByteArray> roleNames() const;

public slots:
	void notesChanged();

private:
	NoteStorage::Ptr m_noteStorage;
};

#endif /* CORE_NOTESTORAGE_NOTESTORAGEMODEL_H_ */
