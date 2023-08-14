#pragma once
#ifndef _STL_CONFIG_H_
#define _STL_CONFIG_H_
// (1)  �������û�ж���bool��true��false�����������
// (2)  ��������ı�׼����ⲻ֧��drand48(),����__STL_NO_DRAND48
// (3)  ��������޷�����static members of template classes, ����__STL_STATIC_TEMPLATE_MEMBER_BUG
// (4)  �������֧��partial specialization of class templates, ����__STL_CLASS_PARTIAL_SPECIALIZATION
// (5)  �������֧��partial specialization of class templates,����__STL_FUNCTION_TMPL_PARTIAL_ORDER
// (6)  ��������������function templateʱָ����template arguments,
// ����__STL_EXPLICIT_FUNCTION_TMPL_ARGS
// (7)  �������δ֧��typename, ��typename����Ϊnull macro
// (8)  �������֧��template members of classes,����__STL_MEMBER_TEMPLATES
// (9)  ���������֧��explicit, ����explicitΪnull macro
// (10) ��������޷���ǰһ��template paramters�趨��һ��template paramters��Ĭ��ֵ
// ����__STL_LIMITED_DEFAULT_TEMPLATES 
// (11) ����������non-type template parameters ִ�� function template�����Ƶ�������
// ����__STL_NON_TYPE_TMPL_PARAM_BUG
// (12) ���������֧�ֵ�������->,����_SGI_STL_NO_ARROW_OPERATOR
// (13) �������֧��exceptions,����__STL_USE_EXCEPTIONS 
// (14) ����__STL_USE_NAMESPACE��ʹ�Զ����using std::list
// (15) �籾�������SGI����������,��ʹ����δʹ��pthreads������threads
// ����__STL_SGI_THREADS
// (16) �籾�������WIN32����������,���ڶ��߳�ģʽ
// ����__STL_WIN32THREADS
// (17) ������namespace��ص�macros ��__STD
// (18) ������exception��ص�macros ��__STL_TRY
// (19) ����__STL_ASSERTIONS�Ƿ���,��__stl_assert����Ϊ���Բ�����null macro

#ifdef _PTHREADS
#	define __STL_PTHREADS
#endif
# if defined(__sgi)&&!defined(__GNUC__)
// ʹ��SGI STL ����ʹ��CNU C++
# if !defined(_BOOL)
#	define __STL_NEED_BOOL
# endif
# if !defined(TYPENAME_IS_KEYWORD)
#	define __STL_NEED_TYPENAME
# endif
# if !def _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#	#define __STL_CLASS_PARTIAL_SPECIALIZATION
# endif
# ifdef _MEMBER_TEMPLATES
#	define __STL_MEMBER_TEMPLATES
# endif
# if !defined(_EXPLICIT_IS_KEYWORD)
#	define __STL_NEED_EXPLICIT
# endif
# ifdef __EXCEPTIONS
#	define __STL_USE_EXCEPTIONS
# endif
# endif
# if (_COMPILER_VERSION >=721)&&defined(_NAMESPACES)
#	define __STL_USE_NAMESPACES
# endif
# if !defined(_NOTHREADS)&& !defined(__STL_PTHREADS)
#	define __STL_SGI_THREADS
# endif 

# ifdef __GNUC__
#	include<_G_config.h>
#	if __GNUC__ < 2 || ( __GNUC__==2 __GNUC_MINOR__ < 8)
#		define __STL_STATIC_TEMPLATE_MEMBER_BUG
#		define __STL_NEED_TYPENAME
#		define __STL_NEED_EXPLICIT
#	else // GNUC 2.8+
#		define __STL_CLASS_PARTIAL_SPECIALIZATION
#		define __STL_FUNCITON_TMPL_PARTIAL_ORDER
#		define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#		define __STL_MEMBER_TEMPLATES
#	endif
#   // glibc pre 2.0 is very buggy
#	if !defined(_NOTHREADS)&& __GLIBC__>=2&& defined (_G_USING_THUNKS)
#		define __STL_PTHREADS
#	endif
#	ifdef __EXCEPTIONS
#		define __STL_USE_EXCEPTIONS
#	endif
# endif

