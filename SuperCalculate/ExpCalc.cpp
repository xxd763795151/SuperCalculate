#include "stdafx.h"
#include "ExpCalc.h"


CExpCalc::CExpCalc()
{
}


CExpCalc::~CExpCalc()
{
}

void CExpCalc::AddFlag(wchar_t* expression)
{
	wchar_t expTmp[MAXBYTE] = { 0 };
	unsigned posTmp = 0;
	expTmp[0] = expression[0];
	unsigned iLength = wcslen(expression);
	for (size_t pos = 1; pos < iLength; ++pos)
	{
		if ((isdigit(expression[pos - 1]) && expression[pos]!='.') 
			&& !isdigit(expression[pos])
			)
			expTmp[++posTmp] = '#';
		expTmp[++posTmp] = expression[pos];
	}
	if (isdigit(expression[iLength - 1]))
	{
		expTmp[++posTmp] = '#';
	}
	wcscpy(expression, expTmp);
}

void CExpCalc::Postfix(wchar_t * expression)
{
	std::stack<wchar_t> cstack;
	std::wstring strPostfix = L"";
	for (size_t pos = 0; pos < wcslen(expression);)
	{
		if (isdigit(expression[pos]) || expression[pos] == '#' || expression[pos] == '.')
		{
			strPostfix += expression[pos];
			++pos;
		}
		else
		{
			if (cstack.empty())
			{
				cstack.push(expression[pos]);
				++pos;
			}
			else if (PreorityCmp(cstack.top(), expression[pos]) == '<')
			{
				cstack.push(expression[pos]);
				++pos;
			}
			else if (PreorityCmp(cstack.top(), expression[pos]) == '>')
			{

				wchar_t c = cstack.top();
				cstack.pop();
				strPostfix += c;
			}
			else if (PreorityCmp(cstack.top(), expression[pos]) == '=')
			{
				cstack.pop();
				++pos;
			}
		}
	}
	while (!cstack.empty())
	{
		strPostfix += cstack.top();
		cstack.pop();
	}
		
	wcscpy(expression, strPostfix.c_str());
}

wchar_t CExpCalc::PreorityCmp(wchar_t c1, wchar_t c2)
{
	wchar_t cResult = '=';
	std::wstring strBig = L"++,+-,+),-+,--,-),*+,*-,**,*//,*),//+,//-,//*,////,//),)+,)-,)*,)//,)),";
	std::wstring strSmall = L"+*,+//,+(,-*,-//,-(,*(,//(,(+,(-,(*,(//,((,";
	std::wstring strEqual = L"()";
	std::wstring str = L"";
	str = c1;
	str += c2;
	if (strBig.npos != strBig.find(str))
		cResult = '>';
	else if (strSmall.npos != strSmall.find(str))
		cResult = '<';
	else
		;
	return cResult;
}

bool CExpCalc::Calculate(wchar_t * expression, wchar_t * result)
{
	std::stack<std::wstring> pcStack;
	wchar_t cValue[MAXBYTE] = { 0 };
	unsigned num = 0;
	for (size_t pos = 0; pos < wcslen(expression); ++pos)
	{
		if (isdigit(expression[pos]) || expression[pos] == '.')
		{
			cValue[num++] = expression[pos];
		}
		else if (expression[pos] == '#')
		{
			pcStack.push(cValue);
			memset(cValue, 0, MAXBYTE);
			num = 0;
		}
		else if (IsOperator(expression[pos]))
		{
			/*wchar_t* cf =( wchar_t*)pcStack.top().c_str();
			if (!isdigit(cf[0])) return false;*/
			if (pcStack.empty()) return false;
			double second = _wtof(pcStack.top().c_str()); pcStack.pop();
			/*wchar_t* cs = (wchar_t*)pcStack.top().c_str();
			if (!isdigit(cs[0])) return false;*/
			if (pcStack.empty()) return false;
			double first = _wtof(pcStack.top().c_str()); pcStack.pop();
			wchar_t cRet[MAXBYTE] = { 0 };
			double result = Calc(first, second, expression[pos]);
			//_gcvt(result, 10, cRet);
			swprintf(cRet, L"%lf", result);
			pcStack.push(cRet);
		}
	}
	wcscpy(result, pcStack.top().c_str());
	return true;
}


double CExpCalc::Calc(double first, double second, wchar_t op)
{
	double iResult;
	switch (op)
	{
	case '+':
		iResult = first + second;
		break;
	case '-':
		iResult = first - second;
		break;
	case '*':
		iResult = first * second;
		break;
	case '/':
		iResult = first / second;
		break;
	}
	return iResult;
}

int CExpCalc::IsOperator(wchar_t c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/') ? 1 : 0;
}
