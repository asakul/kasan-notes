/**
 * @file evernotenotebook.cpp
 */

#include "evernotenotebook.h"

EvernoteNotebook::EvernoteNotebook(const Notebook::id_t& id) : Notebook(id, "evernote")
{
}

EvernoteNotebook::~EvernoteNotebook()
{
}

void EvernoteNotebook::setGuid(const QString& guid)
{
	m_guid = guid;
}

QString EvernoteNotebook::guid() const
{
	return m_guid;
}
