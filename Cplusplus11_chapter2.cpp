//	----------------------------------------------------
//	Right Value
//	左值 - 具名的变量或对象;能够取地址;表达式结束后持久存在的对象
		   
//	右值 - 不具名;不能够取地址;表达式中的临时对象
//		   临时变量，原始字面量(0,true,etc)，lambda，std::move返回值，T&&函数返回值等

//	常量左值引用是一个万能类型，可以接受左值、右值、常量左值和常量右值，而非常量左值只能接受左值
//	const Obj& = Get_Obj();		//	true
//	Obj& = Get_Obj();			//	false

//	引用叠加 - 形如T&& param,auto&& param这种未定的引用类型(universal references)，它作为参数时可能被左值引用或右值引用的参数初始化
//			  这种类型相比于右值引用会发生变化
//	规则     - 右值引用叠加到右值引用仍然是右值引用；其他引用类型的叠加则是左值引用
//	使用     - 表达式中是universal references，它会根据初始化的对象推导为左值引用还是右值引用
//             因为左值无法直接赋予给右值引用 -> 语法报错，如果需要左值赋予给右值引用，借助std::move将左值强制转换为右值，转移资源的拥有者
//	编译器会将具名的右值引用视为左值处理，未命名的右值引用视为右值处理如下:

void print_value(int& _value)
{
	printf("lvalue\n");
}
void print_value(int&& _value)
{
	printf("rvalue\n");
}
void Print_Value(int&& _value)		//	若_value参数为2，输出是"lvalue"
{
	print_value(_value);			//	具名的右值引用会被当做左值处理
}

//	浅拷贝   - 多个指针变量指向同一块内存
//	深拷贝   - 每个指针变量管理各自的内存
//	移动构造 - 同一块内存只有一个指针变量管理，新的指针变量会特换掉旧的指针变量，资源的转移减少临时对象不必要的创建、拷贝和销毁
//	(C++11以前清空vector的内容，就是把它和一个临时的空对象交换，临时对象析构的时候就会释放掉原vecotr的资源)

//	设计和实现类时，如果存在大量申请资源或资源拷贝的情况，应设计移动构造函数和移动赋值函数，从而提高程序的效率(仅仅是资源的转移)
//	通常，移动构造函数和常量左值拷贝构造函数同时提供，移动不成还可以拷贝，提高程序的可靠性
//	即，std::move用于具备移动构造/移动赋值的对象，不然会尝试调用对象的常量左值拷贝构造函数

//	Summary:	常量左值引用是万能类型；非常量左值引用只能接受左值初始化；右值引用只能直接接受右值初始化


//	----------------------------------------------------
//	std::forward
//	右值引用是独立于值类型的(左值/右值)，右值引用作为函数的形参时，在函数内部转发该参数时它已不再是右值引用，而作为左值处理

//	在函数模板中为了保持参数原有的类型，不论是左值引用、右值引用还是universal references，引入了std::forward
void Print_Value(int&& _value)
{
	print_value(_value);						//	lvalue
	print_value(std::forward<int>(_value));		//	original type
	print_value(std::move(_value));				//	rvalue
}
Print_Value(1);		//	r_value
int x = 1;
Print_Value(x);		//	l_value
Print_Value(std::forward<int>(x));		//	x -> l_value,argument -> r_value


//	----------------------------------------------------
//	追踪返回类型(返回类型后置),没有返回值的追踪返回类型声明为void
template<typename T1,typename T2>
decltype(t1+t2)Sum(T1& t1,T2& t2)
{
	return t1+t2;								//	syntax error
}												//	编译器从左至右读入参数，t1与t2在使用前未声明
template<typename T1,typename T2>
auto Sum(T1& t1,T2& t2) -> decltype(t1+t2)		//	similar to lambda syntax
{
	return t1+t2;
}

auto pf() -> auto(*)() -> int(*)()
{
	return nullptr;
}
//	自右向左分析:
//	auto(*)()->int(*)() 这是一个函数，返回类型是函数指针
//	pf()是一个函数，它的返回类型仍然是函数指针，只是该函数指针指向返回函数指针的函数auto(*)()

template<typename... B>
class T:private A<B...> {};
class T<x,y>:private A<x,y> {};					//	多个参数的派生类,模板参数包在推导中作为一个参数传递

template<typename... B>
class T:private A<B>... {};
class T<x,y>:private A<x>,private A<y> {};		//	多重继承的派生类

//	variadia template
//	refer to (MSDN -- https://msdn.microsoft.com/zh-cn/library/dn439779.aspx)
//	右值引用 + 完美转发 + 可变参数模板 + auto + decltype
template<typename Func,typename... Args>
inline auto FunctionWrapper(Func&& func,Args&&... args)->decltype(func(std::forward<Args>(args)...))
{
	return func(std::forward<Args>(args)...);
}
								//	(args)... is a function parameter pack
								//	(args...) is not a function parameter pack

//	template<typename... Args> returntype functionname(Args... args);
//	template<typename... Args> returntype functionname(Args&... args);
//	template<typename... Args> returntype functionname(Args&&... args);
//	template<typename... Args> returntype functionname(Args*... args);
//	template<typename... Args> returntype functionname(const Args&... args);


//	----------------------------------------------------
//	STL的emplace、emplace_back等方法在容器内部构造对象，减少内存的拷贝移动，提高了效率。但是，没有构造函数的对象无法使用
//	set/map的内部是红黑树，元素的key会自动排序；unordered_set/unordered_map的内部是散列表，不会自动排序，操作元素的效率更高
//但是自定义类型需要提供hash函数和比较函数