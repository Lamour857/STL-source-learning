//���ļ�ʵ��һ���򵥵�stl�ռ�������,����һЩ��Ҫ�ӿ�
//STL�淶�£�allocator�ı�Ҫ�ӿ�
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
		//��
		//Ϊʲô��ʹ��::
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
		//Ϊʲôʹ��::
		::operator delete(buffer);
	}
	template<class T1, class T2>
	inline void _construct(T1 p, const T2& value) {
		//placement new,��p��ָ�Ŀռ乹��value
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
		//���ڱ�֤ͬһ����������ָ��Ĳ��
		typedef ptrdiff_t	difference_type;
		template<class U>
		struct rebind {
			typedef allocator<U> other;
		};
		//default constructor
		allocator();
		//copy cicnstructor
		allocator(const allocator&);
		//����constructor
		template <class U>allocator(const allocator<U>& other);
		// default constructor
		~allocator();
		//����const����ĵ�ַ
		pointer address(reference x)const {
			return (pointer)&x;
		}
		//����const����ĵ�ַ
		const_pointer const_address(const_reference x)const {
			return (const_pointer)&x;
		}
		//���ÿռ�
		pointer allocate(size_type n, const void *= 0) {
			_allocate((difference_type)n, (pointer)0);
		}
		//�黹���õĿռ�
		void deallocate(pointer p, size_type n) { _deallocate(p) }
		//��pָ��Ŀռ乹��value
		void construct(pointer p, const T& value) {
			_construct(p, value);
		}
		//ֻ����o���������������ͷ���placement new ���ڴ�
		void destroy(pointer p) {
			_destroy(p);
		}
		//���ؿ����õ������
		size_type max_size()const {
			return size_type(UINT_MAX / sizeof(T))
		}
	};

}// end of namespace mystl
#endif


