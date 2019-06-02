#pragma once
class CExpCalc
{
public:
	CExpCalc();
	~CExpCalc();

public:
	//给字符串增加一个标记
	void AddFlag(wchar_t* expression);
	//中缀表达式转换为后缀表达式
	void Postfix(wchar_t * expression);
	//优先级比较,c1栈顶运算符，c2当前扫描读到的运算符
	wchar_t PreorityCmp(wchar_t c1, wchar_t c2);
	//计算表达式
	bool Calculate(wchar_t * expression, wchar_t * result);
	//是运算符返回1，否则返回0
	int IsOperator(wchar_t c);
	//计算,前3个参数为入参数,最后一个为出参,返回1操作成功,返回0操作失败
	double Calc(double first, double second, wchar_t op);
};

