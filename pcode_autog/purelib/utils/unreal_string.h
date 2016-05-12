// unreal_string.h
#ifndef _UNREAL_STRING_H_
#define _UNREAL_STRING_H_

#include <ostream>
#include <string>
#include <memory>
#include "xxfree.h"

namespace purelib {
    using namespace gc;

    namespace native_utils {
        template<typename _Elem> inline
            size_t strlen(const _Elem* _Str)
        {
            const _Elem* pos = _Str;

            while( * (pos++) );

            return (pos - _Str - 1);
        }

        template<typename _Elem> inline
            void reversesb(_Elem* _Str)
        {
            size_t start = 0;
            size_t last = strlen(_Str) - 1;
            while( start != last ) 
            {
                if(_Str[start] != _Str[last])
                {
                    std::swap(_Str[start], _Str[last]);
                }
                ++start, --last;
            }
        }

        template<typename _Elem> inline
            void reversesb(_Elem* _Str, size_t _Count)
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

        template<typename _Elem, size_t _Size> inline
            void reversesba(_Elem (&_Str)[_Size])
        {
            size_t start = 0;
            size_t last = _Size - 2;
            while( start != last ) 
            {
                //  const char* type = typeid(_Str[start]).name();
                if(_Str[start] != _Str[last])
                {   
                    std::swap(_Str[start], _Str[last]);
                }
                ++start, --last;
            }
        }

        template<typename _Elem> inline
            int strcmp(const _Elem* _Str1, const _Elem* _Str2)
        {
            int ret = 0 ;

            while( ! (ret = (unsigned int)*_Str1 - (unsigned int)*_Str2) && *_Str2)
                ++_Str1, ++_Str2;

            return ret;
        }

        template<typename _Elem> inline
            _Elem* strcpy(_Elem* _DstStr, const _Elem* _SrcStr)
        {
            _Elem* _Rp = _DstStr;

            while( ( *(_DstStr++) = *(_SrcStr++) ) );

            return _Rp;
        }
        template<typename _Elem> inline
            _Elem* strcpy_s(_Elem* _DstStr, size_t _DstSize, const _Elem* _SrcStr)
        {
            if( _DstSize <= (strlen(_SrcStr)) )
            {
                return 0;
            }

            return strcpy(_DstStr, _SrcStr);
        }
        template<typename _Elem, size_t _DstSize> inline
            _Elem* strcpy_s(_Elem (&_DstStr)[_DstSize], const _Elem* _SrcStr)
        {
            return strcpy_s(_DstStr, _DstSize, _SrcStr);
        }


        template<typename _Elem> inline
            _Elem* strncpy(_Elem* _DstStr, const _Elem* _SrcStr, size_t _Count)
        {    
            _Elem* _Rp = _DstStr;

            while( ( (_DstStr - _Rp) < _Count ) && 
                ( *(_DstStr++) = *(_SrcStr++) ) );

            *_DstStr = (_Elem)0;

            return _Rp;
        }

        template<typename _Elem> inline
            _Elem* strncpy_s(_Elem* _DstStr, size_t _DstSize, const _Elem* _SrcStr, size_t _Count)
        {
            if(_DstSize <= _Count)
            {
                return 0;
            }

            return strncpy(_DstStr, _SrcStr, _Count);
        }

        template<typename _Elem, size_t _DstSize> inline
            _Elem* strncpy_s(_Elem (&_DstStr)[_DstSize], const _Elem* _SrcStr, size_t _Count)
        {    
            return strncpy_s(_DstStr, _DstSize, _SrcStr, _Count);
        }

        template<typename _Elem> inline
            _Elem* strcat(_Elem* _DstStr, const _Elem* _SrcStr)
        {
            _Elem * _Rp = _DstStr;

            while( *(_DstStr) ) ++_DstStr;

            while( (*(_DstStr++) = *(_SrcStr++)) );

            return ( _Rp );                 
        }

        template<typename _Elem> inline
            _Elem* strcat_s(_Elem* _DstStr, size_t _DstSize, const _Elem* _SrcStr)
        {
            if(_DstSize <= strlen(_DstStr) + strlen(_SrcStr))
            {
                return 0;
            }

            return strcat(_DstStr, _SrcStr);
        }

