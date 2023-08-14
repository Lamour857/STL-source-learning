#pragma once
#ifndef STL_ALGOBASE_H_
#define STL_ALGOBASE_H_
#include<cstring>
#include"stl_iterator.h"
#include"type_traits.h"
#include<utility>
using std::pair;
// 区间相等返回true，第二系列元素较多不考虑
template<class InputIterator1,class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		if (*first1 != *first2)
			return false;
	}
	return true;
}
template<class InputIterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!binary_pred(*first1,*first2))
			return false;
	}
	return true;
}
// 将[first,last)内所有元素改填新值
template<class ForwardIterator,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)
		*first = value;
}
// 将[first,last)内前n个元素改填新值
template<class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}
// 将两个ForwardIterator所指对象对调
template<class ForwardIterator1,class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	// 双层构造提取出迭代器元素的型别
	__iter_swap(a, b, value_type(a));
	// 也可以直接取出
	// typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
	// *a = *b;
	// *b = tmp;
}
template<class ForwardIterator1, class ForwardIterator2,class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}
// 按字典序比较[first1,last1),[first2,last2)中的元素
// 第一序列较小返回true
template<class InputIterator1,class InputIterator2>
bool lexicograhical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2) {
			return true;
		}
		if (*first2 < *first1) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}
template<class InputIterator1, class InputIterator2,class Compare>
bool lexicograpgical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2)) {
			return true;
		}
		if (comp(*first2, *first1)) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}
inline bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}
template<class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}
template<class T,class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}
template<class T>
inline const T& min(const T& a, const T& b) {
	return a < b ? a : b;
}
template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? a : b;
}
// 平行比较两个序列,指出两者之间第一个不匹配点
template<class InputIterator1,class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	// 如果序列一元素个数少于第二序列,则结果不可预期
	while (first != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}
template<class InputIterator1, class InputIterator2,class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	// 如果序列一元素个数少于第二序列,则结果不可预期
	while (first != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}
template<class T>
inline void swap(T&a,T& b){
	T tmp = a;
	a = b;
	b = tmp;
}
// 将[first,last)内的元素复制到[result,result+(last-first))
// copy完全泛化版本
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}
// copy特殊版本(1):重载
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
// copy特殊版本(2):重载
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
// copy_dispatch完全泛化版本
template<class InputIterator,class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};
// copy_dispatch偏特化版本(1),T*
template<class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};
// copy_dispatch偏特化版本(2),const T*
template<class T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(const T* first,const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};
// __copy的InputIterator版
template<class  InputIterator,class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	// 速度较慢
	for (; first != last; ++result, ++first) {
		*result = *first;
	}
	return result;
}
// __copy的RandomAccessIterator版
template<class RandomAccessIterator,class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}
template<class RandomAccessIterator, class OutputIterator,class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	// 速度较快
	for (Distance n = last - first; n > 0; --n, ++result, +first) {
		*result = *first;
	}
	return result
}
// copy_t 所指对象具备trivial assignment operator
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
// copy_t 所指对象具备non trivial assignment operator
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}
// 求存在于[first1,last1)或存在于[first2,last2)的所有元素
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		// 分别移动迭代器,将元素较小者(A区)记录于result
		// 移动A区迭代器,另一区不动,再次比较
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	// 若有一区到达尾端
	// 将为到达尾端的所有剩余元素copy到result
	return copy(first2, last2, copy(first1, last1, result));
}
// 输出区间内的每个元素相对顺序和S1相同
// 求存在于[first1,last1)且存在于[first2,last2)的所有元素
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		
		if (*first1 < *first2) {
			++first1;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}
