#pragma once
#ifndef MAP_H_
#define MAP_H_
#include"memory.h"
#include<functional>
#include<utility>
using std::pair;
using std::less;
template<class Key,class T,class Compare=less<Key>,class Alloc=alloc>
class map {
public:
	// typedefs
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const Key, T> value_type;
	typedef Compare key_compare;
	// 内部类(functor) value_compare, 用于调用元素比较函数
	class value_compare :public binary_function<value_type, value_type, bool> {
		friend class map<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c):comp(c){}
	public:
		bool operator()(const value_type& x,cinst value_type& y)const{
			return comp(x.first, y.first);
		}
	};
private:
	typedef rb_tree<key_type, data_type, selectlst<value_type>, key_compare, Alloc> rep_type;
	rep_type t;
public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	// iterator 设置为const_iterator, 迭代器无法进行写入操作
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::reverse_itertor reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	
	// allocation/deallocation
	map():t(Compare()){}
	explicit map(const Compare& comp):t(comp){}

	template<class InputIterator>
	map(InputIterator first, InputIterator last):t(Compare()){
		t.insert_unique(first, last);
	}
	template<class InputIterator>
	map(InputIterator first, InputIterator last, const Compare& comp) : t(comp) {
		t.insert_unique(first, last);
	}
	map(const map<Key,T,Compare,Alloc>& x):t(x.t){}
	map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}
	
	// 省略基本accessors, 保留重要accessors
	T& operator[](const key_type& k) {
		// 实值未知，产生一个于与实值型别相同的
		// 临时对象value_type(K,T())代替
		// 将该元素插入map, insert(value_type(k,T()))
		// 插入时返回pair, 第一元素时迭代器,
		// 指向插入适当的新元素,或重复的旧元素
		// 取出pair的第一元素(insert(value_type(k,T()))).first
		// 第一元素时迭代器, 提领该迭代器 *((insert(value_type(k,T()))).first)
		// 提领后获得map元素, 是一个pair,取其第二元素获得其实值
		// (*((insert(value_type(k,T()))).first)).second
		return (*((insert(value_type(k, T()))).first)).second;
	}

};

#endif
