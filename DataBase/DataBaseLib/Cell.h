#pragma once
#include "Common.h"

class ICell
{
public:
    virtual ~ICell(){}
    virtual void SetData(void * data) = 0;
    virtual const void * GetData() const = 0;
    virtual TYPE_ID GetType() const = 0;
};

class WholeCell : public ICell
{
public:
    WholeCell(size_t data = 0) : m_Data(data) {}
    virtual void SetData(void * data) override;
    virtual const void * GetData() const override;
    virtual TYPE_ID GetType() const override;
private:
    size_t m_Data;
};

class DoubleCell : public ICell
{
public:
    DoubleCell(double data = 0) : m_Data(data) {}
    virtual void SetData(void * data) override;
    virtual const void * GetData() const override;
    virtual TYPE_ID GetType() const override;
private:
    double m_Data;
};

class StringCell : public ICell
{
public:
    StringCell(const std::string & data = std::string()) : m_Data(data) {}
    virtual void SetData(void * data) override;
    virtual const void * GetData() const override;
    virtual TYPE_ID GetType() const override;
private:
    std::string m_Data;
};


