// nsconv standard header, contain several methods for typecast between numeric and string
#ifndef _NSCONV_H_
#define _NSCONV_H_
#include <sstream>
#include <iomanip>
#ifdef _WIN32
#    include <ObjBase.h>
#endif
#include "simpleppdef.h"
#include "simple_ptr.h"
#include <string.h>
#include "unreal_string.h"

namespace simplepp_1_13_201301 {

namespace nsc {


#define _str_para(_Tys) _Tys, std::char_traits<_Tys>, std::allocator<_Tys>
#define _str_t(_Tys) std::basic_string< _str_para(_Tys) >
#define _ss_t(_Tys) std::basic_stringstream< _str_para(_Tys) >
typedef std::ios_base& (*ios_flag)(std::ios_base&);

// convert numeric[char/short/int/long/long long/float/double] to string[std::string/std::wstring].
template<typename _Tys, typename _Tyn> inline
_str_t(_Tys) to_string(const _Tyn& _Numeric
    , ios_flag radix = std::dec)
{ 
    _ss_t(_Tys) ioss;

    ioss << radix << _Numeric;

    return ioss.str();
}

template<typename _Tys, typename _Tyn> inline
_str_t(_Tys)& to_string(_str_t(_Tys)& _String, const _Tyn& _Numeric
    , ios_flag radix = std::dec)
{ 
    _ss_t(_Tys) ioss;

    ioss << radix << _Numeric;

    ioss >> _String;

    return _String;
} /*@method@ to_string @method@*/


// convert string[std::string/std::wstring] to numeric[char/short/int/long/long long/float/double].
template<typename _Tyn, typename _Tys> inline
_Tyn to_numeric(const _str_t(_Tys)& _String 
    , ios_flag radix = std::dec)
{ 
    _Tyn _Numeric = _Tyn();
    _ss_t(_Tys) ioss;

    ioss << radix << _String;
    ioss >> _Numeric;

    return _Numeric;
}

template<typename _Tyn, typename _Tys> inline
_Tyn& to_numeric(_Tyn& _Numeric, const _str_t(_Tys)& _String
    , ios_flag radix = std::dec)
{
    _ss_t(_Tys) ioss;

    ioss << radix << _String;
    ioss >> _Numeric;

    return _Numeric;
} /*@method@ to_numeric @method@*/

// convert string[NTCS] to numeric[char/short/int/long/long long/float/double].
template<typename _Tyn, typename _Ch> inline
_Tyn to_numeric(const _Ch* _String, ios_flag radix = std::dec)
{ 
    _Tyn _Numeric = _Tyn();
    _ss_t(_Ch) ioss;

    ioss << radix << _String;
    ioss >> _Numeric;

    return _Numeric;
}

template<typename _Tyn, typename _Ch> inline
_Tyn& to_numeric(_Tyn& _Numeric, const _Ch* _String, ios_flag radix = std::dec)
{
    _ss_t(_Ch) ioss;

    ioss << radix << _String;
    ioss >> _Numeric;

    return _Numeric;
} /*@method@ to_numeric @method@*/

inline 
bool _Is_visible_char(int _C)
{
    return (_C > 0x20 && _C < 0x7F);
}

template<typename _Ch> inline
size_t strtrim(_Ch* _Str)
{
    if(NULL == _Str || !*_Str) {
        return 0;
    }

    _Ch* _Ptr = _Str - 1;

    while( !_Is_visible_char(*(++_Ptr)) && *_Ptr ) ;

    _Ch* _First_vsp = _Ptr;
    _Ch* _Last_vsp = _Ptr;
    if(*_Ptr) {
        while(*(++_Ptr))
        {
            if(_Is_visible_char(*_Ptr)) {
                _Last_vsp = _Ptr;
            }
        }
    }

    size_t _N = _Last_vsp - _First_vsp + 1;
    if(_Ptr != _Str) {
        ::memmove(_Str, _First_vsp, _N);
        _Str[_N] = '\0';
    }

    return _N;
}

template<typename _Ch> inline
int strtrim(_Ch* _Str, int _StrLen)
{
    if(NULL == _Str || !*_Str) {
        return 0;
    }
    _Ch* _Ptr = _Str - 1;

    while( !_Is_visible_char(*(++_Ptr)) && --_StrLen ) ;

    if(_StrLen > 0) {
        while( !_Is_visible_char(_Ptr[_StrLen - 1]) ) --_StrLen ;
        _Ptr[_StrLen] = (_Ch)'\0';
    }

    if(_Ptr != _Str) {
        ::memmove(_Str, _Ptr, _StrLen);
        _Str[_StrLen] = (_Ch)'\0';
    }

    return _StrLen;
}

template<typename _Ch>
_str_t(_Ch)& strtrim(_str_t(_Ch)& _String)
{
    _String.resize(strtrim(const_cast<_Ch*>(_String.c_str()), _String.length()));
    return _String;
}


template<typename _Ch, typename _Fn, typename _Arg> inline
void split(const _Ch* _Src, _Ch delim, _Fn _Func, _Arg& arg)
{
    std::basic_string<_Ch> temp;
    while( *_Src != '\0' && *_Src != ' ')
    {
        if( *_Src != delim )
        {
            temp.push_back(*_Src);
        }
        else {
            if(!temp.empty())
            {
                _Func(temp, arg);
                temp.clear();
            }
        }
        ++_Src;
    }
    if(!temp.empty())
    {
        _Func(temp, arg);
    }
}

template<typename _Ch> inline
void strtoupper(_Ch* source)
{
    while( *source != '\0' )
    {
        *source = toupper(*source);
        ++source;
    }
}

template<typename _Ch> inline
void strtolower(_Ch* source)
{
    while( *source != '\0' )
    {
        *source = tolower(*source);
        ++source;
    }
}

#ifdef _WIN32
typedef gc::unreal_string<char, gc::pod_free, true> pod_string;
typedef gc::unreal_string<wchar_t, gc::pod_free, true> pod_wstring;

inline pod_string transcode(const wchar_t* _Source)
{
    int cch = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);
    char* target = (char*)malloc(cch);
    WideCharToMultiByte(CP_ACP, 0, _Source, -1, target, cch, NULL, NULL);
    return  pod_string(target, cch);
}

inline pod_wstring transcode(const char* _Source)
{
    int cch = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
    wchar_t* target = (wchar_t*)malloc(cch * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, _Source, -1, target, cch);
    return pod_wstring(target, cch);
}

template<typename _Ch> inline
std::basic_string<_Ch> _newguid(void)
{
    std::basic_stringstream<_Ch> swaper;

    _GUID g;
    CoCreateGuid(&g);

    swaper << std::hex 
        << std::setw(8) << std::setfill(_Ch('0')) << g.Data1
        << _Ch('-')
        << std::setw(4) << std::setfill(_Ch('0')) << g.Data2
        << _Ch('-') 
        << std::setw(4) << std::setfill(_Ch('0')) << g.Data3
        << _Ch('-')
        << std::setw(8) << std::setfill(_Ch('0')) << *(reinterpret_cast<unsigned __int32*>(g.Data4))
        << std::setw(4) << std::setfill(_Ch('0')) << *(reinterpret_cast<unsigned __int16*>(g.Data4)+2);

    return swaper.str();
}

template<typename _Ch> inline
void _newguid(std::basic_string<_Ch>& outs)
{
    std::basic_stringstream<_Ch> swaper;

    _GUID g;
    CoCreateGuid(&g);

    swaper << std::hex 
        << std::setw(8) << std::setfill(_Ch('0')) << g.Data1
        << _Ch('-')
        << std::setw(4) << std::setfill(_Ch('0')) << g.Data2
        << _Ch('-') 
        << std::setw(4) << std::setfill(_Ch('0')) << g.Data3
        << _Ch('-')
        << std::setw(8) << std::setfill(_Ch('0')) << *(reinterpret_cast<unsigned __int32*>(g.Data4))
        << std::setw(4) << std::setfill(_Ch('0')) << *(reinterpret_cast<unsigned __int16*>(g.Data4)+2);

    outs = swaper.str();
}
#endif

#ifdef _MSC100
template<typename _Ch> inline
_str_t(_Ch)& strtrim(_str_t(_Ch)&& _String)
{
    return strtrim(_String);
}
#endif


}; // namespace: simplepp_1_13_201301::nsc
}; // namespace: simplepp_1_13_201301

#endif /* _NSCONV_ */
/*
* Copyright (c) 2012-2013 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
**/

