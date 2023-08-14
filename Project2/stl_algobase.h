#pragma once
#ifndef STL_ALGOBASE_H_
#define STL_ALGOBASE_H_
#include<cstring>
#include"stl_iterator.h"
#include"type_traits.h"
#include<utility>
using std::pair;
// ������ȷ���true���ڶ�ϵ��Ԫ�ؽ϶಻����
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
// ��[first,last)������Ԫ�ظ�����ֵ
template<class ForwardIterator,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)
		*first = value;
}
// ��[first,last)��ǰn��Ԫ�ظ�����ֵ
template<class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}
// ������ForwardIterator��ָ����Ե�
template<class ForwardIterator1,class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	// ˫�㹹����ȡ��������Ԫ�ص��ͱ�
	__iter_swap(a, b, value_type(a));
	// Ҳ����ֱ��ȡ��
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
// ���ֵ���Ƚ�[first1,last1),[first2,last2)�е�Ԫ��
// ��һ���н�С����true
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
// ƽ�бȽ���������,ָ������֮���һ����ƥ���
template<class InputIterator1,class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	// �������һԪ�ظ������ڵڶ�����,��������Ԥ��
	while (first != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}
template<class InputIterator1, class InputIterator2,class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	// �������һԪ�ظ������ڵڶ�����,��������Ԥ��
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
// ��[first,last)�ڵ�Ԫ�ظ��Ƶ�[result,result+(last-first))
// copy��ȫ�����汾
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}
// copy����汾(1):����
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
// copy����汾(2):����
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
// copy_dispatch��ȫ�����汾
template<class InputIterator,class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};
// copy_dispatchƫ�ػ��汾(1),T*
template<class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};
// copy_dispatchƫ�ػ��汾(2),const T*
template<class T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(const T* first,const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};
// __copy��InputIterator��
template<class  InputIterator,class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	// �ٶȽ���
	for (; first != last; ++result, ++first) {
		*result = *first;
	}
	return result;
}
// __copy��RandomAccessIterator��
template<class RandomAccessIterator,class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}
template<class RandomAccessIterator, class OutputIterator,class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	// �ٶȽϿ�
	for (Distance n = last - first; n > 0; --n, ++result, +first) {
		*result = *first;
	}
	return result
}
// copy_t ��ָ����߱�trivial assignment operator
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
// copy_t ��ָ����߱�non trivial assignment operator
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}
// �������[first1,last1)�������[first2,last2)������Ԫ��
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		// �ֱ��ƶ�������,��Ԫ�ؽ�С��(A��)��¼��result
		// �ƶ�A��������,��һ������,�ٴαȽ�
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
	// ����һ������β��
	// ��Ϊ����β�˵�����ʣ��Ԫ��copy��result
	return copy(first2, last2, copy(first1, last1, result));
}
// ��������ڵ�ÿ��Ԫ�����˳���S1��ͬ
// �������[first1,last1)�Ҵ�����[first2,last2)������Ԫ��
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
// �������[first1,last1)�Ҳ�������[first2,last2)��Ԫ��
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
// �������[first1,last1)�Ҳ�������[first2,last2)������Ԫ��
// �Լ�������[first2,last2)�Ҳ�������[first1,last1)������Ԫ��
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
// �������ڵ��ظ�Ԫ��
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
// ���ҵ�һ����������������Ԫ��
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
// ������value��ȵ�Ԫ�ظ���
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
// ������pred������Ϊtrue��Ԫ�ظ���
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
// ������value��ȵ�Ԫ��
template<class InputIterator,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)++first;
	return first;
}
// ������pred���㷵��true��Ԫ��
template<class InputIterator, class T,class Predicate>
InputIterator find(InputIterator first, InputIterator last, const T& value,Predicate pred) {
	while (first != last && pred(*first))++first;
	return first;
}
// ������һ[first1,last1)������,�������ж�[first2,lasst2)
// ���޷���ȫƥ��,����last1
// dispatch funciton ���ͺ���
template<class ForwardIterator1,class ForwardIterator2>
inline  ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	typedef typename iterator_traits<ForwardIterator1>::iterator_category categoty1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category categoty2;
	// ����������ʱ������Ŀ�꺯��
	return __find_end(first1, last1, first2, last2, category1(), category2());
}
// forward iterators��
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
			// �ҳ�ĳ���������״γ��ֵĵ�,û�ҵ�����last1
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
	// ʹ��reverse_iterator
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;

	reviter1 rlast1(first1);
	reviter2 rlast2(first2);
	reviter1 rresule = search(reviter(last1), rlast1, reviter2(last2), rlast2);

	if (rresule == rlast1)
		return last1;
	else {
		// ת������������
		BidirectionalIterator1 result = rresult.base();
		// �����ص���������ͷ��
		advance(result, -distance(first2, last2));
		return result;
	}
}
// ��[first2,last2)�е�Ԫ��Ϊ����Ŀ��,Ѱ��������[first1,last1)��һ�γ��ֵĵص�
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
// ��[first2,last2)�е�Ԫ��Ϊ����Ŀ��,Ѱ��������[first1,last1)��һ�γ��ֵĵص�,ָ����Ԫ����Ϊpred
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
// ���º���ʵ����[first,last),���ı�Ԫ������
template<class InputIterator,class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}
// ��gen()�Ľ����д��[first,last)�������Ԫ��
template<class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	
	for (; first != last; ++first)
		*first = gen();
}
// ��gen()�Ľ����д��first��ʼ��n��Ԫ��
template<class OutputIterator,class Size,class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++first)
		*first = gen();
	return first;
}
// �ж����ж���ÿ��Ԫ���Ƿ񶼴���������һ
template<class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			// ��������ز�����
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
			// ��������ز�����
			return false;
		else if (comp(*first1,*first2))
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}
// ���ص�����ָ����ֵ����Ԫ��
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
// ����������ļ��Ϻϲ�������һ�οռ�
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
// ����������ļ��Ϻϲ�������һ�οռ�
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
// ���ص�����ָ����ֵ��С��Ԫ��
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
// ��[first,last)�е�Ԫ����������,pred�ж�Ϊtrue����ǰ��,����֤�������λ��
template<class BidirectionalIterator,class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	while (true) {
		while (true) {
			if (first == last)
				return first;
			else if (pred(*first))
				++first;
			else
				// ͷָ����ָ�����ƶ�����
				break;
		}
		--last;
		while (true) {
			if (first == last)
				return first;
			else if (!pred(*last))
				--last;
			else 
				// βָ����ָ�����ƶ�����
				break;
		}
		iter_swap(first, last);
		++first;
	}
}
// ɾ���������Ƴ�Ԫ��,�ռ�δ���ͷ�
template<class ForwardIterator,class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = find(first, last, value);
	ForwardIterator next = first;
	// remove_copy �����¾������ص�
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
// ɾ���������Ƴ�Ԫ��,�ռ�δ���ͷ�
template<class ForwardIterator,class Predicate >
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
	first = find_if(first, last, pred);
	ForwardIterator next = first;
	// remove_copy �����¾������ص�
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
// [first, last)����old_value��new_valueȡ��
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
// ˫��������λ����Ӱ��Ч��
// ��ת[first,last)
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
// ��[first,middle)��[middle,last)Ԫ�ػ���
// �������ƶ�����Ӱ��Ч�ʰ�
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
		// �ж�ǰ���Ƚ������Ǻ���Ƚ���
		if (first == middle) {
			if (i == last)return;
			// ����,�Ժ���û�н�����Ԫ�ؽ���
			middle = i;
		}
		else if (i == last)
			// ����,��ǰ��û�н�����Ԫ�ؽ���
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
// ��[first1,last1)����[first2,last2)�״γ��ֵ�
// ���޷���ȫƥ���򷵻�last1
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