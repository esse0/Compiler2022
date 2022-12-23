#include "stdafx.h"
namespace Parm
{
	PARM getparm(int argc, wchar_t* argv[])
	{
		PARM parm = { L"", L"", L"" };

		for (int i = 1; i < argc; i++)
		{
			// превышена длина входного параметра
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(101)
			else
			{
				if (wcsncmp(argv[i], PARM_IN, wcslen(PARM_IN)) == 0)
					wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				else if (wcsncmp(argv[i], PARM_OUT, wcslen(PARM_OUT)) == 0)
					wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));
				else if (wcsncmp(argv[i], PARM_LOG, wcslen(PARM_LOG)) == 0)
					wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
			}
		}

		if (*parm.in == 0)//если не задан in
			throw ERROR_THROW(100); // параметр должен быть задан

		bool inIsRight = false;
		for (size_t i = 0; i < wcslen(parm.in); i++)
			if (parm.in[i] == '.' && i != wcslen(parm.in) - 1)
				inIsRight = true;

		if (!inIsRight)
			throw ERROR_THROW(102);

		if (*parm.out == 0)//если не задан out
		{
			for (size_t i = 0; parm.in[i] != '.'; i++)
				parm.out[i] = parm.in[i];

			wcsncat_s(parm.out, PARM_OUT_DEFAULT_EXT, sizeof(PARM_OUT_DEFAULT_EXT));
		}

		if (*parm.log == 0)//если не задан log
		{
			for (size_t i = 0; parm.in[i] != '.'; i++)
				parm.log[i] = parm.in[i];
			wcsncat_s(parm.log, PARM_LOG_DEFAULT_EXT, sizeof(PARM_LOG_DEFAULT_EXT));
		};

		return parm;
	};
};

