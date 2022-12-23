#pragma once
#include "stdafx.h"
#include "IT.h"
#include "LT.h"

#define ID_MAXSIZE			32						//макс число символов идентификатора
#define SCOPED_ID_MAXSIZE   (ID_MAXSIZE*2-1)		//макс число символов идентификатор + область видимости
#define MAXSIZE_TI			4096					//макс число количество строк в таблице идентификаторов
#define TI_INT_DEFAULT		0x00000000				//значение по умолчанию для integer
#define TI_STR_DEFAULT		0x00					//значение по умолчанию для sting
#define TI_SYM_DEFAULT		0x00					//значение по умолчанию для symbol;
#define TI_BOL_DEFAULT		0x00					//значение по умолчанию для symbol;
#define TI_NULLIDX			0xffffffff				//нет элемента таблицы идентификаторов
#define STR_MAXSIZE			255						//максимальная длина строкового литерала
#define TI_INT_MAXSIZE		4294967295				//максимальное значение для типа integer + 1
#define TI_INT_MINSIZE		0						//минимальное значение для типа integer - 1

#define MAX_PARAMS_COUNT		5			   			//максимальное количество параметров у функции
#define POW_PARAMS_CNT			2						//кол-во параметров у функции pow
#define RANDOM_PARAMS_CNT		1						//кол-во параметров у функции rand
#define STRLEN_PARAMS_CNT		1						//кол-во параметров у функции lenght
#define INTTOSTR_PARAMS_CNT		1						//кол-во параметров у функции int to string
#define CONCAT_PARAMS_CNT		2						//кол-во параметров у функции concat
#define COPY_PARAMS_CNT			1						//кол-во параметров у функции copy
#define INPUT_PARAMS_CNT		0						//кол-во параметров у функций input
#define GDATETIME_PARAMS_CNT	0

#define RANDOM_TYPE			IT::IDDATATYPE::UINT
#define POW_TYPE			IT::IDDATATYPE::UINT
#define STRLEN_TYPE			IT::IDDATATYPE::UINT
#define INT_TO_STR_TYPE		IT::IDDATATYPE::STR
#define CONCAT_TYPE			IT::IDDATATYPE::STR
#define COPY_TYPE			IT::IDDATATYPE::STR

#define GTIME_TYPE			IT::IDDATATYPE::STR
#define GDATE_TYPE			IT::IDDATATYPE::STR

#define ININT_TYPE			IT::IDDATATYPE::UINT
#define INSTR_TYPE			IT::IDDATATYPE::STR
#define INSYM_TYPE			IT::IDDATATYPE::SYM

namespace IT {
	enum IDDATATYPE { UINT = 1, STR = 2, SYM = 3, BOL = 4, PROC = 5, UNDEF };								//типы данных идентификаторов: числовой, строковый, символьный, логический, неопределенный
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5, Z = 6 };												//типы идентификаторов: V = переменная, F = функция, P =параметр, L =литерал, S = стандартная функция Z - знак
	enum STDFNC { F_POW, F_POWER, F_RANDOM, F_STRLEN, F_NOT_STD,											//стандартные функции
			F_INT_TO_STR, F_CONCAT, F_COPY, F_ININT, F_INSTR, F_INSYM, F_GDATE, F_GTIME };			

	static const IDDATATYPE POW_PARAMS[] = { IT::IDDATATYPE::UINT, IT::IDDATATYPE::UINT };				//параметры функции  
	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::UINT };									//параметры функции  			
	static const IDDATATYPE STRLEN_PARAMS[] = { IT::IDDATATYPE::STR };									//параметры функции length
	static const IDDATATYPE INT_TO_STR_PARAMS[] = { IT::IDDATATYPE::UINT };								//параметры функции int to string
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };				//параметры функции concat
	static const IDDATATYPE COPY_PARAMS[] = { IT::IDDATATYPE::STR };									//параметры функции copy
	//static const IDDATATYPE INPUT_PARAMS[] = { IT::IDDATATYPE:: };									//параметры функции copy

	struct Entry	// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		char	id[SCOPED_ID_MAXSIZE];		// индентификатор (автоматически усекается до ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора

		union
		{
			unsigned int vint;					// значение integer
			struct
			{
				int len;							// количество символов в string
				char str[STR_MAXSIZE - 1];	// символы string
			} vstr;									// значение string
			char symbol;							//значение symbol
			struct
			{
				int count;					// количество параметров функции
				IDDATATYPE* types;			//типы параметров функции
			} params;
			bool vbol;
		} value;		// значение идентификатора

		Entry()							//конструктор без параметров
		{
			this->value.vint = TI_INT_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.params.count = NULL;
			this->value.symbol = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) //конструктор с параметрами
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};

	struct IdTable				// экземпляр таблицы идентификаторов
	{
		int maxsize;			// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;				// текущий размер таблицы идентификаторов < maxsize
		Entry* table;			// массив строк таблицы идентификаторов
	};

	IdTable Create(int size);		// емкость таблицы идентификаторов < TI_MAXSIZE// создать таблицу идентификаторов

	void Add(				// добавить строку в таблицу идентификаторов
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		Entry entry			// строка таблицы идентификаторов
	);
	int isId(				// возврат: номер строки (если есть), TI_NULLIDX (если нет)
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		char id[SCOPED_ID_MAXSIZE]	// идентификатор
	);
	bool SetValue(IT::Entry* entry, char* value);	//задать значение идентификатора
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void makeOutWithIT(IT::IdTable& idtable);
}