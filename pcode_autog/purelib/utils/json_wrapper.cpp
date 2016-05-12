//#include <cocos2d.h>
#include "simple_ptr.h"
#include "nsconv.h"
#include "xxfsutility.h"
#include "json_wrapper.h"
#include <assert.h>
#include "oslib.h"

// nullobj

jsonw::object jsonw::s_null_obj;

#define NEX_UNREACHABLE assert(false)
using namespace thelib::gc;
using namespace jsonwrapper;

object& object::operator[](const char* name)
{
    // do add new member
    if (!IsObject())
        SetObject();

    if (!HasMember(name)) {
        value_base empty_obj(rapidjson::kObjectType);
        AddMember(name, empty_obj, s_pool_allocator_);
    }

    return at(name);
}

bool    object::value_of(const char* key, bool default_value)
{
    if(IsObject() && HasMember(key))
    {
        auto& tmp = at(key);
        if(tmp.IsBool())
        {
            return tmp.GetBool();
        }
        else if(tmp.IsString())
        {
            return atoi(tmp.GetString()) != 0;
        }
    }
    return default_value;
}

int    object::value_of(const char* key, int default_value)
{
    if(IsObject() && HasMember(key))
    {
        auto& tmp = at(key);
        if(tmp.IsInt())
        {
            return tmp.GetInt();
        }
        else if(tmp.IsString())
        {
            return atoi(tmp.GetString());
        }
    }
    return default_value;
}

long long object::value_of(const char* key, long long default_value)
{
    if(IsObject() && HasMember(key))
    {
        auto& tmp = at(key);
        if(tmp.IsInt64())
        {
            return tmp.GetInt64();
        }
        else if(tmp.IsString())
        {
            return atoll(tmp.GetString());
        }
    }
    return default_value;
}

double object::value_of(const char* key, double default_value)
{
    if(IsObject() && HasMember(key))
    {
        auto& tmp = at(key);
        if(tmp.IsDouble())
        {
            return tmp.GetDouble();
        }
        else if(tmp.IsString())
        {
            return atof(tmp.GetString());
        }
    }
    return default_value;
}

const char* object::value_of(const char* key, const char* default_value)
{
    if(IsObject() && HasMember(key))
    {
        auto& tmp = at(key);
        if(tmp.IsString())
        {
            return tmp.GetString();
        }
    }
    return default_value;
}

bool object::value_of(size_t idx, bool default_value)
{
    if(IsObject() && idx < Size())
    {
        auto& tmp = at(idx);
        if(tmp.IsBool())
        {
            return tmp.GetBool();
        }
        else if(tmp.IsString())
        {
            return atoi(tmp.GetString()) != 0;
        }
    }
    return default_value;
}

int object::value_of(size_t idx, int default_value)
{
    if(IsArray() && idx < Size())
    {
        auto& tmp = at(idx);
        if(tmp.IsInt())
        {
            return tmp.GetInt();
        }
        else if(tmp.IsString())
        {
            return atoi(tmp.GetString());
        }
    }
    return default_value;
}

long long object::value_of(size_t idx, long long default_value)
{
    if(IsArray() && idx < Size())
    {
        auto& tmp = at(idx);
        if(tmp.IsInt64())
        {
            return tmp.GetInt64();
        }
        else if(tmp.IsString())
        {
            return atoll(tmp.GetString());
        }
    }
    return default_value;
}

double object::value_of(size_t idx, double default_value)
{
    if(IsArray() && idx < Size())
    {
        auto& tmp = at(idx);
        if(tmp.IsDouble())
        {
            return tmp.GetDouble();
        }
        else if(tmp.IsString())
        {
            return atof(tmp.GetString());
        }
    }
    return default_value;
}

const char* object::value_of(size_t idx, const char* default_value)
{
    if(IsArray() && idx < Size())
    {
        auto& tmp = at(idx);
        if(tmp.IsString())
        {
            return tmp.GetString();
        }
    }
    return default_value;
}

rapidjson::MemoryPoolAllocator<> jsonwrapper::object::s_pool_allocator_;

/* ---------------------- friendly split line ------------------------- */

document::document(void)
{
	SetNull();
}

document::document(char* source)
{
    parse(source);
}

document& document::parse(char* source)
{
    this->ParseInsitu<1>(source);
    return *this;
}
