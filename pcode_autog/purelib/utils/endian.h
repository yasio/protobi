#ifndef _XXENDIAN_H_
#define _XXENDIAN_H_
#include "politedef.h"

#ifdef _WIN32
#include <WinSock2.h>
//#include <Mswsock.h>
#include <Windows.h>
//#include <Mstcpip.h>
#else
#include <arpa/inet.h>
#endif

#include "xxbswap.h"

namespace purelib {
namespace endian {

static const uint16_t __internal_for_comfirm_byte_endian = 0x0001;
static const bool is_little_endian = *((bool*)&__internal_for_comfirm_byte_endian);

static
uint64_t __bswap64(uint64_t value)
{
    return
        ( static_cast<uint64_t>( htonl( static_cast<uint32_t>(value) ) 
        ) << 32 )
        | 
        ( static_cast<uint64_t>( 
        ntohl( static_cast<uint32_t>(
        static_cast<uint64_t>(value) >> 32 
        ) ) ) );
}
static
uint64_t __pseudo_bswap64(uint64_t value)
{
    return value;
}

/* 函数指针形式：
debug  性能略低于直接调用（每1亿次 慢100ms左右）
release模式下和直接调用相当
*/
static uint64_t (*htonll)(uint64_t);

static
void auto_resources_init(void)
{
    if(is_little_endian)
    {
        htonll = __bswap64;
    }
    else {
        htonll = __pseudo_bswap64;
    }
}

static
const int INITIALIZER_VAL0 = (auto_resources_init(), 3389);

#define ntohll htonll
#define hton ntoh

template<typename _Int> inline
_Int ntoh(_Int value)
{
    return ntohl(value);
}

template<> inline
bool ntoh(bool value)
{
    return value;
}

template<> inline
uint8_t ntoh(uint8_t value)
{
    return value;
}

template<> inline
uint16_t ntoh(uint16_t value)
{
    return htons(value);
}

template<> inline
uint32_t ntoh(uint32_t value)
{
    return htonl(value);
}

template<> inline
uint64_t ntoh(uint64_t value)
{
    return htonll(value);
}

template<> inline
int8_t ntoh(int8_t value)
{
    return value;
}

template<> inline
int16_t ntoh(int16_t value)
{
    return htons(value);
}

template<> inline
int32_t ntoh(int32_t value)
{
    return htonl(value);
}

template<> inline
int64_t ntoh(int64_t value)
{
    return htonll(value);
}

template<typename _Int> inline
char* set_value(_Int value, char* to)
{
        *( (_Int*)to ) = (value);
        return to;
}

template<typename _Int> inline
_Int& get_value(_Int& value, const char* from)
{
    return (value = ( *( (_Int*)from ) ));
}

inline
char* set_value(const char* value, char* to, size_t size)
{
    return (char*)::memcpy(to, value, size);
}

inline
char* get_value(char* value, const char* from, size_t size)
{
    return (char*)::memcpy(value, from, size);
}

// non auto step intefraces
template<typename _Int> inline
void to_netval_i(_Int value, char* const to)
{
        *( (_Int*)to ) = hton<_Int>(value);
}

template<typename _Int> inline
void to_hostval_i(_Int& value, const char* const from)
{
    value = ntoh<_Int>( *( (_Int*)from ) );
}

inline
void to_netval_i(const char* const value, char* const to, size_t size)
{
    ::memcpy(to, value, size);
}

inline
void to_hostval_i(char* const value, const char* const from, size_t size)
{
    ::memcpy(value, from, size);
}

// auto step converters
template<typename _Int> inline
char*& to_netval(_Int value, char*& to)
{
        *( (_Int*)to ) = hton<_Int>(value);
        to += (sizeof(_Int));
        return to;
}

template<typename _Int> inline
_Int& to_hostval(_Int& value, const char*& from)
{
    value = ntoh<_Int>( *( (_Int*)from ) );
    from += (sizeof(_Int));
    return value;
}

inline
char*& to_netval(const char* const value, char*& to, size_t size)
{
        ::memcpy(to, value, size);
        to += (size);
        return to;
}

inline
char* to_hostval(char* const value, const char*& from, size_t size)
{
    ::memcpy(value, from, size);
    from += (size);
    return value;
}


}; // namespace thelib::endian

}; // namespace thelib

#endif
/*
* Copyright (c) 2012-2013 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
**/