        template<typename _Elem, size_t _DstSize> inline
            _Elem* strcat_s(_Elem* _DstStr, const _Elem* _SrcStr)
        {
            return strcat_s(_DstStr, _DstSize, _SrcStr);
        }

        template<typename _Elem> inline
            _Elem toupper(_Elem ch)
        {
            return ( (ch >= 'a' && ch <= 'z') ? (ch - 0x20) : ch );
        }

        template<typename _Elem> inline
            _Elem tolower(_Elem ch)
        {
            return ( (ch >= 'A' && ch <= 'Z') ? (ch + 0x20) : ch );
        }

        template<typename _Elem> inline
            void strtoupper_noncopy(_Elem* _DstStr)
        {
            while(*_DstStr != 0x0)
            {
                if(*_DstStr >= 'a' && *_DstStr <= 'z')
                {
                    *_DstStr -= 0x20;
                }
                ++_DstStr;
            }
        }

        template<typename _Elem> inline
            void strtolower_noncopy(_Elem* _DstStr)
        {
            while(*_DstStr != 0x0)
            {
                if(*_DstStr >= 'A' && *_DstStr <= 'Z')
                {
                    *_DstStr += 0x20;
                }
                ++_DstStr;
            }
        }

    };

    // TEMPLATE CLASS unreal_string forward declare
    template<typename _Elem, 
        typename _Cleaner = pseudo_cleaner<_Elem> >
    class unreal_string;

    typedef unreal_string<char, pod_cleaner<char> >          managed_cstring;
    typedef unreal_string<wchar_t, pod_cleaner<wchar_t> >    managed_wcstring;
    typedef unreal_string<char, array_cleaner<char> >        managed_string;
    typedef unreal_string<wchar_t, array_cleaner<wchar_t> >  managed_wstring;
    typedef unreal_string<char, pseudo_cleaner<char> >       unmanaged_string;
    typedef unreal_string<wchar_t, pseudo_cleaner<wchar_t> > unmanaged_wstring;

    template<typename _Elem, 
        typename _Cleaner>
    class unreal_string
    {
        typedef unreal_string<_Elem, _Cleaner> _Myt;

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

        unreal_string(const _Elem* _Ptr, size_t _Length)
        {
            _Tidy();
            this->assign(_Ptr, _Length);
        }

        unreal_string(const _Elem* _First, const _Elem* _Last)
        {
            _Tidy();
            this->assign(_First, _Last);
        }

        unreal_string(_Elem* _Ptr)
        {
            _Tidy();
            this->assign(_Ptr);
        }

        unreal_string(_Elem* _Ptr, size_t _Length)
        {
            _Tidy();
            this->assign(_Ptr, _Length);
        }

        unreal_string(_Elem* _First, _Elem* _Last)
        {
            _Tidy();
            this->assign(_First, _Last);
        }

        unreal_string(const std::basic_string<_Elem>& _Right)
        {
            _Tidy();
            this->assign(_Right);
        }

        unreal_string(const _Myt& _Right)
        {
            this->assign(_Right);
        }

        unreal_string(_Myt&& _Right)
        {
            this->assign(_Right);
        }

        template<typename _OtherElem, typename _OtherCleaner>
        unreal_string(const unreal_string<_OtherElem, _OtherCleaner>& _Right)
        {
            this->assign(_Right);
        }

        template<typename _OtherElem, typename _OtherCleaner>
        unreal_string(unreal_string<_OtherElem, _OtherCleaner>&& _Right)
        {
            this->assign(_Right);
        }

        ~unreal_string(void)
        {
            _Mysize = 0;
            _Cleaner::cleanup(this->_Bx._Ptr);
            this->_Bx._Const_Ptr = nullptr;
        }

        _Myt& assign(const _Elem* _Ptr)
        { // shallow copy
            if(_Ptr != nullptr) {
                this->_Bx._Const_Ptr = _Ptr;
                this->_Mysize = native_utils::strlen(_Ptr);
            }
            return *this;
        }

        _Myt& assign(const _Elem* _Ptr, size_t _Length)
        { // shallow copy
            if(_Ptr != nullptr) {
                this->_Bx._Const_Ptr = _Ptr;
                this->_Mysize = _Length;
            }
            return *this;
        }

