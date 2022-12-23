#pragma once
#include "GRB.h"

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 19,

		Rule(NS('S'), GRB_ERROR_SERIES, 3,						// Неверная структура программы	
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')),
			Rule::Chain(6, TS('g'), TS('f'), TS('i'), NS('P'), NS('G'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('K'), TS('}'))
		),

		Rule(NS('T'), GRB_ERROR_SERIES + 2, 2,								// Ошибка в теле функции
			Rule::Chain(3, TS('{'), NS('Q'), TS('}')),
			Rule::Chain(4, TS('{'), NS('K'), NS('Q'), TS('}'))
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 1, 2,								// Не найден список параметров функции	
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 3, 2,								// Ошибка в списке параметров функции						
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E')),
			Rule::Chain(2, TS('t'), TS('i'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 4, 2,								// Ошибка в вызове функции						
			Rule::Chain(3, TS('('), NS('N'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 5, 4,								// Ошибка в списке параметров функции		
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('N')),
			Rule::Chain(3, TS('l'), TS(','), NS('N'))
		),

		Rule(NS('R'), GRB_ERROR_SERIES + 6, 4,								// Ошибка при констуировании условного выражения	
			Rule::Chain(2, TS('r'), NS('Y')),
			Rule::Chain(2, TS('w'), NS('Y')),
			Rule::Chain(4, TS('r'), NS('Y'), TS('w'), NS('Y')),
			Rule::Chain(4, TS('w'), NS('Y'), TS('r'), NS('Y'))
		),
		Rule(NS('Z'), GRB_ERROR_SERIES + 8, 4,								// Ошибка в условии цикла/условного выражения	
			Rule::Chain(3, TS('i'), NS('L'), TS('i')),
			Rule::Chain(3, TS('i'), NS('L'), TS('l')),
			Rule::Chain(3, TS('l'), NS('L'), TS('l')),
			Rule::Chain(3, TS('l'), NS('L'), TS('i'))
		),

		Rule(NS('L'), GRB_ERROR_SERIES + 9, 6,				// Неверный условный оператор		/ ЛОГИЧЕСКИЕ ОПЕРАТОРЫ
			Rule::Chain(1, TS('0')), // ==
			Rule::Chain(1, TS('1')), // !=
			Rule::Chain(1, TS('2')), // >
			Rule::Chain(1, TS('3')), // <
			Rule::Chain(1, TS('4')), // >=
			Rule::Chain(1, TS('5'))  // <=
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 10, 5,											// Неверный арифметический оператор
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('%')),
			Rule::Chain(1, TS('/'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 11, 12,										// Ошибка в арифметичском выражении
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('('), TS('-'), TS('i'), TS(')')),
			Rule::Chain(6, TS('('), TS('-'), TS('i'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(2, TS('p'), NS('F')),
			Rule::Chain(3, TS('i'), NS('A'), NS('W')),
			Rule::Chain(3, TS('l'), NS('A'), NS('W')),
			Rule::Chain(4, TS('i'), NS('F'), NS('A'), NS('W')),
			Rule::Chain(4, TS('p'), NS('F'), NS('A'), NS('W'))
		),

		Rule(NS('K'), GRB_ERROR_SERIES + 12, 30,							// Недопустимая синтаксическая конструкция
			Rule::Chain(4, TS('?'), NS('Z'), NS('R'), NS('K')),				// if
			Rule::Chain(5, TS('c'), NS('Z'), TS('d'), NS('H'), NS('K')),	// while
			Rule::Chain(3, TS('?'), NS('Z'), NS('R')),						// if
			Rule::Chain(4, TS('c'), NS('Z'), TS('d'), NS('H')),				// while	

			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('V'), TS(';'), NS('K')),	// декларация + присваивание
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('W'), TS(';'), NS('K')),	// декларация + присваивание
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('K')),	// декларация
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('K')),	// присваивание
			Rule::Chain(5, TS('i'), TS(':'), NS('V'), TS(';'), NS('K')),	// присваивание

			Rule::Chain(4, TS('o'), NS('V'), TS(';'), NS('K')),				// вывод
			Rule::Chain(4, TS('^'), NS('V'), TS(';'), NS('K')),						// перевод строки
			Rule::Chain(6, TS('i'), TS('='), TS('U'), NS('F'), TS(';'), NS('K')),	// присваивание c помощью вызова функции 

			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('K')),				// вызов функции

			Rule::Chain(5, TS('i'), TS('6'), TS('l'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('6'), TS('i'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('7'), TS('l'), TS(';'), NS('K')),
			Rule::Chain(5, TS('i'), TS('7'), TS('i'), TS(';'), NS('K')),

			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('V'), TS(';')),	// декларация + присваивание
			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('W'), TS(';')),	// декларация + присваивание
			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),				// присваивание
			Rule::Chain(4, TS('i'), TS(':'), NS('V'), TS(';')),	// присваивание
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),				// декларация
			Rule::Chain(3, TS('o'), NS('V'), TS(';')),						// вывод
			Rule::Chain(3, TS('^'), NS('V'), TS(';')),								// перевод строки
					
			Rule::Chain(5, TS('i'), TS('='), TS('U'), NS('F'), TS(';')),	// присваивание c помощью вызова функции
			Rule::Chain(3, TS('i'), NS('F'), TS(';')),			// вызов функции

			Rule::Chain(4, TS('i'), TS('6'), TS('l'), TS(';')),
			Rule::Chain(4, TS('i'), TS('6'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('7'), TS('l'), TS(';')),
			Rule::Chain(4, TS('i'), TS('7'), TS('i'), TS(';'))
		),

		Rule(NS('X'), GRB_ERROR_SERIES + 13, 24,								// Недопустимая синтаксическая конструкция в теле цикла/условного выражения	

			Rule::Chain(4, TS('?'), NS('Z'), NS('R'), NS('X')),				// if
			Rule::Chain(5, TS('c'), NS('Z'), TS('d'), NS('H'), NS('X')),	// while
			Rule::Chain(3, TS('?'), NS('Z'), NS('R')),						// if
			Rule::Chain(4, TS('c'), NS('Z'), TS('d'), NS('H')),				// while	

			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('X')),		// присваивание
			Rule::Chain(5, TS('i'), TS(':'), NS('V'), TS(';'), NS('X')),		// присваивание
			Rule::Chain(4, TS('o'), NS('V'), TS(';'), NS('X')),					// вывод
			Rule::Chain(4, TS('^'), NS('V'), TS(';'), NS('X')),						// перевод строки
			Rule::Chain(6, TS('i'), TS('='), TS('U'), NS('F'), TS(';'), NS('X')),	// присваивание c помощью вызова функции 

			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('X')),						// вызов функции

			Rule::Chain(5, TS('i'), TS('6'), TS('l'), TS(';'), NS('X')),
			Rule::Chain(5, TS('i'), TS('6'), TS('i'), TS(';'), NS('X')),
			Rule::Chain(5, TS('i'), TS('7'), TS('l'), TS(';'), NS('X')),
			Rule::Chain(5, TS('i'), TS('7'), TS('i'), TS(';'), NS('X')),

			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),						// присваивание
			Rule::Chain(3, TS('o'), NS('V'), TS(';')),								// вывод
			Rule::Chain(3, TS('^'), NS('V'), TS(';')),								// перевод строки
			Rule::Chain(4, TS('i'), TS(':'), NS('V'), TS(';')),						// присваивание
			Rule::Chain(5, TS('i'), TS('='), TS('U'), NS('F'), TS(';')),			// присваивание c помощью вызова функции 

			Rule::Chain(3, TS('i'), NS('F'), TS(';')),								// вызов функции

			Rule::Chain(4, TS('i'), TS('6'), TS('l'), TS(';')),
			Rule::Chain(4, TS('i'), TS('6'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('7'), TS('l'), TS(';')),
			Rule::Chain(4, TS('i'), TS('7'), TS('i'), TS(';'))
		),
		Rule(NS('Q'), GRB_ERROR_SERIES + 7, 4,						// Ошибка при конструировании return
			Rule::Chain(3, TS('e'), NS('V'), TS(';')),
			Rule::Chain(6, TS('e'), TS('('), TS('-'), TS('i'), TS(')'), TS(';')),
			Rule::Chain(2, TS('e'), TS(';')),
			Rule::Chain(5, TS('e'), TS('('), TS('l'), TS(')'), TS(';'))
		),
		Rule(NS('Y'), GRB_ERROR_SERIES + 16, 4,						// Ошибка в теле условного выражения
			Rule::Chain(4, TS('['), NS('X'), NS('Q'), TS(']')),
			Rule::Chain(3, TS('['), NS('X'), TS(']')),
			Rule::Chain(3, TS('['), NS('Q'), TS(']')),
			Rule::Chain(2, TS('['), TS(']'))
		),
		Rule(NS('H'), GRB_ERROR_SERIES + 17, 4,						// ошибка в теле цикла	
			Rule::Chain(4, TS('['), NS('X'), NS('Q'), TS(']')),
			Rule::Chain(3, TS('['), NS('Q'), TS(']')),
			Rule::Chain(3, TS('['), NS('X'), TS(']')),
			Rule::Chain(2, TS('['), TS(']'))
		),
		Rule(NS('V'), GRB_ERROR_SERIES + 15, 2,						// Правила для идентификаторов или литералов	
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		
		Rule(NS('U'), GRB_ERROR_SERIES + 3, 2,						// Правила для идентификатора функции	
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('p'))
		),
		Rule(NS('G'), GRB_ERROR_SERIES + 2, 2,						// ошибка в теле процедуры
			Rule::Chain(4, TS('{'), TS('e'), TS(';'), TS('}')),
			Rule::Chain(5, TS('{'), NS('K'), TS('e'), TS(';'), TS('}'))
		)
	);
}