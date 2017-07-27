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
	Notebook* parentNotebook = static_cast<Notebook*>(parent.internalPointer());
	if(parentNotebook == nullptr)
	{
		return createIndex(row, column, const_cast<Notebook*>(m_noteStorage->rootNotebook().get()));
	}
	else
	{
		if(parent.row() < parentNotebook->notebooksCount())
		{
			parentNotebook = parentNotebook->notebookByIndex(parent.row()).get();
			return createIndex(row, column, const_cast<Notebook*>(parentNotebook));
		}
		else
		{
			return createIndex(-1, -1, nullptr);
		}
	}
}

QModelIndex NoteStorageModel::parent(const QModelIndex& child) const
{
	Notebook* parentNotebook = static_cast<Notebook*>(child.internalPointer());
	if(parentNotebook == nullptr)
	{
		return QModelIndex();
	}
	else
	{
		auto grandparentNotebook = parentNotebook->parent().lock();
		if(!grandparentNotebook)
			return QModelIndex();
		int indexOfNotebook = -1;
		for(int i = 0; i < grandparentNotebook->notebooksCount(); i++)
		{
			if(grandparentNotebook->notebookByIndex(i).get() == parentNotebook)
			{
				indexOfNotebook = i;
				break;
			}
		}
		if(indexOfNotebook < 0)
			return QModelIndex();

		return createIndex(indexOfNotebook, child.column(), grandparentNotebook.get());
	}
}

int NoteStorageModel::rowCount(const QModelIndex& parent) const
{
	int row = parent.row();
	if(row < 0)
		row = 0;

	Notebook* parentNotebook = static_cast<Notebook*>(parent.internalPointer());
	if(parentNotebook == nullptr)
	{
		return m_noteStorage->rootNotebook()->notebooksCount() + m_noteStorage->rootNotebook()->notesCount();
	}

	if(row >= parentNotebook->notebooksCount())
		return 0;

	parentNotebook = parentNotebook->notebookByIndex(row).get();

	return parentNotebook->notebooksCount() + parentNotebook->notesCount();
}

int NoteStorageModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant NoteStorageModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		auto parentNotebook = static_cast<Notebook*>(index.internalPointer());
		if(!parentNotebook)
			return QVariant();

		if(index.row() < parentNotebook->notebooksCount())
		{
			return parentNotebook->notebookByIndex(index.row())->title();
		}
		else
		{
			int itemIndex = index.row() - parentNotebook->notebooksCount();
			if(itemIndex >= parentNotebook->notesCount())
				return QVariant();

			auto note = parentNotebook->noteByIndex(itemIndex);
			if(!note)
				return QVariant();

			return note->title();
		}
	}
	else if(role == NoteRole)
	{
		auto parentNotebook = static_cast<Notebook*>(index.internalPointer());
		if(!parentNotebook)
			return QVariant();

		if(index.row() < parentNotebook->notebooksCount())
		{
			return QVariant();
		}
		else
		{
			int itemIndex = index.row() - parentNotebook->notebooksCount();
			if(itemIndex >= parentNotebook->notesCount())
				return QVariant();

			auto note = parentNotebook->noteByIndex(itemIndex);
			if(!note)
				return QVariant();

			return QVariant::fromValue(note);
		}
	}
	else if(role == NotebookRole)
	{
		auto parentNotebook = static_cast<Notebook*>(index.internalPointer());
		if(!parentNotebook)
			return QVariant();

		if(index.row() < parentNotebook->notebooksCount())
		{
			return QVariant::fromValue(parentNotebook->notebookByIndex(index.row()));
		}
		else
		{
			return QVariant::fromValue(parentNotebook->shared_from_this());
		}
	}
	else
	{
		return QVariant();
	}
}

QHash<int, QByteArray> NoteStorageModel::roleNames() const
{
	return QHash<int, QByteArray> {{Qt::DisplayRole, "display"}, {NoteRole, "note_object"}};
}

void NoteStorageModel::notesChanged()
{
	beginResetModel();
	endResetModel();
}

