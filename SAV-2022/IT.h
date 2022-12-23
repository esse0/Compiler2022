#pragma once
#include "stdafx.h"
#include "IT.h"
#include "LT.h"

#define ID_MAXSIZE			32						//���� ����� �������� ��������������
#define SCOPED_ID_MAXSIZE   (ID_MAXSIZE*2-1)		//���� ����� �������� ������������� + ������� ���������
#define MAXSIZE_TI			4096					//���� ����� ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT		0x00000000				//�������� �� ��������� ��� integer
#define TI_STR_DEFAULT		0x00					//�������� �� ��������� ��� sting
#define TI_SYM_DEFAULT		0x00					//�������� �� ��������� ��� symbol;
#define TI_BOL_DEFAULT		0x00					//�������� �� ��������� ��� symbol;
#define TI_NULLIDX			0xffffffff				//��� �������� ������� ���������������
#define STR_MAXSIZE			255						//������������ ����� ���������� ��������
#define TI_INT_MAXSIZE		4294967295				//������������ �������� ��� ���� integer + 1
#define TI_INT_MINSIZE		0						//����������� �������� ��� ���� integer - 1

#define MAX_PARAMS_COUNT		5			   			//������������ ���������� ���������� � �������
#define POW_PARAMS_CNT			2						//���-�� ���������� � ������� pow
#define RANDOM_PARAMS_CNT		1						//���-�� ���������� � ������� rand
#define STRLEN_PARAMS_CNT		1						//���-�� ���������� � ������� lenght
#define INTTOSTR_PARAMS_CNT		1						//���-�� ���������� � ������� int to string
#define CONCAT_PARAMS_CNT		2						//���-�� ���������� � ������� concat
#define COPY_PARAMS_CNT			1						//���-�� ���������� � ������� copy
#define INPUT_PARAMS_CNT		0						//���-�� ���������� � ������� input
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
	enum IDDATATYPE { UINT = 1, STR = 2, SYM = 3, BOL = 4, PROC = 5, UNDEF };								//���� ������ ���������������: ��������, ���������, ����������, ����������, ��������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5, Z = 6 };												//���� ���������������: V = ����������, F = �������, P =��������, L =�������, S = ����������� ������� Z - ����
	enum STDFNC { F_POW, F_POWER, F_RANDOM, F_STRLEN, F_NOT_STD,											//����������� �������
			F_INT_TO_STR, F_CONCAT, F_COPY, F_ININT, F_INSTR, F_INSYM, F_GDATE, F_GTIME };			

	static const IDDATATYPE POW_PARAMS[] = { IT::IDDATATYPE::UINT, IT::IDDATATYPE::UINT };				//��������� �������  
	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::UINT };									//��������� �������  			
	static const IDDATATYPE STRLEN_PARAMS[] = { IT::IDDATATYPE::STR };									//��������� ������� length
	static const IDDATATYPE INT_TO_STR_PARAMS[] = { IT::IDDATATYPE::UINT };								//��������� ������� int to string
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };				//��������� ������� concat
	static const IDDATATYPE COPY_PARAMS[] = { IT::IDDATATYPE::STR };									//��������� ������� copy
	//static const IDDATATYPE INPUT_PARAMS[] = { IT::IDDATATYPE:: };									//��������� ������� copy

	struct Entry	// ������ ������� ���������������
	{
		int			idxfirstLE;			// ������ ������ ������ � ������� ������
		char	id[SCOPED_ID_MAXSIZE];		// �������������� (������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// ��� ������
		IDTYPE		idtype;				// ��� ��������������

		union
		{
			unsigned int vint;					// �������� integer
			struct
			{
				int len;							// ���������� �������� � string
				char str[STR_MAXSIZE - 1];	// ������� string
			} vstr;									// �������� string
			char symbol;							//�������� symbol
			struct
			{
				int count;					// ���������� ���������� �������
				IDDATATYPE* types;			//���� ���������� �������
			} params;
			bool vbol;
		} value;		// �������� ��������������

		Entry()							//����������� ��� ����������
		{
			this->value.vint = TI_INT_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.params.count = NULL;
			this->value.symbol = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) //����������� � �����������
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};

	struct IdTable				// ��������� ������� ���������������
	{
		int maxsize;			// ������� ������� ��������������� < TI_MAXSIZE
		int size;				// ������� ������ ������� ��������������� < maxsize
		Entry* table;			// ������ ����� ������� ���������������
	};

	IdTable Create(int size);		// ������� ������� ��������������� < TI_MAXSIZE// ������� ������� ���������������

	void Add(				// �������� ������ � ������� ���������������
		IdTable& idtable,	// ��������� ������� ���������������
		Entry entry			// ������ ������� ���������������
	);
	int isId(				// �������: ����� ������ (���� ����), TI_NULLIDX (���� ���)
		IdTable& idtable,	// ��������� ������� ���������������
		char id[SCOPED_ID_MAXSIZE]	// �������������
	);
	bool SetValue(IT::Entry* entry, char* value);	//������ �������� ��������������
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void makeOutWithIT(IT::IdTable& idtable);
}