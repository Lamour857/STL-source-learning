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
	// ָ��Ԫ��ָ���ָ��
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
			// ���mapβ�˱��ÿռ䲻��
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
	// ���������ź�deque�Ľṹ
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
			// �����������ʣ��Ԫ��
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
		// �����֮ǰ��Ԫ�ظ���
		difference_type index = pos - start;
		if (index < (size() >> 1)) {
			// �����֮ǰ��Ԫ�ؽ���
			// �ƶ������֮ǰ��Ԫ��
			copy_backward(start, pos, next);
			pop_front();
		}
		else {
			// �����֮���Ԫ�ؽ���
			// �ƶ������֮���Ԫ��
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
		// �������֮ǰԪ�ظ�������
		push_front(front());
		iterator front1 = start;
		++front1;
		iterator front2 = front1;
		++front2;
		pos = start + index;
		iterator pos1 = pos;
		++pos1;
		// Ԫ���ƶ�
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
		// �������������deque
		clear();
		return finish;
	}
	else {
		difference_type n = last - first;		// ������䳤��
		difference_type elems_before = first - start;	// �������ǰ����Ԫ�ظ���
		if (elems_before < (size() - n / 2)) {
			// ����ƶ�ǰ��Ԫ��
			copy_backward(start,first,last);
			iterator new_start = start + n;
			// ϴ������Ԫ��
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur) {
				// �ͷ�����ռ�
				data_alloctor::deallocate(*cur, buffer_size());
			}
			// ���������
			start = new_start;
		}
		else {
			// ��Ԫ����ǰ�ƶ�
			copy(last, finish, first);
			iterator new_finish = finish - n;
			// ��������Ԫ��
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur) {
				// �ͷ�����ռ�
				data_alloctor::deallocate(*cur, bufffer_size());
			}
			// �趨��β��
			finish = new_finish;
		}
		return start + elems_before;
	}
}
template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::clear() {
	// ���ͷβ�����ÿһ��������
	for (map_pointer node = strt.node + 1; node < finish.node; ++node) {
		destroy(*node, *node + buffer_size());
		// �ͷŻ������ڴ�
		data_allocator::deallocate(*node, buffer_size);
	}
	if (start.node != finish.node) {	// ������ͷβ����������
		// ��ͷ����������Ԫ������
		destroy(start.cur, start.last);
		// ��β����������Ԫ������
		destroy(finish.first, finish.cur);
		// �ͷ�β������
		data_allocator::deallocate(finish.first, buffer_size);
	}
	else {	//ֻ��һ��������
		// �����������ڵ�Ԫ��,���ͷſռ�
		destroy(strat.cur, finish.cur);
	}
	finish = start;
}

template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux() {
	// ����һ�������ĵ�һ��Ԫ������
	destroy(start.cur);
	deallocate_node(start.first);
	start.set_node(start.node + 1);
	start.cur = start.first;
}
template<class T,class Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux() {
	// �ͷ����һ��������
	deallocate_node(finish.first);
	finish.set_node(finish.node - 1);
	finish.cur = fiinsh.last - 1;
	destroy(finish.cur);

}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
	value_type t_copy = t;
	// �ж��Ƿ���Ҫ����map
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
		// �������ռ����������Ҫ�Ľڵ���
		// ���ƶ��ռ��ڵ�Ԫ�أ�ʹ���˾��п��пռ�
		new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		if (new_nstart < start.node)
			copy(start.node, finish.node + 1, new_nstart);
	}
	else {
		size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
		// ����һ���¿ռ�
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		// copy ԭmap����
		copy(start.node, finish.node + 1, new_nstart);
		// �ͷ�ԭmap
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
	// ����ĳ������������ػ�һ��map
	reserve_map_at_back();
	// �����»�����+
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
	// �ڵ���
	size_type num_nodes = num_elements / buffer_size() + 1;
	
	// map����Ľڵ��������ʱ��Ҫ�Ľڵ���+2��ǰ�������ã�
	map_size = max(initial_map_size(), num_nodes + 2);
	map = map_allocator::allocator(map_size);

	// ���ó�����map_size���ڵ��map

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
	create_map_and_nodes(n);	// ����deque�ṹ
	map_pointer cur;
	__STL_TRY{
		// Ϊÿ���ڵ�Ļ������趨��ֵ
		for (cur = start.node; cur < finish.node; ++cur) {
			uninitialized_fill(*cuf, *cur + buffer_size(), value);
		}
		// β�˿����б��ÿռ�
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

	T* cur;				// ָ������Ԫ��
	T* first;			// ָ�򻺳���ͷ
	T* last;			// ָ�򻺳���β
	map_pointer node;	// ָ��ܿ�����
	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n!= 0 ? n; (sz < 512 ? size_t(512 / sz) : size_t(1));
	}
	// �������н�����������Եʱ,����һ��������
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

	//  �������ؼ�: ���������ӵ�����
	difference_type operator-(const self& x)const {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);	// �л�����һ�ڵ�
			cur = first;		// �ĵ�һ��Ԫ��
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
			set_node(node - 1);	// �л�����һ�ڵ�
			cur = last;		// �����һ��Ԫ��
		}
		--cur
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}

	// �����ȡ, ����������ֱ����Ծn������
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			// Ŀ��λ����ͬһ������
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