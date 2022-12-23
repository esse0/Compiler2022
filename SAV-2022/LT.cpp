#include "stdafx.h"

namespace LT {
	Entry::Entry()
	{
		lexema = NULL;
		sn = NULL;
		idxTI = NULLDX_TI;
	}

	Entry::Entry(unsigned char lexema, int snn, int idxti)
	{
		this->lexema = lexema;
		this->sn = snn;
		this->idxTI = idxti;
	}

	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
			throw ERROR_THROW(202);
		LexTable lextable;
		lextable.table = new Entry[lextable.maxsize = size];
		lextable.size = NULL;
		return  lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size >= lextable.maxsize)
			throw ERROR_THROW(202);
		lextable.table[lextable.size++] = entry;
	}

	void makeOutWithLT(LT::LexTable& lextable)
	{

		bool flagtemp = false;
		std::cout << "\n-----------------  Таблица лексем ---------------------\n" << std::endl;
		for (int i = 0; i < lextable.size; )
		{
			if (lextable.table[i].lexema == '#')
			{
				flagtemp = true;
				i++;
				continue;
			}
			if (flagtemp)
			{
				flagtemp = false;
				std::cout << lextable.table[i].lexema;
				std::cout << std::endl;
				i++;
				continue;
			}
			int line = lextable.table[i].sn;
			std::cout << std::setw(3) << line << " | ";
			while (lextable.table[i].sn == line)
			{
				std::cout << lextable.table[i].lexema;
				if (lextable.table[i].idxTI != NULLDX_TI)
					std::cout << "[" << lextable.table[i].idxTI << "]";
				i++;
			}
			if (lextable.table[i].lexema != '#')
				std::cout << std::endl;
		}
		std::cout << "-------------------------------------------------------------------------\n\n";
	}

	void makeOutWithLTFILE(std::ostream* stream, LT::LexTable& lextable) 
	{
		bool flagtemp = false;
		*stream << "\n-----------------  Таблица лексем ---------------------\n" << std::endl;
		for (int i = 0; i < lextable.size; )
		{
			if (lextable.table[i].lexema == '#')
			{
				flagtemp = true;
				i++;
				continue;
			}
			if (flagtemp)
			{
				flagtemp = false;
				*stream << lextable.table[i].lexema;
				*stream << std::endl;
				i++;
				continue;
			}
			int line = lextable.table[i].sn;
			*stream << std::setw(3) << line << " | ";
			while (lextable.table[i].sn == line)
			{
				*stream << lextable.table[i].lexema;
				if (lextable.table[i].idxTI != NULLDX_TI)
					* stream << "[" << lextable.table[i].idxTI << "]";
				i++;
			}
			if (lextable.table[i].lexema != '#')
				* stream << std::endl;
		}
		*stream << "-------------------------------------------------------------------------\n\n";
	}
}