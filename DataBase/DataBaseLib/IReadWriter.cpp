#include "IReadWriter.h"

bool IReadWriterDB::Load(DataBase * base, const std::wstring & dir)
{
    m_Base = base;
    return BrowseFileSystem(dir);
}

bool IReadWriterDB::Save(DataBase * base, const std::wstring & dir)
{
    if (base == nullptr)
    {
        return false;
    }
    m_Base = base;
    return SaveImpl(dir);
}

bool IReadWriterDB::BrowseFileSystem(const std::wstring & dir)
{
    std::wstring folderPath = dir;
    if (folderPath.empty())
    {
        return false;
    }

    if (folderPath.rfind(L"\\*") == std::wstring::npos)
    {
        folderPath += L"\\*";
    }

    WIN32_FIND_DATA wfd;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    hFile = ::FindFirstFile(folderPath.c_str(), &wfd);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    do
    {
        if (std::wstring(wfd.cFileName)[0] == L'.' ||
            (std::wstring(wfd.cFileName).find(L"..") != std::wstring::npos))
        {
            continue;
        }

        if (IsDirectory(wfd.dwFileAttributes))
        {
            BuildNewFolderPath(folderPath, std::wstring(wfd.cFileName));
            BrowseFileSystem(folderPath);
        }
        else
        {
            std::wstring::size_type pos = folderPath.rfind(L"\\*");
            if (pos == std::wstring::npos)
            {
                return false;
            }

            std::wstring filePath = folderPath.substr(0, folderPath.size() - (folderPath.size() - pos) + 1);
            filePath += wfd.cFileName;
            if (!LoadImpl(filePath))
            {
                return false;
            }
        }

    } while (FindNextFile(hFile, &wfd) != 0);

    FindClose(hFile);

    return true;
}

void IReadWriterDB::BuildNewFolderPath(std::wstring & folderPath, const std::wstring & newFolderName)
{
    folderPath = folderPath.substr(0, folderPath.size() - 1);
    folderPath += newFolderName;
    folderPath += L"\\*";
}

SimpleReadWriter::SimpleReadWriter(IEncryptor * encryptor)
{
    m_Encryptor.reset(encryptor);
}

bool SimpleReadWriter::LoadImpl(const std::wstring & filePath)
{
    StartThread(reinterpret_cast<void *>(new Operation(OperationType::LOAD, filePath)));
    return true;
}

bool SimpleReadWriter::SaveImpl(const std::wstring & dir)
{
    StartThread(reinterpret_cast<void *>(new Operation(OperationType::SAVE, dir )));
    return true;
}

void SimpleReadWriter::LoadThreadImpl(const std::wstring & filePath)
{
    std::ifstream in(filePath);
    if (!in.is_open())
    {
        throw ReadWriterExeption(std::string("Cannot open file ") + utils::Wstring2String(filePath));
    }

    std::wstring::size_type pos = filePath.rfind(L"\\");
    std::wstring tableName;
    if (pos != std::wstring::npos)
    {
        tableName = filePath.substr(pos + 1, filePath.size() - pos);
    }
    
    std::unique_ptr<Table> table(new Table(TableInfo(m_Base->GetValidTableId(), utils::Wstring2String(tableName))));
    std::string buff;
    bool isFirst = true;
    while (std::getline(in, buff))
    {
        std::string::size_type pos = buff.find(VALUE_SEPARATOR);
        if (pos == std::string::npos)
        {
            ReadWriterExeption("Incorrect file type");
        }

        std::string str;
        if (isFirst) // table header
        {
            isFirst = false;
            while ((pos = buff.find(VALUE_SEPARATOR)) != std::string::npos)
            {
                str = buff.substr(0, pos);
                std::string::size_type posVal = str.find(TYPE_SEPARATOR);
                if (posVal == std::string::npos)
                {
                    ReadWriterExeption("Incorrect file type");
                }
                std::string name = str.substr(0, posVal);
                if (name.compare("ID") != 0)
                {
                    size_t size = std::stoi(str.substr(posVal + 1, str.size() - posVal));
                    table->AddColumn(name, static_cast<TYPE_ID>(size));
                }
                
                buff = buff.substr(pos + 1, buff.size() - pos);
            }
            continue;
        }

        // parse values
        size_t idx = -1;
        std::vector<ICell *> row;
        while ((pos = buff.find(VALUE_SEPARATOR)) != std::string::npos)
        {
            ++idx;
            if (idx == 0)
            {
                buff = buff.substr(pos + 1, buff.size() - pos);
                continue;
            }
            if (idx > table->GetTableStructure().size())
            {
                break;
            }
            switch (table->GetTableStructure()[idx].second)
            {
            case TYPE_ID::UI_TYPE_ID:
            {
                std::string result;
                std::string tmp = buff.substr(0, pos);
                std::vector<byte> value(tmp.begin(), tmp.end());
                m_Encryptor->Decrypt(value, result);
                row.push_back(new WholeCell(std::stoi(result)));
            }
                break;
            case TYPE_ID::D_TYPE_ID: 
            {
                std::string result;
                std::string tmp = buff.substr(0, pos);
                std::vector<byte> value(tmp.begin(), tmp.end());
                m_Encryptor->Decrypt(value, result);
                row.push_back(new DoubleCell(std::stof(result)));
            }
                break;
            case TYPE_ID::AARR_TYPE_ID: 
            {
                std::string result;
                std::string tmp = buff.substr(0, pos);
                std::vector<byte> value(tmp.begin(), tmp.end());
                m_Encryptor->Decrypt(value, result);
                row.push_back(new StringCell(result));
            }
                break;
            }

            buff = buff.substr(pos + 1, buff.size() - pos);
        }

        table->AddRow(row);
    }
    
    m_Base->AddTable(table.release());
}

