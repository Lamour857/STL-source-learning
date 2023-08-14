#pragma once
#ifndef HEAP_H_
#define HEAP_H_
#include"type_traits.h"
#include"stl_iterator.h"
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}
template<class RandomAccessIterator,class Distance,class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}
template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value) {
		// 当未到达顶端，且父节点小于新值
		// 大顶堆
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__pop_heap_aux(first, last, value_type(first));
}
template<class RandomAccessIterator,class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}
template<class RandomAccessIterator,class T,class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}
template<class RandomAccessIterator, class T, class Distance>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	while (secondChild < len) {
		if (*(first + secondChild) < *(first + (secondChild - 1))) {
			secondChild--;
			// 令较大子值为洞值
		}
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	*(first + hole) = value;
}
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1) {
		pop_heap(firsst, last--);
	}
}
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__make_heap(first, last, value_type(first), distance_type(first));
}
template<class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
	if (last - first < 2)return;
	Distance len = last - first;
	// 找出第一个需要重排的子树头部
	Distance holeIndex = (len - 2) / 2;

	while (true) {
		__adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
		if (holeIndex == 0)return;		// 走完根节点结束
		holeIndex--;	// 头部向前一个节点
	}
}
#endif