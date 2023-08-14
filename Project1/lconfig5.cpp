#include<iostream>
using namespace std;
template<class I,class O>
struct testStruct
{
	testStruct() { cout << "I,O" << endl; }
};
template<class T>
struct testStruct<T*,T*>
{
	testStruct() { cout << "T*,T*" << endl; }
};
template<class T>
struct testStruct<const T*,T*>
{
	testStruct() { cout << "cont T*,T*" << endl; }
};
int main(void) {
	testStruct<int, char>obj1;
	testStruct<int*, int*>obj2;
	testStruct<const int*, int*>obj3;
}