        _Myt& assign(const _Elem* _First, const _Elem* _Last)
        { // shallow copy
            if(_First != nullptr && _First <= _Last) {
                this->_Bx._Const_Ptr = _First;
                this->_Mysize = _Last - _First;
            }
            return *this;
        }

        _Myt& assign(_Elem* _Ptr)
        { // shallow copy
            if(_Ptr != nullptr) {
                this->_Bx._Ptr = _Ptr;
                this->_Mysize = native_utils::strlen(_Ptr);
            }
            return *this;
        }

        _Myt& assign(_Elem* _Ptr, size_t _Length)
        { // shallow copy
            if(_Ptr != nullptr) {
                this->_Bx._Ptr = _Ptr;
                this->_Mysize = _Length;
            }
            return *this;
        }

        _Myt& assign(_Elem* _First, _Elem* _Last)
        { // shallow copy
            if(_First != nullptr && _First <= _Last) {
                this->_Bx._Ptr = _First;
                this->_Mysize = _Last - _First;
            }
            return *this;
        }

        _Myt& assign(const std::basic_string<_Elem>& _Right)
        {
            static_assert(!_Cleaner::value, "stl-string can't assign to a managed unreal_string");

            this->_Bx._Const_Ptr = _Right.c_str();
            this->_Mysize = _Right.size();
            return *this;
        }

        _Myt& assign(const _Myt& _Right)
        {
            static_assert(!_Cleaner::value, "the managed unreal string can't assign to this managed unreal_string object!");
            
            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }

        _Myt& assign(_Myt&& _Right)
        {
            this->assign(_Right);
            /*_Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;*/
        }

        template<typename _OtherElem, typename _OtherCleaner>
        _Myt& assign(const unreal_string<_OtherElem, _OtherCleaner>& _Right)
        {
            static_assert(std::is_same<_Elem, _OtherElem>::value, "can't assign unreal string assign, type not same!");
            static_assert(!_Cleaner::value || !_OtherCleaner::value, "the unreal string can't assign to this managed unreal_string object!");
            
            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }

        template<typename _OtherElem, typename _OtherCleaner>
        _Myt& assign(unreal_string<_OtherElem, _OtherCleaner>&& _Right)
        {
            this->assign(_Right);
        }
        /*_Myt& assign(const managed_cstring& _Right)
        {
            static_assert(!_Cleaner::value, "managed_cstring can't assign to a managed unreal_string object");

            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }

        _Myt& assign(const managed_string& _Right)
        {
            static_assert(!_Cleaner::value, "managed_string can't assign to a managed unreal_string object");

            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }

        _Myt& assign(const managed_wcstring& _Right)
        {
            static_assert(!_Cleaner::value, "managed_wcstring can't assign to a managed unreal_string object");

            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }

        _Myt& assign(const managed_wstring& _Right)
        {
            static_assert(!_Cleaner::value, "managed_wstring can't assign to a managed unreal_string object");

            _Tidy();
            this->_Bx._Const_Ptr = _Right.raw();
            this->_Mysize = _Right.size();
            return *this;
        }*/

        _Myt& operator=(_Elem *_Ptr)
        {	// assign [_Ptr, <null>)
            return (this->assign(_Ptr));
        }

        _Myt& operator=(const _Elem *_Ptr)
        {	// assign [_Ptr, <null>)
            return (this->assign(_Ptr));
        }

        _Myt& operator=(const std::basic_string<_Elem>& _Right)
        {	// assign [_Ptr, <null>)
            return (this->assign(_Right));
        }

        _Myt& operator=(const _Myt& _Right)
        {
            return this->assign(_Right);
        }

        _Myt& operator=(_Myt&& _Right)
        {
            return this->assign(_Right);
        }

        template<typename _OtherElem, typename _OtherCleaner>
        _Myt& operator=(const unreal_string<_OtherElem, _OtherCleaner>& _Right)
        {
            return this->assign(_Right);
        }

        template<typename _OtherElem, typename _OtherCleaner>
        _Myt& operator=(unreal_string<_OtherElem, _OtherCleaner>&& _Right)
        {
            return this->assign(_Right);
        }

        

