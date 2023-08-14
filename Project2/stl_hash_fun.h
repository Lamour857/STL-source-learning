#pragma once
#ifndef STL_HASH_FUN_H_
#define STL_HASH_FUN_H_
#include"stl_config.h"
#include<algorithm>
template<class Key>struct hash{};

inline size_t __stl_hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s) {
		h = 5 * h + *s;
	}
	return size_t(h);
}
template<> struct hash<char*> {
	size_t operator()(const char* s)const { return __stl_hash_string(s); }
};
template<> struct hash<const char*> {
	size_t operator()(const char* s)const { return __stl_hash_string(s); }
};
template<> struct hash<char> {
	size_t operator()(char s)const { return x; }
};
template<> struct hash<unsigned char> {
	size_t operator()(unsigned char s)const { return x; }
};
template<> struct hash<short> {
	size_t operator()(short s)const { return x; }
};
template<> struct hash<unsigned short> {
	size_t operator()(unsigned short s)const { return x; }
};
template<> struct hash<int> {
	size_t operator()(int s)const { return x; }
};
template<> struct hash<unsigned int> {
	size_t operator()(unsigned int s)const { return x; }
};
template<> struct hash<long> {
	size_t operator()(long s)const { return x; }
};
template<> struct hash<unsigned long> {
	size_t operator()(unsigned long s)const { return x; }
};
#endif