#pragma once
#ifndef STL_NUMERIC_H_
#define STL_NUMERIC_H_
#include"stl_iterator.h"
// ����init��[first,last)��Ԫ���ܺ�
template <class InputIterator,class T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init = init + *first;
	return init;
}
template <class InputIterator,class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation) {
	for (; first != last; ++first) {
		// ��ÿһ��Ԫ�ؽ��ж�Ԫ����
		init = binary_op(init, *first);
	}
	return init;
}
// ����[first,last)������Ԫ�صĲ��
// ���[first+1,lasst)�е�ÿ��������i
// ��*i-*(i-1)��ֵ��*(result+(i-first))
template<class InputIterator,class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)return result;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last) {
		iterator_traits<InputIterator>::value_type tmp = *first;
		*++result = tmp - value;
		value = tmp;
	}
	return ++result;
}
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result,BinaryOperation binary_op) {
	if (first == last)return result;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last) {
		iterator_traits<InputIterator>::value_type tmp = *first;
		*++result =binary_op(tmp,value);
		value = tmp;
	}
	return ++result;
}
// ����һ���ڻ�
// �������ʼ��Ϊinit
// ���[first1,last1)�ڵ�ÿһ��������i,ִ��result=result+(*i)* *(first2+(i-first));
template<class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
	for (; first1 != last1; ++first1, ++first2) {
		init = init + (*first1 * *first2);
	}
	return init;
}
template<class InputIterator1, class InputIterator2, class T, class BinaryOperatorn1,class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init,BinaryOperatorn1 binary_op1,BinaryOperation2 binary_op2){
	for (; first1 != last1; ++first1, ++first2) {
		init = binary_op1(init, binary_op2(*first, *first2));
	}
	return init;
}
// ����ֲ��ܺ�
// ��*first��ֵ��*result
// ��*first��*(first+1)�ĺ͸�ֵ��*(result+1)
// �Դ�����
template<class InputIterator,class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)return result;
	*result = *first;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last) {
		value = value + *first;
		++result = value;
	}
	return ++result;
}
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result,BinaryOperation binary_op) {
	if (first == last)return result;
	*result = *first;
	iterator_traits<InputIterator>::value_type value = *first;
	while (++first != last) {
		value = binary_op(value, *first);
		++result = value;
	}
	return ++result;
}
// ����n�ݴη�
template<class T,class Integer>
inline T power(T x, Integer n) {
	// ָ��������ʽΪ�˷�
	return power(x, n, multiplies<T>());
}
template<class T,class Integer,class MonoidOperation>
T power(T x, Integer n, MonoidOperation op) {
	// ������
	if (n == 0)
		return identity_element(op);
	else {
		while ((n & 1) == 0) {
			// ���n��ż��
			n >> 1;
			x = op(x, x);
		}
	}
	T result = x;
	n >>= 1;
	while (n != 0) {
		x = op(x, x);
		if ((n & 1) != 0) {
			// n ������
			result = op(result, op);
		}
		n >>= 1;
	}
	return result;
}
// ��[first,last)��������value,value+1,value+2...
template<class ForwardIterator,class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
	while (first != last)*first++ = value++;
}
#endif