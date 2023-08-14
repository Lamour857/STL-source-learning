#pragma once
#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_
#include"vector.h"
#include"stl_iterator.h"
#include"heap.h"
#include"stl_config.h"
template<class T, class Sequence = vector<T>, class Compare = less<typename Sequence::value_type>>
class priority_queue {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;

protected:
	// 底层容器
	Sequence c;
	// 元素大小比较标准
	Compare comp;
public:
	priority_queue():c(){}
	explicit priority_queue(const Compare& x):c(),comp(x){}

	// 以下make_heap(), push_heap(), pop_heap()都是泛型算法
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& x) :
		c(first, last), comp(x) {
		make_heap(c.begin(), c.end(), comp);
	}
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last) :
		c(first, last) {
		make_heap(c.begin(), c.end());
	}

	bool empty()const { return c.empty(); }
	size_type size()const { return c.size(); }
	reference top() { return c.front(); }
	void push(const value_type& x) {
		__STL_TRY{
			c.push_back(x);
			push_heap(c.begin(), c.end(), comp);
		}
		__STL_UNWIND(c.clear());
	}
	void pop() {
		__STL_TRY{
			// 并非真正将元素弹出, 而是重排heap
			// 以底层容器的pop_back() 将其弹出
			pop_heap(c.begin(),c.end(),comp);
			c.pop_back();
		}
	}
};
#endif
