#pragma once
#include "stdafx.h"

namespace FST
{
	struct RELATION					//ребро:символ -> вершина графа переходов КА
	{
		unsigned char symbol;				//симпол перехода	
		short nnode;				//номер смежной вершины
		RELATION(
			unsigned char c = 0x00,	//символ перехода
			short ns = 0			//номер смежной вершины
		);
	};

	struct NODE						//вершина графа переходов
	{
		short n_relation;			//количество инцидентных ребер
		RELATION* relations;		//инцидентные ребра
		NODE();
		NODE
		(short n,				//количество инцидентных ребер
			RELATION rel, ...		//список ребер
		);
	};

	struct FST						//недетерминированный конечный автомат
	{
		unsigned char* string;				//цепочка (строка,завершается 0х00)
		short position;				//текущая позиция в цепочке
		short nstates;				//количество состояний автомата
		NODE* nodes;				//граф переходов: [0] - начальное состояние, [nstate-1] - конечное
		short* rstates;				//возможные состояния автомата на данной позиции
		FST(short ns, NODE n, ...);
		FST(unsigned char* s, FST& fst);
		FST(
			unsigned char* s,		//цепочка (строка, завершаеся 0х00)
			short ns,				//количество состояний автомата
			NODE n, ...				//список состояний (граф переходов)
		);
	};
	bool execute(
		FST& fst
	);


}