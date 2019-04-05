#include "DataBase.h"

DataBase::~DataBase()
{
    for (auto & table : m_Tables)
    {
        if (table != nullptr)
        {
            delete table;
            table = nullptr;
        }
    }
}

bool DataBase::AddTable(Table * table)
{
    if (table == nullptr)
    {
        return false;
    }

    if (IsTableExist(table->GetTableInfo()))
    {
        return false;
    }

    m_Tables.push_back(table);

    return true;
}

void DataBase::Print(std::ostream & out)
{
    out << "DataBase name: " << m_DataBaseName << std::endl;
    out << "-----------------------------------------------------------------------" << std::endl;
    for (const auto & table : m_Tables)
    {
        out << "Table id:" << table->GetTableInfo().id << " Table: " << table->GetTableInfo().tableName << std::endl;
        table->Print(out);
    }
}

bool DataBase::IsTableExist(const TableInfo & tableInfo)
{
    for (auto & table : m_Tables)
    {
        if (table != nullptr && 
            (table->GetTableInfo().tableName.compare(tableInfo.tableName) == 0 ||
             table->GetTableInfo().id == tableInfo.id))
        {
            return true;
        }
    }
    return false;
}
