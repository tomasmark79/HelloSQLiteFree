// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <CSQLite/CSQLite.hpp>
#include <CSQLite/version.h>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#include <memory>

namespace library
{
  CSQLite::CSQLite (const std::string &assetsPath)
    : m_assetsPath (assetsPath),
      db (assetsPath + "/InventDB.db", SQLite::OPEN_READWRITE)

  {
    LOG_INFO ("CSQLite v." + std::string (CSQLITE_VERSION) + " constructed.");
    LOG_DEBUG ("Assets Path: " + this->m_assetsPath);

    buffer = std::make_shared<std::vector<std::string> > ();

    this->printAll ();
    LOG_DEBUG ("---------CHANGED---------------------");
    // there
    // this->replaceTextInTable ("Users", "UserName", "Kryl Karel",
    //                           "Soukromý Martin");
    // and back - just demo
    this->replaceTextInTable ("Users", "UserName", "Soukromý Martin",
                              "Kryl Karel");
    this->printAll ();
  }

  CSQLite::~CSQLite () { LOG_DEBUG ("CSQLite deconstructed."); }

  int
  CSQLite::getRowsOfTable (const std::string &tableName,
                           std::shared_ptr<std::vector<std::string> > buffer)
  {
    std::vector<std::string> rows;
    try
    {
      SQLite::Statement query (db, "SELECT * FROM " + tableName);
      while (query.executeStep ())
      {
        int id = query.getColumn (0);
        const char *name = query.getColumn (1);
        buffer->push_back (std::to_string (id) + ", " + name);
        LOG_INFO ("Row: " + std::to_string (id) + ", " + name);
      }
    }
    catch (std::exception &e)
    {
      LOG_E << "exception: " << e.what () << std::endl;
      return 1;
    }
    catch (...)
    {
      LOG_E << "unknown exception" << std::endl;
      return 2;
    }
    return 0;
  }

  int CSQLite::printAll ()
  {
    std::vector<std::string> tables
      = { "Departments", "Devices", "Users", "Rooms", "InventoryRecords" };
    for (const auto &table : tables)
    {
      try
      {
        this->getRowsOfTable (table, buffer);
      }
      catch (std::exception &e)
      {
        LOG_E << "Failed to create table " << table << ": " << e.what ()
              << std::endl;
        return 1;
      }
    }
    return 0;
  }

  int CSQLite::replaceTextInTable (const std::string &tableName,
                                   const std::string &columnName,
                                   const std::string &oldText,
                                   const std::string &newText)
  {
    // it is database transaction
    SQLite::Transaction transaction (db);
    try
    {
      SQLite::Statement query (db, "UPDATE " + tableName + " SET " + columnName
                                     + " = ? WHERE " + columnName + " = ?");
      query.bind (1, newText);
      query.bind (2, oldText);
      query.exec ();

      transaction.commit ();
    }
    catch (std::exception &e)
    {
      transaction.rollback ();
      LOG_E << "exception: " << e.what () << std::endl;
      return 1;
    }
    catch (...)
    {
      transaction.rollback ();
      LOG_E << "unknown exception" << std::endl;
      return 2;
    }
    return 0;
  }

} // namespace library