#pragma once
#ifndef _STL_CONSTRUCT_H_
#define _STL_CONSTRUCT_H_
//���ﶨ��ȫ�ֺ���construct()��destroy(),�������Ĺ��������
//��stl��allocator��׼��
// �ڴ����ò�����allocate()����,�ڴ��ͷŲ���deallocate()����
//�����������::construct()����,��������������::destroy()����
#include<new.h>
#include"type_traits.h"
template<class T1, class T2>
inline void construct(T1* p, const T2& value) {
	new(p)T1(value);	//placement new����p��ָλ�ù���value
}
template<class T>
inline void destroy(T* pointer) {
	pointer->~T();		//ֱ�ӵ�����������
}
template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
	//��Ҫ�������������һ�ζ���
	__destroy(first, last, value_type(first));
}
//�ж�Ԫ�ص���ֵ�ͱ��Ƿ���trivial destructor
template<class ForwardIterator,class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor);
}
//���Ԫ�ص���ֵ�ͱ���non-trivial destructor,��һ����
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
	for (; first < last; ++first)
		destroy(&*first);
}
//���Ԫ�ص���ֵ�ͱ���trivial destructor,ʲô������
template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}
//��char*��wchar_t*���ػ�

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {};
#endif