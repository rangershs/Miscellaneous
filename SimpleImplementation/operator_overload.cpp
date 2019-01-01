/*
运算符重载

	1	不是所有运算符都可以重载，[sizeof],[?:],[.],[::]不能被重载
	2	[->],[[]],[()],[=]只能以类的成员函数的形式重载
	3	[<<],[>>]必须重载为全局函数，若重载为成员函数，则需要去类istream和ostream中重载，这是不可能的
	4	以类的成员函数重载时，一元运算符没有参数，二元运算符有一个参数
		以全局函数重载时，一元运算符有一个参数，二元运算符有两个参数，且至少一个参数是类对象
	5	[++],[--]分前置和后置两种情况，允许以成员函数和友元函数两种方式重载，其中形参int的作用是区分前置和后置，C++选择增加一个占位参数实现后置版本
*/

#include <iostream>

class Complex
{
public:
	Complex(double _real=1.1,double _image=2.2):real(_real),image(_image) {}
	~Complex() {}

	Complex operator+(const Complex& A) const;
	friend Complex operator-(const Complex& A,const Complex& B);
	Complex& operator+=(const Complex& A);
	friend std::istream& operator>>(std::istream& is,Complex& A);
	friend std::ostream& operator<<(std::ostream& os,const Complex& A);

	Complex& operator++();
	Complex operator++(int);
	friend Complex& operator--(Complex& A);
	friend Complex operator--(Complex& A,int);
private:
	double real;
	double image;
};

Complex Complex::operator+(const Complex& A) const
{
	Complex C;
	C.real = this->real + A.real;
	C.image = this->image + A.image;
	return C;
}

Complex operator-(const Complex& A, const Complex& B)
{
	Complex C;
	C.real = A.real - B.real;
	C.image = A.image - B.image;
	return C;
}

Complex& Complex::operator+=(const Complex& A)
{
	this->real += A.real;
	this->image += A.image;
	return *this;
}

std::istream& operator>>(std::istream& is,Complex& A)
{
	is >> A.real >> A.image;
	return is;
}

std::ostream& operator<<(std::ostream& os,const Complex& A)
{
	os << A.real << A.image;
	return os;
}

Complex& Complex::operator++()
{
	this->real++;
	this->image++;
	return *this;
}

Complex Complex::operator++(int)
{
	Complex tmp(*this);
	this->real++;
	this->image++;
	return tmp;
}

Complex& operator--(Complex& A)
{
	A.real--;
	A.image--;
	return A;
}

Complex operator--(Complex& A,int)
{
	Complex tmp(A);
	A.real--;
	A.image--;
	return tmp;
}

int main()
{
	return 0;
}