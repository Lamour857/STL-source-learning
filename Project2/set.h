#pragma once
#ifndef SET_H_
#define SET_H_
#include"memory.h"
#include<utility>	// for pair
#include<functional>
#include"rb_tree.h"
#include"stl_config.h"
using std::pair;
using std::less;
template<class Key,class Compare=less<Key>,class Alloc=alloc>
class set {
public:
	//typedefs
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;		// �����ʵ�� set
public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	// iterator ����Ϊconst_iterator, �������޷�����д�����
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_itertor reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	// allocation/deallocation
	set() :t(Compare()) {}
	explicit set(const Compare& comp) :t(comp) {}

	template<class InputIterator>
	set(InputIterator first, InputIterator last) : t(Compare()) {
		t.insert_unique(first, last);
	}

	template<class InputIterator>
	set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) {
		t.insert_unique(first, last);
	}
	set(const set<Key, Compare, Alloc>& x) :t(x.t) {}


	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	// set ����ΪRB-tree���Ѿ��ṩ
	// accessors
	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const { return t.key_comp(); }
	iterator begin()const { return t.begin(); }
	iterator end()const { return t.end(); }
	reverse_iterator rbegin()const { return t.rbegin(); }
	reverse_iterator rend()const { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

	// insert/erase
	// ���뵥��Ԫ�� x
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool>p = t.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}
	// ָ��λ�ò���Ԫ�� x
	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;	
		// �Լ�д��rb_tree�в�����������ذ汾
		return t.insert_unique((rep_iterator&)position, x);
	}
	// ����[first,last)���Ԫ��
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	// ɾ����������ָԪ��
	void erase(iterator position) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	// ɾ����ֵΪ x ��Ԫ��
	size_type erase(const key_type& x) {
		return t.erase(x);
	}
	// ɾ��[first,last)֮���Ԫ��
	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}
	void clear() { t.clear(); }

	// set operations
	iterator find(const key_type& x)const { return t.find(x); }
	size_type count(const key_type& x)const { return t.count(x); }
	iterator lower_bound(const key_type& x)const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x)const {
		return t.upper_bound(x);
	}
	friend bool operator== __STL_NULL_TMPL_ARGS(const set&, const set&);
	friend bool operator< __STL_NULL_TMPL_ARGS(const set&, const set&);
};
template<class Key,class Compare,class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t == y.t;
}
template<class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t < y.t;
}
#endif
