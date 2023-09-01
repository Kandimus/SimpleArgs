//=================================================================================================
//===
//=== simpleargs.h
//===
//=== Copyright (c) 2020 by RangeSoft.
//=== All rights reserved.
//===
//=== Litvinov "VeduN" Vitaliy O.
//===
//=================================================================================================
//===
//=== Класс для обработки параметров командной строки
//===
//=================================================================================================

#include "simpleargs.h"

unsigned int rSimpleArgs::parse(unsigned int argc, const char **argv)
{
	rItem* curarg = nullptr;

	for (unsigned int ii = 1; ii < argc; ++ii) {
		std::string arg = argv[ii];

		if (arg[0] == '-') {
			if (arg.size() < 2 || arg[0] != '-') {
				return ii;
			}

			if (arg[1] != '-') {
				parseShort(arg);
				continue;
			}

			rItem* oldarg = curarg;

			curarg = parseFull(arg);

			if (curarg) {
				if (curarg->m_isSwitch) {
					curarg = nullptr;
				} else if (curarg->m_isSet) {
					curarg = nullptr;
				}
				continue;
			}

			if (oldarg) {
				curarg = oldarg;
			}
		}

		if (curarg) {
			curarg->m_value = arg;
            ++curarg->m_isSet;
			curarg = nullptr;

			continue;
		} else {
			m_argument.push_back(arg);
		}
	}

	return 0;
}
