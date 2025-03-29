#ifndef __CSQLITE_H__
#define __CSQLITE_H__

// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

// Public API

namespace library
{
  class CSQLite
  {
  public:
    CSQLite (const std::string &assetsPath);
    ~CSQLite ();
    static CSQLite &getInstance (const std::string &assetsPath = "");
    const std::string getAssetsPath () const;

    int getRowsOfTable (const std::string &tableName,
                        std::shared_ptr<std::vector<std::string> > buffer);

    int printAll ();

    int replaceTextInTable (const std::string &tableName,
                            const std::string &columnName,
                            const std::string &oldText,
                            const std::string &newText);

  private:
    std::shared_ptr<std::vector<std::string> > buffer;
    SQLite::Database db;
    std::string m_assetsPath;
  };

} // namespace library

#endif // __CSQLITE_H__
