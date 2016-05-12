// unreal_string.h
#ifndef _UNREAL_STRING_H_
#define _UNREAL_STRING_H_

#include <ostream>
#include <string>
#include <memory>
#include "xxfree.h"

namespace simplepp_1_13_201301 {

namespace gc {

template<typename _Elem>
struct strlen_type
{
};

template<>
struct strlen_type<char>
{
    static size_t strlen(const char* _Str)
    {
        return ::strlen(_Str);
    }
};

template<>
struct strlen_type<wchar_t>
{
    static size_t strlen(const wchar_t* _Str)
    {
        return ::wcslen(_Str);
    }
};

template<typename _Elem, 
    typename _Free<_Elem>::type = pseudo_free<_Elem>, 
    bool _AutoRelease = false>
class unreal_string;

template<typename _Unreal_String, bool _Auto>
struct _Unreal_string_cleaner
{
    static void cleanup(_Unreal_String* target)
    { // default do nothing
        (void) target;
    }
};

template<typename _Unreal_String>
struct _Unreal_string_cleaner<_Unreal_String, true>
{
    static void cleanup(_Unreal_String* target)
    {
        target->release();
    }
};

template<typename _Elem, 
    typename _Free<_Elem>::type _Destroy, 
    bool _AutoRelease>
class unreal_string
{
    typedef unreal_string<_Elem, _Destroy, _AutoRelease> _Myt;

public:
    unreal_string(void)
    {
        _Tidy();
    }

    unreal_string(const _Elem* _Ptr)
    {
        _Tidy();
        this->assign(_Ptr);
    }

    unreal_string(const _Elem* _Ptr, size_t len)
    {
        this->assign(_Ptr, len);
    }

    unreal_string(const std::basic_string<_Elem>& stlstr)
    {
        this->assign(stlstr);
    }

    ~unreal_string(void)
    {
        _Unreal_string_cleaner<_Myt, _AutoRelease>::cleanup(this);
    }

    _Myt& assign(const _Elem* _Ptr)
    {
        if(_Ptr != nullptr) {
            this->_Mystr = _Ptr;
            this->_Mysize = strlen_type<_Elem>::strlen(_Ptr);
        }
        return *this;
    }

    _Myt& assign(const _Elem* _Ptr, size_t _Size)
    {
        if(_Ptr != nullptr) {
            this->_Mystr = _Ptr;
            this->_Mysize = _Size;
        }
        return *this;
    }

    _Myt& assign(const std::basic_string<_Elem>& stlstr)
    {
        this->_Mystr = stlstr.c_str();
        this->_Mysize = stlstr.length();
        return *this;
    }

    size_t length(void) const
    {
        return _Mysize;
    }

    size_t size(void) const
    {
        return _Mysize;
    }

    const _Elem* c_str(void) const
    {
        return _Mystr;
    }

    void clear(void) 
    {
        _Mystr = nullptr;
        _Mysize = 0;
    }

    void release(void) {
        if(_Mystr != nullptr) {
            _Destroy((_Elem*)_Mystr); // for default this operation not real destroy the memory
            this->clear();
        }
    }

    const _Elem*& ptr_ref(void)
    {
        return _Mystr;
    }

    size_t& length_ref(void)
    {
        return _Mysize;
    }

    bool empty(void) const
    {
        return nullptr == this->_Mystr || 0 == this->_Mysize;
    }

    int compare(const _Elem* _Right) const
    {
        if(this->_Mystr != nullptr && _Right != nullptr)
        {
            return ::strcmp(this->_Mystr, _Right);
        }
        return this->_Mystr - _Right;
    }

    int compare(const std::basic_string<_Elem>& _Right) const
    {
        return this->compare(_Right.c_str());
    }

    int compare(const _Myt& _Right) const
    {
        return this->compare(_Right.c_str());
    }

protected:
    void _Tidy(void)
    {
        this->_Mystr = nullptr;
        this->_Mysize = 0;
    }

private:
    const _Elem* _Mystr;
    size_t       _Mysize;
};

/// operator comparands
template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator==(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for unreal_string equality
    return (_Left.compare(_Right) == 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator==(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for NTCS vs. unreal_string equality
    return (_Right.compare(_Left) == 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator==(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test for unreal_string vs. NTCS equality
    return (_Left.compare(_Right) == 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator==(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for std::string vs. unreal_string equality
    return (_Right.compare(_Left) == 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator==(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test for unreal_string vs. std::string equality
    return (_Left.compare(_Right) == 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator!=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for unreal_string inequality
    return (!(_Left == _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator!=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for NTCS vs. unreal_string inequality
    return (!(_Left == _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator!=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test for unreal_string vs. NTCS inequality
    return (!(_Left == _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator!=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test for std::string vs. unreal_string inequality
    return (!(_Left == _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator!=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test for unreal_string vs. std::string inequality
    return (!(_Left == _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if unreal_string < unreal_string
    return (_Left.compare(_Right) < 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if NTCS < unreal_string
    return (_Right.compare(_Left) > 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string < NTCS
    return (_Left.compare(_Right) < 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if std::string < unreal_string
    return (_Right.compare(_Left) > 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string < std::string
    return (_Left.compare(_Right) < 0);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if unreal_string > unreal_string
    return (_Right < _Left);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if NTCS > unreal_string
    return (_Right < _Left);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string > NTCS
    return (_Right < _Left);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if std::string > unreal_string
    return (_Right < _Left);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string > std::string
    return (_Right < _Left);
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if unreal_string <= unreal_string
    return (!(_Right < _Left));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if NTCS <= unreal_string
    return (!(_Right < _Left));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string <= NTCS
    return (!(_Right < _Left));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if std::string <= unreal_string
    return (!(_Right < _Left));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator<=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string <= std::string
    return (!(_Right < _Left));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if unreal_string >= unreal_string
    return (!(_Left < _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if NTCS >= unreal_string
    return (!(_Left < _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string >= NTCS
    return (!(_Left < _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Right)
    {    // test if std::string >= unreal_string
    return (!(_Left < _Right));
    }

template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    bool operator>=(
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string >= std::string
    return (!(_Left < _Right));
    }

/// operator <<
template<typename _Elem,
    typename _Free<_Elem>::type _Destroy,
    bool _AutoRelease> inline
    std::basic_ostream<_Elem>& operator<<(
        std::basic_ostream<_Elem>& _Ostr,
        const unreal_string<_Elem, _Destroy, _AutoRelease>& _Str)
    {    // insert a unreal_string
    if(!_Str.empty()) {
        _Ostr << _Str.c_str();
    }
    return (_Ostr);
    }

}; // namespace: simplepp_1_13_201301::gc

}; // namespace: simplepp_1_13_201301

#endif 
/* _UNREAL_STRING_H_ */

/*
* Copyright (c) 2012-2013 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
V3.0:2011 */

