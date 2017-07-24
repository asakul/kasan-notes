/**
 * @file notebook.cpp
 */

#include "notebook.h"

#include "doctest/doctest.h"

Notebook::Notebook(const id_t& id, const QString& backendId) : m_id(id), m_backendId(backendId)
{
}

Notebook::~Notebook()
{
}

Notebook::id_t Notebook::id() const
{
	return m_id;
}

QString Notebook::backendId() const
{
	return m_backendId;
}

void Notebook::setTitle(const QString& title)
{
	m_title = title;
}

QString Notebook::title() const
{
	return m_title;
}

void Notebook::addNote(const Note::Ptr& note)
{
	m_notes.append(note);
}

size_t Notebook::notesCount() const
{
	return m_notes.size();
}

Note::Ptr Notebook::noteByIndex(size_t index) const
{
	if(index >= m_notes.size())
		return nullptr;

	return m_notes[index];
}

Note::Ptr Notebook::noteById(const Note::id_t& id) const
{
	for(const auto& x : m_notes)
	{
		if(x->id() == id)
			return x;
	}
	return nullptr;
}

void Notebook::removeNote(const Note::Ptr& note)
{
	for(auto it = m_notes.begin(); it != m_notes.end(); ++it)
	{
		if((*it) == note)
		{
			m_notes.erase(it);
			return;
		}
	}
}

void Notebook::addNotebook(const Notebook::Ptr& notebook)
{
	m_notebooks.append(notebook);
}

size_t Notebook::notebooksCount() const
{
	return m_notebooks.size();
}

Notebook::Ptr Notebook::notebookByIndex(size_t index) const
{
	return m_notebooks[index];
}


TEST_CASE("Notebook basic functionality")
{
	Notebook notebook(1, "foo_backend");

	SUBCASE("Returns given ID")
	{
		REQUIRE(notebook.id() == 1);
	}

	SUBCASE("Returns given backend ID")
	{
		REQUIRE(notebook.backendId() == "foo_backend");
	}

	SUBCASE("Title manipulation")
	{
		SUBCASE("Default title is empty")
		{
			REQUIRE(notebook.title().isEmpty());
		}

		SUBCASE("Set/get title")
		{
			notebook.setTitle("foo_title");
			REQUIRE(notebook.title() == "foo_title");
		}
	}

	SUBCASE("Notes manipulation")
	{
		auto note1 = std::make_shared<Note>(1, "foo_backend");
		auto note2 = std::make_shared<Note>(2, "foo_backend");

		SUBCASE("No notes by default")
		{
			REQUIRE(notebook.notesCount() == 0);
		}

		notebook.addNote(note2);
		notebook.addNote(note1);

		SUBCASE("Get by index")
		{
			REQUIRE(notebook.noteByIndex(0) == note2);
			REQUIRE(notebook.noteByIndex(1) == note1);
		}

		SUBCASE("Get by ID")
		{
			REQUIRE(notebook.noteById(2) == note2);
			REQUIRE(notebook.noteById(1) == note1);
		}

		SUBCASE("Remove note")
		{
			notebook.removeNote(note2);
			REQUIRE(notebook.notesCount() == 1);
			REQUIRE(notebook.noteById(2) == nullptr);
		}
	}
}
