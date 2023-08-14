#pragma once
#ifndef STL_FUNCTION_H_
#define STL_FUNCTION_H_

// unary_function����һԪ���������ͱ�ͷ���ֵ�ͱ�
template<class Arg,class Result>
struct unary_function {
	typedef Arg argument_type;
	typedef Result result_type;
};
// binary_function���ֶ�Ԫ�����ĵ�һ�����ͱ�
// �ڶ������Ա��Լ�����ֵ�ͱ�
template<class Arg1,class Arg2,class Result>
struct binary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
// ��
template<class T>
struct plus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x + y;
	}
};
// ��
template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x - y;
	}
};
// ��
template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x * y;
	}
};
// ��
template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x / y;
	}
};
// ȡģ
template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x % y;
	}
};
// ȡ��
template<class T>
struct negate :public unary_function<T, T, T> {
	T operator()(const T& x)const {
		return -x;
	}
};
// ֤ͬԪ��
// ��ֵA���Ԫ����op����õ�A�Լ�
template<class T>
inline T identity_element(plus<T>) {
	return T(0);
}
template<class T>
inline T identity_element(multiplies<T>) {
	return T(1);
}
template<class T>
struct equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x == y; }
};
template<class T>
struct non_equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x != y; }
};
template<class T>
struct greater :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x > y; }
};
template<class T>
struct less :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x < y; }
};
template<class T>
struct greater_equal :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x >= y; }
};
template<class T>
struct less_equal :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x <= y; }
};
template<class T>
struct logical_and :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x && y; }
};
template<class T>
struct logical_or :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x || y; }
};
template<class T>
struct logical_not :public unary_function<T, bool> {
	bool operator()(const T& x, const T& y)const { return !x; }
};
// ֤ͬ����,��ֵ���ش˺��������б仯
// ������set,ָ��RB-tree �����KeyofValue op
template<class T>
struct identity :public unary_function<T, T> {
	const T& operator()(const T& x)const { return x; }
};
// ����pair�ĵ�һԪ��
template<class Pair>
struct select1st :public unary_function<Pair, typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x)const {
		return x.first;
	}
};
// ����pair�ĵڶ�Ԫ��
template<class Pair>
struct select2n :public unary_function<Pair, typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x)const {
		return x.second;
	}
};
// ���ص�һ����,���Եڶ�����
template<class Arg1, class Arg2>
struct project1st :public binary_function<Arg1, Arg2, Arg1> {
	Arg1 operator()(const Arg1&, const Arg2& y)const { return x; }
};
// ���صڶ�����,���Ե�һ����
template<class Arg1, class Arg2>
struct project2nd :public binary_function<Arg1, Arg2, Arg1> {
	Arg2 operator()(const Arg1&, const Arg2& y)const { return y; }
};
// function adapter
// 
// ��ʾadaptable predicate���߼���ֵ
template<class Predicate>
class unary_negate :public unary_function<typename Predicate::argument_type, bool> {
protected:
	// �ڲ���Ա
	Predicate pred;
public:
	explicit unary_negate(const Predicate& x) :pred(x) {}
	bool operator()(const typename Predicate::argument_type& x)const {
		// ��pred�����������Ϸ�
		return !pred(x);
	}
};
// �����������ܹ��������ʹ��unary_negate
template<class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {

	return unary_negate<Predicate>(pred);
}
// ��ʾadaptable binary predicate���߼���ֵ
template<class Predicate>
class binary_negate :public binary_function<typename Predicate::first_argument_type,
	typename Predicate::second_argument_type,
	bool> {
protected:
	// �ڲ���Ա
	Predicate pred;
public:
	explicit binary_negate(const Predicate& x) :pred(x) {}
	bool operator() (
		const typename Predicate::first_argument_type& x,const typename Predicate::second_argument_type& y) const{
		return !pred(x, y);
	}
};
// �����������ܹ��������ʹ��binary_negate
template<class Predicate>
inline binary_negate<Predicate>not2(const Predicate& pred) {
	return binary_negate<Predicate>(pred);
}
// ��adaptable binary function ת��Ϊ unary function
template<class Operation>
class binder1st :public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
protected:
	// �ڲ���Ա
	Operation op;
	typename Operation::first_argument_type value;
