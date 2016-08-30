#ifndef BASE64_H
#define BASE64_H
#include <vector>
#include <string>
using std::string;
using std::vector;

namespace Base64
{
std::string base64_encode(const std::string &);
std::string base64_decode(const std::string &);

const std::string &SALT1 = "LM::TB::BB";
const std::string &SALT2 = "__:/__77";
const std::string &SALT3 = "line=wowC++";

std::string EncryptB64 (std::string s)
    {
        s = SALT1 + s + SALT2 + SALT3;
        s = base64_encode (s); 
        s.insert (7, SALT3);
        s += SALT1;
        s = base64_encode (s);
        s = SALT2 + SALT3 + s + SALT1;
        s = base64_encode (s);
        s.insert (1, "L"); 
        s.insert (7, "M"); 
        return s;
    }
std::string DecryptB64 (std::string s)
    {
        s = s.erase (7, 1); 
        s = s.erase (1, 1);
        s = base64_decode (s);
        s = s.substr (SALT2.length() + SALT3.length());
        s = s.substr (0, s.length() - SALT1.length());
        s = base64_decode (s);
        s = s.substr (0, s.length() - SALT1.length());
        s = s.erase (7, SALT3.length());
        s = base64_decode (s);
        s = s.substr (SALT1.length());
        s = s.substr (0, s.length() - SALT2.length() - SALT3.length());
        return s;
    }

const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode (const std::string &s)
    {
        std::string ret;
        int val = 0;
        int bits = -6;
        const unsigned int b63 = 0x3F; 
        for (const auto &c : s)
            {
                val = (val << 8) + c;
                bits += 8;
                while (bits >= 0)
                    {
                        ret.push_back(BASE64_CODES[(val >> bits) & b63]);
                        bits -= 6;
                    }
            }
        if (bits > -6) 
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

        while (ret.size() % 4)
            ret.push_back('=');

        return ret;
    }

std::string base64_decode(const std::string &s)
    {
        std::string ret;
        std::vector<int> vec(256, -1);
        for (int i = 0; i < 64; i++)
            vec [BASE64_CODES[i]] = i;
        int val = 0, bits = -8; 
        for (const auto &c : s)
            {
                if (vec[c] == -1) break;
                val = (val << 6) + vec[c]; 
                bits += 6; 
                           
                if (bits >= 0)
                    {

                        ret.push_back(char((val >> bits) & 0xFF));
                        bits -= 8; 
                    }
            }

        return ret;
    }
}
#endif 