// 求存在于[first1,last1)且不存在于[first2,last2)的元素
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {

		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	return result;
}
// 求存在于[first1,last1)且不存在于[first2,last2)的所有元素
// 以及存在于[first2,last2)且不存在于[first1,last1)的所有元素
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {

		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return copy(first2,last2,copy(first1,last1,result));
}
// 查找相邻的重复元素
template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (++next != last) {
			if (*first == *next)return next;
			first = next;
		}
	}
	return last;
}
// 查找第一次满足条件的相邻元素
template<class ForwardIterator,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (++next != last) {
			if (binary_pred(*first,*next))return next;
			first = next;
		}
	}
	return last;
}
// 返回与value相等的元素个数
template<class InputIterator,class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (*first == value)
			++n;
	}
	return n;
}
// 返回与pred计算结果为true的元素个数
template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (pred(*first))
			++n;
	}
	return n;
}
// 查找与value相等的元素
template<class InputIterator,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)++first;
	return first;
}
// 查找与pred计算返回true的元素
template<class InputIterator, class T,class Predicate>
InputIterator find(InputIterator first, InputIterator last, const T& value,Predicate pred) {
	while (first != last && pred(*first))++first;
	return first;
}
// 在序列一[first1,last1)区间内,查找序列二[first2,lasst2)
// 若无法完全匹配,返回last1
// dispatch funciton 派送函数
template<class ForwardIterator1,class ForwardIterator2>
inline  ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	typedef typename iterator_traits<ForwardIterator1>::iterator_category categoty1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category categoty2;
	// 产生两个临时对象传入目标函数
	return __find_end(first1, last1, first2, last2, category1(), category2());
}
// forward iterators版
template<class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	ForwardIterator2 first2, ForwardIterator2 last2,
	forward_iterator_tag, forward_iterator_tag) 
{
	if (first2 == last2)
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {
			// 找出某个子序列首次出现的点,没找到返回last1
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result == last1)
				return result;
			else {
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}template<class BidirectionalIterator1,class BidirectionalIterator2>
BidirectionalIterator1 __find_end
(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	BidirectionalIterator2 first2, BidirectionalIterator2 last2,
	bidirectional_iterator_tag, bidirectional_iterator_tag) 
{
	// 使用reverse_iterator
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;

	reviter1 rlast1(first1);
	reviter2 rlast2(first2);
	reviter1 rresule = search(reviter(last1), rlast1, reviter2(last2), rlast2);

	if (rresule == rlast1)
		return last1;
	else {
		// 转回正常迭代器
		BidirectionalIterator1 result = rresult.base();
		// 调整回到子序列起头处
		advance(result, -distance(first2, last2));
		return result;
	}
}
// 以[first2,last2)中的元素为查找目标,寻找他们在[first1,last1)第一次出现的地点
template<class InputIterator,class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator iter = first2; iter != last2; ++iter) {
			if (*first1 == *iter)
				return first1;
		}
	}
	return last1;
}
// 以[first2,last2)中的元素为查找目标,寻找他们在[first1,last1)第一次出现的地点,指定二元运算为pred
template<class InputIterator, class ForwardIterator,class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2,BinaryPredicate comp) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator iter = first2; iter != last2; ++iter) {
			if (comp(*first1, *iter))
				return first1;
		}
	}
	return last1;
}
// 将仿函数实行与[first,last),不改变元素内容
template<class InputIterator,class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}
// 将gen()的结果填写在[first,last)间的所有元素
template<class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	
	for (; first != last; ++first)
		*first = gen();
}
// 将gen()的结果填写在first开始的n个元素
template<class OutputIterator,class Size,class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++first)
		*first = gen();
	return first;
}
// 判断序列二的每个元素是否都存在于序列一
template<class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			// 涵盖情况必不成立
			return false;
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}
template<class InputIterator1, class InputIterator2,class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1))
			// 涵盖情况必不成立
			return false;
		else if (comp(*first1,*first2))
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}
// 返回迭代器指向数值最大的元素
template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first)result = first;
	return result;
}
template<class ForwardIterator,class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*result, *first))result = first;
	return result;
}
// 将经过排序的集合合并置于另一段空间
template<class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copu(first2, last2, copy(first1, last1, result));
}
// 将经过排序的集合合并置于另一段空间
template<class InputIterator1, class InputIterator2, class OutputIterator,class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result,Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2 , *first1)) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copu(first2, last2, copy(first1, last1, result));
}
// 返回迭代器指向数值最小的元素
template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result > *first)result = first;
	return result;
}
template<class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last)return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*first, *result))result = first;
	return result;
}
// 将[first,last)中的元素重新排列,pred判定为true放在前段,不保证保留相对位置
template<class BidirectionalIterator,class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	while (true) {
		while (true) {
			if (first == last)
				return first;
			else if (pred(*first))
				++first;
			else
				// 头指针所指符合移动条件
				break;
		}
		--last;
		while (true) {
			if (first == last)
				return first;
			else if (!pred(*last))
				--last;
			else 
				// 尾指针所指符合移动条件
				break;
		}
		iter_swap(first, last);
		++first;
	}
}
// 删除但并不移除元素,空间未被释放
template<class ForwardIterator,class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = find(first, last, value);
	ForwardIterator next = first;
	// remove_copy 允许新旧容器重叠
	return first == last ? first : remove_copy(++next, last, first, value);
}
template<class InputIterator,class OutputItertor,class T>
OutputItertor remove_copy(InputIterator first, InputIterator last, OutputItertor result, const T& value) {
	for (; first != last; ++first) {
		if (*first != value) {
			*result = *first;
			++result;
		}
	}
	return result;
}
// 删除但并不移除元素,空间未被释放
template<class ForwardIterator,class Predicate >
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
	first = find_if(first, last, pred);
	ForwardIterator next = first;
	// remove_copy 允许新旧容器重叠
	return first == last ? first : remove_copy_if(++next, last, first, pred);
}
template<class InputIterator, class OutputItertor, class Predicate>
OutputItertor remove_copy_if(InputIterator first, InputIterator last, OutputItertor result, Predicate pred) {
	for (; first != last; ++first) {
		if (!pred(*first)) {
			*result = *first;
			++result;
		}
	}
	return result;
}
// [first, last)所有old_value以new_value取代
template<class ForwardIterator,class T>
void replace(ForwardIterator first, ForwardIterator last,const T& old_value,const T& new_value) {
	for (; first != last; ++first)
		if (*first == old_value)*first = new_value;
}
template<class InputIterator,class OutputIterator,class T>
OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
	for (; first != last; ++first, ++result) {
		*result = *first == old_value ? new_value : *first;
	}
}
template<class ForwardIterator,class Predicate,class T>
void replace(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) {
	for (; first != last; ++first)
		if (pred(*first))*first = new_value;
}
template<class InputIterator, class OutputIterator,class Predicate,class T>
OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) {
	for (; first != last; ++first, ++result) {
		*result = pred(*first) ? new_value : *first;
	}
}
// dispatch function
// 双向或随机定位能力影响效率
// 反转[first,last)
template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
	__reverse(first, last, iterator_category(first));
}
// bidirectional iterator
template<class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while (true) {
		if (first == last || first == --last)
			return;
		else
			iter_swap(first++, last);
	}
}
// random access iterator
template<class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	while (first < last)iter_swap(first++, --last);
}
template<class BidirectionalIterator,class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
	while (first != last) {
		--last;
		*result = *last;
		++resukt;
	}
	return result;
}
// dispatch function
// 将[first,middle)和[middle,last)元素互换
// 迭代器移动能力影响效率吧
template<class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	if (first == middle || middle == last)return;
	__rotate(first, middle, last, distance_tupe(first), iterator_category(first));
}
// forward iterator
template<class ForwardIterator,class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag) {
	for (ForwardIterator i = middle;;) {
		iter_swap(first, i);
		++first;
		++i;
		// 判断前段先结束还是后段先结束
		if (first == middle) {
			if (i == last)return;
			// 调整,对后面没有交换的元素交换
			middle = i;
		}
		else if (i == last)
			// 调整,对前面没有交换的元素交换
			i = middle;
	}
}
// bidirectional iterator
template<class BidirectionalIterator,class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
}
// random access iterator
template<class RandomAccessIterator,class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag) {
	Distance n = __gcd(last - first, middle - first);
	while (n--)
	{
		__rotate_cycle(first, last, first + n, middle - first, value_type(first));
	}
}
template<class EuclidenRingElement>
EuclidenRingElement __gcd(EuclidenRingElement m, EuclidenRingElement n) {
	while (n != 0) {
		EuclidenRingElement t = m % n;
		m = n;
		n = t;
	}
	return m;
}
template<class RandomAccessIterator,class Distance,class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*) {
	T value = *initial;
	RandomAccessIterator ptr1 = initial;
	RandomAccessIterator ptr2 = ptr1 + shift;
	while(ptr2 != initial) {
		*ptr1 = *ptr2;
		ptr1 = ptr2;
		if (last - ptr2 > shift)
			ptr2 += shift;
		else
			ptr2 = first + (shift - (last - ptr2));
	}
	*ptr1 = value;
}
template<class ForwardIterator,class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}
// 在[first1,last1)查找[first2,last2)首次出现点
// 若无法完全匹配则返回last1
template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
}
template<class ForwardIterator1, class ForwardIterator2,class Distance1,class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*) {
	Distance1 d1 = 0;
	distance(first1, last1, d1);
	Distance2 d2 = 0;
	distance(first2, last2, d2);
	if (d1 < d2)return last1;

	ForwardIterator1 current1 = first1;
	ForwardIterator2 current2 = first2;
}
#endif