public:
	binder1st(const Operation& x, const typename Operation::first_argument_type& y) :op(x),value(y){}
	
	typename Operation::result_type operator()
		(const typename Operation::second_argument_type& x)const {
		// ��value��Ϊ��һ����
		return op(value, x);
	}
};
// binde1st�ĸ�������
template<class Operation,class T>
inline binder1st<Operation>bind1st(const Operation& op, const T& x) {
	typedef typename Operation::first_argument_type arg1_type;
	return binder1st<Operation>(op, arg1_type(x));
}
// ��adaptable binary function ת��Ϊ unary function
template<class Operation>
class binder2nd :public unary_function<typename Operation::first_argument_type, typename Operation::result_type> {
protected:
	// �ڲ���Ա
	Operation op;
	typename Operation::second_argument_type value;
public:
	binder2nd(const Operation& x, const typename Operation::second_argument_type& y) :op(x), value(y) {}

	typename Operation::result_type operator()
		(const typename Operation::first_argument_type& x)const {
		// ��value��Ϊ��һ����
		return op(x,value);
	}
};
// binde1st�ĸ�������
template<class Operation, class T>
inline binder2nd<Operation>bind2nd(const Operation& op, const T& x) {
	typedef typename Operation::second_argument_type arg2_type;
	return binder1st<Operation>(op arg2_type(x));
}
// ��֪����adaptable unary function f(),g(),�������������h()
// ʹh(x)=f(g(x))
template<class Operation1,class Operation2>
class unary_compose :public unary_function
	<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	// �ڲ���Ա
	Operation1 op1;
	Operation2 op2;
public:
	// constructor
	unary_compose(const Operation1& x, const Operation2& y) :op1(x),op2(y){}

	typename Operation1::result_type operator()
		(const typename Operation2::argument_type& x)const {
		return op1(op2(x));
	}
};
// unary_compose�ĸ�������
template<class Operation1,class Operation2>
inline unary_compose<Operation1,Operation2>
compose1(const Operation1& op1, const Operation2& op2) {
	return unary_compose<Operation1, Operation2>(op1, op2);
}
// ��֪һ��adaptable binary function f() ��
// ����adaptable unary functions g1(),g2()
// �������������h(),ʹ��h(x)=f(g1(),g2())
template<class Operation1,class Operation2,class Operation3>
class binary_compose :
	public unary_function<typename Operation2::argument_type,
	typename Operaion2::result_type> {
protected:
	Operation1 op1;
	Operation2 op2;
	Operation3 op3;
public:
	// constructor
	binary_compose(const Operation1& x, const Operation2& y, const Operation3& z):op1(x),op2(y),op3(z){}

	typename Operation1::result_type operator()
		(const typename Operation2::argument_type& x)const {
		return op1(op2(x), op3(x));
	}
};
// binary_compose�ĸ�������
template<class Operation1,class Operation2,class Operation3>
inline binary_compose<Operation1,Operation2,Operation3>
compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
	return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}
// ��װһԪ����ָ��
template<class Arg,class Result>
class pointer_to_unary_function :public unary_function<Arg, Result> {
protected:
	// ����ָ����Ϊ�ڲ���Ա
	Result (*ptr)(Arg);
public:
	pointer_to_unary_function(){}
	explicit pointer_to_unary_function(Result(*x)(Arg)) :ptr(x) {}
	Result operator()(Arg x)const { return ptr(x); }
};
template<class Arg,class Result>
inline pointer_to_unary_function<Arg,Result>
ptr_fun(Result(*x)(Arg)) {
	return pointer_to_unary_function<Arg, Result>(x);
}
// ��װ��Ԫ����ָ��
template<class Arg1,class Arg2,class Result>
class pointer_to_binary_function :public binary_function<Arg1, Arg2, Result> {
protected:
	// ����ָ����Ϊ�ڲ���Ա
	Result(*ptr)(Arg1, Arg2);
public:
	pointer_to_binary_function(){}
	explicit pointer_to_binary_function(Result (*x)(Arg1,Arg2)):ptr(x){}

