#define _ZLIB_SUPPORT
#ifdef _ZLIB_SUPPORT
#if defined(_WIN32) && !defined(_WP8)
#include <zlib.h>/* from cocos2dx */
#pragma comment(lib, "libzlib.lib")
#else
#include <zlib.h>
#endif
#endif
#include <assert.h>
#include "crypto_wrapper.h"

//#ifdef _WIN32
//#pragma comment(lib, "ws2_32.lib")
//#endif

using namespace purelib;

#ifdef _ZLIB_SUPPORT

std::string crypto::zlib::compress(const unmanaged_string& in, int level)
{
    
    // calc destLen
    uLongf destLen = compressBound(in.size());
    std::string out(destLen, '\0');
    
    // do compress
    int ret = ::compress2((Bytef*)(&out.front()), &destLen, (const Bytef*)in.c_str(), in.size(), level); 
    
    if (ret == Z_OK)
    {
        out.resize(destLen);
    }
    return std::move(out);
}

std::string crypto::zlib::uncompress(const unmanaged_string& in)
{ 
    return crypto::zlib::inflate(in); 
}

std::string crypto::zlib::deflate(const purelib::unmanaged_string& in, int level)
{
    int err;
    Bytef buffer[128];
    z_stream d_stream; /* decompression stream */

    // strcpy((char*)buffer, "garbage");

    d_stream.zalloc = nullptr;
    d_stream.zfree = nullptr;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in = (Bytef*)in.c_str();
    d_stream.avail_in = in.size();
    d_stream.next_out = buffer;
    d_stream.avail_out = sizeof(buffer);

    err = deflateInit(&d_stream, level);
    if (err != Z_OK) // TODO: log somthing
        return "";

    std::string output;
    for (;;)
    {
        err = deflate(&d_stream, Z_FINISH);

        if (err == Z_STREAM_END)
        {
            output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
            break;
        }

        switch (err)
        {
        case Z_NEED_DICT:
            err = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            deflateEnd(&d_stream);
            return "";
        }

        // not enough buffer ?
        if (err != Z_STREAM_END)
        {
            output.insert(output.end(), buffer, buffer + sizeof(buffer));

            d_stream.next_out = buffer;
            d_stream.avail_out = sizeof(buffer);
        }
    }

    err = deflateEnd(&d_stream);
    if (err != Z_OK)
        return "";

    return std::move(output);
}

std::string crypto::zlib::inflate(const unmanaged_string& compr)
{ // inflate
    int err;
    Bytef buffer[128];
    z_stream d_stream; /* decompression stream */

    // strcpy((char*)buffer, "garbage");

    d_stream.zalloc = nullptr;
    d_stream.zfree = nullptr;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in = (Bytef*)compr.c_str();
    d_stream.avail_in = compr.size();
    d_stream.next_out = buffer;
    d_stream.avail_out = sizeof(buffer);

    err = inflateInit(&d_stream);
    if (err != Z_OK) // TODO: log somthing
        return "";
    // CHECK_ERR(err, "inflateInit");

    std::string output;
    for (;;)
    {
        err = inflate(&d_stream, Z_NO_FLUSH);

        if (err == Z_STREAM_END)
        {
            output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
            break;
        }

        switch (err)
        {
        case Z_NEED_DICT:
            err = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&d_stream);
            return "";
        }

        // not enough memory ?
        if (err != Z_STREAM_END)
        {
            // *out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);
            output.insert(output.end(), buffer, buffer + sizeof(buffer));

            d_stream.next_out = buffer;
            d_stream.avail_out = sizeof(buffer);
        }
    }

    err = inflateEnd(&d_stream);
    if (err != Z_OK)
        return "";

    return std::move(output);
}


#endif


std::string crypto::hash::md5(const unmanaged_string& plaintext, bool rawoutput)
{
    int     count = 32;
    char    ciphertext[32];
    if(!rawoutput) {
        md5chars(plaintext.c_str(), plaintext.length(), ciphertext);
    }
    else {
        count = 16;
        ::md5(plaintext.c_str(), plaintext.length(), ciphertext);
    }

    return std::string(ciphertext, count);
}

std::string crypto::http::b64dec(const unmanaged_string& ciphertext)
{
    std::string plaintext;
    plaintext.reserve( ciphertext.length() );

    base64_decodestate state;
    base64_init_decodestate(&state);
    int r1 = base64_decode_block(ciphertext.c_str(), ciphertext.length(), (char*)plaintext.c_str(), &state);

    plaintext.assign(plaintext.c_str(), r1);
    return std::move(plaintext);
}

