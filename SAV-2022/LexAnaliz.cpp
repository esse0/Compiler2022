#pragma once
#define __STDC_WANT_LIB_EXT1__ 1
#include "LexAnaliz.h"
#include "stdafx.h"
#include "Graphs.h"

#include <string.h>
#include <cstring>
#include <stdio.h>

#include "IT.h"
#include "LT.h"
#include "Parm.h"


namespace Lex
{
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_SEPARATORS, FST::FST(GRAPH_INKR) },
		{ LEX_LOGIC_EQUALS, FST::FST(GRAPH_LOGIC_EQ) },
		{ LEX_LOGIC_NOT_EQUALS, FST::FST(GRAPH_LOGIC_NOT_EQ) },
		{ LEX_LOGIC_MORE, FST::FST(GRAPH_LOGIC_MORE) },
		{ LEX_LOGIC_LESS, FST::FST(GRAPH_LOGIC_LESS) },
		{ LEX_LOGIC_MORE_EQ, FST::FST(GRAPH_LOGIC_EQ_MORE) },
		{ LEX_LOGIC_LESS_EQ, FST::FST(GRAPH_LOGIC_EQ_LESS) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_UINT) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_BOOLEAN) },
		{ LEX_VOID, FST::FST(GRAPH_VOID) },
		{ LEX_STDFUNC, FST::FST(GRAPH_POW) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INT_TO_CHAR) },
		{ LEX_STDFUNC, FST::FST(GRAPH_CONCAT) },
		{ LEX_STDFUNC, FST::FST(GRAPH_RANDOM) },
		{ LEX_STDFUNC, FST::FST(GRAPH_STRLEN) },
		{ LEX_STDFUNC, FST::FST(GRAPH_COPY) },
		{ LEX_STDFUNC, FST::FST(GRAPH_ININT) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INSYM) },
		{ LEX_STDFUNC, FST::FST(GRAPH_INSTR) },
		{ LEX_STDFUNC, FST::FST(GRAPH_GDATE) },
		{ LEX_STDFUNC, FST::FST(GRAPH_GTIME) },
		{ LEX_LITERAL, FST::FST(GRAPH_UINT_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_BOOLEAN_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_SYMBOL_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_INT_NEGATIVE) },
		{ LEX_TYPE, FST::FST(GRAPH_TYPE) },
		{ LEX_MAIN, FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_SYMBOL) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_WRITE, FST::FST(GRAPH_WRITE) },
		{ LEX_NEWLINE, FST::FST(GRAPH_WRITELINE) },
		{ LEX_IF, FST::FST(GRAPH_IF) },
		{ LEX_WHILE, FST::FST(GRAPH_WHILE) },
		{ LEX_DO, FST::FST(GRAPH_DO) },
		{ LEX_ISFALSE, FST::FST(GRAPH_ISFALSE) },
		{ LEX_ISTRUE, FST::FST(GRAPH_ISTRUE) },
		{ LEX_ID, FST::FST(GRAPH_ID) },
		{ LEX_LEFT_SHIFT, FST::FST(GRAPH_SHIFT_LEFT) },
		{ LEX_RIGHT_SHIFT, FST::FST(GRAPH_SHIFT_RIGHT) }
	};

	char* getScopeName(IT::IdTable idtable, char* prevword)
	{
		char* a = new char[5];
		a = (char*)"main\0";
		if (strcmp(prevword, MAIN) == 0)
			return a;
		for (int i = idtable.size - 1; i >= 0; i--)
			if (idtable.table[i].idtype == IT::IDTYPE::F || idtable.table[i].idtype == IT::IDTYPE::S)
				return idtable.table[i].id;
		return nullptr; // не найдено имя функции
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == IN_CODE_QUOTE || *id == LEX_MINUS || *id == IN_CODE_NOT_DOUBLE_QUOTE || *id == NULL
			|| *id == IN_CODE_BOL_T || *id == IN_CODE_BOL_F)
			return true;
		return false;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(RANDOM, id))
			return IT::STDFNC::F_RANDOM;
		else if (!strcmp(POW, id))
			return IT::STDFNC::F_POW;
		else if (!strcmp(STRLEN, id))
			return IT::STDFNC::F_STRLEN;
		else if (!strcmp(INT_TO_STR, id))
			return IT::STDFNC::F_INT_TO_STR;
		else if (!strcmp(CONCAT, id))
			return IT::STDFNC::F_CONCAT;
		else if (!strcmp(COPY, id))
			return IT::STDFNC::F_COPY;
		else if (!strcmp(ININT, id))
			return IT::STDFNC::F_ININT;
		else if (!strcmp(INSYM, id))
			return IT::STDFNC::F_INSYM;
		else if (!strcmp(INSTR, id))
			return IT::STDFNC::F_INSTR;
		else if (!strcmp(GTIME, id))
			return IT::STDFNC::F_GTIME;
		else if (!strcmp(GDATE, id))
			return IT::STDFNC::F_GTIME;

		return IT::STDFNC::F_NOT_STD;
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) // получаем индекс повторного литерала(по значению)
	{
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::UINT:
					if (ittable.table[i].value.vint == (unsigned int)atoll(value))
						return i;
					break;
				case IT::IDDATATYPE::STR:
					char buf[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) // без кавычек
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp((char*)ittable.table[i].value.vstr.str, buf) == 0)
						return i;
					break;
				case IT::IDDATATYPE::SYM:
					if (ittable.table[i].value.symbol == value[1])
						return i;
					break;
				case IT::IDDATATYPE::BOL:
					if (ittable.table[i].value.vbol == (value[0] == 't' ? true : false))
						return i;
					break;
				}
			}
		}
		return TI_NULLIDX;
	}

	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (idtype == nullptr)
			return IT::IDDATATYPE::UNDEF;
		if (!strcmp(TYPE_SYMBOL, idtype))
			return IT::IDDATATYPE::SYM; // символьный ид
		if (!strcmp(TYPE_VOID, idtype))
			return IT::IDDATATYPE::PROC; // процедуры
		if (!strcmp(TYPE_STRING, idtype))
			return IT::IDDATATYPE::STR;  // строковый ид
		if (!strcmp(TYPE_INTEGER, idtype))
			return IT::IDDATATYPE::UINT;	 // числовой  ид
		if (!strcmp(TYPE_BOOLEAN, idtype))
			return IT::IDDATATYPE::BOL;	 // логичесий  ид
		if (isdigit(*curword))
			return IT::IDDATATYPE::UINT;				// числовой литерал
		else if (*curword == IN_CODE_QUOTE)
			return IT::IDDATATYPE::STR;	// строковый литерал
		else if (*curword == IN_CODE_NOT_DOUBLE_QUOTE)
			return IT::IDDATATYPE::SYM;	// символьный литерал
		else if (*curword == IN_CODE_BOL_T || *curword == IN_CODE_BOL_F)
			return IT::IDDATATYPE::BOL;	// символьный литерал

		return IT::IDDATATYPE::UNDEF;		// неопределенный тип, индикатор ошибки
	}

	char* getNextLiteralName()						// сгенерировать следующее имя литерала
	{
		static int literalNumber = 1;
		char* buf = new char[SCOPED_ID_MAXSIZE];
		char num[3];
		strcpy_s(buf, ID_MAXSIZE, "L");
		_itoa_s(literalNumber++, num, 10);
		strcat(buf, num);

		return buf;
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)					// индекс первой встречи в таблице лексем
	{
		if (itTableIndex == TI_NULLIDX)		// если идентификатор встречается впервые
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return TI_NULLIDX;
	}

	IT::Entry* getEntry(		// формирует и возвращает строку ТИ
		Lex::LEX& tables,						// ТЛ + ТИ
		char lex,								// лексема
		char* id,								// идентификатор
		char* idtype,							// предыдущая (тип)
		bool isParam,							// признак параметра функции
		bool isFunc,							// признак функции
		Log::LOG log,							// протокол
		int line,								// строка в исходном тексте
		bool& lex_ok)							// флаг ошибки(по ссылке)
	{
		// тип данных
		IT::IDDATATYPE type = getType(id, idtype);
		int index = IT::isId(tables.idtable, id);	// индекс существующего идентификатора или -1
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);
		if (index != TI_NULLIDX)
			return nullptr;							// уже существует
		IT::Entry* itentry = new IT::Entry;
		itentry->iddatatype = type; // сохраняем тип данных


		itentry->idxfirstLE = getIndexInLT(tables.lextable, index); // индекс первой строки в таблице лексем 

		if (lex == LEX_LITERAL) // литерал
		{
			bool int_ok = IT::SetValue(itentry, id);
			if (int_ok && itentry->iddatatype == IT::IDDATATYPE::UINT)
			{
				char p[11];
				_i64toa(itentry->value.vint, p, 10);
				index = getLiteralIndex(tables.idtable, p, type);
				if (index != TI_NULLIDX)
					return nullptr;
			}
			if (!int_ok)
			{
				// превышен максимальный размер числового литерала
				Log::WriteError(log.stream, Error::GetError(313, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				//использование пустого строкового литерала
				Log::WriteError(log.stream, Error::GetError(310, line, 0));
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else // идентификатор
		{
			switch (type)
			{
				//значения по дефолту
			case IT::IDDATATYPE::STR:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = TI_STR_DEFAULT;
				break;
			case IT::IDDATATYPE::UINT:
				itentry->value.vint = TI_INT_DEFAULT;
				break;
			case IT::IDDATATYPE::SYM:
				itentry->value.symbol = TI_SYM_DEFAULT;
				break;
			case IT::IDDATATYPE::BOL:
				itentry->value.vbol = TI_BOL_DEFAULT;
				break;
			}

			if (isFunc) //функции библиотеки
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_RANDOM:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = RANDOM_TYPE;
					itentry->value.params.count = RANDOM_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[RANDOM_PARAMS_CNT];

					for (int k = 0; k < RANDOM_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::RANDOM_PARAMS[k];

					break;
				}
				case IT::STDFNC::F_POW:
				{
					strncat(id, "er", ID_MAXSIZE);
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = POW_TYPE;
					itentry->value.params.count = POW_PARAMS_CNT;

					itentry->value.params.types = new IT::IDDATATYPE[POW_PARAMS_CNT];

					for (int k = 0; k < POW_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::POW_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_STRLEN:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = STRLEN_TYPE;
					itentry->value.params.count = STRLEN_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[STRLEN_PARAMS_CNT];

					for (int k = 0; k < STRLEN_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::STRLEN_PARAMS[k];

					break;
				}
				case IT::STDFNC::F_INT_TO_STR:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INT_TO_STR_TYPE;
					itentry->value.params.count = INTTOSTR_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INTTOSTR_PARAMS_CNT];

					for (int k = 0; k < INTTOSTR_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::INT_TO_STR_PARAMS[k];

					break;
				}
				case IT::STDFNC::F_CONCAT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = CONCAT_TYPE;
					itentry->value.params.count = CONCAT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[CONCAT_PARAMS_CNT];

					for (int k = 0; k < CONCAT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::CONCAT_PARAMS[k];

					break;
				}
				case IT::STDFNC::F_COPY:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = COPY_TYPE;
					itentry->value.params.count = COPY_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[COPY_PARAMS_CNT];

					for (int k = 0; k < COPY_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];

					break;
				}
				case IT::STDFNC::F_ININT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = ININT_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					break;
				}
				case IT::STDFNC::F_INSYM:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INSYM_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					break;
				}
				case IT::STDFNC::F_INSTR:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = INSTR_TYPE;
					itentry->value.params.count = INPUT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[INPUT_PARAMS_CNT];
					break;
				}
				case IT::STDFNC::F_GTIME:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = GTIME_TYPE;
					itentry->value.params.count = GDATETIME_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[GDATETIME_PARAMS_CNT];
					for (int k = 0; k < GDATETIME_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_GDATE:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = GDATE_TYPE;
					itentry->value.params.count = GDATETIME_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[GDATETIME_PARAMS_CNT];
					for (int k = 0; k < GDATETIME_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_NOT_STD:
					itentry->idtype = IT::IDTYPE::F;
				break;

				}

			}
			else if (isParam)
				itentry->idtype = IT::IDTYPE::P;
			else
				itentry->idtype = IT::IDTYPE::V;
			if (!isFunc)
			{
				memset(itentry->id, '\0', SCOPED_ID_MAXSIZE);
				strncat(itentry->id, id, SCOPED_ID_MAXSIZE);
			}
			else
			{
				char temp[SCOPED_ID_MAXSIZE] = "";
				strncat(temp, id, ID_MAXSIZE);
				memset(itentry->id, '\0', SCOPED_ID_MAXSIZE);
				strncat(itentry->id, id, SCOPED_ID_MAXSIZE);
			}
		}

		// -------------------------------------------------------
		int i = tables.lextable.size; // индекс в ТЛ текущего ИД

		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_TYPE)
		{
			// в объявлении отсутствует ключевое слово type
			Log::WriteError(log.stream, Error::GetError(304, line, 0));
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			// в объявлении не указан тип функции
			Log::WriteError(log.stream, Error::GetError(303, line, 0));
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			// невозможно определелить тип
			Log::WriteError(log.stream, Error::GetError(300, line, 0));
			lex_ok = false;
		}
		return itentry;
	}

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm) {

		static bool lex_ok = true;
		tables.lextable = LT::Create(LT_MAXSIZE);
		tables.idtable = IT::Create(MAXSIZE_TI);

		bool isParam = false, isFunc = false, isMain = false;
		int enterPoint = NULL;
		char curword[STR_MAXSIZE], nextword[STR_MAXSIZE];
		int curline;

		std::stack <char*> scopes;			// стек для хранения имени текущей области видимости
		for (int i = 0; i < in.words->size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < in.words->size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;
			isFunc = false;
			int idxTI = TI_NULLIDX;
			for (int j = 0; j < N_GRAPHS; j++) {
				FST::FST fst((unsigned char*)curword, graphs[j].graph);
				if (FST::execute(fst)) {
					char lexema = graphs[j].lexema;
					
					switch (lexema)
					{
					case LEX_MAIN:
						enterPoint++;
						break;
					case LEX_SEPARATORS: 
					{
						switch (*curword) {
						case LEX_LEFTHESIS:		// открывающая скобка - параметры  функции
						{
							isParam = true;
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))
							{
								char* functionname = new char[ID_MAXSIZE];								//функция для области видимости
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
								if (scopename == nullptr)
									break;
								memset(functionname, '\0', ID_MAXSIZE);
								strncat(functionname, scopename, ID_MAXSIZE);
								scopes.push(functionname);
							}
							break;
						}
						case LEX_RIGHTTHESIS:	// закрывающая скобка
						{
							isParam = false;
							// конец области видимости
							if ((*in.words[i - 1].word == LEX_LEFTHESIS || (i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE)))
								&& !scopes.empty())
								scopes.pop();
							break;
						}
						case LEX_LEFTBRACE:		// начало области видимости
						{
							isMain = true;
							char* functionname = new char[ID_MAXSIZE];
							char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
							if (scopename == nullptr)
								break;
							memset(functionname, '\0', ID_MAXSIZE);
							strncat(functionname, scopename, ID_MAXSIZE);
							scopes.push(functionname);
							break;
						}
						case LEX_BRACELET:		// конец области видимости
						{
							isMain = false;
							// только в этом случае закрываем область видимости
							if (*in.words[i + 1].word == LEX_ID_TYPE || *in.words[i + 1].word == LEX_MAIN)
							{
								if (!scopes.empty())
									scopes.pop();
							}
							break;
						}

					}
						lexema = *curword;
						break;
					}
					case LEX_ID:
					case LEX_STDFUNC:
					case LEX_LITERAL:
					{
						char id[STR_MAXSIZE] = "";
						idxTI = NULLDX_TI;										// индекс идентификатора в ТИ
						if (*nextword == LEX_LEFTHESIS || IT::isId(tables.idtable, curword) != TI_NULLIDX)
						{
							isFunc = true;
							if (getStandFunction(curword) == IT::STDFNC::F_NOT_STD)
								strncat(id, "_", ID_MAXSIZE);
						}
						char* idtype = (isFunc && i > 1) ?						// тип идентификатора
							in.words[i - 2].word : in.words[i - 1].word;		// пропускаем ключевое слово function
						if (i == 0)
							idtype = nullptr;
						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), ID_MAXSIZE);
						strncat(id, curword, ID_MAXSIZE);
						if (isLiteral(curword))
							strcpy_s(id, curword); // литерал заменяется своим значением
						IT::Entry* itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);
						
						if (itentry != nullptr) // первая встреча - объявление
						{
							if (isFunc && itentry->idtype != IT::IDTYPE::S) // если функция - сохранить список параметров
							{
								itentry->value.params.count = NULL;
								itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
								for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++) 
								{
									if (k == in.words->size || in.words[k].word[0] == LEX_SEPARATOR)
										break;
									if (ISTYPE(in.words[k].word))
									{
										if (itentry->value.params.count >= MAX_PARAMS_COUNT)
										{
											Log::WriteError(log.stream, Error::GetError(306, curline, 0));
											lex_ok = false;
											break;
										}
										itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
									}
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else // повторный идентификатор (уже есть)
						{
							int i = tables.lextable.size - 1; // попытка переопределить идентификатор
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_TYPE || tables.lextable.table[i].lexema == LEX_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_VOID || tables.lextable.table[i].lexema == LEX_VOID)
							{
								Log::WriteError(log.stream, Error::GetError(305, curline, 0));
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);	// индекс существующего идентификатора
							if (lexema == LEX_LITERAL)
							{
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); // или литерала
								if (idxTI == -1)
								{
									char p[10];
									_i64toa(itentry->value.vint, p, 10);
									idxTI = getLiteralIndex(tables.idtable, p, getType(id, in.words[i - 1].word));
								}
							}
						}
					}
					break;
					}
					LT::Entry* ltentry = new LT::Entry(lexema, curline, idxTI);
					LT::Add(tables.lextable, *ltentry);
					break;
				}
				else if (j == N_GRAPHS - 1) // цепочка не распознана
				{
					Log::WriteError(log.stream, Error::GetError(201, curline, 0));
					lex_ok = false;
				}
			}
		}
		if (enterPoint == NULL) // не найдена точка входа
		{
			Log::WriteError(log.stream, Error::GetError(301));
			lex_ok = false;
		}
		if (enterPoint > 1) //больше 1 точки входа
		{
			Log::WriteError(log.stream, Error::GetError(302));
			lex_ok = false;
		}
		if (isMain)
		{
			Log::WriteError(log.stream, Error::GetError(317));
			lex_ok = false;
		}
		return lex_ok;
	}
}