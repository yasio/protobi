// @version:2.0 @author:Lake Goo
#ifndef _CJSON_WRAPPER_H_
#define _CJSON_WRAPPER_H_
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "politedef.h"
#include <assert.h>
#include <sstream>
#include <iomanip>

namespace jsonwrapper{};
namespace jsonw = jsonwrapper;
namespace jsonwrapper {
	typedef rapidjson::GenericDocument<rapidjson::UTF8<char>> document_base;
    typedef rapidjson::GenericValue<rapidjson::UTF8<char> > value_base;
    typedef rapidjson::Type value_id;
    typedef rapidjson::Writer<rapidjson::StringBuffer> minified_writer_impl;
    typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> formatted_writer_impl;

    class object;
    class document;

    // only use libxml2 will conflicit with this macro
#define BAD_CAST (jsonw::object&)

    extern object s_null_obj;
    class object : public value_base
    {
        friend class document;
    public:
        //! Default constructor creates a null value.
        object() : value_base(){}

        //! shallow copy reference
        object(const value_base& rhs) = delete;

        //! Copy constructor is not permitted.
    private:
        object(const object& rhs) = delete;
        

    public:

        value_base& base() { return static_cast<value_base&>(*this); }

        //! Constructor with JSON value type.
        /*! This creates a Value of specified type with default content.
        \param type	Type of the value.
        \note Default content for number is zero.
        */
        object(value_id type) : value_base(type){};

        //! Constructor for boolean value.
        object(bool b) : value_base(b){};

        //! Constructor for int value.
        object(int i) : value_base(i){};

        //! Constructor for unsigned value.
        object(unsigned u) : value_base(u){};

        //! Constructor for int64_t value.
        object(int64_t i64) : value_base(i64){};

        //! Constructor for uint64_t value.
        object(uint64_t u64) : value_base(u64){};

        //! Constructor for double value.
        object(double d) : value_base(d){};

        //! Constructor for constant string (i.e. do not make a copy of string)
        object(const Ch* s) : value_base(s){};

        ////! Constructor for copy-string (i.e. do make a copy of string)
        //object(const Ch* s, SizeType length, Allocator& allocator) { SetStringRaw(s, length, allocator); }

        ////! Constructor for copy-string (i.e. do make a copy of string)
        //object(const Ch*s, Allocator& allocator) { SetStringRaw(s, internal::StrLen(s), allocator); }
        
        object&     operator[](const char* name);
        
        object&     at(const char* name){ return static_cast<object&>(((value_base&)*this)[name]); }
        object&     at(size_t idx){ return static_cast<object&>(((value_base&)*this)[idx]); }

        bool        value_of(const char* key, bool default_value = false);
        int         value_of(const char* key, int default_value = 0);
        long long   value_of(const char* key, long long default_value = 0);
        double      value_of(const char* key, double default_value = 0.0);
        const char* value_of(const char* key, const char* default_value = "");

        //template<typename _Ty>
        //_Ty         value_of(size_t idx, _Ty default_value = _Ty()); // pseudo template decl, no impl
        int         value_of(size_t idx, int default_value = 0);
        long long   value_of(size_t idx, long long default_value = 0);
        double      value_of(size_t idx, double default_value = 0.0);
        bool        value_of(size_t idx, bool default_value = false);
        const char* value_of(size_t idx, const char* default_value = "");

        // array always
        object& array_always(const char* key)
        {
            if (!IsObject())
                SetObject();

            return array(key);
        }

        object& array(const char* key)
        {
            if (IsObject()) {
                value_base value(rapidjson::kArrayType);
                AddMember(key, value, s_pool_allocator_);
                return at(key);
            }
            return s_null_obj;
        }

        // if object exist, modify value; else add new object with value
        template<typename _Ty>
        object&   insert_always(const char* key, const _Ty& val)
        {
            if (!IsObject())
                SetObject();

            return insert(key, val);
        }

        template<typename _Ty>
        object&    push_back_always(const _Ty& val)
        {
            if (!IsArray()) 
                SetArray();

            return push_back(val);
        }

        template<typename _Ty>
        object&    push_back_always(_Ty&& val)
        {
            if (!IsArray()) 
                SetArray();

            return push_back(val);
        }

        // if object exist, do nothing
        template<typename _Ty>
        object&   insert(const char* key, const _Ty& val)
        {
            if (IsObject()) {
                AddMember(key, val, s_pool_allocator_);
                return at(key);
            }
            return s_null_obj;
        }

        template<typename _Ty>
        object&    push_back(const _Ty& val)
        {
            if (IsArray()) {
                PushBack(val, s_pool_allocator_);
                return at(this->Size() - 1);
            }

            return s_null_obj;
        }

        template<typename _Ty>
        object&    push_back(_Ty&& val)
        {
            if (IsArray()) {
                PushBack(val, s_pool_allocator_);
                return at(this->Size() - 1);
            }
            return s_null_obj;
        }

        template<typename _Operation>
        void        cforeach(_Operation op)
        {
            if(IsArray())
            {
                for (auto it = this->onBegin(); it != this->onEnd(); ++it)
                    op(static_cast<object&>(*it));
            }
            else if (IsObject())
            {
                for (auto it = this->MemberonBegin(); it != this->MemberonEnd(); ++it)
                    op(static_cast<object&>(*it));
            }
        }

        template<typename _Writer = minified_writer_impl>
        std::string  to_string(bool auto_remove_cache = false) const
        {
            rapidjson::StringBuffer buf;
            _Writer writer(buf);
            Accept(writer);
            auto bRet = buf.GetString();

            if (auto_remove_cache)
                remove_cache(); // remove writter cache

            return std::move(bRet);
        }

		// assign operators
		template<typename _Ty>
		object& operator=(const _Ty& val)
		{
			base() = val;
			return *this;
		}


        static void remove_cache(void){ s_pool_allocator_.Clear(); }

    private:
        static rapidjson::MemoryPoolAllocator<> s_pool_allocator_;
    }; /* class thelib::jsonwrapper::object */

	class document : public document_base
    {
    public:
        document(void);
        document(char* source);
        document& parse(char* source);
    }; /* class jsonwrapper::document */


}; /* namespace jsonwrapper */



#endif

