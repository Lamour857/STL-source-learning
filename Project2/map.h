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
	// �ڲ���(functor) value_compare, ���ڵ���Ԫ�رȽϺ���
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
	// iterator ����Ϊconst_iterator, �������޷�����д�����
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
	
	// ʡ�Ի���accessors, ������Ҫaccessors
	T& operator[](const key_type& k) {
		// ʵֵδ֪������һ������ʵֵ�ͱ���ͬ��
		// ��ʱ����value_type(K,T())����
		// ����Ԫ�ز���map, insert(value_type(k,T()))
		// ����ʱ����pair, ��һԪ��ʱ������,
		// ָ������ʵ�����Ԫ��,���ظ��ľ�Ԫ��
		// ȡ��pair�ĵ�һԪ��(insert(value_type(k,T()))).first
		// ��һԪ��ʱ������, ����õ����� *((insert(value_type(k,T()))).first)
		// �������mapԪ��, ��һ��pair,ȡ��ڶ�Ԫ�ػ����ʵֵ
		// (*((insert(value_type(k,T()))).first)).second
		return (*((insert(value_type(k, T()))).first)).second;
	}

};

#endif
