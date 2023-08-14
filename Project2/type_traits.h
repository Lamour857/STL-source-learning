#pragma once
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
// type traits 用于萃取出对象的各种特性
// 如: 有无trivial constructor
//	   有无trivial copy constructor
//	   有无trivial assignment
//	   是否为POD型别
struct __true_type{};
struct __false_type{};

template <class type>
struct __type_traits {
	// 这个成员通知能自动将__type_traits特化的编译器说这个__type_traits是特殊的
	typedef __true_type this_dummy_menber_must_be_first;
	
	typedef __false_type has_trivial_default__constructor;
	typedef __false_type has_trivial_copy_constructor;
	typedef __false_type has_trivial_assignment_operator;
	typedef __false_type has_trivial_destructor;
	typedef __false_type is_POD_type;
};

// 对__type_traits的各种POD类型的偏特化
template<>
struct __type_traits<char> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<signed char> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<unsigned char> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<short> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<unsigned short> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<int> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<unsigned int> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<long> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<unsigned long> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<float> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<double> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template<>
struct __type_traits<long double> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
template < class T>
struct __type_traits<T*> {
	typedef __true_type has_trivial_default__constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

#endif