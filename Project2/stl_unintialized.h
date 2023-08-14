#pragma once
#ifndef _STL_UNINITIALIZED_H_
#define _STL_UNINITIALIZED_H_
#include<cstring>	//for memmove
//����ȫ�ֺ���,���������ƴ���ڴ�����
template<class InputIterrator,class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterrator first, InputIterrator last, ForwardIterator result);

//��first,last��Χ�ڲ���x�ĸ���Ʒ
template<class ForwardIterator,class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x);

//uninitialized_fill_nΪָ����Χ�ڵ�����Ԫ���趨��ͬ��ֵ
template<class ForwardIterator,class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	return __uninitialized_fill_n(firest, n, x, value_type(first));
	//��������value_type()ȡ��first��value type
}
//��ȡ��first��value type,�жϸ��ͱ��Ƿ�ΪPOD�ͱ�
template<class ForwardIterator, class Size, class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x,T1*) {
	//__type_traits����
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
//POD: Plain Old Data,�����ͱ��ͳC struct �ͱ�
//POD�ͱ��Ȼӵ��trival ctor/dtor/copy/assihnment
//�ɶ�POD�ͱ��������Ч�ʵĴ�����д�ַ�

//��copy construction ��ͬ��assignment�������漰��̬�ڴ����
//��destructor��trivial
// ��POD�ͱ�ִ������תΪ���º���
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
	return fill_n(firest, n, x);
}
// �������POD�ͱ�ִ������תΪ���º���
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
	ForwardIterator cur = first;
	//����ʡ�Ը��е��쳣����
	for (; n > 0; --n, ++cur) {
		//�˴�Ӧ���ڴ����ʧ�ܵ��쳣
		construct(&*cur, x);
	}
	return cur;
}


// uninitilized_copy��first, last��Χ�ڲ���x�ĸ���Ʒ
template <class InputIterator,class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
	// value_type()ȡ��first��value type,�ж��Ƿ�ΪPOD�ͱ�
	return __uninitialized_copy(first, last, result, value_type(result));
}
template <class InputIterator,class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD();
}

//�����POD�ͱ�
template<class InputIterator,class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
	return copy(first, last, result);
}
//����Ƿ�POD�ͱ�
template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
	for (; first != last; ++first, ++cur) {
		//ʡ���˶��ڴ���䲻��������쳣����
		construct(&*cur, *first);
	}
}
//���char*��wchar_t�����ͱ�����Ч�ʵ�������ֱ���ƶ��ڴ�����
//���const char*���ػ��汾
inline char* uninitialized_copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
//���const wchar_t*���ػ��汾
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

//uninitialized_fill ��ָ����Χ�ڲ���x�ĸ���Ʒ
//����ȡ��first��value type
template <class ForwardIterator,class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
	__uninitialized_fill(first, last, x, value_type(first));
}
//�ж��Ƿ�ΪPOD�ͱ�
template <class ForwardIterator, class T,class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x, is_POD());
}
//�����POD�ͱ�
template<class ForwardIterator,class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
	fill(first, last, x);
}
//����Ƿ�POD�ͱ�
template<class ForwardIterator,class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
	for (; cur != last; ++cur) {
		//ʡ�Զ��ڴ���䲻����쳣����
		construct(&*cur, x);
	}
}

#endif