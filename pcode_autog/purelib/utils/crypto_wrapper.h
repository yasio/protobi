 #ifndef _CRYPTO_WRAPPER_H_
#define _CRYPTO_WRAPPER_H_
#include <string>
#include <vector>
#include <assert.h>
#include "unreal_string.h"
#include "crypto_utils.h"

using namespace purelib;

namespace crypto {

    /// IV is defined intrin
    namespace aes {

        /// padding
        namespace privacy {
            template<PaddingMode = PaddingMode::PKCS7>
            struct padding_spec
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::PKCS7(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::ISO10126>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ISO10126(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::ANSIX923>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ANSIX923(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::Zeros>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ZEROS(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::None>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return 0; }
            };

            template<CipherMode = CipherMode::CBC>
            struct mode_spec
            {
                static void encrypt(const char* in, size_t inlen, 
                    char* out, size_t outlen, const char* private_key, int keybits = 256)
                {
                    detail::cbc_encrypt(in, inlen, out, outlen, private_key, keybits);
                }

                static void decrypt(const char* in, size_t inlen,
                    char* out, size_t& outlen, const char* private_key, int keybits = 256)
                {
                    detail::cbc_decrypt(in, inlen, out, outlen, private_key, keybits);
                }
            };

            template<>
            struct mode_spec<CipherMode::ECB>
            {
                static void encrypt(const char* in, size_t inlen,
                    char* out, size_t outlen, const char* private_key, int keybits = 256)
                {
                    detail::ecb_encrypt(in, inlen, out, outlen, private_key, keybits);
                }

                static void decrypt(const char* in, size_t inlen,
                    char* out, size_t& outlen, const char* private_key, int keybits = 256)
                {
                    detail::ecb_decrypt(in, inlen, out, outlen, private_key, keybits);
                }
            };
        };

        /// encrypt decrypt APIs 

        template<CipherMode cipherMode = CipherMode::CBC, PaddingMode paddingMode = PaddingMode::PKCS7, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT encrypt(const _BYTE_SEQ_CONT& in, const char* key = DEFAULT_KEY)
        {
            auto out = in;
            privacy::padding_spec<paddingMode>::perform(out);

            privacy::mode_spec<cipherMode>::encrypt(out.data(),
                out.size(),
                &out.front(),
                out.size(),
                key);
            return std::move(out);
        }

        template<CipherMode cipherMode = CipherMode::CBC, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT decrypt(const _BYTE_SEQ_CONT& in, const char* key = DEFAULT_KEY)
        {
            size_t outlen = in.size();
            _BYTE_SEQ_CONT out(outlen, 0x0);
            privacy::mode_spec<cipherMode>::decrypt(in.data(),
                in.size(),
                &out.front(),
                outlen,
                key);
            out.resize(outlen);
            return std::move(out);
        }

        template<CipherMode cipherMode = CipherMode::CBC, PaddingMode paddingMode = PaddingMode::PKCS7, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT encrypt(_BYTE_SEQ_CONT&& in, const char* key = DEFAULT_KEY)
        {
            privacy::padding_spec<paddingMode>::perform(in);

            privacy::mode_spec<cipherMode>::encrypt(in.data(),
                in.size(),
                &in.front(),
                in.size(),
                key);
            return std::move(in);
        }

        template<CipherMode cipherMode = CipherMode::CBC, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT decrypt(_BYTE_SEQ_CONT&& in, const char* key = DEFAULT_KEY)
        {
        	size_t outlen = in.size();
            privacy::mode_spec<cipherMode>::decrypt(in.data(),
                in.size(),
                &in.front(),
                outlen,
                key);
            in.resize(outlen);
            return std::move(in);
        }

        /// wrappers, strongly encrypt, low speed; false: use ECB: fast speed,  light-weighted encrypt
        namespace overlapped {

            template<CipherMode cipherMode = CipherMode::CBC, PaddingMode paddingMode = PaddingMode::PKCS7, typename _ByteSeqCont = std::string>
            inline void encrypt(_BYTE_SEQ_CONT& inout, const char* key = DEFAULT_KEY)
            {
                privacy::padding_spec<paddingMode>::perform(inout);

                privacy::mode_spec<cipherMode>::encrypt(inout.c_str(),
                    inout.size(),
                    &inout.front(),
                    inout.size(),
                    key);
            }

            template<CipherMode cipherMode = CipherMode::CBC, typename _ByteSeqCont = std::string>
            inline void decrypt(_BYTE_SEQ_CONT& inout, const char* key = DEFAULT_KEY)
            {
                size_t outlen = 0;
                privacy::mode_spec<cipherMode>::decrypt(inout.data(),
                    inout.size(),
                    &inout.front(),
                    outlen,
                    key);

                inout.resize(outlen);
            }
        }
        
    } /* end of namespace crypto::aes */

    namespace zlib {
        /*
        ** level values: 
        ** Z_NO_COMPRESSION         0
        ** Z_BEST_SPEED             1
        ** Z_BEST_COMPRESSION       9
        ** Z_DEFAULT_COMPRESSION  (-1)
        **
        */
        std::string compress(const unmanaged_string& in, int level = -1); // zlib 1.2.8 Z_DEFAULT_COMPRESSION is -1
        std::string uncompress(const unmanaged_string& in);
		std::string deflate(const unmanaged_string& in, int level = -1); // zlib 1.2.8 Z_DEFAULT_COMPRESSION is -1
        std::string inflate(const unmanaged_string& in);
    };

    namespace http {
//#define UC_KEY "W4J687g4QefaK7V0RfK9eaNfZ0Ich8ObY4wafdU4qb83vcO3n1l1f8Gc51k7O2W7"
//
//        typedef enum {
//            kAuthTypeEncode,
//            kAuthTypeDecode,
//        } AuthType;

        //std::string encrypt(const unmanaged_string& plaintext, const char* key = UC_KEY);

        //std::string decrypt(const unmanaged_string& ciphertext, const char* key = UC_KEY);

        std::string b64dec(const unmanaged_string& ciphertext);

        std::string b64enc(const unmanaged_string&  plaintext);

        std::string urldecode(const unmanaged_string& ciphertext);

        std::string urlencode(const unmanaged_string& input);

        // std::string uc_authcode(const std::string& input_string, AuthType operation = kAuthTypeDecode, const char* key = "", unsigned int expiry = 0);

    };

    namespace hash {
        std::string md5(const unmanaged_string& string, bool rawoutput = false);
    };
};

#endif

