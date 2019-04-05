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

const size_t STR_TYPE_SIZE = 1024 * 1024 * 10; // 10 Mb
enum class TYPE_ID { UI_TYPE_ID, D_TYPE_ID, AARR_TYPE_ID, UNKNOWN };


class DBExeption : public std::exception
{
public:
    DBExeption(const std::string & message, unsigned errCode = 0) : exception(message.c_str()), m_ErrCode(errCode){}
    unsigned GetCode() const { return m_ErrCode; }
private:
    unsigned m_ErrCode;
};

