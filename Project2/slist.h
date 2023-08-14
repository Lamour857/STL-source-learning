#pragma once
#ifndef SLIST_H_
#define SLIST_H_
#include<cstddef>
#include"stl_iterator.h"
#include"memory.h"
// 节点基本结构
struct __slist_node_base {
	__slist_node_base* next;
};
// 节点结构
template<class T>
struct __slist_node:public __slist_node_base {
	T data;
};
inline __slist_node_base* __slist_make_link
(__slist_node_base* prev_node,__slist_node_base* new_node){
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}
inline size_t __slist_size(__slist_node_base* node) {
	size_t result = 0;
	for (; node != 0; node = node->next) {
		++result;
	}
	return result;
}
// 迭代器基本结构
struct __slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_catagory;

	__slist_node_base* node;	// 指向节点基本结构

	__slist_iterator_base(__slist_node_base* x) :node(x){}
	void  incr() { node = node->next; }
	bool operator ==(const __slist_iterator_base& x)const {
		return node == x.node;
	}
	bool operator!= (const __slist_iterator_base & x)const {
		return node != x.node;
	}
};
// 迭代器结构
template<class T,class Ref,class Ptr>
struct __slist_iterator :public __slist_iterator_base {
	typedef __slist_iterator<T, T&, T*> iterator;
	typedef __slist_iterator<T, const T&, const T*> const_iterator;
	typedef __slist_iterator<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __slist_node<T> list_node;

	// constructor
	__slist_iterator(list_node* x):__slist_iterator_base(x){}
	__slist_iterator():__slist_iterator_base(0){}
	__slist_iterator(const iterator& x):__slist_iterator_base(x.node){}

	reference operator*()const { return ((list_node*)node)->data; }
	pointer operator->()const { return &(operator*()); }

	self& operator++() {
		incr();
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		incr();
		return tmp;
	}
};
template<class T,class Alloc=alloc>
class slist {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __slist_iterator<T, T&, T*> iterator;
	typedef __slist_iterator<T, const T&, const T*> const_iterator;
private:
	typedef __slist_node<T> list_node;
	typedef __slist_node_base list_node_base;
	typedef __slist_iterator_base iterator_base;
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
};
#endif