// nsconv standard header, contain several methods for typecast between numeric and string
#ifndef _NSCONV_H_
#define _NSCONV_H_
#include <sstream>
#include <iomanip>
#ifdef _WIN32
#    include <ObjBase.h>
#endif
#include "politedef.h"
#include <string.h>
#include <vector>
#include <tuple>
#include "unreal_string.h"

namespace purelib {

namespace nsconv {};
namespace nsc = nsconv;
    
namespace nsconv {

typedef std::ios_base& (*ios_flag)(std::ios_base&);

template<typename _Nty, typename _Elem, typename _Alloc> inline
    std::basic_string<_Elem, std::char_traits<_Elem>, _Alloc> to_xstring(const _Nty& numeral, ios_flag radix = std::dec)
{
    std::basic_stringstream<_Elem, std::char_traits<_Elem>, _Alloc> swaper;

    swaper << radix << numeral;

    return swaper.str();
}

template<typename _Nty, typename _Elem, typename _Alloc> inline
    std::basic_string<_Elem, std::char_traits<_Elem>, _Alloc>& to_xstring(const _Nty& numeral, std::basic_string<_Elem, std::char_traits<_Elem>, _Alloc>& output, ios_flag radix = std::dec)
{
    std::basic_stringstream<_Elem, std::char_traits<_Elem>, _Alloc> swaper;

    swaper << radix << numeral;
    swaper >> output;

    return output;
}

// convert numeric[char/short/int/long/long long/float/double] to string[std::string/std::wstring].
template<typename _Nty> inline
std::string to_string(const _Nty& numeral, ios_flag radix = std::dec)
{ 
    std::stringstream swaper;

    swaper << radix << numeral;

    return swaper.str();
}

template<typename _Nty> inline
std::string& to_string(const _Nty& numeral, std::string& text, ios_flag radix = std::dec)
{ 
    std::stringstream swaper;

    swaper << radix << numeral;
    swaper >> text;

    return text;
}
template<typename _Nty> inline
std::wstring to_wstring(const _Nty& numeral, ios_flag radix = std::dec)
{ 
    std::wstringstream swaper;

    swaper << radix << numeral;

    return swaper.str();
}

template<typename _Nty> inline
std::wstring& to_wstring(const _Nty& numeral, std::wstring& text, ios_flag radix = std::dec)
{ 
    std::wstringstream swaper;

    swaper << radix << numeral;

    swaper >> text;

    return text;
} 
/*@method@ to_string @method@*/


// convert string[std::string/std::wstring] to numeric[char/short/int/long/long long/float/double].
template<typename _Nty, typename _Elem> inline
_Nty to_numeric(const std::basic_string<_Elem>& text, ios_flag radix = std::dec)
{ 
    _Nty numeral = _Nty();
    std::basic_stringstream<_Elem> swaper;

    swaper << radix << text;
    swaper >> numeral;

    return numeral;
}

template<typename _Nty, typename _Elem> inline
_Nty& to_numeric(const std::basic_string<_Elem>& text, _Nty& numeral, ios_flag radix = std::dec)
{
    std::basic_stringstream<_Elem> swaper;

    swaper << radix << text;
    swaper >> numeral;

    return numeral;
} /*@method@ to_numeric @method@*/

// convert string[NTCS] to numeric[char/short/int/long/long long/float/double].
template<typename _Nty, typename _Elem> inline
_Nty to_numeric(const _Elem* text, ios_flag radix = std::dec)
{ 
    _Nty numeral = _Nty();
    std::basic_stringstream<_Elem> swaper;

    swaper << radix << text;
    swaper >> numeral;

    return numeral;
}

template<typename _Nty, typename _Elem> inline
_Nty& to_numeric(const _Elem* text, _Nty& numeral, ios_flag radix = std::dec)
{
    std::basic_stringstream<_Elem> swaper;

    swaper << radix << text;
    swaper >> numeral;

    return numeral;
} /*@method@ to_numeric @method@*/

inline 
bool _Is_visible_char(int _Char)
{
    return (_Char > 0x20 && _Char < 0x7F);
}

template<typename _Elem> inline
size_t strtrim(_Elem* _Str)
{
    if(NULL == _Str || !*_Str) {
        return 0;
    }

    _Elem* _Ptr = _Str - 1;

    while( !_Is_visible_char(*(++_Ptr)) && *_Ptr ) ;

    _Elem* _First = _Ptr;
    _Elem* _Last = _Ptr;
    if(*_Ptr) {
        while(*(++_Ptr))
        {
            if(_Is_visible_char(*_Ptr)) {
                _Last = _Ptr;
            }
        }
    }

    size_t _Count = _Last - _First + 1;
    if(_Ptr != _Str) {
        ::memmove(_Str, _First, _Count);
        _Str[_Count] = '\0';
    }

    return _Count;
}

template<typename _Elem> inline
int strtrim(_Elem* _Str, int _StrLen)
{
    if(NULL == _Str || !*_Str) {
        return 0;
    }
    _Elem* _Ptr = _Str - 1;

    while( !_Is_visible_char(*(++_Ptr)) && --_StrLen ) ;

    if(_StrLen > 0) {
        while( !_Is_visible_char(_Ptr[_StrLen - 1]) ) --_StrLen ;
        _Ptr[_StrLen] = (_Elem)'\0';
    }

    if(_Ptr != _Str) {
        ::memmove(_Str, _Ptr, _StrLen);
        _Str[_StrLen] = (_Elem)'\0';
    }

    return _StrLen;
}

template<typename _Elem>
std::basic_string<_Elem>& strtrim(std::basic_string<_Elem>& _String)
{
    _String.resize(strtrim(const_cast<_Elem*>(_String.c_str()), _String.length()));
    return _String;
}

#ifdef __cxx11
template<typename _Elem> inline
std::basic_string<_Elem>& strtrim(std::basic_string<_Elem>&& _String)
{
    return strtrim(_String);
}
#endif


/* 
* nsc::split API (override+6)
*/
template<typename _Elem, typename _Fty> inline
void split(const _Elem* s, const _Elem delim, _Fty op)
{
    const _Elem* _Start = s; // the start of every string
    const _Elem* _Ptr = s;   // source string iterator
    while( *_Ptr != '\0' )
    {
        if(delim == *_Ptr/* && _Ptr != _Start*/) 
        {
            if (_Ptr != _Start)
                op(std::basic_string<_Elem>(_Start, _Ptr));
            _Start = _Ptr + 1;
        }
        ++_Ptr;
    }
    if(_Start != _Ptr) {
        op(std::basic_string<_Elem>(_Start, _Ptr));
    }
}

template<typename _Elem, typename _Fty> inline
void split(const _Elem* s, const _Elem* delims, _Fty op)
{
    const _Elem* _Start = s; // the start of every string
    const _Elem* _Ptr = s;   // source string iterator
    size_t      _Lend = strlen(delims);
    while ((_Ptr = strstr(_Ptr, delims)) != nullptr)
    {
        if (_Ptr != _Start)
        {
            op(std::basic_string<_Elem>(_Start, _Ptr));
        }
        _Start = _Ptr + _Lend;
        _Ptr += _Lend;
    }
    if (*_Start) {
        op(std::basic_string<_Elem>(_Start));
    }
}

/* any of delims */
template<typename _Fty> inline
void split(const std::string& s, const char* delims, _Fty op)
{
    size_t start = 0;
    size_t last = s.find_first_of(delims, start);
    while (last != std::string::npos)
    {
        if (last > start)
            op(s.substr(start, last - start));
        last = s.find_first_of(delims, start = last + 1);
    }
    if (start < s.size())
    {
        op(s.substr(start));
    }
}

template<typename _Elem> inline
std::vector<std::string> split(const _Elem* s, const _Elem delim)
{
    std::vector<std::basic_string<_Elem> > output;
    nsc::split(s, delim, [&output](std::basic_string<_Elem>&& value)->void{
        output.push_back(std::move(value));
    });
    return std::move(output);
}

template<typename _Elem> inline
std::vector<std::string> split(const _Elem* s, const _Elem* delims)
{
    std::vector<std::basic_string<_Elem> > output;
    nsc::split(s, delims, [&output](std::basic_string<_Elem>&& value)->void{
        output.push_back(std::move(value));
    });
    return std::move(output);
}

inline
std::vector<std::string> split(const std::string& s, const char* delim )
{
    std::vector< std::string > output;
    nsconv::split(s, delim, [&output](std::string&& value)->void {
        output.push_back(std::move(value));
    });
    return std::move(output);
}

inline
std::string& replace(std::string& string, const std::string& replaced_key, const std::string& replacing_key) 
{
    std::string::size_type pos = 0;
    while( (pos = string.find(replaced_key, pos)) != std::string::npos )
    {
        (void)string.replace(pos, replaced_key.length(), replacing_key);
        pos += replacing_key.length();
    }
    return string;
}

inline
std::string& replace(std::string&& string, const std::string& replaced_key, const std::string& replacing_key) 
{
    return replace(string, replaced_key, replacing_key);
}

inline
std::string rsubstr(const std::string& string, size_t off)
{
    if(string.length() >= off)
    {
        return string.substr(string.length() - off);
    }
    return "";
}

template<typename _Elem> inline
void strtoupper(_Elem* source)
{
    while( *source != '\0' )
    {
        *source = toupper(*source);
        ++source;
    }
}

template<typename _Elem> inline
void strtolower(_Elem* source)
{
    while( *source != '\0' )
    {
        *source = tolower(*source);
        ++source;
    }
}

/// charstring2hexstring
/// hexstring2hexbinary
static char char2hex(const char ch)
{
    return isdigit(ch) ? (ch - '0') : (tolower(ch) - 'a' + 10);
}

static char hex2char(const char hex)
{
    return ( hex < 0xa ? (hex + '0') : (hex + 'a' - 10) );
}

static std::string charstring2hexstring(const std::string& charstring, bool delim = false)
{
    char low;
    char high;
    size_t len = charstring.length();

    std::string result;
    result.reserve(charstring.size() << 1);

    for(size_t i = 0; i < len; ++i)
    {
        auto ch = charstring[i];
        high = ch >> 4;
        low = ch & 0x0f;
        result.push_back( hex2char(high) );
        result.push_back( hex2char(low) );
        if(delim)
        {
            result.push_back(' ');
        }
    }

    return std::move(result);
}

static std::string hexstring2hexbinary(const std::string& hexstring)
{
    char low;
    char high;
    size_t len = hexstring.length();

    std::string result;
    result.reserve(hexstring.size() / 2);

    enum {
        take_low,
        take_high,
    } state;
    state = take_low;
    for(size_t i = 0; i < len; ++i)
    {
        switch(state) {
        case take_low:
            low = char2hex(hexstring[i]);
            state = take_high;
            break;
        case take_high:
            high = char2hex(hexstring[i]);
            result.push_back( ( (uint8_t)low << 4 | (uint8_t)high ) );
            state = take_low;
            break;
        }
    }

    return std::move(result);
}

template<typename _Elem> inline
    void reverse_sb(_Elem* _Str, size_t _Count)
{
    size_t start = 0;
    size_t last = _Count - 1;
    while( start != last ) 
    {
        if(_Str[start] != _Str[last])
        {
            std::swap(_Str[start], _Str[last]);
        }
        ++start, --last;
    }
}

static
std::tuple<int, int> parse2i(const std::string& strValue, const char delim)
{
    char* endptr = nullptr;
    int value1 = (int)strtol(strValue.c_str(), &endptr, 10);
    if (*endptr == delim)
    {
        return std::make_tuple(value1, (int)strtol(endptr + 1, nullptr, 10));
    }

    return std::make_tuple(value1, 0);
}

static
std::tuple<int, int, int>  parse3i(const std::string& strValue, const char delim)
{
    char* endptr = nullptr;
    int value1 = (int)strtol(strValue.c_str(), &endptr, 10);
    if (*endptr == delim)
    {
        int value2 = (int)strtol(endptr + 1, &endptr, 10);
        if (*endptr == delim)
        {
            return std::make_tuple(value1, value2, (int)strtol(endptr + 1, nullptr, 10));
        }
        return std::make_tuple(value1, value2, 0);
    }

    return std::make_tuple(value1, 0, 0);
}

static
std::tuple<float, float>  parse2f(const std::string& strValue, const char delim)
{
    char* endptr = nullptr;
    float value1 = strtof(strValue.c_str(), &endptr);
    if (*endptr == delim)
    {
        return std::make_tuple(value1, strtof(endptr + 1, nullptr));
    }

    return std::make_tuple(value1, .0f);
}

static
std::tuple<float, float, float> parse3f(const std::string& strValue, const char delim)
{
    char* endptr = nullptr;
    float value1 = strtof(strValue.c_str(), &endptr);
    if (*endptr == delim)
    {
        float value2 = strtof(endptr + 1, &endptr);
        if (*endptr == delim)
        {
            return std::make_tuple(value1, value2, strtof(endptr + 1, nullptr));
        }
        return std::make_tuple(value1, value2, .0f);
    }

    return std::make_tuple(value1, .0f, .0f);
}

static
std::tuple<int, int, int, int> parse4i(const std::string& strValue, const char delim)
{
	char* endptr = nullptr;
	auto value1 = (int)strtol(strValue.c_str(), &endptr, 10);
	if (*endptr == delim)
	{
		auto value2 = (int)strtol(endptr + 1, &endptr, 10);
		if (*endptr == delim)
		{
			auto value3 = (int)strtol(endptr + 1, &endptr, 10);
			if (*endptr == delim)
			{
				return std::make_tuple(value1, value2, value3, (int)strtol(endptr + 1, nullptr, 10));
			}
			return std::make_tuple(value1, value2, value3, 0);
		}
		return std::make_tuple(value1, value2, 0, 0);
	}

	return std::make_tuple(value1, 0, 0, 0);
}

static
std::tuple<float, float, float, float> parse4f(const std::string& strValue, const char delim)
{
    char* endptr = nullptr;
    float value1 = strtof(strValue.c_str(), &endptr);
    if (*endptr == delim)
    {
        float value2 = strtof(endptr + 1, &endptr);
        if (*endptr == delim)
        {
            float value3 = strtof(endptr + 1, &endptr);
            if (*endptr == delim)
            { 
                return std::make_tuple(value1, value2, value3, strtof(endptr + 1, nullptr));
            }
            return std::make_tuple(value1, value2, value3, .0f);
        }
        return std::make_tuple(value1, value2, .0f, .0f);
    }

    return std::make_tuple(value1, .0f, .0f, .0f);
}

#ifdef _WIN32

enum code_page {
    code_page_acp = CP_ACP,
    code_page_utf8 = CP_UTF8
};

inline std::string transcode(const wchar_t* wcb, code_page cp = code_page_acp)
{
    int buffersize = WideCharToMultiByte(cp, 0, wcb, -1, NULL, 0, NULL, NULL);
    std::string buffer(buffersize, '\0');
    WideCharToMultiByte(cp, 0, wcb, -1, &buffer.front(), buffersize, NULL, NULL);
    buffer.resize(buffersize - 1);
    return  std::move(buffer);
}

inline std::string transcode(const std::wstring& wcb, code_page cp = code_page_acp)
{
    int buffersize = WideCharToMultiByte(cp, 0, wcb.c_str(), -1, NULL, 0, NULL, NULL);
    std::string buffer(buffersize, '\0');
    WideCharToMultiByte(cp, 0, wcb.c_str(), -1, &buffer.front(), buffersize, NULL, NULL);
    buffer.resize(buffersize - 1);
    return  std::move(buffer);
}

inline std::wstring transcode(const char* mcb, code_page cp = code_page_acp)
{
    int buffersize = MultiByteToWideChar(cp, 0, mcb, -1, NULL, 0);
    std::wstring buffer(buffersize, '\0');
    MultiByteToWideChar(cp, 0, mcb, -1, &buffer.front(), buffersize);
    buffer.resize(buffersize - 1);
    return std::move(buffer);
}

inline std::wstring transcode(const std::string& mcb, code_page cp = code_page_acp)
{
    int buffersize = MultiByteToWideChar(cp, 0, mcb.c_str(), -1, NULL, 0);
    std::wstring buffer(buffersize, '\0');
    MultiByteToWideChar(cp, 0, mcb.c_str(), -1, &buffer.front(), buffersize);
    buffer.resize(buffersize - 1);
    return std::move(buffer);
}

inline std::string to_utf8(const char* ascii_text)
{
    return transcode(transcode(ascii_text).c_str(), code_page_utf8);
}

inline std::string to_ascii(const char* utf8_text)
{
    return transcode(transcode(utf8_text, code_page_utf8).c_str());
}

/* utils GUID
**
*/
#include "xxbswap.h"

#define GUID_BUF_SIZE ( sizeof(_GUID) * 2 + sizeof(void*) )

inline void create_guid(LPTSTR outs)
{
    _GUID guid;
    CoCreateGuid(&guid);

    wsprintf(outs, TEXT("%08X-%04X-%04X-%04X-%04X%08X"), 
        guid.Data1,
        guid.Data2,
        guid.Data3,
        __bswap16(*(reinterpret_cast<unsigned __int16*>(guid.Data4))),
        __bswap16(*(reinterpret_cast<unsigned __int16*>(guid.Data4 + 2))),
        __bswap32(*(reinterpret_cast<unsigned __int32*>(guid.Data4 + 4)))
        );
}

inline void create_guid_v2(LPTSTR outs)
{
    _GUID guid;
    CoCreateGuid(&guid);

    wsprintf(outs, TEXT("%08X%04X%04X%016I64X"), 
        guid.Data1,
        guid.Data2,
        guid.Data3,
        __bswap64(*(reinterpret_cast<unsigned __int64*>(guid.Data4)))
        );
}

template<typename _Elem> inline
std::basic_string<_Elem> create_guid(void)
{
    std::basic_stringstream<_Elem> swaper;

    _GUID g;
    CoCreateGuid(&g);

    swaper << std::hex 
        << std::setw(8) << std::setfill(_Elem('0')) << g.Data1
        << _Elem('-')
        << std::setw(4) << std::setfill(_Elem('0')) << g.Data2
        << _Elem('-') 
        << std::setw(4) << std::setfill(_Elem('0')) << g.Data3
        << _Elem('-')
        << std::setw(8) << std::setfill(_Elem('0')) << *(reinterpret_cast<unsigned __int32*>(g.Data4))
        << std::setw(4) << std::setfill(_Elem('0')) << *(reinterpret_cast<unsigned __int16*>(g.Data4)+2);

    return swaper.str();
}

template<typename _Elem> inline
void create_guid(std::basic_string<_Elem>& outs)
{
    std::basic_stringstream<_Elem> swaper;

    _GUID g;
    CoCreateGuid(&g);

    swaper << std::hex 
        << std::setw(8) << std::setfill(_Elem('0')) << g.Data1
        << _Elem('-')
        << std::setw(4) << std::setfill(_Elem('0')) << g.Data2
        << _Elem('-') 
        << std::setw(4) << std::setfill(_Elem('0')) << g.Data3
        << _Elem('-')
        << std::setw(8) << std::setfill(_Elem('0')) << *(reinterpret_cast<unsigned __int32*>(g.Data4))
        << std::setw(4) << std::setfill(_Elem('0')) << *(reinterpret_cast<unsigned __int16*>(g.Data4)+2);

    outs = swaper.str();
}
#endif


}; // namespace: purelib::nsconv



}; // namespace: purelib

#endif /* _NSCONV_ */
/*
* Copyright (c) 2012-2014 by xseekerj  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
**/

