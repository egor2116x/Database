#pragma once
#include "Common.h"

class IReadWriter
{
public:
    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual ~IReadWriter();
};

class ReadWriterSimple : public IReadWriter
{
public:
    ReadWriterSimple(std::string dir) {}
    virtual bool Load() override;
    virtual bool Save() override;
private:
    std::string m_DirPath;
};