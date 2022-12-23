#pragma once
#include "IT.h"
#include "LT.h"
#include "stdafx.h"

#define TYPE_INTEGER	"uint"
#define TYPE_STRING		"string"
#define TYPE_SYMBOL		"char"
#define TYPE_BOOLEAN	"bool"
#define RANDOM			"random"
#define INT_TO_STR		"intToStr"
#define CONCAT			"concat"
#define COPY			"copy"
#define ININT			"inputInt"
#define INSTR			"inputString"
#define INSYM			"inputChar"
#define TYPE_VOID		"void"
#define MAIN			"Main"
#define POW				"pow"
#define STRLEN			"strLen"
#define GTIME			"getTimeNow"
#define GDATE			"getDateNow"

#define ISTYPE(str) ( !strcmp(str, TYPE_INTEGER) || !strcmp(str, TYPE_STRING) || !strcmp(str, TYPE_SYMBOL) || !strcmp(str, TYPE_BOOLEAN) )

namespace Lex
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;
		LEX() {}
	};
	struct Graph
	{
		char lexema;
		FST::FST graph;
	};
	IT::Entry* getEntry		// формирует и возвращает строку ТИ
	(
		Lex::LEX& tables,						// ТЛ + ТИ
		char lex,								// лексема
		char* id,								// идентификатор
		char* idtype,							// предыдущая (тип)
		bool isParam,							// признак параметра функции
		bool isFunc,							// признак функции
		Log::LOG log,							// протокол
		int line,								// строка в исходном тексте
		bool& rc_err							// флаг ошибки(по ссылке)
	);

	struct ERROR_S									// тип исключения для throw ERROR_THROW | ERROR_THROW_IN
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];			// сообщение			
		struct
		{
			short line = -1;						//номер строки (0, 1, 2, ...)
			short col = -1;						    //номер позиции в строке(0, 1, 2, ...)
		} position;
	};

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
	int getIndexInLT(LT::LexTable& lextable, int itTableIndex);					// индекс первой встречи в таблице лексем
};