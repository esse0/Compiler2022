#include "stdafx.h"
#include "MFST.h"

int FST_TRACE_n = -1;
char rbuf[205],		// для правила
sbuf[205],		// для стека
lbuf[1024];	// для ленты

namespace MFST
{
	MfstState::MfstState() //конструктор
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};

	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrulechain) //конструктор
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	};

	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain)		// конструктор запоминаем правило
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis() //конструктор
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) //конструктор
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	};

	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; }; //конструктор
	Mfst::Mfst(Lex::LEX plex, GRB::Greibach pgrebach) //конструктор
	{
		grebach = pgrebach;
		lex = plex;
		lenta = new short[lenta_size = lex.lextable.size];		// массив для ленты, состоящией из символов таблицы лексем
		for (int k = 0; k < lenta_size; k++)
			lenta[k] = TS(lex.lextable.table[k].lexema);	// заполнение массива терминалами
		lenta_position = 0;
		st.push(grebach.stbottomT);		// дно стека в стек
		st.push(grebach.startN);		// стартовый символ в стек
		nrulechain = -1;
	};

	Mfst::RC_STEP Mfst::step(const Log::LOG& log)		// шаг автомата
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)	// если лента не закончилась
		{
			if (ISNS(st.top()))			// если на вершине стеке нетерминал
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)		// если найдено правило по нетерминалу на вершине стека... заполняем номер текущего правила
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)	// получаем следующую цепочку по терминалу из ленты и заполяем цепочку
					{
						MFST_TRACE1(log)			// вывод ++номера шага автомата, правила, ленты и стека
							savestate(log);		// сохраняем состояние
						st.pop();			// извлекаем из стека символ
						push_chain(chain);	// помещаем цепочку в стек
						rc = NS_OK;			// найдено правило и цепочка, цепочка записана в стек
						MFST_TRACE2(log)			// вывод номера шага автомата, ленты и стека
					}
					else		// не найдена подходящая цепочка
					{
						MFST_TRACE4(log, "TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN);
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE;	// восстановить состояние автомата
					}
				}
				else rc = NS_ERROR;		// неизвестный нетерминал
			}
			else if ((st.top() == lenta[lenta_position]))	// если на вершине стека терминал и он совпадает 
			{
				lenta_position++;	// сдвигаем ленту
				st.pop();
				nrulechain = -1;
				rc = TS_OK;
				MFST_TRACE3(log)		// вывод ++номера шага автомата, ленты и стека
			}
			else
			{
				MFST_TRACE4(log, "TS_NOK/NS_NORULECHAIN")		// вывод ++номера шага автомата и сообщения
					rc = reststate(log) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else {
			rc = LENTA_END;
			MFST_TRACE4(log, "LENTA_END")
		};
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain) //цепочка в стек с обратной стороны
	{
		for (int k = chain.size - 1; k >= 0; k--)
			st.push(chain.nt[k]);
		return true;
	};

	bool Mfst::savestate(const Log::LOG& log)
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6(log, "SAVESTATE:", storestate.size());		// вывод текста и размера стека для сохранения состояний
		return true;
	};

	bool Mfst::reststate(const Log::LOG& log) //восстановить состояние автомата
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5(log, "RESSTATE")
				MFST_TRACE2(log)
		};
		return rc;
	};

	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
				diagnosis[j].lenta_position = -1;
		};
		return rc;
	};

	bool Mfst::start(const Log::LOG& log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(log);

		switch (rc_step)
		{
		case LENTA_END:         MFST_TRACE4(log, "------>LENTA_END")
			* log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d:всего строк %d, синтаксический анализ выполнен без ошибок ", 0, lenta_size);
			*log.stream << std::setw(4) << std::left << 0 << ":всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок " << std::endl;
			rc = true;
			break;
		case NS_NORULE:         MFST_TRACE4(log, "------>NS_NORULE")
			* log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << getDiagnosis(0, buf) << std::endl;
			*log.stream << getDiagnosis(1, buf) << std::endl;
			*log.stream << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN:       MFST_TRACE4(log, "------>NS_NORULECHAIN") break;
		case NS_ERROR:             MFST_TRACE4(log, "------>NS_ERROR") break;
		case SURPRISE:             MFST_TRACE4(log, "------>SURPRISE") break;
		};
		return rc;
	};

	char* Mfst::getCSt(char* buf) //вывод стека
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			short p = st._Get_container()[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	};

	char* Mfst::getCLenta(char* buf, short pos, short n) //вывод ленты 
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};

	char* Mfst::getDiagnosis(short n, char* buf) //вывод диагностики
	{
		char* rc = (char*)"";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::GetError(610);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lex.lextable.table[lpos].sn, err.message);
			rc = buf;
		};
		return rc;
	};

	void Mfst::printrules(const Log::LOG& log)//вывод правил
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
	};

	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
}