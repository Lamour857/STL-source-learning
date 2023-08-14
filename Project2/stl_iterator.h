#pragma once

#ifndef _STL_ITERATOR_H_
#define _STL_ITERATOR_H_
#include <cstddef>	//for ptrdiff_t
#include<iostream>
using std::istream;
using std::ostream;
// ���ֵ���������
// ֻд������
struct input_iterator_tag{};
// ֻ��������
struct outout_iterator_tag {};
// �����������ϲ����ĵ��������
struct forward_iterator_tag{};
// ˫�������
struct bidirectional_iterator_tag{};
// �ṩָ������������˫�������
struct random_access_iterator_tag{};

//iteratorģ��,���п����ĵ�����Ӧ�̳��ڴ�
template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator {
	typedef Category	iterator_category;
	typedef T			value_type;
	typedef Distance	difference_type;
	typedef Pointer		pointer;
	typedef Reference	reference;
};
//������ȡ��traits
template<class Iterator>
struct iterator_traits {
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};

// ���ԭ��ָ�����Ƶ�traitsƫ�ػ���
template<class T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag		iterator_categoty;
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef T*								pointer;
	typedef T&								reference;
};

// ���point to const ��ԭ��ָ���traitsƫ�ػ���
template <class T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag		iterator_category;
	typedef T								value_type;
	typedef ptrdiff_t						pointer;
	typedef const T*						pointer;
	typedef const T&						reference;
};

// ����������������
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

// ������������distance type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* 
distance_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// ������������value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//distance() ��������������֮��ľ���,����difference pointer
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,input_iterator_tag) {
	terator_traits<InputIterator>::difference_type n = 0;
	while (first != last) {
		++first; ++n;
	}
	return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
	// ��Բ�ͬ���͵ĵ�����,distance�����в�ͬ�ļ������������ķ�ʽ
	// �������ʹ�� if ���� type_traits �ж�InputIterator���ͱ�
	// �����ַ�ʽ�����ӳ������еĸ���
	// ���ǿ��Ե�����Ƕ__distance(),��__distance����
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	
	return __distance(first, last, category());
}

// advance()���� ʵ�ֶԵ������ĵ���
template <class InputIterator,class Distance>
void advance(InputIterator& i, Distance n) {
	__advance(i, n, iterator_category(i));
}

template <class InputIterator,class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
	while (n--)++i;
}

template <class BidirectionalIterator,class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
	if (n >= 0) {
		while (n--)++i;
	}
	else {
		while (n++)--i;
	}
}

template <class RandomAccessIterator,class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
	i += n;
}
// �����������, ����ֵ�޸�Ϊ����(β��)
template<class Container>
class back_insert_iterator {
protected:
	// �ײ�����
	Container* container;
public:
	typedef output_iterator_tag iterator_categoty;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;
	explicit back_insert_iterator(Container& x):container(&x){}
	back_insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		// ����ֵ����תΪ���õײ��push_back
		container->push_back(value);
		return *this;
	}
	// ���������������������
	back_insert_iterator<Container>& operator*() { return *this };
	back_insert_iterator<Container>& operator++() { return *this };
	back_insert_iterator<Container>& operator++(int) { return *this };
};
// ��������: ���ӱ�����ʹ������������
template<class Container>
inline back_insert_iterator<Container>back_insert(Container& x) {
	return back_insert_iterator<Container>(x);
}
// �����������, ����ֵ�޸�Ϊ����(ͷ��)
template<class Container>
class front_insert_iterator {
protected:
	// �ײ�����
	Container* container;
public:
	typedef output_iterator_tag iterator_categoty;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;
	explicit front_insert_iterator(Container& x) :container(&x) {}
	front_insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		// ����ֵ����תΪ���õײ��push_back
		container->push_front(value);
		return *this;
	}
	// ���������������������
	front_insert_iterator<Container>& operator*() { return *this };
	front_insert_iterator<Container>& operator++() { return *this };
	front_insert_iterator<Container>& operator++(int) { return *this };
};
// ��������: ���ӱ�����ʹ������������
template<class Container>
inline front_insert_iterator<Container>front_insert(Container& x) {
	return front_insert_iterator<Container>(x);
}
// �����������,����ֵ�޸�Ϊ����(ָ��λ��)
template<class Container>
class insert_iterator {
protected:
	// �ײ�����
	Container* container;
	typedef Container::iterator iter;
public:
	typedef output_iterator_tag iterator_categoty;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;
	explicit insert_iterator(Container& x,typename Container::iterator i) :container(&x),iter(i){}
	insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		// ����ֵ����תΪ���õײ��push_back
		iter = container->insert(iter, value);
		++iter;
		return *this;
	}
	// ���������������������
	insert_iterator<Container>& operator*() { return *this };
	insert_iterator<Container>& operator++() { return *this };
	insert_iterator<Container>& operator++(int) { return *this };
};
// ��������: ���ӱ�����ʹ������������
template<class Container,class Iterator>
inline insert_iterator<Container>inserter(Container& x,Iterator i) {
	return insert_iterator<Container>(x,iter(i));
}
// reverse_iterator
template<class Iterator>
class reverse_iterator
{
protected:
	// ��Ӧ�����������
	Iterator current;
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;
	typedef Iterator iterator_type;	// ���������
	typedef reverse_iterator<Iterator> self;	// ���������
public:
	reverse_iterator(){}
	explicit reverse_iterator(iterator_type x):current(x){}
	reverse_iterator(const self& x):current(x.current){}

