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

#pragma once

#include <vector>
#include <string>


//-------------------------------------------------------------------------------------------------
//
class rSimpleArgs
{
private:
	struct rItem
	{
		bool          m_isSwitch;
		unsigned int  m_isSet;
		std::string   m_fullname;
		unsigned char m_shortname;
		std::string   m_value;
	};

public:
	virtual ~rSimpleArgs();
	static rSimpleArgs& instance() { static rSimpleArgs Singleton; return Singleton; }
private:
	rSimpleArgs();
	rSimpleArgs(const rSimpleArgs& );
	rSimpleArgs& operator=(rSimpleArgs& );

public:
	rSimpleArgs& addSwitch(const std::string& name, const unsigned char altname);
	rSimpleArgs& addOption(const std::string& name, const unsigned char altname, const std::string& default_value);
	rSimpleArgs& setSwitch(const std::string& name, bool isset);
	rSimpleArgs& setOption(const std::string& name, const std::string& value);
	unsigned int isSet(const std::string& name);
	std::string  getOption(const std::string& name);
	std::string  getArgument(unsigned int num);
	unsigned int getCountArgument(void);
	unsigned int parse(unsigned int argc, const char **argv);

private:
	std::vector<rSimpleArgs::rItem> m_list;
	std::vector<std::string> m_argument;

	rItem* findItem(const std::string& name);
	void parseShort(const std::string& arg);
	rItem* parseFull(const std::string& arg);
};



