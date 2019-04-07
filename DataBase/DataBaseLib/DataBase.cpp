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
    std::lock_guard<std::mutex> guard(m_Mutex);
    m_Tables.push_back(table);

    return true;
}

bool DataBase::DropTable(const std::string & tableName)
{
    for (auto table = m_Tables.begin(); table != m_Tables.end(); table++)
    {
        if ((*table)->GetTableInfo().tableName.compare(tableName) == 0)
        {
            std::lock_guard<std::mutex> guard(m_Mutex);
            m_Tables.erase(table);
            return true;
        }
    }
    return false;
}

void DataBase::Print(std::ostream & out)
{
    out << "DataBase name: " << m_DataBaseName << std::endl;
    out << "-----------------------------------------------------------------------" << std::endl;
    std::lock_guard<std::mutex> guard(m_Mutex);
    for (const auto & table : m_Tables)
    {
        table->Print(out);
    }
}

Table & DataBase::Query(const std::string & tableName, const std::vector<RequestData> & requestString)
{
    for (const auto & table : m_Tables)
    {
        if (table->GetTableInfo().tableName.compare(tableName) == 0)
        {
            return table->Query(requestString);
        }
    }

    return Table(TableInfo(99999, "tmp"));
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

