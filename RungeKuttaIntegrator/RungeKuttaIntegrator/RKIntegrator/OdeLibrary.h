#pragma once
#include <string>
#include <map>
#include "ode_t.h"



class OdeLibrary
{
public:
	static ode_t* get(const std::map<std::string, std::string>&);
	static void returnBack(ode_t*);
};

ode_t* OdeLibrary::get(const std::map<std::string, std::string>& settings)
{
	const std::map<std::string, std::string>::const_iterator pair = settings.find("ode");
	ode_t* res = nullptr;

	auto equal = [&pair](const std::string& name) -> bool
	{ return name == pair->second; };

	if (pair != settings.end())
	{
		if (equal("DoublePendulum"))
			res = new DoublePendulum();

		if (equal("LorenceAttractor"))
			res = new LorenceAttractor();
	}

	return res;
}

void OdeLibrary::returnBack(ode_t* ode)
{
	if (ode) delete ode;
}
