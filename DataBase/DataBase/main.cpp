#include "DataBase.h"

int main()
{
    try
    {
        DataBase base("testBase");
        TableInfo info = { base.GetValidTableId(), "TestTable" };
        std::unique_ptr<Table> table(new Table(info));
        
        table->AddColumn("Field1", TYPE_ID::UI_TYPE_ID);
        table->AddColumn("Field2", TYPE_ID::D_TYPE_ID);
        table->AddColumn("Field3", TYPE_ID::AARR_TYPE_ID);
        table->AddColumn("Field4", TYPE_ID::AARR_TYPE_ID);

        std::vector<ICell *> row;
        row.push_back(new WholeCell(5));
        row.push_back(new DoubleCell(3.14));
        row.push_back(new StringCell("test string"));
        row.push_back(new StringCell("another test string"));

        table->AddRow(row);
        base.AddTable(table.release());
        base.Print(std::cout);
    }
    catch (const DBExeption & e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cin.get();
    return 0;
}