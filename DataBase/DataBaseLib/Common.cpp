#include "Common.h"

const wchar_t * DATA_BASE_DIR = L"data";

FileHandleGuard::FileHandleGuard(HANDLE handle) : m_Handle(handle)
{
}

FileHandleGuard::~FileHandleGuard()
{
    if (m_Handle != nullptr)
    {
        ::CloseHandle(m_Handle);
    }
}

HANDLE FileHandleGuard::Release()
{
    HANDLE h = m_Handle;
    m_Handle = nullptr;
    return h;
}

void FileHandleGuard::Reset(HANDLE handle)
{
    if (m_Handle != nullptr)
    {
        ::CloseHandle(m_Handle);
    }
    m_Handle = handle;
}

std::wstring utils::BuildFullPathTo(const std::wstring & dirName)
{
    DWORD dwResult = NO_ERROR;
    std::vector<WCHAR> pathBuffer;
    pathBuffer.resize(MAX_PATH);
    std::wstring fullPath;

    for (;;)
    {
        dwResult = GetModuleFileName(NULL, &pathBuffer.at(0), static_cast<DWORD>(pathBuffer.size()));
        if (dwResult == 0)
        {
            return std::wstring();
        }
        else if (dwResult == pathBuffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            pathBuffer.resize(pathBuffer.size() * 2);
            continue;
        }
        break;
    }

    fullPath.assign(&pathBuffer.at(0), dwResult);
    std::wstring::size_type pos = fullPath.find_last_of(L'\\');
    if (pos == std::wstring::npos)
    {
        return std::wstring();
    }

    fullPath = fullPath.substr(0, pos);

    fullPath += L'\\';
    fullPath += dirName;

    return fullPath;
}

std::wstring utils::String2Wstring(const std::string & str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(str);
    return wstr;
}

std::string utils::Wstring2String(const std::wstring & wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string str = converter.to_bytes(wstr);
    return str;
}
