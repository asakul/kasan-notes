/**
 * @file notestoragemodel.cpp
 */

#include "notestoragemodel.h"
#include "doctest/doctest.h"
#include "fakeit/fakeit.hpp"
#include "test/stubs/stubnote.h"

#include "log.h"

NoteStorageModel::NoteStorageModel(const NoteStorage::Ptr& noteStorage) : m_noteStorage(noteStorage)
{
}

NoteStorageModel::~NoteStorageModel()
{
}

QModelIndex NoteStorageModel::index(int row, int column,
		const QModelIndex& parent) const
{
	Category* parentCat = static_cast<Category*>(parent.internalPointer());
	if(parentCat == nullptr)
	{
		return createIndex(row, column, const_cast<Category*>(&m_rootCategory));
	}
	else
	{
		if(parentCat->subcategories.size() > parent.row())
		{
			parentCat = &parentCat->subcategories[parent.row()];
			return createIndex(row, column, const_cast<Category*>(parentCat));
		}
		else
		{
			return createIndex(row, column, const_cast<Category*>(&m_rootCategory));
		}
	}
}

QModelIndex NoteStorageModel::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int NoteStorageModel::rowCount(const QModelIndex& parent) const
{
	int row = parent.row();
	if(row < 0)
		row = 0;

	Category* parentCat = static_cast<Category*>(parent.internalPointer());
	if(parentCat == nullptr)
	{
		return m_rootCategory.notes.size() + m_rootCategory.subcategories.size();
	}

	if(parentCat->subcategories.size() <= row)
		return 0;

	parentCat = &parentCat->subcategories[row];

	return parentCat->notes.size() + parentCat->subcategories.size();
}

int NoteStorageModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant NoteStorageModel::data(const QModelIndex& index, int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();

	auto parentCat = static_cast<Category*>(index.internalPointer());
	if(!parentCat)
		return QVariant();

	if(index.row() < parentCat->subcategories.size())
	{
		return parentCat->subcategories.at(index.row()).name;
	}
	else
	{
		int itemIndex = index.row() - parentCat->subcategories.size();
		if(itemIndex >= parentCat->notes.size())
			return QVariant();

		auto note = m_noteStorage->getNote(parentCat->notes.at(itemIndex));
		if(!note)
			return QVariant();

		return note->title();
	}
}

void NoteStorageModel::notesChanged()
{
	beginResetModel();
	m_rootCategory.notes.clear();
	m_rootCategory.subcategories.clear();

	auto allNotes = m_noteStorage->allNotes();
	for(const auto& note : allNotes)
	{
		auto path = note->path();
		auto& category = m_rootCategory.getByPath(path);
		category.notes.append(note->id());
		LOG(DEBUG) << "Note: " << note->title() << ": " << note->id();
	}

	endResetModel();
}

DOCTEST_TEST_CASE("NoteStorageModel builds correct tree")
{
	auto storage = std::make_shared<NoteStorage>();
	NoteStorageModel model(storage);

	SUBCASE("Empty storage - empty model")
	{
		REQUIRE(model.rowCount() == 0);
	}

	SUBCASE("One note in the root")
	{
		auto note = std::make_shared<StubNote>(1, "foo");
		note->setPath("/");
		note->setTitle("Foo");
		storage->addNote(note);
		model.notesChanged();

		REQUIRE(model.rowCount() == 1);
		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "Foo");
	}

	SUBCASE("One note in the root, one in subcategory")
	{
		auto note1 = std::make_shared<StubNote>(1, "foo");
		note1->setPath("/");
		note1->setTitle("Root note");
		storage->addNote(note1);

		auto note2 = std::make_shared<StubNote>(2, "foo");
		note2->setPath("/category");
		note2->setTitle("Note in category");
		storage->addNote(note2);

		model.notesChanged();

		REQUIRE(model.rowCount() == 2);
		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "category");
		REQUIRE(model.data(model.index(1, 0), Qt::DisplayRole).toString() == "Root note");
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note in category");
	}

	SUBCASE("Two notes in subcategory")
	{
		auto note1 = std::make_shared<StubNote>(1, "foo");
		note1->setPath("/category");
		note1->setTitle("Note1");
		storage->addNote(note1);

		auto note2 = std::make_shared<StubNote>(2, "foo");
		note2->setPath("/category");
		note2->setTitle("Note2");
		storage->addNote(note2);

		model.notesChanged();

		REQUIRE(model.rowCount() == 1);
		REQUIRE(model.rowCount(model.index(0, 0)) == 2);

		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "category");
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note1");
		REQUIRE(model.data(model.index(1, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note2");
	}
}
