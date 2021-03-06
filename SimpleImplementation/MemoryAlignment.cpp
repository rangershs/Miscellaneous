/*
 *	内存对齐	- 提高处理器访问存储设备的效率，减少总线的访问周期，由编译器在编译期间计算，与内存对齐系数有关(Windows/Linux操作系统、编译器种类)
 *																					   ||
 *																					   \/
 *																				取结构体中最大内存成员大小与默认对齐字节的最小值(用struct举例)
 *	#pragma pack(show)
 *	#pragma pack(1)
 *	#pragma pack()		从空间上来说节约了内存，但同时也降低了访问效率，而且需要信息的发送方和接收方约定使用相同的内存对齐方式，否则数据会出错
 */

/*	------------------------------------------------------------------------------	*/
/*	From yangshuangtao https://blog.csdn.net/yangshuangtao/article/details/45192149	*/
 
int main()
{
 
	class S{};
	 
	std::cout << sizeof(S);
	 
	return 0;
 
}
 
/*程序居然输出为1！到底是怎么回事？首先我在这里要声明一点——类在未初始化之前确实不会分配空间，这里探讨的是sizeof（类）的问题，详细情况看下面的例子
 
C++中的类所占内存空间总结
 
 
    类所占内存的大小是由成员变量（静态变量除外）决定的，成员函数（这是笼统的说，后面会细说）是不计算在内的。
 
摘抄部分：
 
      成员函数还是以一般的函数一样的存在。a.fun()是通过fun(a.this)来调用的。所谓成员函数只是在名义上是类里的。其实成员函数的大小不在类的对象里面，
      同一个类的多个对象共享函数代码。而我们访问类的成员函数是通过类里面的一个指针实现，而这个指针指向的是一个table，table里面记录的各个成员函数的地址（当然不同的编译可能略有不同的实现）。
      所以我们访问成员函数是间接获得地址的。所以这样也就增加了一定的时间开销，这也就是为什么我们提倡把一些简短的，调用频率高的函数声明为inline形式（内联函数）。*/
 
class CBase 
{ 

}; 
//sizeof(CBase)=1；
 
/*为什么空的什么都没有是1呢？
c++要求每个实例在内存中都有独一无二的地址。		//注意这句话！！！！！！！！！！ 
空类也会被实例化，所以编译器会给空类隐含的添加一个字节，这样空类实例化之后就有了独一无二的地址了。所以空类的sizeof为1。*/
 
class CBase 
{ 
	int a; 
	char p; 
}; 
//sizeof(CBase)=8;
/*记得对齐的问题。int 占4字节		//注意这点和struct的对齐原则很像！
char占一字节，补齐3字节*/
 

class CBase  
{ 
public: 
	CBase(void); 
	virtual ~CBase(void); 
private: 
	int   a; 
	char *p; 
}; 
/*再运行：
sizeof(CBase)=12
C++ 类中有虚函数的时候有一个指向虚函数的指针（vptr），在32-bit系统分配指针大小为4字节。无论多少个虚函数，只有这一个指针，4字节。*/
//注意一般的函数是没有这个指针的，而且也不占类的内存。
 

class CChild : public CBase  
{ 
public: 
	CChild(void); 
	~CChild(void); 
 
	virtual void test();
private: 
	int b; 
}; 
/*输出：sizeof(CChild)=16；
可见子类的大小是本身成员变量的大小加上父类的大小。		//其中有一部分是虚拟函数表的原因，一定要知道
父类子类共享一个虚函数指针*/

 
#include <iostream>
using namespace std;

class a {};
 
class b {};
 
class c:public a
{
	virtual void fun()=0;
};
 
class d:public b,public c{};
 
int main()
 
{
	cout << "sizeof(a)" << sizeof(a) << endl;
	 
	cout << "sizeof(b)" << sizeof(b) << endl;
	 
	cout << "sizeof(c)" << sizeof(c) << endl;
	 
	cout << "sizeof(d)" << sizeof(d) << endl;
 
	return 0;
}
 
/*程序执行的输出结果为：
 
sizeof(a) =1
 
sizeof(b)=1
 
sizeof(c)=4
 
sizeof(d)=8
 
前三种情况比较常见，注意第四种情况。类d的大小更让初学者疑惑吧，类d是由类b，c派生迩来的，它的大小应该为二者之和5，为什么却是8呢？
这是因为为了提高实例在内存中的存取效率．类的大小往往被调整到系统的整数倍．并采取就近的法则，离哪个最近的倍数，就是该类的大小，所以类d的大小为8个字节．(内存对齐)
 
 
总结：
 
空的类是会占用内存空间的，而且大小是1，原因是C++要求每个实例在内存中都有独一无二的地址。
 
（一）类内部的成员变量：
 
 
普通的变量：是要占用内存的，但是要注意对齐原则（这点和struct类型很相似）。
static修饰的静态变量：不占用内容，原因是编译器将其放在全局变量区。
 
 
（二）类内部的成员函数：
 
普通函数：不占用内存。
虚函数：要占用4个字节(32-bit)，用来指定虚函数的虚函数表的入口地址。所以一个类的虚函数所占用的地址是不变的，和虚函数的个数是没有关系的。*/

/*	------------------------------------------------------------------------------	*/
