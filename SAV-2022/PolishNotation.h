#pragma once
#include "stdafx.h"

typedef std::vector <LT::Entry> ltvec;
typedef std::vector <int> intvec;

namespace Polish
{
	int PolishNotation(int i, Lex::LEX& lex);
	int getPriority(unsigned char e);
	int searchExpression(Lex::LEX lex);
};