        _Myt& operator+=(const _Myt& _Right);

        _Myt& operator+=(const _Elem *_Ptr);

        _Myt& operator+=(_Elem _Ch);

        static const bool managed(void)
        { // return if free memory automatically
            return _Cleaner::value;
        }

        void  set_size(size_t _Newsize)
        {
            this->_Mysize = _Newsize;
        }

        void shrink(size_t _Newsize) 
        {
            if(_Mysize > _Newsize)
            {
                this->_Mysize = _Newsize;
            }
        }

        _Elem& at(size_t _Off)
        {	// subscript nonmutable sequence
            return (this->_Bx._Ptr[_Off]);
        }

        const _Elem& at(size_t _Off) const
        {	// subscript nonmutable sequence
            return (this->_Bx._Const_Ptr[_Off]);
        }

        _Elem& operator[](size_t _Off)
        {	// subscript nonmutable sequence
            return (this->_Bx._Ptr[_Off]);
        }

        const _Elem& operator[](size_t _Off) const
        {	// subscript nonmutable sequence
            return (this->_Bx._Const_Ptr[_Off]);
        }

        std::basic_string<_Elem> to_string(void) const
        {   // deep copy: return a c++ stl string
            static const _Elem s_empty[] = {(_Elem)0};
            if(!this->empty()) {
                return std::basic_string<_Elem>(this->_Bx._Const_Ptr, this->size());
            }
            return s_empty;
        }

        const _Elem* c_str(void) const
        {	// return pointer to null-terminated nonmutable array
            if(this->_Bx._Const_Ptr != nullptr) {
                return this->_Bx._Const_Ptr;
            }
            static const _Elem s_empty[] = {(_Elem)0};
            return s_empty;
        }

        const char* data(void) const 
        {	// return pointer to nonmutable array
            return this->c_str();
        }

        size_t length(void) const
        {	// return length of sequence
            return _Mysize;
        }

        size_t size(void) const
        {	// return length of sequence
            return _Mysize;
        }

        void clear(void) 
        {
            _Mysize = 0;
            *this->_Bx._Const_Ptr = (_Elem)'\0';
        }

        _Elem*& raw(void)
        { // return internal pointer which can be change by exnternal, use careful
            return this->_Bx._Ptr;
        }

        const _Elem*const& raw(void) const
        { // return internal pointer which can be change by exnternal, use careful
            return this->_Bx._Const_Ptr;
        }

        bool empty(void) const
        {
            return 0 == this->_Mysize || nullptr == this->_Bx._Ptr;
        }

        int compare(const _Elem* _Right) const
        {
            if(!this->empty() && _Right != nullptr)
            {
                return memcmp(this->_Bx._Const_Ptr, _Right, _Mysize - 1);
            }
            return this->_Bx._Const_Ptr - _Right;
        }

        int compare(const std::basic_string<_Elem>& _Right) const
        {
            return this->compare(_Right.c_str());
        }

        int compare(const _Myt& _Right) const
        {
            return this->compare(_Right.c_str());
        }

        operator std::basic_string<_Elem>(void) const
        {
            return this->to_string();
        }

        std::string substr(std::string::size_type _Off, std::string::size_type _Count = std::string::npos) const
        {
            return this->to_string().substr(_Off, _Count);
        }

    protected:
        void _Tidy(void)
        {
            this->_Mysize = 0;
            this->_Bx._Const_Ptr = nullptr;
        }

    private:
        enum
        {	// length of internal buffer, [1, 16]
            _BUF_SIZE = 16 / sizeof (_Elem) < 1 ? 1
            : 16 / sizeof (_Elem)};
        enum
        {	// roundup mask for allocated buffers, [0, 15]
            _ALLOC_MASK = sizeof (_Elem) <= 1 ? 15
            : sizeof (_Elem) <= 2 ? 7
            : sizeof (_Elem) <= 4 ? 3
            : sizeof (_Elem) <= 8 ? 1 : 0};
        union _Bxty
        {	// storage for small buffer or pointer to larger one
            //_Elem _Buf[_BUF_SIZE];
            const _Elem* _Const_Ptr; // always shallow copy
            _Elem*       _Ptr;       // managed pointer
            //char _Alias[_BUF_SIZE];	// to permit aliasing
        } _Bx;
        size_t       _Mysize;
    };

