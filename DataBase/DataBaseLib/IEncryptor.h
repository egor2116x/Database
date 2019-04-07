#pragma once
#include "Common.h"

class IEncryptor
{
public:
    IEncryptor(const std::string & key) : m_Key(key) {}
    virtual bool Encrypt(const std::string & str, std::vector<byte> & buff) = 0;
    virtual bool Decrypt(const std::vector<byte> & data, std::string & buff) = 0;
    virtual ~IEncryptor() {};
protected:
    std::string m_Key;
};

class EncryptorXOR : public IEncryptor
{
public:
    EncryptorXOR(const std::string & key) : IEncryptor(key) {}
    bool Encrypt(const std::string & str, std::vector<byte> & buff) override;
    bool Decrypt(const std::vector<byte> & data, std::string & buff) override;
};

