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

rSimpleArgs::rItem* rSimpleArgs::parseFull(const std::string& arg/*, bool& valueSet*/)
{
    std::string name = arg.substr(2);
	std::string value = "";
	int pos = name.find('=');

    //valueSet = false;
	if (pos >= 0) {
		value = name.substr(pos + 1);
		name  = name.substr(0, pos);
        //valueSet = true;
	}

	for (auto& item : m_list) {
		if (item.m_fullname == name) {
            item.m_isSet += pos >= 0;
			if (value.size()) {
				item.m_value = value;
			}

			return &item;
		}
	}

	return nullptr;
}