    /// operator +
    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_string<_Elem> operator+(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // unreal_string + unreal_string
        return ( _Left.to_string() + _Right.to_string() );
    }

    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_string<_Elem> operator+(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::string& _Right)
    {    // unreal_string + std::string
        return ( _Left.to_string() + _Right );
    }

    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_string<_Elem> operator+(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const char* _Right)
    {    // unreal_string + NTCS
        return ( _Left.to_string() + _Right );
    }

    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_string<_Elem> operator+(
        const std::string& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // std::string + unreal_string
        return ( _Left + _Right.c_str() );
    }

    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_string<_Elem> operator+(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // NTCS + unreal_string
        return ( std::string(_Left) + _Right.c_str() );
    }

    /// operator comparands
    template<typename _Elem,
        typename _Cleaner> inline
        bool operator==(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for unreal_string equality
        return (_Left.compare(_Right) == 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator==(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for NTCS vs. unreal_string equality
        return (_Right.compare(_Left) == 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator==(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test for unreal_string vs. NTCS equality
        return (_Left.compare(_Right) == 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator==(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for std::string vs. unreal_string equality
        return (_Left.compare(_Right.c_str()) == 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator==(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test for unreal_string vs. std::string equality
        return (_Right.compare(_Left.c_str()) == 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator!=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for unreal_string inequality
        return (!(_Left == _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator!=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for NTCS vs. unreal_string inequality
        return (!(_Left == _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator!=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test for unreal_string vs. NTCS inequality
        return (!(_Left == _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator!=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test for std::string vs. unreal_string inequality
        return (!(_Left == _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator!=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test for unreal_string vs. std::string inequality
        return (!(_Left == _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if unreal_string < unreal_string
        return (_Left.compare(_Right) < 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if NTCS < unreal_string
        return (_Right.compare(_Left) > 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string < NTCS
        return (_Left.compare(_Right) < 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if std::string < unreal_string
        return (_Left.compare(_Right.c_str()) > 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string < std::string
        return (_Right.compare(_Left.c_str()) < 0);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if unreal_string > unreal_string
        return (_Right < _Left);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if NTCS > unreal_string
        return (_Right < _Left);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string > NTCS
        return (_Right < _Left);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if std::string > unreal_string
        return (_Right < _Left);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string > std::string
        return (_Right < _Left);
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if unreal_string <= unreal_string
        return (!(_Right < _Left));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if NTCS <= unreal_string
        return (!(_Right < _Left));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string <= NTCS
        return (!(_Right < _Left));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if std::string <= unreal_string
        return (!(_Right < _Left));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator<=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string <= std::string
        return (!(_Right < _Left));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if unreal_string >= unreal_string
        return (!(_Left < _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>=(
        const _Elem* _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if NTCS >= unreal_string
        return (!(_Left < _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const _Elem* _Right)
    {    // test if unreal_string >= NTCS
        return (!(_Left < _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>=(
        const std::basic_string<_Elem>& _Left,
        const unreal_string<_Elem, _Cleaner>& _Right)
    {    // test if std::string >= unreal_string
        return (!(_Left < _Right));
    }

    template<typename _Elem,
        typename _Cleaner> inline
        bool operator>=(
        const unreal_string<_Elem, _Cleaner>& _Left,
        const std::basic_string<_Elem>& _Right)
    {    // test if unreal_string >= std::string
        return (!(_Left < _Right));
    }

    /// operator <<
    template<typename _Elem,
        typename _Cleaner> inline
        std::basic_ostream<_Elem>& operator<<(
        std::basic_ostream<_Elem>& _Ostr,
        const unreal_string<_Elem, _Cleaner>& _Str)
    {    // insert a unreal_string
        if(!_Str.empty()) {
            _Ostr << _Str.c_str();
        }
        return (_Ostr);
    }

}; // namespace: simplepp_1_13_201307

#endif 
/* _UNREAL_STRING_H_ */

/*
* Copyright (c) 2012-2013 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
V3.0:2011 */

