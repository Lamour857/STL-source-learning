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
	iterator start;				// ʹ�ÿռ�ͷ
	iterator finish;			// ʹ�ÿռ�β
	iterator end_of_storage;	// ���ÿռ�β

	// member function
	void insert_aux(iterator position, const T& x);
	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}
	// ��䲢���Գ�ʼ��
	void fill__initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	// ���ÿռ䲢��������
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
		// ����ĳλ���ϵ�Ԫ��
		if (position + 1 != end()) {
			// ����Ԫ����ǰ�ƶ�
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
		// ���б��ÿռ�
		// �ڱ��ÿռ乹��һ��Ԫ��,�����һ��Ԫ��ֵ��Ϊ��ֵ
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;
		copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	}
	else {
		// �ޱ��ÿռ�
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		// �����µ����ݿռ�
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try {
			// ��ԭvector�����ݿ�������vector
			new_finish = uninitialized_copy(start, position, new_start);
			// Ϊ��Ԫ���趨��ֵx
			construct(new_finish, x);
			++new_finish;
			// ��ԭvector�ı��ÿռ������Ҳ����
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...) {
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}

		// �����ͷ�ԭvector
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
			// ���ÿռ� ���� ����Ԫ�ظ���
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				// �����֮�������Ԫ�ظ��� ���� ����Ԫ�ظ���
				uninitialized_copy(finish - n, finish, finish);
				// vector β�˱�Ǻ���
				finish += n;
				copy_backward(position, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else {
				// �����֮�������Ԫ�ظ��� С�ڵ��� ����Ԫ�ظ���
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else {
			// ���ÿռ� С�� ����Ԫ�ظ���
			const size_type old_size = size();
			const size_type len = old_size + max(old_size, n);

			// �����µ�vector�ռ�
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			__STL_TRY{
				// ����vector�Ĳ����֮ǰ��Ԫ�ظ��Ƶ��µĿռ�
				new_finish = uninitialized_copy(start,position,new_start);
			// ������Ԫ������ռ�
			new_finish = uninitialized_fill_n(new_finish, n, x);
			// ����vector�Ĳ����֮���Ԫ�ظ��Ƶ��¿ռ�
			new_finish = uninitialized_copy(position, finish, new_finish);
			}
# ifdef __STL_USE_EXCEPTIONS
				catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deaalocate(new_start, len);
				throw;
			}
# endif /* __STL_USE_EXCEPTIONS*/
		// �ͷžɵ�vector
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;

		}

	}
}
#endif