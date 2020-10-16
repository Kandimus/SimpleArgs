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

template <class T>
const rItem* rSimpleArgs::findItem(const T& name)
{
	for(auto& item : m_list) {
		if (item.m_name == name) {
			return &item;
		}
	}

	return nullptr;
}

void rSimpleArgs::setAltSplit(bool altsplit)
{
	m_altsplit = altsplit;
}

template <class T>
void rSimpleArgs::setPrefix(const T& fullprefix, const T& altprefix)
{
	m_fullPrefix = fullprefix;
	m_
}

template <class T>
void rSimpleArgs::add(T name)
{
	add(name, T());
}

template <class T>
void rSimpleArgs::add(const T& name, const T& altname)
{
	m_list.push_back(rSimpleArgs::rItem());

	m_list.back().m_count   = 0;
	m_list.back().m_name    = name;
	m_list.back().m_altname = altname;
}

template <class T>
void rSimpleArgs::add(const T& name, const T& altname, unsigned int count)
{
	m_list.push_back(rSimpleArgs::rItem());

	m_list.back().m_count   = count;
	m_list.back().m_name    = name;
	m_list.back().m_altname = altname;
	m_list.back().m_values.resize(count, T());
}

template <class T>
bool rSimpleArgs::isSet(const T& name)
{
	const rItem* item = findItem(name);

	return item ? item->m_isSet : false;
}

template <class T>
const T& rSimpleArgs::getArg(const T& name)
{
	const rItem* item = findItem(name);

	if (item) {
		if (item->m_values.size()) {
			return item->m_values[0];
		} else {
			return T();
		}
	}

	return T();
}

template <class T>
const T& rSimpleArgs::getArg(const T& name, unsigned int num)
{
	const rItem* item = findItem(name);

	if (item) {
		if (num < item.m_values.size()) {
			return item.m_values[num];
		} else {
			return T();
		}
	}

	return T();
}


//-------------------------------------------------------------------------------------------------
//
template <class T>
void rSimpleArgs::parse(unsigned int argc, char **argv)
{
	for (unsigned int ii = 0; ii < argc; ++ii) {
		T
	}
}


//-------------------------------------------------------------------------------------------------
//
void rThreadMaster::CloseAll()
{
	for (DINT ii = List.size() - 1; ii >= 0; --ii) {
		string name = List[ii]->Class->GetRTTI();

		List[ii]->Class->Finish();
		pthread_join(*List[ii]->Thread, NULL);

		if((List[ii]->Flags & TMF_DELETE) && List[ii]->Class)
		{
			delete List[ii]->Class;
		}

		delete List[ii];

		TRACEERROR("--------- Поток %s закрыт!", name.c_str());
	}

	List.clear();

	Finish();
}






UDINT rThreadMaster::CalcSysInfo(rCPUState &cpu_start)
{
	struct sysinfo sys_info;
	rCPUState  cpu;

	if(sysinfo(&sys_info) != -1)
	{
		UDINT freemem = (sys_info.freeram * sys_info.mem_unit) / 1024;

		CurSysInfo.ModifyMem = freemem - CurSysInfo.FreeMem;
		CurSysInfo.FreeMem   = freemem;
//		printf("Free memory %ikb (%+ikb)\n", FreeMem, ModifyMem);
	}

	GetCPUState(cpu);

	float active = cpu.GetActive() - cpu_start.GetActive();
	float idle   = cpu.GetIdle()   - cpu_start.GetIdle();
	float total  = active + idle;
	float usage  = (100.f * active / total);

	CurSysInfo.ModifyCPU = usage - CurSysInfo.CPUUsage;
	CurSysInfo.CPUUsage  = usage;

	SysInfo.push_back(CurSysInfo);

    return 0;
}


//
UDINT rThreadMaster::GetCPUState(rCPUState &cpu)
{
	FILE *fstat = fopen("/proc/stat", "r");

	if(nullptr == fstat)
	{
		return -1;
	}

	//read values from /proc/pid/stat
	bzero(&cpu, sizeof(rCPUState));

	if (fscanf(fstat, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
						 &cpu.User, &cpu.Nice, &cpu.System, &cpu.Idle, &cpu.IOWait, &cpu.IRQ, &cpu.SoftIRQ, &cpu.Steal, &cpu.Guest, &cpu.GuestNice) == EOF)
	{
		fclose(fstat);
		return -1;
	}

	fclose(fstat);

	//cpu.TotalUsage = cpu.User + cpu.Nice + cpu.System + cpu.Idle + cpu.IOWait + cpu.IRQ + cpu.SoftIRQ + cpu.Steal + cpu.Guest + cpu.GuestNice;

	return TRITONN_RESULT_OK;
}



