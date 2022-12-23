#include "stdafx.h"
#include "Semantic.h"

namespace Semantic
{
	bool Semantic::semanticsCheck(Lex::LEX& tables, Log::LOG& log)
	{
		bool sem_ok = true;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
				//В объявлении не указан тип идентификатора
			case LEX_TYPE:
			{
				if (tables.lextable.table[i + 1].lexema != LEX_ID_TYPE)
				{
					sem_ok = false;
					Log::WriteError(log.stream, Error::GetError(303, tables.lextable.table[i].sn, 0));
				}
				break;
			}
			//Деление на ноль
			case LEX_DIRSLASH:
			case LEX_PROCENT:
			{
				int k = i;
				if (tables.lextable.table[i + 1].lexema == LEX_ID)
				{
					for (k; k > 0; k--)
					{
						if (tables.lextable.table[k].lexema == LEX_ID)
						{
							if (tables.idtable.table[tables.lextable.table[k].idxTI].id == tables.idtable.table[tables.lextable.table[i + 1].idxTI].id) 
							{
								if (tables.lextable.table[k + 2].lexema == LEX_LITERAL && tables.idtable.table[tables.lextable.table[k + 2].idxTI].value.vint == 0) //проверяем тип и значение
								{
									sem_ok = false;
									Log::WriteError(log.stream, Error::GetError(321, tables.lextable.table[i].sn, 0));
								}
							}
						}
					}
				}
				if (tables.lextable.table[i + 1].lexema == LEX_LITERAL)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint == 0)
					{
						sem_ok = false;
						Log::WriteError(log.stream, Error::GetError(321, tables.lextable.table[k].sn, 0));
					}
				}
				break;
			}
			case LEX_EQUAL: //выражение =
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX) // левый операнд 
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					bool ignore = false;

					for (int k = i + 1; tables.lextable.table[k].lexema != LEX_SEPARATOR; k++) //пока не дойдем до ;
					{
						if (k == tables.lextable.size) 
							break; // синтакс ошибка - нет ; // в равенстве не может быть ;
						if (tables.lextable.table[k].idxTI != TI_NULLIDX) // если идентификатор - проверить совпадение типов
						{
							if (!ignore)
							{
								IT::IDDATATYPE righttype = tables.idtable.table[tables.lextable.table[k].idxTI].iddatatype;
								if (lefttype != righttype) // типы данных в выражении не совпадают
								{
									Log::WriteError(log.stream, Error::GetError(314, tables.lextable.table[k].sn, 0));
									sem_ok = false;
									break;
								}
							}
							// если лексема сразу после идентиф скобка - это вызов функции
							if (tables.lextable.table[k + 1].lexema == LEX_LEFTHESIS)
							{
								ignore = true;
								continue;
							}
							// закрывающая скобка после списка параметров
							if (ignore && tables.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
							{
								ignore = false;
								continue;
							}
						}
						if (lefttype == IT::IDDATATYPE::STR || lefttype == IT::IDDATATYPE::SYM || lefttype == IT::IDDATATYPE::BOL) // справа только литерал, ид или вызов строковой ф-ции
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR || l == LEX_DIRSLASH || l == LEX_PROCENT) // выражения недопустимы =+ =- =* и тд
							{
								Log::WriteError(log.stream, Error::GetError(316, tables.lextable.table[k].sn, 0));
								sem_ok = false;
								break;
							}
						}
					}
				}
				break;
			}
			case LEX_STDFUNC:
			case LEX_ID: // проверка типа возвращаемого значения  
			{
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];

				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F) //функция
					{
						for (int k = i + 1; tables.lextable.table[k].lexema != LEX_BRACELET; k++)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_RETURN)
							{
								int next = tables.lextable.table[k + 1].idxTI; // след. за return
								if (next != TI_NULLIDX)
								{
									if (tables.idtable.table[next].iddatatype != e.iddatatype)			// тип функции и возвращаемого значения не совпадают
									{
										Log::WriteError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, 0));
										sem_ok = false;
										break;
									}
								}
								if (next != TI_NULLIDX && e.iddatatype == IT::IDDATATYPE::PROC || next == TI_NULLIDX && e.iddatatype != IT::IDDATATYPE::PROC)
								{
									Log::WriteError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, 0));
									sem_ok = false;
									break;
								}
							}

							if (k == tables.lextable.size)
								break;
						}
					}
				}
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION) // именно вызов
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S) // точно функция
					{
						int paramscount = NULL;
						// проверка передаваемых параметров
						for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
						{
							// проверка соответствия передаваемых параметров прототипам
							if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
							{
								paramscount++;
								if (e.value.params.count == NULL)
									break;
								IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
								if (ctype != e.value.params.types[paramscount - 1])
								{
									// Несовпадение типов передаваемых параметров
									Log::WriteError(log.stream, Error::GetError(309, tables.lextable.table[i].sn, 0));
									sem_ok = false;
									break;
								}
							}
							if (j == tables.lextable.size)
								break;
						}
						if (paramscount != e.value.params.count)
						{
							// Количество передаваемых и принимаемых параметров не совпадает
							Log::WriteError(log.stream, Error::GetError(308, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
						if (paramscount > 5)
						{
							// Слишком много параметров в вызове
							Log::WriteError(log.stream, Error::GetError(307, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
					}
				}
				break;
			}
			case LEX_LEFT_SHIFT:	case LEX_RIGHT_SHIFT:
			case LEX_LOGIC_MORE:	case LEX_LOGIC_LESS:
			case LEX_LOGIC_MORE_EQ:	case LEX_LOGIC_LESS_EQ:
			{
				// левый и правый операнд - числовой тип
				bool flag = true;
				if (i <= 1) flag = false;

				if (i > 1 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX)
				{
					if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::UINT)
						flag = false;
				}
				if (tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::UINT)
						flag = false;
				}
				if (!flag)
				{
					// строка или неизвестный ид в условии

					if (tables.lextable.table[i].lexema == LEX_LEFT_SHIFT || tables.lextable.table[i].lexema == LEX_RIGHT_SHIFT)
						Log::WriteError(log.stream, Error::GetError(320, tables.lextable.table[i].sn, 0));
					else
						Log::WriteError(log.stream, Error::GetError(319, tables.lextable.table[i].sn, 0));

					sem_ok = false;
				}
				break;
			}
			case LEX_LOGIC_EQUALS:   case LEX_LOGIC_NOT_EQUALS:
			{
				bool flag = false;
				if (i > 1 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX)
				{
					if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype == IT::IDDATATYPE::UINT
						&& tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						flag = true;
					else if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype == IT::IDDATATYPE::SYM
						&& tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::SYM)
						flag = true;
					else if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype == IT::IDDATATYPE::STR
						&& tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
						flag = true;
					else if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype == IT::IDDATATYPE::BOL
						&& tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::BOL)
						flag = true;
				}
				if (!flag)
				{
					// неизвестный ид в условии
					Log::WriteError(log.stream, Error::GetError(314, tables.lextable.table[i].sn, 0));
					sem_ok = false;
				}
				break;
			}
			}
		}
		return sem_ok;
	}
};