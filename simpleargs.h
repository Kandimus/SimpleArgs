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
template <typename T>
class rSimpleArgs
{
private:
	struct rItem
	{
		bool m_isSet;
		T m_name;
		T m_fullname;
		T m_altname;
		std::vector<T> m_values;
		unsigned int m_count;
	};

public:
	virtual ~rSimpleArgs();
	static rSimpleArgs& instance() { static rSimpleArgs Singleton; return Singleton; }
private:
	rSimpleArgs();
	rSimpleArgs(const rSimpleArgs& );
	rSimpleArgs& operator=(rSimpleArgs& );

public:
	void setAltSplit(bool altsplit);
	void setPrefix(const T& fullprefix, const T& altprefix);
	void add(const T& name);
	void add(const T& name, const T& altname);
	void add(const T& name, const T& altname, unsigned int count = 1);
	void parse(unsigned int argc, char **argv);
	bool isSet(const T& name);
	const T& getArg(const T& name);
	const T& getArg(const T& name, unsigned int num);

private:
	bool m_altSplit = true;
	T    m_fullPrefix = "--";
	T    m_altprefix  = "-";
	std::vector<rSimpleArgs::rItem> m_list;

	const rItem* findItem(const T& name);
};