	Result operator()(Arg1 x, Arg2 y)const { return ptr(x, y); }
};
template<class Arg1,class Arg2,class Result>
inline pointer_to_binary_function<Arg1,Arg2,Result>
ptr_fun(Result(*x)(Arg1, Arg2)) {
	return pointer_to_binary_function<Arg1, Arg2, Result>(x);
}
// adapter function object: to member function
// �޲�,pointer����,non const
template<class S,class T>
class mem_fun_t :public unary_function<T*, S> {
	explicit mem_fun_t(S (T::*pf)()):f(pf){}
	// �������������Ա����
	S operator()(T* p)const { return (p->*f)(); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)();
};
// �޲�,pointer����,const
template<class S, class T>
class const_mem_fun_t :public unary_function<const T*, S> {
	explicit const_mem_fun_t(S(T::* pf)() const) :f(pf) {}
	// �������������Ա����
	S operator()(const T* p)const { return (p->*f)(); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)() const;
};
// �޲�,reference����,non const
template<class S, class T>
class mem_fun_ref_t :public unary_function<T, S> {
	explicit mem_fun_ref_t(S(T::* pf)()) :f(pf) {}
	// �������������Ա����
	S operator()(T& r)const { return (r.*f)(); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)();
};
//  �޲�,reference����,const
template<class S, class T>
class const_mem_fun_ref_t :public unary_function<T, S> {
	explicit const_mem_fun_ref_t(S(T::* pf)()const) :f(pf) {}
	// �������������Ա����
	S operator()(const T& r)const { return (r.*f)(); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)()const;
};
// ��һ������,pointer����,non const
template<class S, class T,class A>
class mem_fun1_t :public binary_function<T*,A, S> {
	explicit mem_fun1_t(S(T::* pf)(A)) :f(pf) {}
	// �������������Ա����
	S operator()(T* p,A x)const { return (p->*f)(x); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)(A);
};
// �޲�,pointer����,const
template<class S, class T,class A>
class const_mem_fun1_t :public binary_function<const T*,A,S> {
	explicit const_mem_fun1_t(S(T::* pf)(A) const) :f(pf) {}
	// �������������Ա����
	S operator()(const T* p ,A x)const { return (p->*f)(x); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)(A) const;
};
// �޲�,reference����,non const
template<class S, class T,class A>
class mem_fun1_ref_t :public binary_function<T,A, S> {
	explicit mem_fun1_ref_t(S(T::* pf)(A)) :f(pf) {}
	// �������������Ա����
	S operator()(T& r,A x)const { return (r.*f)(x); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)(A);
};
//  �޲�,reference����,const
template<class S, class T,class A>
class const_mem_fun1_ref_t :public binary_function<T,A, S> {
	explicit const_mem_fun1_ref_t(S(T::* pf)(A)const) :f(pf) {}
	// �������������Ա����
	S operator()(const T& r,A x)const { return (r.*f)(x); }
private:
	// ��Ա����ָ����Ϊ�ڲ���Ա
	S(T::* f)(A)const;
};
// ��������
template<class S,class T>
inline mem_fun_t<S, T>mem_fun(S (T::* f)()) {
	return mem_fun_t<S, T>(f);
}
template<class S, class T>
inline const_mem_fun_t<S, T>mem_fun(S (T::* f)() const) {
	return const_mem_fun_t<S, T>(f);
}
template<class S, class T>
inline mem_fun_ref_t<S, T>mem_fun_ref(S (T::* f)()) {
	return mem_fun_ref_t<S, T>(f);
}
template<class S, class T>
inline const_mem_fun_ref_t<S, T>mem_fun_ref(S(T::* f)() const) {
	return const_mem_fun_ref_t<S, T>(f);
}
template<class S, class T,class A>
inline mem_fun1_t<S, T, A>mem_fun(S(T::* f)(A)) {
	return mem_fun1_t<S, T, A>(f);
}
template<class S, class T, class A>
inline const_mem_fun1_t<S, T, A>mem_fun(S(T::* f)(A) const) {
	return const_mem_fun1_t<S, T, A>(f);
}
template<class S, class T, class A>
inline mem_fun1_ref_t<S, T, A>mem_fun_ref(S(T::* f)(A)) {
	return mem_fun1_ref_t<S, T, A>(f);
}
template<class S, class T, class A>
inline const_mem_fun1_ref_t<S, T, A>mem_fun_ref(S(T::* f)(A) const) {
	return const_mem_fun1_ref_t<S, T, A>(f);
}
#endif
