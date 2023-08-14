#pragma once
#ifndef _DEQUE_H_
#define _DEQUE_H_
#include"memory.h"
#include"stl_config.h"
#include<cstddef>
template <class T,class Alloc=alloc,size_t BufSiz=0>
class deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T& reference;
	typedef __deque_iterator<T, T&, T*, BufSiz>iterator;

protected:
	// 指向元素指针的指针
	typedef pointer* map_pointer;
	typedef simple_alloc<value_type, Alloc>data_alloctor;
	typedef simple_alloc<pointer, Alloc>map_allocator;

protected:
	// date member
	map_pointer map;
	size_type map_size;
	iterator start;
	iterator finish;
protected:
	void push_back_aux(const value_type& t);
	void push_front_aux(const value_type& t);
	void pop_back_aux();
	void pop_front_aux();
	void reserve_map_at_back(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > map_size - (finish.node - map)) {
			// 如果map尾端备用空间不足
			reallocate_map(nodes_to_add, false);
		}
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map) {
			reallocate_map(nodes_to_add, true);
		}
	}
	iterator insert_aux(iterator pos, const value_type& x);
	void reallocate_map(size_type nodes_to_add, bool add_at_front);
	void fill_initialize(size_type n, const value_type& value);
	// 产生并安排好deque的结构
	void create_map_and_nodes(size_type num_elements);
public:
	// constructor
	deque(int n, const value_type& value) :start(),finish(),map(0),map_size(0){
		fill_initialize(n, value);
	}
	
	void push_back(const value_type& t) {
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, t);
			++finish.cur;
		}
		else {
			push_back_aux(t);
		}
	}
	void push_front(const value_type& t) {
		if (start.cur != start.first) {
			construct(start.cur - 1, t);
			--start.cur;
		}
		else {
			push_front_aux(t);
		}
	}
	void pop_back() {
		if (finish.cur != finish.first) {
			// 缓冲区最后有剩余元素
			--finish.cur;
			destroy(finish.cur);
		}
		else {
			pop_back_aux();
		}
	}
	void pop_front() {
		if (start.cur != start.last - 1) {
			destroy(start.cur);
			++start.cur;
		}
		else {
			pop_front_aux();
		}
	}
	iterator begin() { return start; }
	iterator end() { return finish; }
	iterator erase(iterator first, iterator last);
	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		// 清除点之前的元素个数
		difference_type index = pos - start;
		if (index < (size() >> 1)) {
			// 清除点之前的元素较少
			// 移动清除点之前的元素
			copy_backward(start, pos, next);
			pop_front();
		}
		else {
			// 清除点之后的元素较少
			// 移动清除点之后的元素
			copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else {
			return insert_aux(position, x);
		}
	}
	void clear();
	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	reference front() { return *start; }
	reference back() {
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}
	size_type size()const { return finish - start; }
	size_type max_size()const { return size_type(-1); }
	bool empty()const { return finish == start; }

};
template<class T, class Alloc, size_t BufSize>
deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& x) {
	difference_type index = pos - start;
	value_type x_copy = x;
	if (index < size() / 2) {
		// 如果插入之前元素个数较少
		push_front(front());
		iterator front1 = start;
		++front1;
		iterator front2 = front1;
		++front2;
		pos = start + index;
		iterator pos1 = pos;
		++pos1;
		// 元素移动
		copy(front2, pos1, front1);
	}
	else {
		push_back(back());
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = start + index;
		copy_backward(pos, back2, back1);
	}
	*pos = x_copy;
	return pos;
}
template<class T,class Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator
deque<T, Alloc, BufSize>::erase(iterator first, iterator last) {
	if (first == start && last == finish) {
		// 清除区间是整个deque
		clear();
		return finish;
	}
	else {
		difference_type n = last - first;		// 清除区间长度
		difference_type elems_before = first - start;	// 清除区间前方的元素个数
		if (elems_before < (size() - n / 2)) {
			// 向后移动前方元素
			copy_backward(start,first,last);
			iterator new_start = start + n;
			// 洗后冗余元素
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur) {
				// 释放冗余空间
				data_alloctor::deallocate(*cur, buffer_size());
			}
			// 调整新起点
			start = new_start;
		}
		else {
			// 后方元素向前移动
			copy(last, finish, first);
			iterator new_finish = finish - n;
			// 析构冗余元素
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur) {
				// 释放冗余空间
				data_alloctor::deallocate(*cur, bufffer_size());
			}
			// 设定新尾点
			finish = new_finish;
		}
		return start + elems_before;
	}
}
template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::clear() {
	// 针对头尾以外的每一个缓冲区
	for (map_pointer node = strt.node + 1; node < finish.node; ++node) {
		destroy(*node, *node + buffer_size());
		// 释放缓冲区内存
		data_allocator::deallocate(*node, buffer_size);
	}
	if (start.node != finish.node) {	// 至少有头尾两个缓冲区
		// 将头缓冲区所有元素析构
		destroy(start.cur, start.last);
		// 将尾缓冲区所有元素析构
		destroy(finish.first, finish.cur);
		// 释放尾缓冲区
		data_allocator::deallocate(finish.first, buffer_size);
	}
	else {	//只有一个缓冲区
		// 析构缓冲区内的元素,不释放空间
		destroy(strat.cur, finish.cur);
	}
	finish = start;
}

