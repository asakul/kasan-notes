/**
 * @file notestoragemodel.cpp
 */

#include "notestoragemodel.h"

NoteStorageModel::NoteStorageModel(const NoteStorage::Ptr& noteStorage) : m_noteStorage(noteStorage)
{
}

NoteStorageModel::~NoteStorageModel()
{
}

QModelIndex NoteStorageModel::index(int row, int column,
		const QModelIndex& parent) const
{
	return createIndex(row, column);
}

QModelIndex NoteStorageModel::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int NoteStorageModel::rowCount(const QModelIndex& parent) const
{
	return 0;
}

int NoteStorageModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant NoteStorageModel::data(const QModelIndex& index, int role) const
{
	return QVariant();
}

void NoteStorageModel::notesChanged()
{

}
