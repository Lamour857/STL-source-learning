#pragma once
#ifndef _STL_CONSTRUCT_H_
#define _STL_CONSTRUCT_H_
//这里定义全局函数construct()和destroy(),负责对象的构造和析构
//在stl的allocator标准下
// 内存配置操作由allocate()负责,内存释放操作deallocate()负责
//对象构造操作由::construct()负责,对象析构操作由::destroy()负责
#include<new.h>
#include"type_traits.h"
template<class T1, class T2>
inline void construct(T1* p, const T2& value) {
	new(p)T1(value);	//placement new，在p所指位置构造value
}
template<class T>
inline void destroy(T* pointer) {
	pointer->~T();		//直接调用析构函数
}
template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
	//若要析构迭代器间的一段对象
	__destroy(first, last, value_type(first));
}
//判断元素的数值型别是否有trivial destructor
template<class ForwardIterator,class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor);
}
//如果元素的数值型别有non-trivial destructor,逐一析构
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
	for (; first < last; ++first)
		destroy(&*first);
}
//如果元素的数值型别有trivial destructor,什么都不做
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}
//对char*和wchar_t*的特化

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {};
#endif