void SimpleReadWriter::SaveThreadImpl(const std::wstring & dir)
{
    for (const auto table : m_Base->QueryAll())
    {
        std::wstring fileName = dir + L"\\" + utils::String2Wstring(table->GetTableInfo().tableName);
        std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
        if (!out.is_open())
        {
            throw ReadWriterExeption("Cannot open file " + table->GetTableInfo().tableName);
        }

        const auto & tableStructure = table->GetTableStructure();
        for (const auto & fieldHeader : tableStructure)
        {
            out << fieldHeader.first.c_str() << TYPE_SEPARATOR 
                 << static_cast<size_t>(fieldHeader.second) << VALUE_SEPARATOR;
        }
        out << std::endl;

        for (const auto & row : table->QueryAll())
        {
            for (const auto & cell : row)
            {
                switch (cell->GetType())
                {
                case TYPE_ID::UI_TYPE_ID:
                {
                    size_t value = *(reinterpret_cast<const size_t *>(cell->GetData()));
                    std::vector<byte> buff;
                    m_Encryptor->Encrypt(std::to_string(value), buff);
                    out.write((char*)&buff[0], buff.size());
                }
                break;
                case TYPE_ID::D_TYPE_ID:
                {
                    double value = *(reinterpret_cast<const double *>(cell->GetData()));
                    std::vector<byte> buff;
                    m_Encryptor->Encrypt(std::to_string(value), buff);
                    out.write((char*)&buff[0], buff.size());
                }
                break;
                case TYPE_ID::AARR_TYPE_ID:
                {
                    std::string value = reinterpret_cast<const char *>(cell->GetData());
                    std::vector<byte> buff;
                    m_Encryptor->Encrypt(value, buff);
                    out.write((char*)&buff[0], buff.size());
                }
                break;
                case TYPE_ID::UNKNOWN:
                    throw DBExeption("Unknown data type");
                }
                out << VALUE_SEPARATOR;
            }
            out << std::endl;
        }
    }
}

void SimpleReadWriter::StartThread(void * context)
{
    std::unique_ptr<Operation> operation(reinterpret_cast<Operation *>(context));
    if (operation->type == OperationType::SAVE)
    {
        std::thread t(&SimpleReadWriter::SaveThreadImpl, this, operation->path);
        m_threads.emplace_back(std::move(t));
    }
    else if (operation->type == OperationType::LOAD)
    {
        std::thread t(&SimpleReadWriter::LoadThreadImpl, this, operation->path);
        m_threads.emplace_back(std::move(t));
    }
    else
    {
        throw ReadWriterExeption("Unknown file operation type");
    }   
}


