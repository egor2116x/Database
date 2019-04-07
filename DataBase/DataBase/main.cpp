#include "DataBase.h"
#include "IReadWriter.h"

int main()
{
    try
    {
        // create data base and table example 
        std::unique_ptr<DataBase> base(new DataBase("testBase"));
        TableInfo info = { base->GetValidTableId(), "TestTable" };
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

        std::vector<ICell *> row1;
        row1.push_back(new WholeCell(10));
        row1.push_back(new DoubleCell(5.58));
        row1.push_back(new StringCell("test string1"));
        row1.push_back(new StringCell("another test string1"));

        std::vector<ICell *> row2;
        row2.push_back(new WholeCell(5));
        row2.push_back(new DoubleCell(28.3));
        row2.push_back(new StringCell("test string2"));
        row2.push_back(new StringCell("another test string"));

        table->AddRow(row);
        table->AddRow(row1);
        table->AddRow(row2);

        base->AddTable(table.release());

        TableInfo info1 = { base->GetValidTableId(), "TestTable2" };
        std::unique_ptr<Table> table1(new Table(info1));

        table1->AddColumn("Field1", TYPE_ID::UI_TYPE_ID);
        table1->AddColumn("Field2", TYPE_ID::D_TYPE_ID);
        table1->AddColumn("Field3", TYPE_ID::AARR_TYPE_ID);
        table1->AddColumn("Field4", TYPE_ID::AARR_TYPE_ID);

        std::vector<ICell *> row3;
        row3.push_back(new WholeCell(9));
        row3.push_back(new DoubleCell(7.77));
        row3.push_back(new StringCell("test string3"));
        row3.push_back(new StringCell("another test string"));

        std::vector<ICell *> row4;
        row4.push_back(new WholeCell(10));
        row4.push_back(new DoubleCell(5.58));
        row4.push_back(new StringCell("test string4"));
        row4.push_back(new StringCell("another test string3"));

        std::vector<ICell *> row5;
        row5.push_back(new WholeCell(5));
        row5.push_back(new DoubleCell(28.3));
        row5.push_back(new StringCell("test string5"));
        row5.push_back(new StringCell("another test string4"));

        table1->AddRow(row3);
        table1->AddRow(row4);
        table1->AddRow(row5);

        base->AddTable(table1.release());
        base->Print(std::cout);

        // request example
        std::vector<RequestData> request;
        request.push_back({ "Field1", new WholeCell(5) });
        request.push_back({ "Field4", new StringCell("another test string") });

        auto & responseTable = base->Query(info.tableName, request);
        responseTable.Print(std::cout);

        const std::string key = "apriorit";

        // save and encrypt data base example
        std::unique_ptr<IReadWriterDB> writer(new SimpleReadWriter(new EncryptorXOR(key)));
        writer->Save(base.release(), utils::BuildFullPathTo(DATA_BASE_DIR));
        writer->WaitWhenCompleted();

        // load and decrypt data base example
        std::unique_ptr<DataBase> newBase(new DataBase("LoadedTestBase"));
        std::unique_ptr<IReadWriterDB> reader(new SimpleReadWriter(new EncryptorXOR(key)));
        reader->Load(newBase.get(), utils::BuildFullPathTo(DATA_BASE_DIR));
        reader->WaitWhenCompleted();
        newBase->Print(std::cout);
    }
    catch (const DBExeption & e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (const ReadWriterExeption & e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cin.get();
    return 0;
}