UDINT rThreadMaster::CalcThreadTimeInfo(rThreadInfo *ti)
{
	UDINT worktime = 0;
	UDINT idletime = 0;
	vector<rThreadTimeInfo> vti;

	ti->Class->GetTimeInfo(vti);

	if(vti.size())
	{
		for(UDINT ii = 0; ii < vti.size(); ++ii)
		{
			worktime += vti[ii].Work;
			idletime += vti[ii].Idle;
		}
		worktime /= vti.size();
		idletime /= vti.size();
	}

	if(worktime < ti->TimeAvr.WorkMin) ti->TimeAvr.WorkMin = worktime;
	if(worktime > ti->TimeAvr.WorkMax) ti->TimeAvr.WorkMax = worktime;
	if(idletime < ti->TimeAvr.IdleMin) ti->TimeAvr.IdleMin = idletime;
	if(idletime > ti->TimeAvr.IdleMax) ti->TimeAvr.IdleMax = idletime;

	ti->TimeAvr.WorkAverage = UDINT((LREAL(ti->TimeAvr.WorkAverage) * LREAL(ti->Counter) + LREAL(worktime)) / LREAL(ti->Counter + 1));
	ti->TimeAvr.IdleAverage = UDINT((LREAL(ti->TimeAvr.IdleAverage) * LREAL(ti->Counter) + LREAL(idletime)) / LREAL(ti->Counter + 1));
	++ti->Counter;

	if(worktime > ti->TimeAvr.WorkAverage || 0 == ti->CntAvrMax)
	{
		ti->TimeAvr.WorkAvgMax = UDINT((LREAL(ti->TimeAvr.WorkAvgMax) * LREAL(ti->CntAvrMax) + LREAL(worktime)) / LREAL(ti->CntAvrMax + 1));
		++ti->CntAvrMax;
	}

	ti->TimeInfo.push_back(new rThreadTimeInfo(worktime, idletime));

	//printf("%s : work avr %u us, avr max %u us, max %u us\n", ti->Class->GetRTTI(), ti->TimeAvr.WorkAverage, ti->TimeAvr.WorkAvgMax, ti->TimeAvr.WorkMax);
    return 0;
}


UDINT rThreadMaster::SaveAllTimerInfo()
{
	string filename = "";
	string worktext = "";
	string idletext = "";
	string cputext  = "";
	string memtext  = "";
	string cpumtext = "";
	string memmtext = "";
	UDINT  utime    = rTickCount::UnixTime();

	filename = String_format("%ssysinfo_%u.csv", DIR_TIMEINFO.c_str(), utime);
	cputext  = "cpu;";
	cpumtext = "cpu modify";
	memtext  = "memory;";
	memmtext = "memory modify;";

	for(UDINT ii = 0; ii < SysInfo.size(); ++ii)
	{
		cputext  += String_format("%.2f;", SysInfo[ii].CPUUsage);
		cpumtext += String_format("%.2f;", SysInfo[ii].ModifyCPU);
		memtext  += String_format("%u;"  , SysInfo[ii].FreeMem);
		memmtext += String_format("%i;"  , SysInfo[ii].ModifyMem);
	}
	SysInfo.clear();

	SimpleFileSave(filename, cputext + "\n" + cpumtext + "\n" + memtext + "\n" + memmtext);


	for(UDINT ii = 0; ii < List.size(); ++ii)
	{
		rThreadInfo *thread_info = List[ii];

		filename = String_format("%s%s_%u.csv", DIR_TIMEINFO.c_str(), thread_info->Class->GetRTTI(), utime);
		worktext = "work;";
		idletext = "idle;";

		for(UDINT jj = 0; jj < thread_info->TimeInfo.size(); ++jj)
		{
			worktext += String_format("%u;", thread_info->TimeInfo[jj]->Work);
			idletext += String_format("%u;", thread_info->TimeInfo[jj]->Idle);

			delete thread_info->TimeInfo[jj];
			thread_info->TimeInfo[jj] = nullptr;
		}

		SimpleFileSave(filename, worktext + "\n" + idletext);

		thread_info->TimeInfo.clear();
	}

	return TRITONN_RESULT_OK;
}


UDINT rThreadMaster::GenerateVars(rThreadInfo* ti, const string& alias)
{
	if (!ti || alias.empty()) {
		return TRITONN_RESULT_OK;
	}

	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Work.Max"    , TYPE_UDINT , VARF_R___, &ti->TimeAvr.WorkMax    , U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Work.Min"    , TYPE_UDINT , VARF_R___, &ti->TimeAvr.WorkMin    , U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Work.AvgMax" , TYPE_UDINT , VARF_R___, &ti->TimeAvr.WorkAvgMax , U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Work.Average", TYPE_UDINT , VARF_R___, &ti->TimeAvr.WorkAverage, U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Idle.Max"    , TYPE_UDINT , VARF_R___, &ti->TimeAvr.IdleMax    , U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Idle.Min"    , TYPE_UDINT , VARF_R___, &ti->TimeAvr.IdleMin    , U_DIMLESS, 0));
	rVariable::ListVar.push_back(new rVariable("system.diag." + alias + ".Idle.Average", TYPE_UDINT , VARF_R___, &ti->TimeAvr.IdleAverage, U_DIMLESS, 0));

	return TRITONN_RESULT_OK;
}


