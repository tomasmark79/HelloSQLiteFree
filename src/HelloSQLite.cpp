// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <HelloSQLite/HelloSQLite.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

namespace dotname {

  HelloSQLite::HelloSQLite () {
    LOG_D_STREAM << libName_ << " ...constructed" << std::endl;
  }
  HelloSQLite::HelloSQLite (const std::filesystem::path& assetsPath) : HelloSQLite () {
    assetsPath_ = assetsPath;
    if (!assetsPath_.empty ()) {
      LOG_D_STREAM << "Assets path: " << assetsPath_ << std::endl;

      // Open the database
      dbPtr_ = std::make_unique<SQLite::Database> ((assetsPath_ / "InventDB.db").string (),
                                                   SQLite::OPEN_READWRITE);

      buffer_ = std::make_shared<std::vector<std::string> > ();

      this->printAll ();

    } else {
      LOG_D_STREAM << "Assets path is empty" << std::endl;
    }
  }
  HelloSQLite::~HelloSQLite () {
    LOG_D_STREAM << libName_ << " ...destructed" << std::endl;
  }

  int HelloSQLite::getRowsOfTable (const std::string& tableName,
                                   std::shared_ptr<std::vector<std::string> > buffer) {
    std::vector<std::string> rows;
    try {
      SQLite::Statement query (*dbPtr_, "SELECT * FROM " + tableName);
      while (query.executeStep ()) {
        int id = query.getColumn (0);
        const char* name = query.getColumn (1);
        buffer_->push_back (std::to_string (id) + ", " + name);
        LOG_I_MSG ("Row: " + std::to_string (id) + ", " + name);
      }
    } catch (std::exception& e) {
      LOG_E_STREAM << "exception: " << e.what () << std::endl;
      return 1;
    } catch (...) {
      LOG_E_STREAM << "unknown exception" << std::endl;
      return 2;
    }
    return 0;
  }

  int HelloSQLite::printAll () {
    std::vector<std::string> tables
        = { "Departments", "Devices", "Users", "Rooms", "InventoryRecords" };
    for (const auto& table : tables) {
      try {
        this->getRowsOfTable (table, buffer_);
      } catch (std::exception& e) {
        LOG_E_STREAM << "Failed to create table " << table << ": " << e.what () << std::endl;
        return 1;
      }
    }
    return 0;
  }

  int HelloSQLite::replaceTextInTable (const std::string& tableName, const std::string& columnName,
                                       const std::string& oldText, const std::string& newText) {
    // it is database transaction

    SQLite::Transaction transaction (
        *dbPtr_); // expects a reference to a SQLite::Database object, not a pointer. By dereferencing dbPtr_ with the * operator, you're passing a reference to the actual database object rather than its pointer.
    try {
      SQLite::Statement query (*dbPtr_, "UPDATE " + tableName + " SET " + columnName + " = ? WHERE "
                                            + columnName + " = ?");
      query.bind (1, newText);
      query.bind (2, oldText);
      query.exec ();

      transaction.commit ();
    } catch (std::exception& e) {
      transaction.rollback ();
      LOG_E_STREAM << "exception: " << e.what () << std::endl;
      return 1;
    } catch (...) {
      transaction.rollback ();
      LOG_E_STREAM << "unknown exception" << std::endl;
      return 2;
    }
    return 0;
  }

} // namespace dotname