#pragma once
#ifndef STL_NUMERIC_H_
#define STL_NUMERIC_H_
#include"stl_iterator.h"
// 计算init和[first,last)内元素总和
template <class InputIterator,class T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init = init + *first;
	return init;
}
template <class InputIterator,class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation) {
	for (; first != last; ++first) {
		// 对每一个元素进行二元操作
		init = binary_op(init, *first);
	}
	return init;
}
// 计算[first,last)中相邻元素的差额
// 针对[first+1,lasst)中的每个迭代器i
// 将*i-*(i-1)赋值给*(result+(i-first))
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
// 计算一般内积
// 将结果初始化为init
// 针对[first1,last1)内的每一个迭代器i,执行result=result+(*i)* *(first2+(i-first));
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
// 计算局部总和
// 将*first赋值给*result
// 将*first和*(first+1)的和赋值给*(result+1)
// 以此类推
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
// 计算n幂次方
template<class T,class Integer>
inline T power(T x, Integer n) {
	// 指定运算型式为乘法
	return power(x, n, multiplies<T>());
}
template<class T,class Integer,class MonoidOperation>
T power(T x, Integer n, MonoidOperation op) {
	// 快速幂
	if (n == 0)
		return identity_element(op);
	else {
		while ((n & 1) == 0) {
			// 如果n是偶数
			n >> 1;
			x = op(x, x);
		}
	}
	T result = x;
	n >>= 1;
	while (n != 0) {
		x = op(x, x);
		if ((n & 1) != 0) {
			// n 是奇数
			result = op(result, op);
		}
		n >>= 1;
	}
	return result;
}
// 在[first,last)区间填入value,value+1,value+2...
template<class ForwardIterator,class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
	while (first != last)*first++ = value++;
}
#endif