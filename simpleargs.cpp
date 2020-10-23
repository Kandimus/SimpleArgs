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

rSimpleArgs::rSimpleArgs()
{

}

rSimpleArgs::~rSimpleArgs()
{

}

rSimpleArgs::rItem* rSimpleArgs::findItem(const std::string& name)
{
	for(auto& item : m_list) {
		if (item.m_fullname == name) {
			return &item;
		}
	}

	return nullptr;
}

rSimpleArgs& rSimpleArgs::addSwitch(const std::string& name, const unsigned char shortname)
{
	rItem* item = findItem(name);

	if (item == nullptr) {
		m_list.push_back(rSimpleArgs::rItem());
		item = &m_list.back();
	}

	item->m_isSwitch  = true;
	item->m_isSet     = 0;
	item->m_fullname  = name;
	item->m_shortname = shortname;
	item->m_value     = "";

	return *this;
}

rSimpleArgs& rSimpleArgs::addOption(const std::string& name, const unsigned char shortname, const std::string&  default_value)
{
	rItem* item = findItem(name);

	if (item == nullptr) {
		m_list.push_back(rSimpleArgs::rItem());
		item = &m_list.back();
	}

	item->m_isSwitch  = false;
	item->m_isSet     = 0;
	item->m_fullname  = name;
	item->m_shortname = shortname;
	item->m_value     = default_value;

	return *this;
}

rSimpleArgs& rSimpleArgs::setSwitch(const std::string& name, bool isset)
{
	rItem* item = findItem(name);

	if (item == nullptr) {
		m_list.push_back(rSimpleArgs::rItem());
		item = &m_list.back();

		item->m_fullname = name;
	}

	item->m_isSet = isset;

	return *this;
}

rSimpleArgs& rSimpleArgs::setOption(const std::string& name, const std::string&  value)
{
	rItem* item = findItem(name);

	if (item == nullptr) {
		m_list.push_back(rSimpleArgs::rItem());
		item = &m_list.back();

		item->m_fullname = name;
	}

	item->m_value = value;
	item->m_isSet = isset;

	return *this;
}


unsigned int rSimpleArgs::isSet(const std::string& name)
{
	const rItem* item = findItem(name);
	return item ? item->m_isSet : false;
}

std::string rSimpleArgs::getOption(const std::string& name)
{
	const rItem* item = findItem(name);
	return item ? item->m_value : "";
}

std::string rSimpleArgs::getArgument(unsigned int num)
{
	return num < m_argument.size() ? m_argument[num] : "";
}

unsigned int rSimpleArgs::getCountArgument(void)
{
	return m_argument.size();
}

//-------------------------------------------------------------------------------------------------
//
unsigned int rSimpleArgs::parse(unsigned int argc, char **argv)
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
				}
				continue;
			}

			if (oldarg) {
				curarg = oldarg;
			}
		}

		if (curarg) {
			curarg->m_value = arg;
			curarg = nullptr;
			continue;
		} else {
			m_argument.push_back(arg);
		}
	}

	return 0;
}


void rSimpleArgs::parseShort(const std::string& arg)
{
	std::string str = arg.substr(1);
	for (auto& item : m_list) {
		int pos = str.find(item.m_shortname);
		if(pos >= 0) {
			++item.m_isSet;
		}
	}
}

rSimpleArgs::rItem* rSimpleArgs::parseFull(const std::string& arg)
{
	std::string name  = arg.substr(2);
	std::string value = "";

	int pos = name.find('=');
	if (pos >= 0) {
		value = name.substr(pos + 1);
		name  = name.substr(0, pos);
	}

	for (auto& item : m_list) {
		if (item.m_fullname == name) {
			++item.m_isSet;

			if (value.size()) {
				item.m_value = value;
			}

			return &item;
		}
	}

	return nullptr;
}
