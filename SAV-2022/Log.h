#pragma once
#include "In.h"
#include "Error.h"
#include "stdafx.h"

namespace Log {
	struct LOG { //протокол
		wchar_t logfile[PARM_MAX_SIZE]; //имя файла протокола
		std::ofstream* stream; //выходной поток протокола
	};

	static const LOG INITLOG = { L"",NULL };		//структура начальной инициализации LOG
	LOG getlog(wchar_t logfile[]);					//сформированная структура LOG
	void WriteLine(LOG log, const char* c, ...);		//вывести в протокол конкатенацию строк
	void WriteLine(LOG log, const wchar_t* c, ...);	//вывести в протокол конкатенацию строк
	void WriteLog(LOG log);							//вывести в протокол заголовок
	void WriteParm(LOG log, Parm::PARM parm);		//вывести в протокол информацию о входных параметрах
	void WriteIn(LOG log, In::IN in);				//вывести в протокол информацию о входном потоке
	void WriteError(std::ostream* stream, Error::ERROR error);	//вывести в протокол информацию об ошибке
	void Close(LOG log);							//закрыть протокол
	void WriteLineConsole(char* c, ...);
};