#pragma once
#include <vector>
#include <string>
#include "nuklear/src/nuklear.h"

template <typename T>
class ConfigValue
{
public:
	std::string name;
	T* value;

	ConfigValue(std::string name_, T* value_)
	{
		name = name_;
		value = value_;
	}
};

template <typename T>
class ConfigValueArray
{
public:
	ConfigValueArray(std::string name_, T* value_,int len_)
	{
		name = name_;
		value = value_;
		len = len_;
	}

	std::string name;
	int len;
	T* value;
};

class CConfig
{
protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValueArray<int>*> intArrays;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
	std::vector<ConfigValue<nk_colorf>*> colors;
private:
	void SetupValue(int&, std::string);
	void SetupValue(bool&, std::string);
	void SetupValue(float&, std::string);
	void SetupValue(nk_colorf&, std::string);
	void SetupValueArray(int*, std::string, int);
public:
	CConfig()
	{
		Setup();
	}

	void Setup();

	void SetValueInt(char *, int);
	void SetValueColor(char *, float, float, float, float);
	void SetValueIntArray(char *, char *);

	void Save(char *);
	void Load();
};

extern CConfig* Config;