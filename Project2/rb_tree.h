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

	color_type color;	// 非黑即红
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

// 基层迭代器
struct __rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;

	void increment() {
		if (node->right != 0) {
			// (1)
			// 如果有右子节点
			node = node->right;
			// 则往右走
			while (node->left != 0) {
				// 找到右子树的最左边的节点
				node = node->left;
			}
		}
		else {
			// (2)
			// 没有右子树
			base_ptr y = node->parent;
			while (node == y->right) {
				// 上溯直到不为右子节点
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
			// 以上情况发生于mode为header时，即node为end（）
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

// 正规迭代器
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
	// 指向节点的指针 link_type
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

	link_type create_node(const value_type& x) {
		// 配置空间
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
	link_type header;	// 实现技巧
	Compare key_compare;

	link_type& root()const { return (link_type&)header->parent; }
	link_type& leftmost()const { return (link_type&)header->left; }
	link_type& rightmost()const { return (link_type&)header->right; }

	// 以下6函数方便取得节点x的成员
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
	// 以下6函数方便取得节点x的成员
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

	// 求极值
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
		// 产生一个节点空间令header指向它
		header = get_node();
		// 红色以区分header和root
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
	// 插入新值：不允许节点键值重复
	// 返回pair，第一个元素时rb-tree迭代器指向新增节点
	// 第二元素表示插入成功与否
	pair<iterator, bool>insert_unique(const value_type& x) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {
			y = x;
			comp = key_compare(KeyofValue()(v), key(x));
			x = comp ? left(x) : right;
			// 遇大往左，遇小等往右
		}

		// y 所指即插入点的父节点（叶节点）
		iterator j = iterator(y);
		if (comp) {
			// comp 为真表示应该插入于左侧
			if (j == begin())
				// 如果插入点的父节点是最左节点
				return pair<iterator, bool>(__insert(x, y, v), true);
			else
				// 插入点不是最左节点
				--j;
		}
		if (key_compare(key(j.node), KeyofValue()(v))) {
			// 小于新值，将插入于右侧
			return pair<iterator, bool>(__insert(x, y, v), true);
		}

		return pair<iterator, bool>(j, false);
	}
// 插入新值：允许节点键值重复
// 返回的迭代器指向新增节点
	iterator insert_equal(const value_type& x) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			// 寻找适当的插入点
			y = x;
			x = key_compare(KeyofValue()(v), key(x)) ? left(x) : right(x);
			// 遇大往左，遇小等往右
		}
		return __insert(x, y, v);
		// x 新值插入点，y为插入点的父节点，v为新值
	}
// 真正的插入执行程序
	iterator __insert(base_ptr x_, base_ptr y_, const Value& v) {
		// x_ 为新值插入点，y_ 为新值插入父节点,v 为新值
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		
		// key_compare 作为大小比较准则,是个function object
		if (y == header || x != 0 || key_compare(KeyofValue()(v), key(y))) {
			// 产生新节点
			z = create_node(v);
			// 当y为header时,leftmost()=z
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
			// 新节点成为插入点之父y的右子节点
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
		// x不是根节点, 且x的父节点是红色,这是违背红黑树规则
		// 需要调整
		if (x->parent == x->parent->parent->left) {
			// 父节点是祖父节点的左节点
			// y 为叔叔节点
			__rb_tree_node_base* y = x->parent->parent->right;
			if (y && y->color == __rb_tree_red) {
				// 叔叔节点存在, 且其颜色是红色
				/*  rb_tree_insert_case_4  */
				//		|					   |
				//      G(B)				   G(R)
				//     / \    ==========>     / \
				// (R)P   U(R)			  (B)P   U(B)
				//   /				     	/     
				//  X(R)				   X(R)
				x->parent->color = __rb_tree_black;	// 父节点修改为黑
				y->color = __rb_tree_black;			// 叔叔节点修改为黑
				x->parent->parent->color = __rb_tree_red;	// 祖父节点修改为红
				x = x->parent->parent;		// 当前需要调整的节点设定为祖父节点
			}
			else {
				// 无叔叔节点，或叔叔节点为黑
				if (x == x->parent->right) {
				// 如果新节点为父节点的右节点
				/*  rb_tree_insert_case_5  */
				//      |                      
				//	    G(B)
				//     / \
				// (R)P   U(B)
				//     \	   
				//      X(R) 
					x = x->parent;
					// 将父节点右旋
					__rb_tree_rotate_left(x, root);
				// 右旋可以将case 5 转换成case 6
				// 执行完以上代码现在的状态如下
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
				// 执行完以上代码现在的状态如下
				//     |
				//     P(B)
				//	  / \
				//(R)X   G(R)
				//        \
				//         U(B)
			}	
		}// while 结束
		root->color = __rb_tree_black;
	}

}
// 左旋
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	// x 为旋转点
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

	// 令 y 完全顶替 x 的地位
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
// 右旋
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	// x 为旋转点
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

	// 令 y 完全顶替 x 的地位
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
			// x 键值大于 k
			// 应当往左走
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