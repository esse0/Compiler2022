#include "stdafx.h"
#include "Generator.h"
#include <string>

using namespace std;

namespace Gener
{

	bool CodeGeneration(Lex::LEX& lex, Parm::PARM& parm, Log::LOG& log)
	{

		ofstream ofile(parm.out);

		ofile << BEGIN;
		ofile << EXTERN;
		ofile << ".const\n null_division BYTE 'ERROR: DIVISION BY ZERO', 0\n overflow BYTE 'ERROR: VARIABLE OVERFLOW', 0 \n";

		int whileNum = 0, ifNum = 0;

		stack<int> ifStack;
		stack<int> whileStack;
		stack<bool> lopstack;

		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::L)
			{
				ofile << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].iddatatype == IT::SYM)
				{
					ofile << " BYTE \'" << lex.idtable.table[i].value.symbol << "\', 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::STR)
				{
					ofile << " BYTE \'" << lex.idtable.table[i].value.vstr.str << "\', 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::BOL)
				{
					ofile << " DWORD " << (lex.idtable.table[i].value.vbol ? 1 : 0) << endl;
				}
				if (lex.idtable.table[i].iddatatype == IT::UINT)
				{
					ofile << " DWORD " << lex.idtable.table[i].value.vint << endl;
				}
			}
		}
		ofile << ".data\n";
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::IDTYPE::V)
			{
				ofile << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].iddatatype == IT::SYM)
				{
					ofile << " DWORD ?\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::STR)
				{
					ofile << " DWORD ?\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::BOL)
				{
					ofile << " DWORD 0\n";
				}
				if (lex.idtable.table[i].iddatatype == IT::UINT)
				{
					ofile << " DWORD 0\n";
				}
			}
		}

		stack<string> expressionStack;
		stack<IT::Entry> temp;

		string func_name = "";						// имя локальной функции

		bool flag_while = false,					// внутри цикла?
			flag_main = false,
			flag_return = false;

		ofile << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNCTION:
			{
				i++;
				flag_return = false;
				ofile << (func_name = ITENTRY(i).id) << " PROC ";
				int j = i + 2;
				while (LEXEMA(j) != LEX_RIGHTTHESIS) // пока параметры не кончатся
				{
					if (lex.lextable.table[j].lexema == LEX_ID) // параметр
					{
						ofile << lex.idtable.table[lex.lextable.table[j].idxTI].id << " : ";
						ofile << " DWORD ";
					}
					if (LEXEMA(j) == LEX_COMMA)
					{
						ofile << ", ";
					}
					j++;
				}
				ofile << endl;
				break;
			}
			case LEX_MAIN:
			{
				flag_main = true;
				ofile << "main PROC\n";
				break;
			}
			case LEX_RETURN:
			{
				if (flag_main)
				{
					Log::WriteError(log.stream, Error::GetError(320, lex.lextable.table[i].sn, 0));
					ofile.close();
					return false;
				}
				if (LEXEMA(i + 1) != LEX_SEPARATOR)
				{
					if (LEXEMA(i + 1) != LEX_LEFTHESIS && (ITENTRY(i + 1).iddatatype == IT::IDDATATYPE::UINT
						|| ITENTRY(i + 1).idtype == IT::IDTYPE::P
						|| ITENTRY(i + 1).idtype == IT::IDTYPE::V))
						ofile << "\tmov eax, " << ITENTRY(i + 1).id << "\n";
					else if (LEXEMA(i + 1) != LEX_LEFTHESIS)
						ofile << "\tmov eax,offset " << ITENTRY(i + 1).id << "\n";
					else if (LEXEMA(i + 2) != LEX_MINUS)
						ofile << "\tmov eax, " << ITENTRY(i + 2).id << "\n";
					else
					{
						ofile << "\tmov eax, 0\n";
						ofile << "\tmov ebx," << ITENTRY(i + 3).id;
						ofile << "\n\tsub eax, ebx\n";
					}
				}
				ofile << "\tret\n";
				if (!flag_return)
				{
					ofile << "\nSOMETHINGWRONG:"\
						"\npush offset null_division"\
						"\ncall outstrline\n"\
						"call system_pause"\
						"\npush -1"\
						"\ncall ExitProcess\n"\
						"\nEXIT_OVERFLOW:"\
						"\npush offset overflow"\
						"\ncall outstrline\n"\
						"call system_pause"\
						"\npush -2"\
						"\ncall ExitProcess\n";
					flag_return = true;
				}
				break;
			}
			case LEX_BRACELET:
			{
				if (flag_main)
					break;
				ofile << func_name + " ENDP\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				bool HaveFunc = false;

				while (lex.lextable.table[i].lexema != LEX_SEPARATOR)
				{

					switch (LEXEMA(i))
					{
					case LEX_STDFUNC:
					case LEX_LITERAL:
					{
						if (ITENTRY(i).iddatatype == IT::IDDATATYPE::UINT
							|| ITENTRY(i).iddatatype == IT::IDDATATYPE::BOL)
						{
							ofile << "\tpush " << ITENTRY(i).id << endl;
							expressionStack.push(lex.idtable.table[lex.lextable.table[i].idxTI].id);
							break;
						}
						else
						{
							ofile << "\tpush offset " << ITENTRY(i).id << endl;
							expressionStack.push("offset " + (string)lex.idtable.table[lex.lextable.table[i].idxTI].id);
							break;
						}
					}
					case LEX_ID:
					{
						ofile << "\tpush " << ITENTRY(i).id << endl;
						expressionStack.push(lex.idtable.table[lex.lextable.table[i].idxTI].id);
						break;
					}
					case LEX_SUBST:
					{
						for (int j = 1; j <= (LEXEMA(i + 1) - '0') + 1; j++)
						{
							ofile << "\tpop edx\n";
						}

						for (int j = 1; j <= lex.lextable.table[i + 1].lexema - '0'; j++)
						{
							ofile << "\tpush " << expressionStack.top() << endl;
							expressionStack.pop();
						}

						// Вызывает исключение когда в выражении более одного вызова функции
						if (HaveFunc) {
							Log::WriteError(log.stream, Error::GetError(1, lex.lextable.table[i].sn, 0));
							throw ERROR_THROW_IN(1, lex.lextable.table[i].sn, 0);
						}

						HaveFunc = true;

						ofile << "\t\tcall " << ITENTRY(i - (lex.lextable.table[i + 1].lexema - '0') - 1).id;
						ofile << "\n\tpush eax\n";
						break;
					}
					case LEX_NOT_DOUBLE_QUOTE:
					{
						ofile << "\tmov eax, 0\n\tpop ebx\n";
						ofile << "\tsub eax, ebx\n\tpush eax\n";

						break;
					}
					case LEX_STAR:
					{
						ofile << "\tpop eax\n\tpop ebx\n";
						ofile << "\timul ebx\n\tjo EXIT_OVERFLOW\n\tpush eax\n";

						break;
					}
					case LEX_PLUS:
					{
						ofile << "\tpop eax\n\tpop ebx\n";
						ofile << "\tadd eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax\n";

						break;
					}
					case LEX_MINUS:
					{
						if (LEXEMA(i - 2) == LEX_EQUAL)
						{
							ofile << "\tmov eax, 0\n\tpop ebx\n";
							ofile << "\tsub eax, ebx\n\tpush eax\n";
							break;
						}

						ofile << "\tpop ebx\n\tpop eax\n";
						ofile << "\tsub eax, ebx\n\tpush eax\n";

						break;
					}
					case LEX_DIRSLASH:
					{
						ofile << "\tpop ebx\n\tpop eax\n";
						ofile << "\tcmp ebx,0\n"\
							"\tje SOMETHINGWRONG\n";
						ofile << "\tcdq\n";
						ofile << "\tidiv ebx\n\tpush eax\n";
						break;
					}
					case LEX_PROCENT:
					{

						ofile << "\tpop ebx\n\tpop eax\n";
						ofile << "\tcmp ebx,0\n"\
							"\tje SOMETHINGWRONG\n";
						ofile << "\tcdq\n";
						ofile << "\tidiv ebx\n\tpush edx\n";
						break;
					}
					}

					i++;
				}

				HaveFunc = false;

				ofile << "\tpop " << ITENTRY(result_position).id << endl << endl;

				break;
			}
			case LEX_ID:
			{
				// Для сдвигов / for shift
				if (LEXEMA(i + 1) == LEX_RIGHT_SHIFT || LEXEMA(i + 1) == LEX_LEFT_SHIFT) {
					char oper = LEXEMA(i + 1);

					ofile << "\tmov eax, " << ITENTRY(i).id << endl;
					ofile << "\tmov ebx, " << ITENTRY(i + 2).id << endl;

					ofile << "\tmov ecx, 0" << endl;

					ofile << "\t.WHILE ecx < ebx" << endl;

					switch (oper)
					{
					case LEX_RIGHT_SHIFT:
						ofile << "\t\tsar eax, 1" << endl;
						break;
					case LEX_LEFT_SHIFT:
						ofile << "\t\tsal eax, 1" << endl;
						break;
					}

					ofile << "\t\tinc ecx" << endl;
					ofile << "\t.ENDW" << endl;

					ofile << "\tmov " << ITENTRY(i).id << ", eax" << endl;

					i += 2;
				}
				else if (LEXEMA(i + 1) == LEX_LEFTHESIS && LEXEMA(i - 1) != LEX_FUNCTION
					&& lex.lextable.table[i].sn > lex.lextable.table[i - 1].sn)
				{
					for (int j = i + 1; LEXEMA(j) != LEX_RIGHTTHESIS; j++)
					{
						if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
							temp.push(ITENTRY(j)); // заполняем стек в прямом порядке	
					}
					while (!temp.empty())
					{
						if (temp.top().idtype == IT::IDTYPE::L && (temp.top().iddatatype == IT::IDDATATYPE::STR
							|| temp.top().iddatatype == IT::IDDATATYPE::SYM))
							ofile << "\npush offset " << temp.top().id << "\n";
						else if (temp.top().idtype == IT::IDTYPE::L && temp.top().iddatatype == IT::IDDATATYPE::BOL)
							ofile << "\npush DWORD ptr " << temp.top().id << "\n";
						else
							ofile << "\npush " << temp.top().id << "\n";
						temp.pop();
					}
					ofile << "\ncall " << ITENTRY(i).id << "\n";
				}

				break;
			}
			case LEX_NEWLINE: // перевод строки 
			{
				switch (ITENTRY(i + 1).iddatatype)
				{
				case IT::IDDATATYPE::UINT:
				case IT::IDDATATYPE::BOL:
					ofile << "\n\tpush " << ITENTRY(i + 1).id << "\n\tcall outnumline\n";
					break;
				case IT::IDDATATYPE::SYM:
				case IT::IDDATATYPE::STR:
					if (ITENTRY(i + 1).idtype == IT::IDTYPE::L)
						ofile << "\n\tpush offset " << ITENTRY(i + 1).id << "\n\tcall outstrline\n";
					else
						ofile << "\n\tpush " << ITENTRY(i + 1).id << "\n\tcall outstrline\n";
					break;
				}
				break;
			}
			case LEX_WRITE: // вывод
			{
				switch (ITENTRY(i + 1).iddatatype)
				{
				case IT::IDDATATYPE::UINT:
				case IT::IDDATATYPE::BOL:
					ofile << "\n\tpush " << ITENTRY(i + 1).id << "\n\tcall outnum\n";
					break;
				case IT::IDDATATYPE::SYM:
				case IT::IDDATATYPE::STR:
					if (ITENTRY(i + 1).idtype == IT::IDTYPE::L)
						ofile << "\n\tpush offset " << ITENTRY(i + 1).id << "\n\tcall outstr\n";
					else
						ofile << "\n\tpush " << ITENTRY(i + 1).id << "\n\tcall outstr\n";
					break;
				}
				break;
			}
			case LEX_IF: // условие
			{
				ifNum++;

				switch (ITENTRY(i + 1).iddatatype)
				{
				case IT::IDDATATYPE::BOL:

					ofile << "\tmov eax, " << ITENTRY(i + 1).id << endl;

					ofile << "\tmov ebx, " << ITENTRY(i + 3).id << endl;

					if (LEXEMA(i + 2) == LEX_LOGIC_EQUALS)
						ofile << "\t.IF eax  ==  ebx\n";
					else if (LEXEMA(i + 2) == LEX_LOGIC_NOT_EQUALS)
						ofile << "\t.IF eax  !=  ebx\n";
					else
						Log::WriteError(log.stream, Error::GetError(319, lex.lextable.table[i].sn, 0));
					break;
				case IT::IDDATATYPE::SYM:
					if (ITENTRY(i + 1).id[0] == 'L')
						ofile << "\tmov esi, offset " << ITENTRY(i + 1).id << endl;
					else
						ofile << "\tmov esi, " << ITENTRY(i + 1).id << endl;

					if (ITENTRY(i + 3).id[0] == 'L')
						ofile << "\tmov edi, offset " << ITENTRY(i + 3).id << endl;
					else
						ofile << "\tmov edi, " << ITENTRY(i + 3).id << endl;

					ofile << "\tmov al, BYTE PTR [esi]" << "\n";
					ofile << "\tmov bl, BYTE PTR [edi]" << "\n";

					if (LEXEMA(i + 2) == LEX_LOGIC_EQUALS)
						ofile << "\t.IF al  ==  bl\n";
					else if (LEXEMA(i + 2) == LEX_LOGIC_NOT_EQUALS)
						ofile << "\t.IF al  !=  bl\n";
					else
						Log::WriteError(log.stream, Error::GetError(319, lex.lextable.table[i].sn, 0));
					break;
				case IT::IDDATATYPE::UINT:
					ofile << "\tmov eax, " << ITENTRY(i + 1).id << endl;
					ofile << "\tmov ebx, " << ITENTRY(i + 3).id << endl;

					switch (LEXEMA(i + 2))
					{
					case LEX_LOGIC_EQUALS:
						ofile << "\t.IF eax  ==  ebx\n";
						break;
					case LEX_LOGIC_NOT_EQUALS:
						ofile << "\t.IF eax  !=  ebx\n";
						break;
					case LEX_LOGIC_MORE:
						ofile << "\t.IF eax  >  ebx\n";
						break;
					case LEX_LOGIC_LESS:
						ofile << "\t.IF eax  < ebx\n";
						break;
					case LEX_LOGIC_MORE_EQ:
						ofile << "\t.IF eax  >=  ebx\n";
						break;
					case LEX_LOGIC_LESS_EQ:
						ofile << "\t.IF eax  <=  ebx\n";
						break;
					}
					break;
				case IT::IDDATATYPE::STR:

					if (ITENTRY(i + 1).id[0] == 'L')
						ofile << "\tmov esi, offset " << ITENTRY(i + 1).id << endl;
					else
						ofile << "\tmov esi, " << ITENTRY(i + 1).id << endl;

					if (ITENTRY(i + 3).id[0] == 'L')
						ofile << "\tmov edi, offset " << ITENTRY(i + 3).id << endl;
					else
						ofile << "\tmov edi, " << ITENTRY(i + 3).id << endl;

					ofile << "\tmov eax, 0" << endl;

					ofile << "\t.WHILE eax == 0" << endl
						<< "\n\t\tmov dl, BYTE PTR [edi]" << endl
						<< "\t\t.IF dl == 0" << endl
						<< "\t\t\tmov dl, BYTE PTR [esi]" << endl
						<< "\t\t\t.IF dl == 0" << endl
						<< "\t\t\t\tmov eax, 2" << endl
						<< "\t\t\t.ENDIF" << endl
						<< "\t\t.ENDIF" << endl
						<< endl
						<< "\t\tmov dl, BYTE PTR [edi]" << endl
						<< "\t\tmov bl, BYTE PTR [esi]" << endl
						<< "\t\t.IF dl != bl" << endl
						<< "\t\t\tmov eax, 1" << endl
						<< "\t\t.ENDIF" << endl
						<< endl
						<< "\t\tinc esi;" << endl
						<< "\t\tinc edi;" << endl
						<< "\t.ENDW" << endl << endl;

					if (LEXEMA(i + 2) == LEX_LOGIC_EQUALS)
						ofile << "\t.IF eax  ==  2\n";
					else if (LEXEMA(i + 2) == LEX_LOGIC_NOT_EQUALS)
						ofile << "\t.IF eax  !=  2\n";
					else
						Log::WriteError(log.stream, Error::GetError(319, lex.lextable.table[i].sn, 0));

					break;
				}

				ifStack.push(ifNum);
				lopstack.push(true);

				i += 2;
				break;
			}
			case LEX_SQ_RBRACELET:
			{
				bool isDel = false;

				if (!whileStack.empty() && !lopstack.top())
				{
					flag_while = false;

					ofile << "\tjmp while" << whileStack.top() << endl << endl;

					ofile << "endwhile" << whileStack.top() << ':' << endl;

					whileStack.pop();

					lopstack.pop();

					isDel = true;

					break;
				}

				if (!ifStack.empty() && lopstack.top())
				{
					if (LEXEMA(i + 1) != LEX_ISTRUE && LEXEMA(i + 1) != LEX_ISFALSE)
					{
						ofile << "\n\t.ENDIF\n";

						lopstack.pop();

						ifStack.pop();
					}
					else
						ofile << "\t.ELSE\n";

					break;
				}

				break;
			}
			case LEX_WHILE:
			{
				whileNum++;

				flag_while = true;

				ofile << "while" << whileNum << ":" << endl;

				switch (ITENTRY(i + 1).iddatatype)
				{
				case IT::IDDATATYPE::UINT:
					ofile << "\tmov eax, " << ITENTRY(i + 1).id << endl;
					ofile << "\tmov ebx, " << ITENTRY(i + 3).id << endl;

					switch (LEXEMA(i + 2))
					{
					case LEX_LOGIC_EQUALS:
						ofile << "\n\t.IF eax  !=  ebx\n";
						break;
					case LEX_LOGIC_NOT_EQUALS:
						ofile << "\n\t.IF eax  ==  ebx\n";
						break;
					case LEX_LOGIC_MORE:
						ofile << "\n\t.IF eax  <=  ebx\n";
						break;
					case LEX_LOGIC_LESS:
						ofile << "\n\t.IF eax  >=  ebx\n";
						break;
					case LEX_LOGIC_MORE_EQ:
						ofile << "\n\t.IF eax  <  ebx\n";
						break;
					case LEX_LOGIC_LESS_EQ:
						ofile << "\n\t.IF eax  >  ebx\n";
						break;
					}

					break;
				case IT::IDDATATYPE::BOL:
					ofile << "\tmov eax, " << ITENTRY(i + 1).id << endl;
					ofile << "\tmov ebx, " << ITENTRY(i + 3).id << endl;

					switch (LEXEMA(i + 2))
					{
					case LEX_LOGIC_EQUALS:
						ofile << "\n\t.IF eax  !=  ebx\n";
						break;
					case LEX_LOGIC_NOT_EQUALS:
						ofile << "\n\t.IF eax  ==  ebx\n";
						break;
					}

					break;
				case IT::IDDATATYPE::SYM:
					if (ITENTRY(i + 1).id[0] == 'L')
						ofile << "\tmov esi, offset " << ITENTRY(i + 1).id << endl;
					else
						ofile << "\tmov esi, " << ITENTRY(i + 1).id << endl;

					if (ITENTRY(i + 3).id[0] == 'L')
						ofile << "\tmov edi, offset " << ITENTRY(i + 3).id << endl;
					else
						ofile << "\tmov edi, " << ITENTRY(i + 3).id << endl;

					ofile << "\tmov al, BYTE PTR [esi]" << "\n";
					ofile << "\tmov bl, BYTE PTR [edi]" << "\n";

					switch (LEXEMA(i + 2))
					{
					case LEX_LOGIC_EQUALS:
						ofile << "\n\t.IF al  !=  bl\n";
						break;
					case LEX_LOGIC_NOT_EQUALS:
						ofile << "\n\t.IF al  ==  bl\n";
						break;
					}
					break;

				case IT::IDDATATYPE::STR:
					if (ITENTRY(i + 1).id[0] == 'L')
						ofile << "\tmov esi, offset " << ITENTRY(i + 1).id << endl;
					else
						ofile << "\tmov esi, " << ITENTRY(i + 1).id << endl;

					if (ITENTRY(i + 3).id[0] == 'L')
						ofile << "\tmov edi, offset " << ITENTRY(i + 3).id << endl;
					else
						ofile << "\tmov edi, " << ITENTRY(i + 3).id << endl;

					ofile << "\tmov eax, 0" << endl;

					ofile << "\t.WHILE eax == 0" << endl
						<< "\n\t\tmov dl, BYTE PTR [edi]" << endl
						<< "\t\t.IF dl == 0" << endl
						<< "\t\t\tmov dl, BYTE PTR [esi]" << endl
						<< "\t\t\t.IF dl == 0" << endl
						<< "\t\t\t\tmov eax, 2" << endl
						<< "\t\t\t.ENDIF" << endl
						<< "\t\t.ENDIF" << endl
						<< endl
						<< "\t\tmov dl, BYTE PTR [edi]" << endl
						<< "\t\tmov bl, BYTE PTR [esi]" << endl
						<< "\t\t.IF dl != bl" << endl
						<< "\t\t\tmov eax, 1" << endl
						<< "\t\t.ENDIF" << endl
						<< endl
						<< "\t\tinc esi;" << endl
						<< "\t\tinc edi;" << endl
						<< "\t.ENDW" << endl << endl;

					switch (LEXEMA(i + 2))
					{
					case LEX_LOGIC_EQUALS:
						ofile << "\n\t.IF eax  !=  2\n";
						break;
					case LEX_LOGIC_NOT_EQUALS:
						ofile << "\n\t.IF eax  ==  2\n";
						break;
					}

					break;
				}

				ofile << "\t\tjmp endwhile" << whileNum << endl;
				ofile << "\t.ENDIF" << endl << endl;

				whileStack.push(whileNum);
				lopstack.push(false);

				i += 2;
				break;
			}
			}
		}
		ofile << END;
		ofile.close();
		return true;
	};
}