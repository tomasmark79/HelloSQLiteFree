// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#ifndef __HELLOSQLITE_HPP
#define __HELLOSQLITE_HPP

#include <HelloSQLite/version.h>
#include <filesystem>
#include <string>
#include <vector>
#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

// Public API

namespace dotname {

  class HelloSQLite {

    const std::string libName_ = std::string ("HelloSQLite v.") + HELLOSQLITE_VERSION;
    std::filesystem::path assetsPath_;
    std::shared_ptr<std::vector<std::string> > buffer_;
    std::unique_ptr<SQLite::Database> dbPtr_;

  public:
    HelloSQLite ();
    HelloSQLite (const std::filesystem::path& assetsPath);
    ~HelloSQLite ();

    const std::filesystem::path getAssetsPath () const {
      return assetsPath_;
    }
    void setAssetsPath (const std::filesystem::path& assetsPath) {
      assetsPath_ = assetsPath;
    }

    int getRowsOfTable (const std::string& tableName,
                        std::shared_ptr<std::vector<std::string> > buffer);

    int printAll ();

    int replaceTextInTable (const std::string& tableName, const std::string& columnName,
                            const std::string& oldText, const std::string& newText);
  };

} // namespace dotname

#endif // __HELLOSQLITE_HPP