template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux() {
	// 将第一缓冲区的第一个元素析构
	destroy(start.cur);
	deallocate_node(start.first);
	start.set_node(start.node + 1);
	start.cur = start.first;
}
template<class T,class Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux() {
	// 释放最后一个缓冲区
	deallocate_node(finish.first);
	finish.set_node(finish.node - 1);
	finish.cur = fiinsh.last - 1;
	destroy(finish.cur);

}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
	value_type t_copy = t;
	// 判断是否需要扩充map
	reserve_map_at_front();
	*(start.node - 1) = allocate_node();
	__STL_TRY{
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, t_copy);
	}catch (...) {
		// commit or rollback
		start.set_node(start.node + 1);
		start.cur = start.first;
		deallocate_node(*(start.node - 1));
		throw;
	}
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
	size_type old_num_nodes = finish, node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_start;
	if (map_size > 2 * new_num_nodes) {
		// 若整个空间大于两倍需要的节点数
		// 则移动空间内的元素，使两端均有空闲空间
		new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		if (new_nstart < start.node)
			copy(start.node, finish.node + 1, new_nstart);
	}
	else {
		size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
		// 配置一块新空间
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		// copy 原map内容
		copy(start.node, finish.node + 1, new_nstart);
		// 释放原map
		map_allocator::deallocate(map, map_size);
		map = new_map;
		map_size = new_map_size;
	}
	start.set_node(new_nstart);
	finish.set_node(new_start + old_num_nodes - 1);
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
	value_type t_copy = t;
	// 符合某种条件则必须重换一个map
	reserve_map_at_back();
	// 配置新缓冲区+
	*(finish.node + 1) = allocate_node();
	__STL_TRY{
		construct(finish.cur,t_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	__STL_UNWIND(deallocate_node(*(finish.noode + 1)));
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
	// 节点数
	size_type num_nodes = num_elements / buffer_size() + 1;
	
	// map管理的节点数，最多时需要的节点数+2（前后扩充用）
	map_size = max(initial_map_size(), num_nodes + 2);
	map = map_allocator::allocator(map_size);

	// 配置出具有map_size个节点的map

	map_pointer nstart = map + (map_size - num_nodes) / 2;
	
	map_pointer nfinish = nstart + num_nodes - 1;

	map_pointer cur;
	__STL_TRY{
		for (cur = nstart; cur <= nfinish; ++cur) {
			*cur = allocate_node();
		}
	}catch (...) {
		// commit or rollback

	}
	start.set_node(nstart);
	finish.set_node(nfinish);
	start.cur = start.first;
	finish.cur = finish.first + num_elements % buffer_size();

}
template <class T,class Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
	create_map_and_nodes(n);	// 产生deque结构
	map_pointer cur;
	__STL_TRY{
		// 为每个节点的缓冲区设定初值
		for (cur = start.node; cur < finish.node; ++cur) {
			uninitialized_fill(*cuf, *cur + buffer_size(), value);
		}
		// 尾端可能有备用空间
		uninitialized_fill(finish.first, finish.cur, value);
	}catch (...) {
		...
	}
	
}
template<class T,class Ref,class Ptr,size_t BufSiz>
struct __deque_iterator {
	typedef __deque_iterator<T, T&, BufSiz> iterator;
	typedef __dequeiterator<T, const T&, const T*, BufSiz> const_iterator;
	static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;

	typedef __deque_iterator self;

	T* cur;				// 指向现行元素
	T* first;			// 指向缓冲区头
	T* last;			// 指向缓冲区尾
	map_pointer node;	// 指向管控中心
	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n!= 0 ? n; (sz < 512 ? size_t(512 / sz) : size_t(1));
	}
	// 迭代器行进到缓冲区边缘时,跳过一个缓冲区
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

	//  迭代器关键: 各个运算子的重载
	difference_type operator-(const self& x)const {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);	// 切换到下一节点
			cur = first;		// 的第一个元素
		}
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}
	self& operator--() {
		if (cur == first) {
			set_node(node - 1);	// 切换到上一节点
			cur = last;		// 的最后一个元素
		}
		--cur
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}

	// 随机存取, 迭代器可以直接跳跃n个距离
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			// 目标位置在同一缓冲区
			cur += n;
		}
		else {
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n)const {
		self tmp = *this;
		return tmp += n;
	}
	self& operator-=(difference_type n) { return *this += -n; }
	self operator-(difference_type n)const {
		self tmp = *thos;
		return tmp -= n;
	}
	reference operator[](difference_type n)const { return *(*this + n); }


};
#endif