TEST_CASE("NoteStorageModel builds correct tree")
{
	auto storage = std::make_shared<NoteStorage>();
	NoteStorageModel model(storage);

	SUBCASE("Empty storage - empty model")
	{
		REQUIRE(model.rowCount() == 0);
	}

	SUBCASE("One note in the root")
	{
		auto note = std::make_shared<Note>(1, "foo");
		note->setTitle("Foo");
		storage->rootNotebook()->addNote(note);
		model.notesChanged();

		REQUIRE(model.rowCount() == 1);
		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "Foo");
	}

	SUBCASE("One note in the root, one in subnotebook")
	{
		auto note1 = std::make_shared<Note>(1, "foo");
		note1->setTitle("Root note");
		storage->rootNotebook()->addNote(note1);

		auto notebook = std::make_shared<Notebook>(1, "foo");
		notebook->setTitle("category");
		storage->rootNotebook()->addNotebook(notebook);
		auto note2 = std::make_shared<Note>(2, "foo");
		note2->setTitle("Note in notebook");
		notebook->addNote(note2);

		model.notesChanged();

		REQUIRE(model.rowCount() == 2);
		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "category");
		REQUIRE(model.data(model.index(1, 0), Qt::DisplayRole).toString() == "Root note");
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note in notebook");
	}

	SUBCASE("Two notes in subnotebook")
	{
		auto notebook = std::make_shared<Notebook>(1, "foo");
		notebook->setTitle("category");
		storage->rootNotebook()->addNotebook(notebook);

		auto note1 = std::make_shared<Note>(1, "foo");
		note1->setTitle("Note1");
		notebook->addNote(note1);

		auto note2 = std::make_shared<Note>(2, "foo");
		note2->setTitle("Note2");
		notebook->addNote(note2);

		model.notesChanged();

		REQUIRE(model.rowCount() == 1);
		REQUIRE(model.rowCount(model.index(0, 0)) == 2);

		REQUIRE(model.data(model.index(0, 0), Qt::DisplayRole).toString() == "category");
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note1");
		REQUIRE(model.data(model.index(1, 0, model.index(0, 0)), Qt::DisplayRole).toString() == "Note2");
	}
}

TEST_CASE("NoteStorageModel: custom data role - NoteRole")
{
	auto storage = std::make_shared<NoteStorage>();
	NoteStorageModel model(storage);

	auto notebook = std::make_shared<Notebook>(1, "foo");
	notebook->setTitle("category");
	storage->rootNotebook()->addNotebook(notebook);

	auto note1 = std::make_shared<Note>(1, "foo");
	note1->setTitle("Note1");
	notebook->addNote(note1);

	auto note2 = std::make_shared<Note>(2, "foo");
	note2->setTitle("Note2");
	notebook->addNote(note2);

	model.notesChanged();

	SUBCASE("If index corresponds to a Note, return this note")
	{
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), NoteStorageModel::NoteRole).value<Note::Ptr>() == note1);
	}
}


TEST_CASE("NoteStorageModel: custom data role - NotebookRole")
{
	auto storage = std::make_shared<NoteStorage>();
	NoteStorageModel model(storage);

	auto notebook = std::make_shared<Notebook>(1, "foo");
	notebook->setTitle("category");
	storage->rootNotebook()->addNotebook(notebook);

	auto note1 = std::make_shared<Note>(1, "foo");
	note1->setTitle("Note1");
	notebook->addNote(note1);

	auto note2 = std::make_shared<Note>(2, "foo");
	note2->setTitle("Note2");
	notebook->addNote(note2);

	model.notesChanged();

	SUBCASE("If index corresponds to a Notebook, return this notebook")
	{
		REQUIRE(model.data(model.index(0, 0), NoteStorageModel::NotebookRole).value<Notebook::Ptr>() == notebook);
	}

	SUBCASE("If index corresponds to a Note, return parent notebook")
	{
		REQUIRE(model.data(model.index(0, 0, model.index(0, 0)), NoteStorageModel::NotebookRole).value<Notebook::Ptr>() == notebook);
		REQUIRE(model.data(model.index(1, 0, model.index(0, 0)), NoteStorageModel::NotebookRole).value<Notebook::Ptr>() == notebook);
	}

}

