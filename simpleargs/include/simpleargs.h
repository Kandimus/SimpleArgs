//=================================================================================================
//===
//=== simpleargs.h
//===
//=== Copyright (c) 2020-2023 by RangeSoft.
//=== All rights reserved.
//===
//=== Litvinov "VeduN" Vitaly
//===
//=================================================================================================
//===
//=== Класс для обработки параметров командной строки
//===
//=================================================================================================
#pragma once

#include <vector>
#include <string>

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
    virtual ~rSimpleArgs() = default;
	static rSimpleArgs& instance() { static rSimpleArgs Singleton; return Singleton; }
private:
    rSimpleArgs() = default;
    rSimpleArgs(const rSimpleArgs&);
    rSimpleArgs& operator=(rSimpleArgs&);

public:
    rSimpleArgs& addSwitch(const std::string& name, const unsigned char shortname)
    {
        auto item = findItem(name);

        if (item == nullptr)
        {
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
    rSimpleArgs& addOption(const std::string& name, const unsigned char shortname, const std::string& default_value)
    {
        rItem* item = findItem(name);

        if (item == nullptr)
        {
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
    rSimpleArgs& setSwitch(const std::string& name, bool isset)
    {
        rItem* item = findItem(name);

        if (item == nullptr)
        {
            m_list.push_back(rSimpleArgs::rItem());
            item = &m_list.back();
            item->m_fullname = name;
        }

        item->m_isSet = isset;
        return *this;
    }
    rSimpleArgs& setOption(const std::string& name, const std::string& value)
    {
        rItem* item = findItem(name);

        if (item == nullptr)
        {
            m_list.push_back(rSimpleArgs::rItem());
            item = &m_list.back();

            item->m_fullname = name;
        }

        item->m_value = value;
        item->m_isSet = true;
        return *this;
    }
    unsigned int isSet(const std::string& name) const
    {
        auto item = findConstItem(name);
        return item ? item->m_isSet : false;
    }
    std::string  getOption(const std::string& name) const
    {
        auto item = findConstItem(name);
        return item ? item->m_value : "";
    }
    std::string  getArgument(unsigned int num) const
    {
        return num < m_argument.size() ? m_argument[num] : "";
    }
    unsigned int getCountArgument() const
    {
        return m_argument.size();
    }

	unsigned int parse(unsigned int argc, const char **argv);

private:
	std::vector<rSimpleArgs::rItem> m_list;
	std::vector<std::string> m_argument;

    const rItem* findConstItem(const std::string& name) const
    {
        for (auto& item : m_list)
        {
            if (item.m_fullname == name)
            {
                return &item;
            }
        }
        return nullptr;
    }

    rItem* findItem(const std::string& name)
    {
        for (auto& item : m_list)
        {
            if (item.m_fullname == name)
            {
                return &item;
            }
        }
        return nullptr;
    }

    void parseShort(const std::string& arg)
    {
        std::string str = arg.substr(1);
        for (auto& item : m_list) {
            int pos = str.find(item.m_shortname);
            if(pos >= 0) {
                ++item.m_isSet;
            }
        }
    }

	rItem* parseFull(const std::string& arg);
};