	// ȡ�����������
	iterator_type base()const { return current; }
	reference operator*()const {
		Iterator tmp = current;
		return *--tmp;
		// ������������˺�һ��ȡֵ
	}
	pointer operator->()const { return &(operator*()); }

	// ǰ�������
	self& operator++() {
		--current;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		--current;
		return tmp;
	}
	// ���˱�ǰ��
	self& operator--() {
		++current;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		++current;
		return tmp;
	}
	self operator+(difference_type n)const {
		return self(current - n);
	}
	self operator+=(difference_type n) {
		current -= n;
		return *this;
	}
	self operator-(difference_type n)const {
		return self(current + n);
	}
	self operator-=(difference_type n) {
		current += n;
		return *this;
	}
	reference operator[](difference_type n)const { return *(*this + n); }
};
// stream iterator
template<class T,class Distance=ptrdiff_t>
class istream_iterator {
	friend bool operator== <>(const istream_iterator<T, Distance>& x, const istream_iterator<T, Distance>& y);
protected:
	istream* stream;
	T value;
	bool end_marker;
	void read() {
		end_marker = (*stream) ? true : false;
		if (end_marker)*stream >> value;
		end_marker = (*stream) ? true : false;
	}
public:
	typedef input_iterator_tag iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef const T* pointer;
	typedef const T& reference;

	istream_iterator():stream(&cin),end_marker(false){}
	istream_iterator(istream& s) :stream(&s) { read(); }

	reference operator*()const { return value; }
	pointer operator->()const { return &(operator*()); }
	// ÿǰ��һ��λ�ã���ȡһ������
	istream_iterator<T, Distance>& operator++() {
		read();
		return *this;
	}
	istream_iterator<T, Distance>& operator++() {
		istream_iterator<T, Distance> tmp = *this;
		read();
		return *this;
	}
};
template<class T, class Distance = ptrdiff_t>
class ostream_iterator {
	friend bool operator== <>(const istream_iterator<T, Distance>& x, const istream_iterator<T, Distance>& y);
protected:
	istream* stream;
	const char* string;
	
public:
	typedef output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	ostream_iterator(ostream& s) :stream(&s), string(0) {}
	ostream_iterator(ostream& s,const char* c) :stream(&s),string(c) { }

	ostream_iterator<T>& operator=(const T& value) {
		*stream << value;
		if (string)*stream << string;
		return *this;
	}
	ostream_iterator<T>& operator*()const { return *this }
	ostream_iterator<T>& operator++() {return *this;}
	ostream_iterator<T>& operator++() {return *this;}
};

#endif