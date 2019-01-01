 new/delete operator
	-	调用 operator new 分配内存，然后在内存上构造对象
	-	调用对象的析构函数，然后再调用 operator delete 释放申请的资源
	-	new/delete 是运算符，C++标准规定不能更改它们的行为

 operator new / operator delete

 	void* operator new(std::size_t) throw(std::bad_alloc);
 	void operator delete(void*) throw();

 	-	函数，允许重载
 	-	与 malloc/free 相同，只负责分配和释放内存

 placement new / placement delete

 	void* operator new(std::size_t,void* p) throw()
 	{
 		return p;
 	}
 	void operator delete(void* p,void* o) throw()
 	{
 		//	only expression,并不需要在这一步释放内存
 	}

 	-	重载的 operator new ，有一个默认的版本
 	-	它并不会分配内存，而是在已分配的内存上构造对象
 	-	针对资源回收，显示调用对象的析构函数后，再调用对应的内存释放方式，
 		如 malloc - free, ::operator new - ::operator delete , new - delete 等等
