#pragma once
#include <map>
#include <string>
#include "rkmethod_t.h"



class RKMethodLibrary
{
public:
	static const rkmethod_t* get(const std::map<std::string, std::string>&);
	static void returnBack(const rkmethod_t*);
};

const rkmethod_t* RKMethodLibrary::get(const std::map<std::string, std::string>& settings)
{
	return &rkmethod_t::Verner_5_6_8;
}

void RKMethodLibrary::returnBack(const rkmethod_t*)
{ }
