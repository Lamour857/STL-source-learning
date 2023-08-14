#pragma once
#ifndef RB_TREE_H_
#define RB_TREE_H_
#include"stl_iterator.h"
#include"memory.h"
#include"stl_config.h"
#include <utility>
using std::pair;
typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;

	color_type color;	// �Ǻڼ���
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while (x->left != 0)x = x->left;
		return x;
	}
	static base_ptr maximum(base_ptr x) {
		while (x->right != 0)x = x->right;
		return x;
	}
};

template<class Value>
struct __rb_tree_node :public __rb_tree_node_base {
	typedef __rb_tree_node<Value>* link_type;
	Value value_field;
};

// ���������
struct __rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;

	void increment() {
		if (node->right != 0) {
			// (1)
			// ��������ӽڵ�
			node = node->right;
			// ��������
			while (node->left != 0) {
				// �ҵ�������������ߵĽڵ�
				node = node->left;
			}
		}
		else {
			// (2)
			// û��������
			base_ptr y = node->parent;
			while (node == y->right) {
				// ����ֱ����Ϊ���ӽڵ�
				node = y;
				y = y->parent;
			}
			if (node->right != y) {
				node = y;
			}
		}
	}
	void decrement() {
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			node = node->right;
			// �������������modeΪheaderʱ����nodeΪend����
		}
		else if (node->left != 0) {
			base_ptr y = node->left;
			while (y->right != 0) {
				y = y->right;
			}
			node = y;
		}
	}
};

// ���������
template <class Value,class Ref,class Ptr>
struct __rb_tree_iterator :public __rb_tree_base_iterator {
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
	typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
	typedef __rb_tree_iterator<Value, Ref, Ptr> self;
	typedef __rb_tree_node<Value>* link_type;

	__rb_tree_iterator(){}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator& it) { node = it.node; }

	reference operator*()const { return link_type(node)->value_field; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
	pointer operator->()const { return &(operator*()); }
#endif
	
	self& operator++() { increment(); return *this; }
	self operator++(int) {
		self tmp = *this;
		increment();
		return tmp;
	}
	self& operator--() { decrement(); return *this; }
	self operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
};
template<class Key,class Value,class KeyofValue,class Compare,class Alloc=alloc>
class rb_tree{
protected:
	typedef void* void_pointer;
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<Value> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	typedef __rb_tree_color_type color_type;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	// ָ��ڵ��ָ�� link_type
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

	link_type create_node(const value_type& x) {
		// ���ÿռ�
		link_type tmp = get_node();
		__STL_TRY{
			construct(&tmp->value_field,x);
		}
		__STL_UNWIND(put_node(tmp));
		return tmp;
	}

	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}

	void destroy_node(link_type p) {
		destroy(&p->vale_field);
		put_node(p);
	}
protected:
	size_type node_count;
	link_type header;	// ʵ�ּ���
	Compare key_compare;

	link_type& root()const { return (link_type&)header->parent; }
	link_type& leftmost()const { return (link_type&)header->left; }
	link_type& rightmost()const { return (link_type&)header->right; }

	// ����6��������ȡ�ýڵ�x�ĳ�Ա
	static link_type& left(link_type x) {
		return (link_type&)(x->left);
	}
	static link_type& right(link_type x) {
		return (link_type&)(x->right);
	}
	static link_type& parent(link_type x) {
		return (link_type&)(x->parent);
	}
	static reference value(link_type x) {
		return x->value_field;
	}
	static const Key& key(link_type x) {
		return KeyofValue()(value(x));
	}
	static color_type& color(link_type x) {
		return (color_type&)(x->color);
	}
	// ����6��������ȡ�ýڵ�x�ĳ�Ա
	static link_type& left(base_ptr x) {
		return (link_type&)(x->left);
	}
	static link_type& right(base_ptr x) {
		return (link_type&)(x->right);
	}
	static link_type& parent(base_ptr x) {
		return (link_type&)(x->parent);
	}
	static reference value(base_ptr x) {
		return x->value_field;
	}
	static const Key& key(base_ptr x) {
		return KeyofValue()(value(link_type(x)));
	}
	static color_type& color(base_ptr x) {
		return (color_type&)(link_type(x)->color);
	}

	// ��ֵ
	static link_type minimum(link_type x) {
		return (link_type)__rb_tree_node_base::minimum(x);
	}
	static link_type maximum(link_type x) {
		return (link_type)__rb_tree_node_base::maximum(x);
	}

