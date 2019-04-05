#pragma once
#include "Table.h"

class DataBase
{
public:
    DataBase(const std::string & dataBaseName) : m_DataBaseName(dataBaseName) {};
    ~DataBase();
    bool AddTable(Table * table);
    size_t GetValidTableId() const { size_t id = m_Tables.size(); return ++id; }
    void Print(std::ostream & out);
private:
    bool IsTableExist(const TableInfo & tableInfo);
private:
    std::vector<Table *> m_Tables;
    std::string m_DataBaseName;
};

