#include <iostream>

class Iterator
{
public:
	Iterator(int _cur_pos):cur_pos(_cur_pos) {}
	~Iterator() {}

	bool operator!=(const Iterator& oth) const
	{
		return this->cur_pos != oth.cur_pos;
	}

	Iterator& operator++()
	{
		++cur_pos;
		return *this;
	}

	const int& operator*() const
	{
		return cur_pos;
	}

/*	void output()
	{
		std::cout << cur_pos << std::endl;
	}*/

private:
	int cur_pos;
};

class Range
{
public:
	Range(int _begin,int _end):begin_pos(_begin),end_pos(_end) {}
	~Range() {}

	Iterator begin()
	{
		return Iterator(begin_pos);
	}

	Iterator end()
	{
		return Iterator(end_pos);
	}

private:
	int begin_pos;
	int end_pos;
};

/*
 *	const修饰以指针或引用的函数返回值，那么返回值的内容不能被修改，并且该值只能赋予const修饰的同类型指针或引用
 *	若const修饰以值传递的函数返回值，const的修饰没有作用，返回值的副本仍然可以修改
 */

/*const Iterator ret_obj()
{
	return Iterator(1);
}

const int ret_int()
{
	return 1;
}*/

int main()
{
	std::cout << "the value is:";
	for(auto& val:Range(1,10))
	{
		std::cout << val << " ";
	}
	std::cout << "\n";

/*	Iterator const_obj = ret_obj();
	++const_obj;
	const_obj.output();

	int const_int = ret_int();
	++const_int;
	std::cout << const_int << std::endl;*/

	return 0;
}

//	返回值后置语法
//template<typename T,typename V>
//auto Range(T begin,T end,V step)->RanImp<decltype(begin+end+step)>
//{
//	return RanImp<decltype(begin+end+step)>(begin,end,step);
//}