#include "Configuration.hpp"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
#include "menu.h"
#include <stdio.h>
#define SETUPVALUE(x) SetupValue(x,#x)
#define SETUPVALUEARRAY(x,y) SetupValueArray(x,#x,y)
void CConfig::Setup()
{
	SETUPVALUE(Setting::VGlowEsp);
	SETUPVALUE(Setting::VGlowEsp);
	SETUPVALUE(Setting::MBunnyhop);
	SETUPVALUE(Setting::AtriggerbotKey);
	SETUPVALUE(Setting::Atriggerbotdelay);
	SETUPVALUE(Setting::AtriggerbotfovBased);
	SETUPVALUE(Setting::VGlowEspAplha);
	SETUPVALUE(Setting::VGlowOnlyVisible);
	SETUPVALUE(Setting::AAimbotenable);

	SETUPVALUEARRAY(Setting::AHitboxList, 34);
	SETUPVALUEARRAY(Setting::AFOVList, 34);
	SETUPVALUEARRAY(Setting::ADelayList, 34);
	SETUPVALUEARRAY(Setting::ArcsList, 34);
	SETUPVALUEARRAY(Setting::ASmoothList, 34);
	SETUPVALUEARRAY(Setting::AKillDelayList, 34);
	SETUPVALUEARRAY(Setting::AStartBList, 34);
	SETUPVALUEARRAY(Setting::AendbulletList, 34);
}

void CConfig::SetupValue(int &value, std::string name)
{
	ints.push_back(new ConfigValue<int>( name, &value));
}

void CConfig::SetupValue(float &value, std::string name)
{
	floats.push_back(new ConfigValue<float>(name, &value));
}

void CConfig::SetupValue(bool &value, std::string name)
{
	bools.push_back(new ConfigValue<bool>(name, &value));
}

void CConfig::SetupValue(nk_colorf &value, std::string name)
{
	colors.push_back(new ConfigValue<nk_colorf>(name, &value));
}

void CConfig::SetupValueArray(int *value, std::string name,int len)
{
	intArrays.push_back(new ConfigValueArray<int>(name, value,len));
}

void CConfig::SetValueInt(char *name, int value)
{
	for (auto i : ints)
	{
		if (strcmp(i->name.c_str(), name) == 0)
		{
			*(i->value) = value;
			return;
		}
	}
}

void CConfig::SetValueColor(char *name, float r, float g, float b, float a)
{
	for (auto i : colors)
	{
		if (strcmp(i->name.c_str(), name) == 0)
		{
			i->value->r = r;
			i->value->g = g;
			i->value->b = b;
			i->value->a = a;
			return;
		}
	}
}

void CConfig::SetValueIntArray(char *name, char *value)
{
	for (auto i : intArrays)
	{
		if (strcmp(i->name.c_str(), name) == 0)
		{
			std::stringstream s(value);
			for (int t = 0; s >> (i->value)[t]; t++);
			return;
		}
	}
}

void CConfig::Save(char *filepath)
{
    static TCHAR path[MAX_PATH];
    std::string folder, file;
 
	sprintf(path, "C:\\privatecheats\\%s", filepath);

	FILE *fp;
	fp = fopen(path, "w+");
 
	for (auto value : ints)
	{
		fprintf(fp, "Gui.SetValueInt(\"%s\", %d); \n", value->name.c_str(), *(value->value));
	}

	//for (auto value : floats);

	//for (auto value : bools);

	for (auto value : intArrays)
	{
		char* temp = new char[2048]();
		char* tempbuf = new char[128]();
		for (int i = 0; i < value->len; i++)
		{
			sprintf(tempbuf, "%d ", (value->value)[i]);
			strcat(temp, tempbuf);
		}
		fprintf(fp, "Gui.SetValueIntArray(\"%s\", \"%s\"); \n", value->name.c_str(), temp);
		delete temp;
		delete tempbuf;
	}

	for (auto value : colors)
	{
		fprintf(fp, "Gui.SetValueColor(\"%s\", %f, %f, %f, %f); \n", value->name.c_str(), value->value->r, value->value->g, value->value->b, value->value->a);
	}

	fprintf(fp, "Gui.RefreshMenuStyle(); \n");

	fclose(fp);
}
 

CConfig* Config = new CConfig();