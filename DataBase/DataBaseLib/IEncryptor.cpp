#include "IEncryptor.h"

bool EncryptorXOR::Encrypt(const std::string & str, std::vector<byte> & buff)
{
    std::vector<byte> data(str.begin(), str.end());
    std::vector<byte> akey(m_Key.begin(), m_Key.end());
    buff.resize(str.size());

    for (int i = 0; i < data.size(); i++) {
        buff[i] = (byte)(data[i] ^ akey[i % akey.size()]);
    }

    return true;
}

bool EncryptorXOR::Decrypt(const std::vector<byte>& data, std::string & buff)
{
    std::vector<byte> buf(data.size());
    std::vector<byte> akey(m_Key.begin(), m_Key.end());

    for (int i = 0; i < data.size(); i++) {
        buf[i] = (byte)(data[i] ^ akey[i % akey.size()]);
    }

    buff.assign(buf.begin(), buf.end());
    return true;
}
