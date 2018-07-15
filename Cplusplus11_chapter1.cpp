//	----------------------------------------------------
//	auto在模板函数中的一个应用
class Foo
{
public:
	Foo();
	~Foo();
	static int get()
	{
		return 0;
	}
};

class Bar
{
public:
	Bar();
	~Bar();
	static const char* get()
	{
		return "1";
	}
};

template<typename T>
void Func()
{
	auto get_value = T::get();		//	编译期由表达式自动推断出函数返回值的类型，否则在模板中添加额外的参数指代
	// do something...
}

int main()
{
	Func<Foo>();
	Func<Bar>();
	return 0;
}

//	----------------------------------------------------
//	decltype在泛型编程中的一个应用
template<typename Container>
class Foo
{
	typedef Container::iterator IT;
public:
	Foo();
	~Foo();
	
	void Func(Container& _containter)
	{
		IT it = _containter.begin();		//	若传入的_containter带有cont属性，那么it的类型应该是const_iterator
		//	do something...
	}
};
//	C++98/03
template<typename Container>
class Foo<typename Container>		//特化处理
{
	typedef Container::const_iterator IT;
public:
	Foo();
	~Foo();
	
	void Func(const Container& _containter)
	{
		IT it = _containter.begin();
		//	do something...
	}
};
//	C++11
template<typename Container>
class Foo
{
	decltype(Container.begin()) IT;
public:
	Foo();
	~Foo();
	
	void Func(Container& _containter)
	{
		IT it = Container.begin();
		//	do something...
	}
};

//	----------------------------------------------------
//	typedef & using
typedef unsigned int uint;
using uint = unsigned int;

typedef void(*pFunc)(int);
using pFunc = void(*)(int);

template<typename T>
typedef std::map<int, T> map_i_T;		//	error,typedef的不足，无法直接定义模板别名
//	C++98/03
template<typename T>		//	外敷模板
struct ST_Map
{
	typedef std::map<int, T> map_i_T;
};

ST_Map<int>::map_i_T map_i_i;		//	to use
//	C++11
template<typename T>
using map_i_T = std::map<int, T>;		//	using包含了typedef的所有功能，在新标准中推荐使用using，代码的可读性也更强

map_i_T<int> map_i_i;		//	to use

/*
 *	C++98/03中类模板允许参数有默认值，但是函数模板不允许参数有默认值
 *	C++11中类模板和函数模板均支持参数有默认值
 *	其中，模板参数中的赋值顺序是自右向左,模板参数自动推导是根据传递的实参决定的
 */

template<typename T>
struct Type
{
	typedef T value_type;
};

template<typename T=int>
void Func(typename Type<T>::value_type val,T=0)
{
	//	do something...
}

int main()
{
	Func(123);				//	T -> int
	Func(123,123.0);		//	T -> double
	return 0;
}

//	----------------------------------------------------
//	std::initializer_list C++11的列表初始化
std::initializer_list<int> iList = {0,7,1,5};		//	它的内部不负责保存元素的拷贝，而是存储元素的引用，因此是高效的，同时应该注意对象及元素的生存期
													//	而vector类的容器，保存的是元素的拷贝，需要把每一个元素都复制一次，效率偏低
/*
 *	列表初始化方式还可以检查类型收窄情况，编译器可能会报narrow conversion的错误或-Wnarrowing的警告
 */
int a = 1.1;		// ok
int b = {1.1};		// error

char c = 1024;		//	ok
char c = {1024};	//	error

float fa = 1e40;	//	ok
float fb = {1e40};	//	error

/*
 *	C++11新的对象初始化方式方便统一，但有其使用的局限性
 *	聚合类型的对象使用列表初始化方式与普通的初始化方式等同，非聚合类型使用列表初始化方式编译可能不通过，特别是自定义类型对象
 *	习惯而言，内置类型(int,char,float...)、普通数组、struct、标准库容器(vector,list,map...)、构造函数参数是std::initializer_list类型，对象初始化时应采用列表初始化方式
 */

/*	聚合类型
 *		普通数组
 *		类(class,stuct,union)	--	无自定义构造函数/无private或protected的非静态数据成员/无基类/无虚函数
 *									不能有{}和=直接初始化的非静态数据成员
 *	聚合类型不存在嵌套关系，即一个类型中的元素是非聚合类型，它自身依旧可能是聚合类型
 *		如普通数组中带有非聚合类型元素，那么普通数组仍旧是聚合类型
*/	

//	----------------------------------------------------
//	for-range	std::for_each

/*
 *	基于范围的for循环中遍历map和set，无法修改关联容器的key值，因为它们的key是只读的
 *	for-range中遍历元素时，尽量不要修改容器本身，这很可能导致迭代器失效，从而出现意想不到的结果
 *	即，良好的编程习惯是尽量不要在迭代过程中修改迭代的容器
 */