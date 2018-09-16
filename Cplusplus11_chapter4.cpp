//	----------------------------------------------------
//	shared_ptr
//	.reset() 可以初始化智能指针，但推荐使用std::make_shared<>()
//	管理动态数组时，需要指定删除器，因为默认的删除器不负责处理动态数组 std::shared_ptr<int> arr_i(new int[10],[](int* p){delete[] p;})
//																 std::shared_ptr<int> arr_i(new int[10],std::default_delete<int[]>())
//	note1:不要用原始指针初始化多个shared_ptr，它们是单独的引用计数，而拷贝构造或拷贝赋值后引用计数会增加
//	note2:类中的方法返回shared_ptr对象，不能直接使用this构造(违背note1)，应该继承std::enable_shared_from_this，调用它的shared_from_this()方法
//	note3:shared_ptr防止循环引用，否则资源得不到正确释放造成泄露

//	[Usage:单独管理资源或管理数组时使用std::unique_ptr，多个对象管理资源时使用std::shared_ptr
//			智能指针和原始指针不要混合使用，要么全部智能指针，要么全部原始指针]

//	unique_ptr
//	不能复制，可以移动，唯一性
//	支持数组 std::unique_ptr<int[]> arr_i(new int[10]);	true
//			std::shared_ptr<int[]> arr_i(new int[10]);	false
//	指定删除器时，需要在<>中指定删除器的类型，函数指针、仿函数或函数对象
//	(lambda在没有捕获变量的情况下可以转换为函数指针，一旦捕获了变量，就不能直接转换为函数指针，可以转换为函数对象std::function<>())
//	(std::function<>() 类型安全的，新标准下尽可能多的使用，除非跨模块、跨dll)


//	weak_ptr
//	不共享指针，不能操作资源，它是资源的监测器
//	[use_count()、expired()、lock()]
//	std::enable_shared_from_this内部使用weak_ptr监测shared_ptr资源情况，通过lock()返回shared_ptr对象，而内部的weak_ptr是通过shared_ptr构造的
//因此，只有在该类的对象使用shared_ptr后，才能调用std::shared_from_this()

struct B;
struct A
{
	A() {}
	~A() {}
	std::shared_ptr<B> bptr;
};
struct B
{
	B() {}
	~B() {}
	std::shared_ptr<A> aptr;
};

void Test()
{
	{
		std::shared_ptr<A> a_ptr(new A);
		std::shared_ptr<B> b_ptr(new B);
		a_ptr->bptr = b_ptr;
		b_ptr->aptr = a_ptr;
	}
}
//	离开作用域后对象析构，释放内部的资源
//	循环引用情况下，离开作用域后智能指针的引用计数不为0，不会调用析构资源不能正确释放
//	将其中一个shared_ptr更改为weak_ptr，赋值时不增加资源的引用计数，离开作用域后智能指针的引用计数为0，调用析构释放内部资源
//另外一个对象的引用计数减1，离开作用域后引用计数再减1，从而不会造成资源泄露



//	智能指针管理第三方库接口返回的内存，一般是原始指针，智能指针的删除器就是第三方库提供的释放内存的接口
//	确保在异常或中途返回情况下资源能正常释放
template<typename T>
shared_ptr<T> MemSafe(T* t)				//	右值，若没有赋值运算给对象，调用完后就会释放，变量的生存期很短暂
{
	return std::shared_ptr<T>(t,[](T* ptr){handle->release(ptr);});
}

#define MemSafe(p) std::shared_ptr<void> p##p(p,[](void* ptr){handle->release(ptr);})