#pragma once
#ifndef _VECTOR_H_
#define _VECTIR_H_
#include"memory.h"
#include"stl_config.h"
template <class T,class Alloc=alloc>
class vector {
public:
	typedef T				value_type;
	typedef value_type*		pointer;
	typedef value_type*		iterator;
	typedef value_type		reference;
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;				// 使用空间头
	iterator finish;			// 使用空间尾
	iterator end_of_storage;	// 可用空间尾

	// member function
	void insert_aux(iterator position, const T& x);
	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}
	// 填充并予以初始化
	void fill__initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	// 配置空间并填满内容
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size()const { return size_type(end() - begin()); }
	size_type capacity()const {
		return size_type(end_of_storage - begin());
	}
	bool empty()const { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }
	vector() :start(0), finish(0), end_of_storage(0) {}
	vector(size_type n, const T& value) { fill__initialize(n, value); }
	vector(int n, const T& value) { fill__initialize(n, value); }
	vector(long n, const T& value) { fill__initialize(n, value); }
	explicit vector(size_type n) { fill_initialize(n, T()); }
	~vector() {
		destroy(start, finish);
		deallocate();
	}
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_back(const T& x) {
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else {
			insert_aux(end(), x);
		}
	}
	void pop_back() {
		--finish;
		destroy(finish);
	}
	iterator erase(iterator position) {
		// 消除某位置上的元素
		if (position + 1 != end()) {
			// 后续元素向前移动
			copy(position + 1, finish, position);
		}
		--finish;
		destroy(finish);
		return position;
	}
	void resize(size_type new_size, const T& x) {
		if (new_size < size()) {
			erase(begin() + new_size, end());
		}
		else {
			insert(end(), new_size - size(), x);
		}
	}
	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }
	void insert(iterator position, size_type n, const T& x);
};
template <class T,class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
	if (finish != end_of_storage) {
		// 还有备用空间
		// 在备用空间构造一个元素,以最后一个元素值作为初值
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;
		copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	}
	else {
		// 无备用空间
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		// 配置新的数据空间
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try {
			// 将原vector的内容拷贝到新vector
			new_finish = uninitialized_copy(start, position, new_start);
			// 为新元素设定初值x
			construct(new_finish, x);
			++new_finish;
			// 将原vector的备用空间的内容也拷贝
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...) {
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}

		// 析构释放原vector
		destroy(begin(), end());
		deallocate();

		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
	
}
template <class T,class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
	if (n != 0) {
		if (size_type(end_of_storage - finish) >= n) {
			// 备用空间 大于 新增元素个数
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				// 插入点之后的现有元素个数 大于 新增元素个数
				uninitialized_copy(finish - n, finish, finish);
				// vector 尾端标记后移
				finish += n;
				copy_backward(position, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else {
				// 插入点之后的现有元素个数 小于等于 新增元素个数
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else {
			// 备用空间 小于 新增元素个数
			const size_type old_size = size();
			const size_type len = old_size + max(old_size, n);

			// 配置新的vector空间
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			__STL_TRY{
				// 将旧vector的插入点之前的元素复制到新的空间
				new_finish = uninitialized_copy(start,position,new_start);
			// 将新增元素填入空间
			new_finish = uninitialized_fill_n(new_finish, n, x);
			// 将旧vector的插入点之后的元素复制到新空间
			new_finish = uninitialized_copy(position, finish, new_finish);
			}
# ifdef __STL_USE_EXCEPTIONS
				catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deaalocate(new_start, len);
				throw;
			}
# endif /* __STL_USE_EXCEPTIONS*/
		// 释放旧的vector
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;

		}

	}
}
#endif