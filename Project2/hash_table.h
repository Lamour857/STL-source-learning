#pragma once
#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include"stl_iterator.h"
#include<cstddef>
#include"stl_config.h"
#include"memory.h"
#include<utility>
#include<vector>
using std::pair;
using std::vector;
// ��ϣ����ά��������Ľڵ�
template<class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};
// hashtable �ĵ�����
template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	// ������Ŀǰ��ָ�ڵ�
	node* cur;
	hashtable* ht;

	__hashtable_iterator(node* n,hashtable* tab):cur(nn),ht(tab){}
	__hashtable_iterator(){}
	reference operator*()const { return cur->val; }
	pointer operator->()const { return &(operator*()); }
	iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {		// �ߵ�����β��
			// ����Ԫ��ֵ��λ��һ��bucket
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->bucket.size()) {
				cur = ht->buket[bucket];
			}
		}
		return *this;
	}
	iterator operator++(int) {
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	bool operator==(const iterator& it)const { return cur == it.cur; }
	bool operator!=(const iterator& it)const { return cur != it.cur; }

};

// hashtable �����ݽṹ
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc=alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;

private:
	// �������߾�Ϊ function objects, <stl_hash_fun.h>�ж���������׼�ͱ�
	hasher hash;
	key_equal equls;
	ExtractKey get_key;

	typedef __hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc> node_allocator;

	vector<node*, Alloc> buckets;
	size_type num_elements;
	// �ڴ����
	node* new_node(const vlue_type& obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY{
			construct(&n->val,obj);
		    return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}
	void delete_node(node* n) {
		destroy(&n->val);
		node_allocator::deallocate(n);
	}
	// constructor
	hashtable(size_type n, const HashFcn& hf, const EualKey& eql):hash(hf),equals(eql),get_key(ExtractKey()),num_elements(0) {
		initiallize_bucket(n);
	}
	void initialize_bucket(n) {
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}
	size_type next_size(size_type n)const { return __stl_net_prime(n); }
	void resize(size_type num_elements_hint) {
		// �������Ԫ�غ��Ԫ�ظ�������vector�Ĵ�С�����ؽ����
		// bucket(list)������vector��С��ͬ
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n) {
			// ��Ҫ��������
			const size_type n = next_size(num_elements_hint);
			if (n > old_n) {
				// �����µ�buckets
				vector<node*, A>tmp(n, (node*)0);
				__STL_TRY{
					for (size_type nucket = 0; bucket < old_n,++bucket) {
						node* first = buckets[bucket];
						while (first) {
							// ÿһ��bucket�е�ÿһ���ڵ㶼��Ҫ���¼���λ��
							// �ҳ��ڵ������ĸ���bucket
							size_type new_bucket = bkt_bum(first->val, n);

							// ���bucketָ���е���һ���ڵ�
							buckets[bucket] = first->next;
							// �½ڵ������bucket�ڣ���Ϊ��Ӧ���еĵ�һ���ڵ�
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							// �����bucket����һ���ڵ�
							first = buckets[bucket];
						}
					}
				// �¾�buckets�Ե�
				buckets.swap(tmp);
				}
			}
		}	
	}
	pair<iterator, bool> insert_uniquue_noresize(const value_type& obj) {
		// ��Ԫ��λ�ڵ�n��bucket
		const size_type n = bkt_num(obj);
		node* first = buckets[n];

		for (node* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				// ���ҵ���ͬ����Ԫ��,ֱ�ӷ���
				return pair<iterator, bool>(iterator(cur, this), false);
			}
		}
		// �����½ڵ�
		node* tmp = new_node(obj);
		// ��ýڵ��Ϊ����ĵ�һ���ڵ�
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}
	iterator insert_equal_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];

		for (node* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				node* tmp = new_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}
		node* tmp = new_node(obj);
		tmp->next = first;
		cur->next = tmp;
		++num_elements;
		return iterator(tmp, this);
	}
	size_type bkt_num(const value_type obj, size_t n)const {
		return bkt_num_key(get_key(obj), n);
	}
	size_type bkt_num(const value_type& obj)const {
		return bkt_num_key(get_key(obj));
	}
	size_type bkt_num_key(const key_type& key)const {
		return bkt_num_key(key, buckets.size());
	}
	size_type bkt_num_key(const key_type& key, size_t n)const {
		return hash(key) % n;
	}
public:
	size_type bucket_count()const { return bucket.size(); }
	size_type max_bucket_count()const {
		return __stl_prime_list[__stl_num_primes - 1];
	}
	pair<iterator, bool> insert_unique(const value_type& obj) {
		// �ж��Ƿ���Ҫ�ؽ����
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}
	void clear() {
		for (size_type i = 0; i < buckets.size(); ++i) {
			node* cur = buuckets[i];
			while (cur != 0) {
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;		// bucket����Ϊnullָ��

		}
		num_elements = 0;
	}
	void copy_from(const hashtable& ht) {
		// ����������е�����
		buckets.clear();
		// Ԥ���Է�bucket��С
		buckets.reserve(ht.buckets.size());
		// �򼺷�buckets���nullָ��
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
		__STL_TRY{
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				if (const node* cur = ht.buckets[i]) {
					node* copy = new_node(cur->val);
					buckets[i] = copy;
					
					// ��������
					for (node* next = cur->next; next; cur = next; next = cur->next) {
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;
		}
		__STL_UNWIND(clear());
	}
	iterator find(const key_tye& key) {
		size_type n = bkt_num_key(key);
		node* first;
		for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next) {

		}
		return iterator(first, this);
	}
	size_type count(const key_type& key)const {
		const size_type n = bkt_num_key(key);
		size_type result = 0;
		for (const node* cur = buckets[n]; cur; cur = cur->next) {
			if (equals(get_key(cur->val), key)) {
				++result;
			}
			return result;
		}
	}
};
static const int __stl_num_primes = 28;
// ������δ�г�ȫ��
static const unsigned long __stl_prime_list[__stl_num_primes] = {
	53,97,193,389,769,1543
};
inline unsigned long __stl_next_prime(unsigned long n) {
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;
	const unsigned long* pos = lower_bound(first, last, n);
	// lower_bound(),�����㷨
	return pos == last ? *(last - 1) : *pos;
}
#endif