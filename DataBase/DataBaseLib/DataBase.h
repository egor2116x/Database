#pragma once
#include "Table.h"

class IQueryBase
{
public:
    virtual Table & Query(const std::string & tableName, const std::vector<RequestData> & requestString) = 0;
    virtual const std::vector<Table *> & QueryAll() const = 0;
};

class DataBase : public IQueryBase
{
public:
    DataBase(const std::string & dataBaseName) : m_DataBaseName(dataBaseName), tableId(0){};
    ~DataBase();
    bool AddTable(Table * table);
    bool DropTable(const std::string & tableName);
    size_t GetValidTableId() { return ++tableId; }
    void Print(std::ostream & out);
    Table & Query(const std::string & tableName, const std::vector<RequestData>& requestString) override;
    const std::vector<Table *> & QueryAll() const override { return m_Tables; }
private:
    bool IsTableExist(const TableInfo & tableInfo);
private:
    std::vector<Table *> m_Tables;
    std::string m_DataBaseName;
    std::mutex m_Mutex;
    size_t tableId;
};

