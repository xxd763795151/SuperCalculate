#pragma once
class CExpCalc
{
public:
	CExpCalc();
	~CExpCalc();

public:
	//���ַ�������һ�����
	void AddFlag(wchar_t* expression);
	//��׺���ʽת��Ϊ��׺���ʽ
	void Postfix(wchar_t * expression);
	//���ȼ��Ƚ�,c1ջ���������c2��ǰɨ������������
	wchar_t PreorityCmp(wchar_t c1, wchar_t c2);
	//������ʽ
	bool Calculate(wchar_t * expression, wchar_t * result);
	//�����������1�����򷵻�0
	int IsOperator(wchar_t c);
	//����,ǰ3������Ϊ�����,���һ��Ϊ����,����1�����ɹ�,����0����ʧ��
	double Calc(double first, double second, wchar_t op);
};

