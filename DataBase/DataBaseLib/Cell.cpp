#include "Cell.h"

void WholeCell::SetData(void * data)
{
    size_t * tmp = reinterpret_cast<size_t *>(data);
    m_Data = *tmp;
}

const void * WholeCell::GetData() const
{
    return reinterpret_cast<const void *>(&m_Data);
}

TYPE_ID WholeCell::GetType() const
{
    return TYPE_ID::UI_TYPE_ID;
}

size_t WholeCell::GetSize() const
{
    return sizeof(size_t);
}

void DoubleCell::SetData(void * data)
{
    double * tmp = reinterpret_cast<double *>(data);
    m_Data = *tmp;
}

const void * DoubleCell::GetData() const
{
    return reinterpret_cast<const void *>(&m_Data);
}

TYPE_ID DoubleCell::GetType() const
{
    return TYPE_ID::D_TYPE_ID;
}

size_t DoubleCell::GetSize() const
{
    return sizeof(double);
}

void StringCell::SetData(void * data)
{
    char * tmp = reinterpret_cast<char *>(data);
    m_Data = tmp;
}

const void * StringCell::GetData() const
{
    return reinterpret_cast<const void *>(m_Data.c_str());
}

TYPE_ID StringCell::GetType() const
{
    return TYPE_ID::AARR_TYPE_ID;
}

size_t StringCell::GetSize() const
{
    return m_Data.size();
}