# if defined(__SUNPRO_CC)
#	define __STL_NEED_BOOL
#	define __STL_NEED_TYPENAME
#	define __STL_NEED_EXPLICIT
#	define __STL_USE_EXCEPTIONS
# endif

# if defined (__COMO__)
#	define __STL_MEMBER_TEMPLATES
#	define __STL_CLASS_PARTIAL_SPECIALIZATION
#	define __STL_USE_EXCEPTIONS
#	define __STL_USE_NAMESPACES
# endif

# if defined(_MSC_VER)
#	if _MSC_VER > 1000
#		include<yvals.h>
#	else
#		define __STL_NO_DRAND48
#	endif
#	define __STL_NO_RAND48
#	define __STL_NEED_TYPENAME
#	if _MSC_VER < 1100
#		define __STL_NEED_EXPLICIT
#	endif
#	define __STL_NON_TYPE_TMPL_PARAM_BUG
#	define __SGI_STL_NO_ARROW_OPERATOR
#	ifdef _CPPUWIND
#		define __STL_USE_EXCEPTIONS
#	endif
#	ifdef _MT
#		define __STL_WIN32THREADS
#	endif
# endif

# if defined (__BORLANDC__)
#	define __STL_NO_DRAND48
#	define __STL_NEED_TYPENAME
#	define __STLLIMITED_DEFAULT_TEMPLATES
#	define __SGI_STL_NO_ARROW_OPERATOR
#	define __STL_NON_TYPE_TMPL_ARAM_BUG
#	ifdef _CPPUNWIND
#		define __STL_USE_EXCEPTIONS
#	endif
#	ifdef __MT__
#		define __STL_USE_WIN32THREADS
#	endif
# endif

# if defined (__STL_NEED_BOOL)
typedef int bool;
#	define true 1
#	define false 0
# endif

# ifdef __STL_NEED_TYPENAME
#	define typename
# endif

# ifdef __STL_NEED_EXPLICIT
# define explicit
# endif

# ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
#	define __STL_NULL_TEMPL_ARGS <>
# else
#	define __STL_NULL_TMPL_ARGS
# endif

# ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
#	define __STL_TEMPLATE_NULL tempate<>
# else 
#	define __STL_TEMPLATE_NULL
# endif

# if defined(__STL_USE_NAMESPACES) &&!defined(__STL_NO_NAMESPACES)
#	define __STD std
#	define __STL_BEGIN_NAMESPACE namespace std{
#	define __STL_END_NAMESPACE }
#	define __STL_USE_NAMESPACE_FOR_RELOPS
#	define __STL_BEGIN_RELOPS_NAMESPACE namespace std{
#	define __STL_END_RELOPS_NAMESPACE }
#	define __STD_RELOPS std
# else
#	define __STD 
#	define __STL_BEGIN_NAMESPACE 
#	define __STL_END_NAMESPACE 
#	define __STL_USE_NAMESPACE_FOR_RELOPS
#	define __STL_BEGIN_RELOPS_NAMESPACE
#	define __STL_END_RELOPS_NAMESPACE
#	define __STD_RELOPS
# endif

# ifdef __STL_USE_EXCEPTIONS
#	define __STL_TRY try
#	define __STLCATCH_ALL catch(...)
#	define __STL_RETHROW throw
#	define __STL_NOTHROW throw()
#	define __STL_UNWIND(action) catch(...){action;throw;}
# else
#	define __STL_TRY 
#	define __STLCATCH_ALL if (false)
#	define __STL_RETHROW
#	define __STL_NOTHROW 
#	define __STL_UNWIND(action)
# endif

# ifdef __STL_ASSERTIONS
#	include<stdio.h>
#	define __stl_assert(expt) \
		if (!(expr)){fprintf(stderr,"%s:%d STL assertion failure: %s\n",__FILE__,__LINE__);abort();}
# else
#	define __stl_assert(expr)
# endif

#endif /* __STL_CONFIG_H_ */
