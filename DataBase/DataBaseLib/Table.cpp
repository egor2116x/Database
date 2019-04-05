#include "Table.h"

Table::Table(const TableInfo & tableInfo)
{
    m_TableInfo.id = tableInfo.id;
    m_TableInfo.tableName = tableInfo.tableName;

    AddColumn("ID", TYPE_ID::UI_TYPE_ID);
}

Table::~Table()
{
    for (auto & row : m_Data)
    {
        for (auto cell = row.begin(); cell != row.end(); cell++)
        {
            delete (*cell);
        }
    }
}

bool Table::AddColumn(const std::string & columnName, TYPE_ID dataType)
{
    if (dataType == TYPE_ID::UNKNOWN)
    {
        return false;
    }

    for (const auto & column : m_TableColoumNames)
    {
        if (column.first.compare(columnName) == 0)
        {
            return false;
        }
    }

    m_TableColoumNames.push_back(std::make_pair(columnName, dataType));
    
    for (auto & row : m_Data)
    {
        switch (dataType)
        {
        case TYPE_ID::AARR_TYPE_ID: 
            row.push_back(new StringCell);
            break;
        case TYPE_ID::D_TYPE_ID: 
            row.push_back(new DoubleCell);
            break;
        case TYPE_ID::UI_TYPE_ID: 
            row.push_back(new WholeCell);
            break;
        }
    }

    return true;
}

bool Table::DeleteColumn(const std::string & columnName)
{
    if (columnName.compare("ID") == 0)
    {
        return false;
    }

    size_t idx = -1;
    for (auto column = m_TableColoumNames.begin(); column != m_TableColoumNames.end(); column++)
    {
        ++idx;
        if (column->first.compare(columnName) == 0)
        {
            m_TableColoumNames.erase(column);
            break;
        }
    }

    if (idx == -1)
    {
        return false;
    }

    size_t i = -1;
    for (auto & row : m_Data)
    {
        if (row.size() < idx)
        {
            throw DBExeption("Incorrect data base structure");
        }
        for (auto cell = row.begin(); cell != row.end(); cell++)
        {
            ++i;
            if (i == idx)
            {
                delete (*cell);
                row.erase(cell);
            }
        }
    }

    return true;
}

bool Table::AddRow(const std::vector<ICell *>& row)
{
    AddId(const_cast<std::vector<ICell *> &>(row));

    if (!IsRowStructureCorrect(const_cast<const std::vector<ICell *> &>(row)))
    {
        return false;
    }

    m_Data.push_back(row);

    return true;
}

void Table::Print(std::ostream & out)
{
    for (const auto & field : m_TableColoumNames)
    {
        out << field.first.c_str() << ":" << static_cast<size_t>(field.second) << "\t";
    }

    out << std::endl;

    for (const auto & row : m_Data)
    {
        for (const auto & cell : row)
        {
            switch (cell->GetType())
            {
            case TYPE_ID::UI_TYPE_ID:
                out << *(reinterpret_cast<const size_t *>(cell->GetData())) << "\t\t";
                break;
            case TYPE_ID::D_TYPE_ID:
                out << *(reinterpret_cast<const double *>(cell->GetData())) << "\t\t";
                break;
            case TYPE_ID::AARR_TYPE_ID:
                out << reinterpret_cast<const char *>(cell->GetData()) << "\t\t";
                break;
            case TYPE_ID::UNKNOWN:
                out << "????????" << "\t\t";
                break;
            }
            
        }
        out << std::endl;
    }
}

bool Table::IsRowStructureCorrect(const std::vector<ICell *>& row) const
{
    if (row.size() != m_TableColoumNames.size())
    {
        return false;
    }

    for (size_t i = 0; i < row.size(); i++)
    {
        if (row[i]->GetType() != m_TableColoumNames[i].second)
        {
            return false;
        }
    }

    return true;
}

bool Table::IsTableStructureCorrect() const
{
    return false;
}

void Table::AddId(std::vector<ICell*>& row)
{
    size_t size = m_Data.size();
    std::unique_ptr<ICell> data(new WholeCell(++size));
    row.insert(row.begin(), data.release());
}

bool Table::CompareCell(ICell * left, ICell * right)
{
    if (left == nullptr || right == nullptr)
    {
        false;
    }

    if (left->GetType() != right->GetType())
    {
        return false;
    }

    switch (left->GetType())
    {
    case TYPE_ID::UI_TYPE_ID:
        size_t dbValue = *(reinterpret_cast<const size_t *>(left->GetData()));
        size_t RequestValue = *(reinterpret_cast<const size_t *>(right->GetData()));
        if (dbValue == RequestValue)
        {
            return true;
        }
        break;
    case TYPE_ID::D_TYPE_ID:
        double dbValue = *(reinterpret_cast<const double *>(left->GetData()));
        double RequestValue = *(reinterpret_cast<const double *>(right->GetData()));
        if (dbValue == RequestValue)
        {
            return true;
        }
        break;
    case TYPE_ID::AARR_TYPE_ID:
        const char * dbValue = (reinterpret_cast<const char *>(left->GetData()));
        const char * RequestValue = (reinterpret_cast<const char *>(right->GetData()));
        if (dbValue == RequestValue)
        {
            return true;
        }
        break;
    }

    return false;
}

size_t Table::FindCellInRow(const std::vector<ICell*>& row, const RequestData & request)
{
    size_t idx = -1;
    for (const auto & cellHeader : m_TableColoumNames)
    {
        ++idx;
        if (cellHeader.first.compare(request.fieldName) == 0 &&
            cellHeader.second == request.cell->GetType())
        {
            break;
        }
    }

    return idx;
}

std::vector<ICell*>& Table::CopyRow(const std::vector<ICell*>& row)
{
    // TODO: insert return statement here
}

Table & Table::Query(const std::vector<RequestData>& requestString)
{
    TableInfo tableInfo(99999, "Tmp");
    Table table(tableInfo);

    if (requestString.size() > m_TableColoumNames.size())
    {
        throw DBExeption("Incorrect request");
    }

    std::vector<ICell *> newRow;
    bool isRow = true;
    for (const auto & row : m_Data)
    {
        for (const auto & request : requestString)
        {
            size_t idx = FindCellInRow(row, request);
            if (idx == -1)
            {
                isRow = false;
                break;
            }

            if (CompareCell(row[idx], request.cell))
            {
                is
            }
        }

        if (isRow)
        {
            auto & newRow = CopyRow(row);
            table.AddRow(newRow);
            isRow = false;
        }
    }

    return table;
}