public:
	typedef __rb_tree_iterator<value_type, reference, pointer>iterator;

private:
	iterator __insert(base_ptr x, base_ptr y, const value_type& v);
	link_type __copy(link_type x, link_type p);
	void __erase(link_type x);
	void init() {
		// ����һ���ڵ�ռ���headerָ����
		header = get_node();
		// ��ɫ������header��root
		color(header) = __rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
public:

	rb_tree(const Compare& comp=Compare()):node_count(0), key_compare(comp) {
		init();
	}
	~rb_tree() {
		clear();
		put_node(header);
	}

	rb_tree<Key, Value, KeyofValue, Compare, Alloc>&
		operator=(const rb_tree<Key, Value, KeyofValue, Compare, Alloc>& x);

public:
	Compare key_comp()const { return key_comp; }
	iterator begin() { return leftmost(); }
	iterator end() { return header; }
	iterator find(const Key& k);
	bool empty()const { return node_count == 0; }
	size_type size()const { return node_count; }
	size_type max_size()const { return size_type(-1); }

public:
	// ������ֵ��������ڵ��ֵ�ظ�
	// ����pair����һ��Ԫ��ʱrb-tree������ָ�������ڵ�
	// �ڶ�Ԫ�ر�ʾ����ɹ����
	pair<iterator, bool>insert_unique(const value_type& x) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {
			y = x;
			comp = key_compare(KeyofValue()(v), key(x));
			x = comp ? left(x) : right;
			// ����������С������
		}

		// y ��ָ�������ĸ��ڵ㣨Ҷ�ڵ㣩
		iterator j = iterator(y);
		if (comp) {
			// comp Ϊ���ʾӦ�ò��������
			if (j == begin())
				// ��������ĸ��ڵ�������ڵ�
				return pair<iterator, bool>(__insert(x, y, v), true);
			else
				// ����㲻������ڵ�
				--j;
		}
		if (key_compare(key(j.node), KeyofValue()(v))) {
			// С����ֵ�����������Ҳ�
			return pair<iterator, bool>(__insert(x, y, v), true);
		}

		return pair<iterator, bool>(j, false);
	}
// ������ֵ������ڵ��ֵ�ظ�
// ���صĵ�����ָ�������ڵ�
	iterator insert_equal(const value_type& x) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			// Ѱ���ʵ��Ĳ����
			y = x;
			x = key_compare(KeyofValue()(v), key(x)) ? left(x) : right(x);
			// ����������С������
		}
		return __insert(x, y, v);
		// x ��ֵ����㣬yΪ�����ĸ��ڵ㣬vΪ��ֵ
	}
