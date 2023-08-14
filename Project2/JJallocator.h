//该文件实现一个简单的stl空间配置器,仅有一些必要接口
//STL规范下，allocator的必要接口
#pragma once
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
#include<new>			//for placement new
#include<cstddef>		//for ptrdiff_t,size_t
#include<iostream>		//for cerr
#include<cstdlib>		//for exit()
#include<climits>		//for UINT_MAX
namespace jj {
	
	template <class T>
	inline T* _allocate(ptrdiff_t size, T*) {
		set_new_handler(0);
		//？
		//为什么会使用::
		T* temp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0) {
			cerr << "out of memory" << endl;
			exit(1);
		}
		return tmp;
	}
	template <class T>
	inline void _deallocate(T* buffer) {
		//?
		//为什么使用::
		::operator delete(buffer);
	}
	template<class T1, class T2>
	inline void _construct(T1 p, const T2& value) {
		//placement new,在p所指的空间构造value
		new(p) T1(value)
	}
	template<class T>
	inline void _destroy(T* ptr) {
		ptr->~T();
	}
	
	template<class T>
	class allocator {
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		//用于保证同一数组中两个指针的差距
		typedef ptrdiff_t	difference_type;
		template<class U>
		struct rebind {
			typedef allocator<U> other;
		};
		//default constructor
		allocator();
		//copy cicnstructor
		allocator(const allocator&);
		//泛化constructor
		template <class U>allocator(const allocator<U>& other);
		// default constructor
		~allocator();
		//返回const对象的地址
		pointer address(reference x)const {
			return (pointer)&x;
		}
		//返回const对象的地址
		const_pointer const_address(const_reference x)const {
			return (const_pointer)&x;
		}
		//配置空间
		pointer allocate(size_type n, const void *= 0) {
			_allocate((difference_type)n, (pointer)0);
		}
		//归还配置的空间
		void deallocate(pointer p, size_type n) { _deallocate(p) }
		//在p指向的空间构造value
		void construct(pointer p, const T& value) {
			_construct(p, value);
		}
		//只调用o的析构函数，不释放其placement new 的内存
		void destroy(pointer p) {
			_destroy(p);
		}
		//返回可配置的最大量
		size_type max_size()const {
			return size_type(UINT_MAX / sizeof(T))
		}
	};

}// end of namespace mystl
#endif


