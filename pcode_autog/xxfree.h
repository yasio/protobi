#ifndef _XXFREE_H_
#define _XXFREE_H_

namespace simplepp_1_13_201301 {

namespace gc {
// TEMPLATE STRUCT destroyer
template<typename _Ty>
struct _Free
{
	typedef void (*type)(_Ty*); 
};

// c++ single object deleter
template<typename _Ty>
void object_free(_Ty* _Ptr)
{
	delete _Ptr;
}

// c++ objects array free function template
template<typename _Ty>
void array_free(_Ty* _Ptr)
{
	delete [] _Ptr;
}

// pod type object free function template
template<typename _Ty>
void pod_free(_Ty* _Ptr)
{
	free(_Ptr);
}

// pseudo free, do not influence the object
template<typename _Ty>
void pseudo_free(_Ty*)
{
}

}; // namespace: simplepp_1_13_201301::gc
}; // namespace: simplepp_1_13_201301

#endif
 /* _XXFREE_H_ */
/*
* Copyright (c) 2012-2013 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
V3.0:2011 */

