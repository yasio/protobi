#include <assert.h>

#include "iconvw.h"

std::string iconvw::u2g(const std::string& in)
{
    return iconvw::convert(in, "utf-8", "gb2312"); // gb2312 can be gbk
}

std::string iconvw::g2u(const std::string& in)
{
    return iconvw::convert(in, "gb2312", "utf-8");
}

std::string iconvw::convert(const std::string& in, const char* fromcode, const char* tocode)
{
    std::string out;
    (void)iconvw::convert(in, out, fromcode, tocode);
    return std::move(out);
}

bool iconvw::convert(const std::string& in, std::string& out, const char* fromcode, const char* tocode)
{
    char buffer[128];

    auto cd = iconv_open(tocode, fromcode);

    if (cd != nullptr)
    {
        size_t outbytesleft = sizeof(buffer);
        size_t inbytesleft = in.size();
        #ifdef _WIN32
        const char* inbuf = (in.c_str());
        #else
        char* inbuf = const_cast<char*>(in.c_str());
        #endif
        char* outbuf = buffer;
        int ret = 0;
        for (;;) {
            ret = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
            if (0 == ret)
            {
                out.insert(out.end(), buffer, buffer + sizeof(buffer)-outbytesleft);
                break;
            }
            else { // buffer insufficiently
                assert(inbytesleft > 0);
                if (outbytesleft == sizeof(buffer))
                { // convert failed, source charset not correct.
                    out.clear();
                    break;
                }
                out.insert(out.end(), buffer, buffer + sizeof(buffer) - outbytesleft);
                outbytesleft = sizeof(buffer);
                outbuf = buffer;
            }
        }
		(void)iconv_close(cd);
		return true;
    }
    return false;
}

