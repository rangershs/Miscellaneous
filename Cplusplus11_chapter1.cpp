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