// �����Ĳ���ִ�г���
	iterator __insert(base_ptr x_, base_ptr y_, const Value& v) {
		// x_ Ϊ��ֵ����㣬y_ Ϊ��ֵ���븸�ڵ�,v Ϊ��ֵ
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		
		// key_compare ��Ϊ��С�Ƚ�׼��,�Ǹ�function object
		if (y == header || x != 0 || key_compare(KeyofValue()(v), key(y))) {
			// �����½ڵ�
			z = create_node(v);
			// ��yΪheaderʱ,leftmost()=z
			left(y) = z;
			if (y == header) {
				root() == z;
				rightmost() = z;
			}
			else if (y == leftmost()) {
				leftmost() = z;
			}
		}
		else {
			z = create_node(v);
			// �½ڵ��Ϊ�����֮��y�����ӽڵ�
			right(y) = z;
			if (y == rightmost()) {
				rightmost() = z;
			}
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;

		__rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}
};
inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	x->color = __rb_tree_red;
	while (x != root && x->parent->color == __rb_tree_red) {
		// x���Ǹ��ڵ�, ��x�ĸ��ڵ��Ǻ�ɫ,����Υ�����������
		// ��Ҫ����
		if (x->parent == x->parent->parent->left) {
			// ���ڵ����游�ڵ����ڵ�
			// y Ϊ����ڵ�
			__rb_tree_node_base* y = x->parent->parent->right;
			if (y && y->color == __rb_tree_red) {
				// ����ڵ����, ������ɫ�Ǻ�ɫ
				/*  rb_tree_insert_case_4  */
				//		|					   |
				//      G(B)				   G(R)
				//     / \    ==========>     / \
				// (R)P   U(R)			  (B)P   U(B)
				//   /				     	/     
				//  X(R)				   X(R)
				x->parent->color = __rb_tree_black;	// ���ڵ��޸�Ϊ��
				y->color = __rb_tree_black;			// ����ڵ��޸�Ϊ��
				x->parent->parent->color = __rb_tree_red;	// �游�ڵ��޸�Ϊ��
				x = x->parent->parent;		// ��ǰ��Ҫ�����Ľڵ��趨Ϊ�游�ڵ�
			}
			else {
				// ������ڵ㣬������ڵ�Ϊ��
				if (x == x->parent->right) {
				// ����½ڵ�Ϊ���ڵ���ҽڵ�
				/*  rb_tree_insert_case_5  */
				//      |                      
				//	    G(B)
				//     / \
				// (R)P   U(B)
				//     \	   
				//      X(R) 
					x = x->parent;
					// �����ڵ�����
					__rb_tree_rotate_left(x, root);
				// �������Խ�case 5 ת����case 6
				// ִ�������ϴ������ڵ�״̬����
				//      |
				//	    G(B)
				//     / \
				// (R)P   U(B)
				//   / 	   
				//  X(R)  
				}
				/*  rb_tree_insert_case_6  */
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(x->parent->parent, root);
				// ִ�������ϴ������ڵ�״̬����
				//     |
				//     P(B)
				//	  / \
				//(R)X   G(R)
				//        \
				//         U(B)
			}	
		}// while ����
		root->color = __rb_tree_black;
	}

}
// ����
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	// x Ϊ��ת��
    // clang-format off
	//     |                       |
	//     X                       Y
	//    / \     L-rotate(N)     / \
	//   L   Y    ==========>    X   R
	//      / \                 / \
	//     M   R               L   M
	__rb_tree_node_base* y = x->right;
	x->right = y->left;
	if (y->left != 0) {
		y->left->parent = x;
	}
	y->parent = x->parent;

	// �� y ��ȫ���� x �ĵ�λ
	if (x == root) {
		root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}
// ����
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	// x Ϊ��ת��
	//      |				   |
	//      X				   Y
	//     / \	R-rotate(X)   / \
	//    Y   R ==========>  L	 X
	//   / \					/ \
	//  L   M				   M   R
	__rb_tree_node_base* y = x->left;
	x->left = y->right;
	if (y->right != 0) {
		y->right->parent = x;
	}
	y->parent = x->parent;

	// �� y ��ȫ���� x �ĵ�λ
	if (x == root) {
		root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->right = x;
	x->parent = y;
}
template<class Key,class Value,class KeyofValue,class Compare,class Alloc>
typename rb_tree<Key,Value,KeyofValue,Compare,Alloc>::iterator
rb_tree<Key, Value, KeyofValue, Compare, Alloc>::find(const Key& k) {
	link_type y = header;
	link_type x = root();

	while (x != 0) {
		if (!key_compare(key(x), k)) {
			// x ��ֵ���� k
			// Ӧ��������
			y = x, x = left(x);
		}
		else {
			x = right(x);
		}
	}
	iterator j = iterator(y);
	return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	
}
#endif