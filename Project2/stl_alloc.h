#pragma once
#ifndef _STL_ALLOC_H_
#define _STL_ALLOC_H_
#include<memory>
//这里定义一二级配置器,名为alloc
//在stl的allocator标准下
// 内存配置操作由allocate()负责,内存释放操作deallocate()负责
//对象构造操作由::construct()负责,对象析构操作由::destroy()负责
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;			//第一级配置器
#else
//第二级配置器
typedef __default_alloc_template<0 /*__NODE_ALLOCATOR_THEREADS*/ , 0> alloc;
#endif
#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC ceer<<"out of memory"<<endl; exit(1)
#endif
//为一二级配置器再包装一个接口,使其符合STL标准
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
//一个初值为0,有待客端设定的内存溢出处理函数指针
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
template<int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	void (*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();		//调用处理例程,企图释放内存
		result = malloc(n);			//再次尝试配置内存
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
		(*my_malloc_handler)();		//调用处理例程,企图释放内存
		result = realloc(p, n);		//再次尝试配置内存
	}	if (result)return(result);
}
typedef __malloc_alloc_template<0> malloc_alloc;

//第二级配置器
enum{__ALIGN=8};		//小型区块上调边界
enum{__MAX_BYTES=128};		//小型区块上限
enum{__NFREELISTS=__MAX_BYTES/__ALIGN};	//frea-list个数
template<bool threads,int inst>
class __default_alloc_template {
private:
	//将bytes上调至8的倍数
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+___ALIGN - 1) & ~(__ALIGN - 1));
	}

	union obj {
		union obj* free_list_link;	//指向下一个obj的指针
		//客端只能看见client_data
		char* client_data[1];
	};

	static obj* volatile free_list[__NFREELISTS];
	
	//根据区块的大小获取,其再链表的下标
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	//返回一个大小为n的对象,并可能加入大小为n的其他区块到freelist
	static void* refill(size_t n);

	//配置一大块空间,可容纳nobjs个大小为size区块
	//nobjs可能会降低
	static char* chunk_alloc(size_t size, int& nobjs);
	
	static char* start_free;	//内存池起始位置
	static char* end_free;		//内存池结束位置
	static size_t heap_size;	//堆大小

public:
	static void* allocate(size_t n) {
		obj* volatile* my_free_list;
		obj* result;

		//大于128则调用第一级配置器
		if (n > (size_t)__MAX_BYTES) {
			return (malloc_alloc::allocate(n));
		}
		//寻找合适的free-list
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0) {
			//没找到可用的free list,重新填充free list
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
		
		//寻找合适的free list回收内存
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	static void reallocate(void* p, size_t old_sz, size_t new_sz);
};
//第二配置器静态数据成员的定义与初值设定
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

//第二适配器的成员函数
//返回一个大小为n的对象,并肯为适当的free list 增加节点
template<bool threads,int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n) {
	int objs = 20;
	//调用chunk_alloc()尝试为nobjs个区块作为free list 的新节点
	//objs 按引用传参
	char* chunk = chunk_alloc(n, nobjs);
	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;

	//若只获得一个区块,该区块返回调用者,free list 无法增加新节点
	if (nobjs == 1)return(chunk);
	my_free_list = free_list + FREELIST_INDEX(n);

	//以下在chunk空间建立free list
	result = (obj*)chunk;	//这一块返回给苦短
	*my_free_list = next_obj = (obj*)(chunk + n);

	//将free list的各节点串接
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
	size_t bytes_left = end_free - start_free;		//内存池剩余空间
	
	
	if (bytes_left >= total_bytes) {
		//剩余空间完全满足需求
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if (bytes_left >= size) {
		//足够供应一个以上的区域
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else {
		//内存池一个区块的空间都无法提供
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		//让内存池中的残余内存还有利用价值
		if (bytes_left > 0) {
			//将剩余内存分配给合适的free list
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			//将剩余空间编入free list
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}

		//配置heap空间,用于内存池,即从堆中获取内存填进内存池
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free) {

			//malloc失败,heap空间不足
			//从其他的free list中回收空间到内存池
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

			end_free = 0;		//到处都没内存可用
			//调用第一级配置器,尝试使用out of memory机制
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			//可能抛出异常,或改善内存不足
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		//修正nobjs,递归调用自身
		return ((chunk_alloc)(size,objs))
	}

}
#endif