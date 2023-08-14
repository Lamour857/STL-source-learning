#pragma once
#ifndef _STL_ALLOC_H_
#define _STL_ALLOC_H_
#include<memory>
//���ﶨ��һ����������,��Ϊalloc
//��stl��allocator��׼��
// �ڴ����ò�����allocate()����,�ڴ��ͷŲ���deallocate()����
//�����������::construct()����,��������������::destroy()����
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;			//��һ��������
#else
//�ڶ���������
typedef __default_alloc_template<0 /*__NODE_ALLOCATOR_THEREADS*/ , 0> alloc;
#endif
#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC ceer<<"out of memory"<<endl; exit(1)
#endif
//Ϊһ�����������ٰ�װһ���ӿ�,ʹ�����STL��׼
template<class T,class Alloc>
class simple_alloc {
public:
	static T* allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void) {
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (0 != n)Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p) {
		Alloc::deallocate(p, sizeof(T));
	}
};
template<int inst>
class __malloc_alloc_template {
private:
	// out of memory:oom
	static void *oom_malloc(size_t);
	static void *oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t n) {
		void* result = malloc(n);
		if (0 == result) result = oom_malloc(n);
		return result;
	}
	static void deallocate(void* p, size_t /* n */) {
		free(p);
	}
	static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz) {
		void* result = realloc(p, new_sz);
		if (0 == result)result = oom_realloc(p, new_sz);
		return result;
	}
	static void (*set_malloc_handler(void (*f)()))(){
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};
//malloc_alloc out of memory handling
//һ����ֵΪ0,�д��Ͷ��趨���ڴ����������ָ��
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
template<int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	void (*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();		//���ô�������,��ͼ�ͷ��ڴ�
		result = malloc(n);			//�ٴγ��������ڴ�
		if (result)return (result);
	}
}
template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
	void(*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC;}
		(*my_malloc_handler)();		//���ô�������,��ͼ�ͷ��ڴ�
		result = realloc(p, n);		//�ٴγ��������ڴ�
	}	if (result)return(result);
}
typedef __malloc_alloc_template<0> malloc_alloc;

//�ڶ���������
enum{__ALIGN=8};		//С�������ϵ��߽�
enum{__MAX_BYTES=128};		//С����������
enum{__NFREELISTS=__MAX_BYTES/__ALIGN};	//frea-list����
template<bool threads,int inst>
class __default_alloc_template {
private:
	//��bytes�ϵ���8�ı���
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+___ALIGN - 1) & ~(__ALIGN - 1));
	}

	union obj {
		union obj* free_list_link;	//ָ����һ��obj��ָ��
		//�Ͷ�ֻ�ܿ���client_data
		char* client_data[1];
	};

	static obj* volatile free_list[__NFREELISTS];
	
	//��������Ĵ�С��ȡ,����������±�
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	//����һ����СΪn�Ķ���,�����ܼ����СΪn���������鵽freelist
	static void* refill(size_t n);

	//����һ���ռ�,������nobjs����СΪsize����
	//nobjs���ܻή��
	static char* chunk_alloc(size_t size, int& nobjs);
	
	static char* start_free;	//�ڴ����ʼλ��
	static char* end_free;		//�ڴ�ؽ���λ��
	static size_t heap_size;	//�Ѵ�С

public:
	static void* allocate(size_t n) {
		obj* volatile* my_free_list;
		obj* result;

		//����128����õ�һ��������
		if (n > (size_t)__MAX_BYTES) {
			return (malloc_alloc::allocate(n));
		}
		//Ѱ�Һ��ʵ�free-list
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0) {
			//û�ҵ����õ�free list,�������free list
			void* r = refill(ROUND_UP(n));
			return r;
		}

		*my_freee_list = result->free_list_link;
		return (result);

	}

	static void deallocate(void* p, size_t n) {
		obj* q = (obj*)p;
		obj* volatile* my_free_list;

		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}
		
		//Ѱ�Һ��ʵ�free list�����ڴ�
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	static void reallocate(void* p, size_t old_sz, size_t new_sz);
};
//�ڶ���������̬���ݳ�Ա�Ķ������ֵ�趨
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
__default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] =
{ 0,0,0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };

//�ڶ��������ĳ�Ա����
//����һ����СΪn�Ķ���,����Ϊ�ʵ���free list ���ӽڵ�
template<bool threads,int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n) {
	int objs = 20;
	//����chunk_alloc()����Ϊnobjs��������Ϊfree list ���½ڵ�
	//objs �����ô���
	char* chunk = chunk_alloc(n, nobjs);
	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;

	//��ֻ���һ������,�����鷵�ص�����,free list �޷������½ڵ�
	if (nobjs == 1)return(chunk);
	my_free_list = free_list + FREELIST_INDEX(n);

	//������chunk�ռ佨��free list
	result = (obj*)chunk;	//��һ�鷵�ظ����
	*my_free_list = next_obj = (obj*)(chunk + n);

	//��free list�ĸ��ڵ㴮��
	for (i = 1;; i++) {
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i) {
			current_obj->free_list_link = 0;
			break;
		}
		else {
			current_obj->free_list_link = next_obj;
		}
	}
	return (result);
}

template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;		//�ڴ��ʣ��ռ�
	
	
	if (bytes_left >= total_bytes) {
		//ʣ��ռ���ȫ��������
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if (bytes_left >= size) {
		//�㹻��Ӧһ�����ϵ�����
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else {
		//�ڴ��һ������Ŀռ䶼�޷��ṩ
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		//���ڴ���еĲ����ڴ滹�����ü�ֵ
		if (bytes_left > 0) {
			//��ʣ���ڴ��������ʵ�free list
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			//��ʣ��ռ����free list
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}

		//����heap�ռ�,�����ڴ��,���Ӷ��л�ȡ�ڴ�����ڴ��
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free) {

			//mallocʧ��,heap�ռ䲻��
			//��������free list�л��տռ䵽�ڴ��
			int i;
			obj* volatile* my_free_list, * p;
			for (i = size; i < __MAX_BYTES; i += __ALIGN) {
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p) {
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return(chunk_alloc(size, nobjs));
				}
			}

			end_free = 0;		//������û�ڴ����
			//���õ�һ��������,����ʹ��out of memory����
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			//�����׳��쳣,������ڴ治��
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		//����nobjs,�ݹ��������
		return ((chunk_alloc)(size,objs))
	}

}
#endif