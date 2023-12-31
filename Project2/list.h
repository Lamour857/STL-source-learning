#pragma once
#ifndef _LIST_H_
#define _LIST_H_
#include"memory.h"
#include"type_traits.h"
// list 的节点
template<class T>
struct __list_node {
	typedef void* void_pointer;
	// 专属空间配置器
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
	void_pointer prev;
	void_pointer next;
	T data;
};
template<class T, class Ref, class Ptr>
struct __list_iterator {
	typedef __list_iterator<T, T&, T*>		iterator;
	typedef __list_iterator<T, Ref, Ptr>	self;

	typedef bidirectional_iterator_tag		iterator_category;
	typedef T								value_type;
	typedef Ptr								pointer;
	typedef Ref								reference;
	typedef __list_node<T*>					link_type;
	typedef size_t							size_type;
	typedef ptrdiff_t						difference_type;

	link_type node;		//普通指针指向list的节点

	//constructor
	__list_iterator(link_type x) :node(x) {}
	__list_iterator() {}
	__list_iterator(const iterator& x) :node(x.node) {}

	bool operator==(const self& x)const { return node == x.node; }
	bool operator!=(const self& x)const { return node != x.node; }

	//dereference
	reference operator*()const { return (*node).data; }

	//member access
	pointer operator->()const { return &(operator*()); }

	//自增: ++iterator
	self& operator++() {
		node = (link_type)((*node).next);
		return *this;
	}
	//自增: iterator++
	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}
	//自减: --iterator
	self& operator++() {
		node = (link_type)((*node).prev);
		return *this;
	}
	//自减: iterator--
	self operator++(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}
};

template <class T,class Alloc=alloc>
class list {
	typedef void* void_pointer;
	typedef size_t   size_type;
protected:
	typedef __list_node<T> list_node;
public:
	typedef list_node* link_type;
protected:
	link_type node;
protected:
	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}
	// 配置一个节点并传回
	link_type get_node() { return list_node_allocator::allocate(); }
	// 释放一个节点
	void put_node(link_type p) { list_noe_allocator::deallocate(p); }
	// 配置并构造一个节点, 带有元素值
	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}
	// 析构并释放一个节点
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}

	// 将[first,last)内的元素移动到position之前
	void transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			(*(link_type((*last.node).prev))).next = position.node;
			(*(link_type((*first.node).prev))).next = last.node;
			(*(link_type((*position.node).prev))).next = first.node;
			link_type tmp = link_type((*position.node).prev);
			(*position.node).prev = (*last.node).prev;
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
	}

public:
	void clear();
	void remove(const T& x);
	void unique();
	// node指针指向尾端空白节点,则符合前闭后开
	iterator begin() { return (link_type)((*node).next); }
	iterator end() { return node; }
	
	bool empty()const { return node->next == node; }
	size_type size()const {
		size_type result = 0;
		distance(begin(), end(), result);
		return result;
	}
	// 取头节点元素值
	reference front() { *begin(); }
	// 取尾节点元素值
	reference back() { return *(--end()); }
	// constructor
	list() { empty_initialize(); }
	void push_back(const T& x) { insert(end(), x); }
	// 在position所指位置插入一个节点
	iterator insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		// 调整双向指针
		tmp->next = position;
		tmp->prev = position.node;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}
	// 插入一个节点,作为头节点
	void push_front(const T& x) { insert(begin(), x); }
	// 插入一个节点,作为尾节点
	void push_back(ocnst T& x) { insert(end(), x); }

	// 移除position所指节点
	iterator erase(iterator position) {
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(position.node);
		return iterator(next_node);
	}
	
	// 移除头节点
	void pop_front() { erase(begin()); }
	// 移除尾节点
	void pop_back() {
		iterator tmp = end();
		erase(--tmp);
	}
	// 将x接合于position所指位置前
	void splice(iterator position, list& x) {
		if (!x.empty()) {
			transfer(position, x.begin(), x.end());
		}
	}
	// 将i所指元素接合于position所指位置之前
	void splice(iterator position, list&, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j)return;
		transfer(position, i, j);
	}
	// 将[first,last)内的所有元素接合于position所指位置之前
	// position不能位于[first,last)之内
	void splice(iterator position, list&, iterator first, iterator last) {
		if (first != last) {
			transfer(position, first, last);
		}
	}
	// 将 x 合并到 *this
	void merge(list<T, Alloc>& x);
	// 将*this的内容转置
	void reverse();
	// 将*this的内容排序,算法:快速排序
	void sort();
	
};
template<class T,class Alloc>
void list<T, Alloc>::reverse() {
	if (node->next == node || link_type(node->next)->next == node)
		return;
	iterator first = begin();
	++first;
	while (first != end()) {
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}
template<class T,class Alloc>
void list<T, Alloc>::sort() {
	if (node->next == node || link_type(node->next)->next == node)
		return;
	// 中介数据存放区
	list<T, Alloc>carry;
	list<T, Alloc>counter[64];
	int fill = 0;
	while (!empty()) {
		// 将begin所指元素接合于carry.begin所指位置之前
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap[counter[i]];
		if (i == fill)++fill;
	}
	for (int i = 1; i < fill; ++i) {
		counter[i].merge(counter[i - 1]);
	}
	swap(counter[fill - 1]);
}
template<class T,class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	// 两个list都已经递增排序
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			itertor next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else {
			++first1;
		}
	}
	if (first2 != last2)transfer(last1, first2, last2);
}

template<class T,class Alloc>
void list<T, Alloc>::clear() {
	// 头节点
	link_type cur = (link_type)node->next;
	while (cur != node) {
		link_type tmp = cur;
		cur = (link_type)cur->next;
		destroy_node(tmp);
	}
	// 恢复node原始状态
	node->next = node;
	node->prev = node;
}
template<class T,class Alloc>
void list<T, Alloc>::remove(const T& value) {
	iterator first = begin();
	iterator last = end();
	while (first != last) {
		iterator next = first;
		++next;
		if (*first == value)erase(first);
		first = next;
	}
}
template<class T,class Alloc>
void list<T,Alloc>::unique() {
	iterator first = begin();
	iterator last = end();
	if (first == last)return;	// 链表为空
	iterator next = first;
	while (++next != last) {
		if (*first == *next) {
			erase(next);
		}
		else {
			first = next;
		}
		next = first;
	}
}
#endif