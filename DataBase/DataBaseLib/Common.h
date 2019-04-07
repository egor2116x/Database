#pragma once
#include <string>
#include <exception>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iomanip>

enum class TYPE_ID { UI_TYPE_ID, D_TYPE_ID, AARR_TYPE_ID, UNKNOWN };
const char TYPE_SEPARATOR = ':';
const char VALUE_SEPARATOR = '|';
const char SIZE_SEPARATOR = '*';
extern const wchar_t * DATA_BASE_DIR;

namespace utils
{
    std::wstring BuildFullPathTo(const std::wstring & dirName);
    std::wstring String2Wstring(const std::string & str);
    std::string Wstring2String(const std::wstring & wstr);
}

class DBExeption : public std::exception
{
public:
    DBExeption(const std::string & message, unsigned errCode = 0) : exception(message.c_str()), m_ErrCode(errCode){}
    unsigned GetCode() const { return m_ErrCode; }
private:
    unsigned m_ErrCode;
};

class ReadWriterExeption : public std::exception
{
public:
    ReadWriterExeption(const std::string & message, unsigned errCode = 0) : exception(message.c_str()), m_ErrCode(errCode) {}
    unsigned GetCode() const { return m_ErrCode; }
private:
    unsigned m_ErrCode;
};

class FileHandleGuard
{
public:
    FileHandleGuard(HANDLE handle = nullptr);
    ~FileHandleGuard();
    HANDLE Get() { return m_Handle; }
    HANDLE Release();
    void Reset(HANDLE handle = nullptr);
private:
    HANDLE m_Handle;
};

