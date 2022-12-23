#include "stdafx.h"
#include "PolishNotation.h"
using namespace std;

namespace Polish
{
	int getPriority(unsigned char e)
	{
		switch (e)
		{
		case LEX_LEFTHESIS: case LEX_RIGHTTHESIS: return 0;
		case LEX_COMMA: return 1;
		case LEX_PLUS: case LEX_MINUS: return 2;
		case LEX_STAR: case LEX_DIRSLASH: case LEX_PROCENT: return 3;
		default: return -1;
		}
	}
	int searchExpression(Lex::LEX lex) {
		bool fl = false;
		Lex::LEX result;
		result.lextable = LT::Create(lex.lextable.size);
		result.idtable = IT::Create(lex.idtable.size);
		for (int i = 0, k = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_EQUAL) {
				lex.lextable.size = PolishNotation(++i, lex);
			}
		}
		return lex.lextable.size;
	}
	int PolishNotation(int lextable_pos, Lex::LEX& lex)
	{
		stack<LT::Entry> stack;														// стек для операций
		queue<LT::Entry> queue;														// очередь для операндов

		LT::Entry temp;		temp.idxTI = -1;	temp.lexema = '#';	temp.sn = -1;	// запрещенная лексема, все лишние элементы будут заменяться на нее
		LT::Entry func;		func.idxTI = -1;	func.lexema = '@';	func.sn = -1;	// лексема для вызова функции
		LT::Entry tilda;	tilda.idxTI = -1;	tilda.lexema = '~';	tilda.sn = -1;	// лексема для отрицательных

		int countLex = 0;															// количество преобразованных лексем
		int countParm = -1;															// количество параметров функции
		int posLex = lextable_pos;																// запоминаем номер лексемы перед преобразованием
		bool findFunc = false;														// флаг для функции
		bool findComma = false;														// флаг для запятой (кол-во параметров +2 сразу)
		bool flagthethis = false;
		int skob = 0;
		int comma = 0;
		char* buf = new char[2];													// буфер для countParm в строковом представлении
		for (int i = lextable_pos; lex.lextable.table[i].lexema != LEX_SEPARATOR; i++, countLex++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_STDFUNC:
			case LEX_ID:															// если идентификатор
			{
				if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F || lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::S)
				{
					findFunc = true;
				}
				if (findFunc)
				{
					countParm++;
				}
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_LITERAL:															// если литерал
			{
				queue.push(lex.lextable.table[i]);
				if (findFunc)
				{
					countParm++;
				}
				continue;
			}
			case LEX_LEFTHESIS:														// если (
			{
				if (lex.lextable.table[i + 1].lexema == LEX_MINUS && lex.lextable.table[i + 2].lexema == LEX_ID)
				{
					flagthethis = true;
					continue;
				}
				stack.push(lex.lextable.table[i]);										// помещаем ее в стек
				continue;
			}
			case LEX_RIGHTTHESIS:														// если )
			{
				if (findFunc)															// если это вызов функции, то лексемы () заменяются на @ и кол-во параметров
				{
					func.sn = lex.lextable.table[i].sn;
					lex.lextable.table[i] = func;
					queue.push(lex.lextable.table[i]);									// добавляем в очередь лексему вызова функции
					_itoa_s(countParm, buf, 2, 10);										// преобразование числа countParm в строку
					stack.top().lexema = buf[0];
					stack.top().idxTI = -1; stack.top().sn = lex.lextable.table[i].sn;						// заполняем лексему, указывающую количество параметров функции
					queue.push(stack.top());											// добавляем в очередь эту лексему
					findFunc = false;
				}
				else {
					while (stack.top().lexema != LEX_LEFTHESIS)						// пока не встретим (
					{
						queue.push(stack.top());										// выталкиваем со стека в очередь
						stack.pop();

						if (stack.empty())
							return 0;
					}
				}
				stack.pop();												// уничтожаем ( или лексему, указывающую количество параметров функции
				continue;
			}
			case LEX_PLUS:															// если знак оператора
			case LEX_MINUS:															// если знак оператора
			case LEX_STAR:															// если знак оператора
			case LEX_DIRSLASH:														// если знак оператора
			case LEX_PROCENT:														// если знак оператора
			{
				if (flagthethis)
				{
					tilda.sn = lex.lextable.table[i].sn;
					lex.lextable.table[i] = tilda;
					queue.push(lex.lextable.table[i + 1]);									// добавляем в очередь лексему вызова функции
					queue.push(lex.lextable.table[i]);									// добавляем в очередь лексему вызова функции
					flagthethis = false;
					i += 2;
					countLex += 2;
					continue;
				}
				while (!stack.empty() && getPriority(lex.lextable.table[i].lexema) <= getPriority(stack.top().lexema))	// пока приоритет текущего оператора 
																							//меньше или равен приоритету оператора в вершине стека
				{
					queue.push(stack.top());											// выталкиваем со стека в выходную строку
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_COMMA:																// если запятая
			{
				findComma = true;
				continue;
			}
			}
		}
		while (!stack.empty())															// если стек не пустой
		{
			if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return 0;
			queue.push(stack.top());													// выталкиваем все в очередь
			stack.pop();
		}
		while (countLex != 0)															// замена текущего выражения в таблице лексем на польскую запись
		{
			if (!queue.empty()) {
				lex.lextable.table[posLex++] = queue.front();
				cout << lex.lextable.table[posLex - 1].lexema << " ";				// вывод в консоль
				queue.pop();
			}
			else
			{
				lex.lextable.table[posLex++] = temp;
			}
			countLex--;
		}
		for (int i = 0; i < posLex; i++)												// восстановление индексов первого вхождения в таблицу лексем у операторов из таблицы идентификаторов
		{
			if (lex.lextable.table[i].lexema == LEX_LITERAL)
				lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE = i;
		}
		return lex.lextable.size;
	}
}

