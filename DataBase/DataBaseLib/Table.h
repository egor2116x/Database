#pragma once
#include "Common.h"
#include "Cell.h"

struct TableInfo
{
    TableInfo(size_t _id = 0, const std::string & _tableName = std::string()) : id(_id), tableName(_tableName) {}
    std::string tableName;
    size_t id;
};

struct RequestData
{
    std::string fieldName;
    ICell *     cell;
};

class Table;
class IQuery
{
public:
    virtual Table & Query(const std::vector<RequestData> & requestString) = 0;
};

class Table : public IQuery
{
public:
    Table(const TableInfo & tableInfo);
    ~Table();
    bool AddColumn(const std::string & columnName, TYPE_ID dataType);
    bool DeleteColumn(const std::string & columnName);
    bool AddRow(const std::vector<ICell *> & row);
    Table & Query(const std::vector<RequestData> & requestString) override;

    void Print(std::ostream & out);
    const TableInfo & GetTableInfo() const { return m_TableInfo; }
private:
    bool IsRowStructureCorrect(const std::vector<ICell *> & row) const;
    bool IsTableStructureCorrect() const;
    void AddId(std::vector<ICell *> & row);
    bool CompareCell(ICell * left, ICell * right);
    size_t FindCellInRow(const std::vector<ICell *> & row, const RequestData & request);
    std::vector<ICell *> & CopyRow(const std::vector<ICell *> & row);
private:
    std::vector<std::pair<std::string, TYPE_ID>> m_TableColoumNames;
    std::vector<std::vector<ICell *>> m_Data;
    TableInfo m_TableInfo;

    
};

