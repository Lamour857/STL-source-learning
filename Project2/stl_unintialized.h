#pragma once
#ifndef _STL_UNINITIALIZED_H_
#define _STL_UNINITIALIZED_H_
#include<cstring>	//for memmove
//定义全局函数,用来填充或复制大块内存数据
template<class InputIterrator,class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterrator first, InputIterrator last, ForwardIterator result);

//在first,last范围内产生x的复制品
template<class ForwardIterator,class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x);

//uninitialized_fill_n为指定范围内的所有元素设定相同初值
template<class ForwardIterator,class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	return __uninitialized_fill_n(firest, n, x, value_type(first));
	//以上利用value_type()取出first的value type
}
//萃取出first的value type,判断该型别是否为POD型别
template<class ForwardIterator, class Size, class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x,T1*) {
	//__type_traits技法
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
//POD: Plain Old Data,标量型别或传统C struct 型别
//POD型别必然拥有trival ctor/dtor/copy/assihnment
//可对POD型别采用最有效率的处置填写手法

//若copy construction 等同于assignment，即不涉及动态内存分类
//且destructor是trivial
// 即POD型别执行流程转为以下函数
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
	return fill_n(firest, n, x);
}
// 如果不是POD型别，执行流程转为以下函数
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
	ForwardIterator cur = first;
	//以下省略该有的异常处理
	for (; n > 0; --n, ++cur) {
		//此处应有内存分配失败的异常
		construct(&*cur, x);
	}
	return cur;
}


// uninitilized_copy在first, last范围内产生x的复制品
template <class InputIterator,class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
	// value_type()取出first的value type,判断是否为POD型别
	return __uninitialized_copy(first, last, result, value_type(result));
}
template <class InputIterator,class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD();
}

//如果是POD型别
template<class InputIterator,class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
	return copy(first, last, result);
}
//如果是非POD型别
template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
	for (; first != last; ++first, ++cur) {
		//省略了对内存分配不足引起的异常处理
		construct(&*cur, *first);
	}
}
//针对char*和wchar_t两种型别，最有效率的做法是直接移动内存内容
//针对const char*的特化版本
inline char* uninitialized_copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
//针对const wchar_t*的特化版本
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

//uninitialized_fill 在指定范围内产生x的复制品
//先萃取出first的value type
template <class ForwardIterator,class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
	__uninitialized_fill(first, last, x, value_type(first));
}
//判断是否为POD型别
template <class ForwardIterator, class T,class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x, is_POD());
}
//如果是POD型别
template<class ForwardIterator,class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
	fill(first, last, x);
}
//如果是非POD型别
template<class ForwardIterator,class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
	for (; cur != last; ++cur) {
		//省略对内存分配不足的异常处理
		construct(&*cur, x);
	}
}

#endif