std::string crypto::http::b64enc(const unmanaged_string&  plaintext)
{
    std::string ciphertext;
    ciphertext.reserve( (plaintext.length() * 2) );
    char* wrptr = (char*)ciphertext.c_str();
    base64_encodestate state;
    base64_init_encodestate(&state);
    int r1 = base64_encode_block(plaintext.c_str(), plaintext.length(), wrptr, &state);
    int r2 = base64_encode_blockend((char*)ciphertext.c_str() + r1, &state);

    wrptr[r1 + r2] = '\0';
    ciphertext.assign(wrptr, r1 + r2);
    return std::move(ciphertext);
}

std::string crypto::http::urlencode(const unmanaged_string& input)
{
    std::string output;
    for( size_t ix = 0; ix < input.size(); ix++ )
    {      
        uint8_t buf[4];
        memset( buf, 0, 4 );
        if( isalnum( (uint8_t)input[ix] ) )
        {      
            buf[0] = input[ix];
        }
        //else if ( isspace( (BYTE)sIn[ix] ) )
        //{
        //    buf[0] = '+';
        //}
        else
        {
            buf[0] = '%';
            buf[1] = ::hex2uchr( (uint8_t)input[ix] >> 4 );
            buf[2] = ::hex2uchr( (uint8_t)input[ix] % 16);
        }
        output += (char *)buf;
    }
    return std::move(output);
};

std::string crypto::http::urldecode(const unmanaged_string& ciphertext)
{
    std::string result = "";

    for( size_t ix = 0; ix < ciphertext.size(); ix++ )
    {
        uint8_t ch = 0;
        if(ciphertext[ix]=='%')
        {
            ch = (::uchr2hex(ciphertext[ix+1])<<4);
            ch |= ::uchr2hex(ciphertext[ix+2]);
            ix += 2;
        }
        else if(ciphertext[ix] == '+')
        {
            ch = ' ';
        }
        else
        {
            ch = ciphertext[ix];
        }
        result += (char)ch;
    }

    return std::move(result);
}


/*
** UC_AUTHCODE implementation, legacy
**
*/
/*#define ord int
#define chr char

std::string crypto::http::uc_authcode(const std::string& input_string, AuthType operation, const char* public_key, unsigned int expiry)
{
    int ckey_length = 4;

    std::string key0 = crypto::http::md5(public_key);
    std::string key1 = crypto::http::md5(key0.substr(0, 16));
    std::string key2 = crypto::http::md5(key0.substr(16, 16));
    std::string key3 = ckey_length ? (operation == kAuthTypeDecode ? input_string.substr(0, ckey_length): nsc::rsubstr(md5(microtime()), ckey_length)) : "";

    std::string cryptkey = key1 + md5(key1 + key3);
    int key_length = cryptkey.length();

    char times[16];
    sprintf(times, "%010d", expiry ? expiry + time(NULL) : 0);
    std::string judge_text = operation == kAuthTypeDecode ? b64dec(input_string.substr(ckey_length)) : times + md5(input_string + key2).substr(0, 16) + input_string;

    int string_length = judge_text.length();

    int i, j, k;
    uint8_t tmp;
    std::string result = "";
    uint8_t box[256];
    for(i = 0; i < 256; ++i)
    {
        box[i] = i;
    }

    uint8_t rndkey[256];
    for(i = 0; i < 256; i++) {
        rndkey[i] = ord(cryptkey[i % key_length]);
    }

    for(j = i = 0; i < 256; i++) {
        j = (j + box[i] + rndkey[i]) % 256;
        tmp = box[i];
        box[i] = box[j];
        box[j] = tmp;
    }

    for(k = j = i = 0; i < string_length; i++) {
        k = (k + 1) % 256;
        j = (j + box[k]) % 256;
        tmp = box[k];
        box[k] = box[j];
        box[j] = tmp;
        result += chr(ord(judge_text[i]) ^ (box[(box[k] + box[j]) % 256]));
    }

    if(operation == kAuthTypeDecode) {
        if((nsc::to_numeric<time_t>(result.substr(0, 10)) == 0 || nsc::to_numeric<time_t>(result.substr(0, 10)) - time(NULL) > 0) && result.substr(10, 16) == md5(result.substr(26) + key2).substr(0, 16)) {
            return result.substr(26);
        } else {
            return "";
        }
    } else {
        return key3 + nsc::replace(b64enc(result), "=", "");
    }
}*/


