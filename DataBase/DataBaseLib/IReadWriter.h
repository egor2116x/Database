#pragma once
#include "Common.h"
#include "IMultiThread.h"
#include "DataBase.h"
#include "IEncryptor.h"

class IReadWriterDB : public IMultiThread
{
public:
    bool Load(DataBase * base, const std::wstring & dir);
    bool Save(DataBase * base, const std::wstring & dir);
    virtual ~IReadWriterDB() {}
protected:
    virtual bool LoadImpl(const std::wstring & filePath) = 0;
    virtual bool SaveImpl(const std::wstring & dir) = 0;    
    virtual void StartThread(void * context) = 0;
private:
    bool BrowseFileSystem(const std::wstring & dir);
    bool IsDirectory(DWORD fileAttribute) const { return (fileAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0; }
    void BuildNewFolderPath(std::wstring & folderPath, const std::wstring & newFolderName);
protected:
    DataBase * m_Base;
};

class SimpleReadWriter : public IReadWriterDB
{
public:
    SimpleReadWriter(IEncryptor * encryptor);
private:
    bool LoadImpl(const std::wstring & filePath) override;
    bool SaveImpl(const std::wstring & dir) override;
    void LoadThreadImpl(const std::wstring & filePath);
    void SaveThreadImpl(const std::wstring & dir);
    void StartThread(void * context) override;
    enum class OperationType { SAVE, LOAD };
    struct Operation
    {
        Operation(OperationType op, const std::wstring & wstr) : type(op), path(wstr) {}
        OperationType type;
        std::wstring  path;
    };
    std::unique_ptr<IEncryptor> m_Encryptor;
};