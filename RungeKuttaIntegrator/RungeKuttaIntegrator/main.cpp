#include <iostream>
#include <fstream>
#include "..\RungeKuttaIntegrator\RKIntegrator\OdeLibrary.h"
#include "..\RungeKuttaIntegrator\RKIntegrator\RKMethodLibrary.h"
#include "..\RungeKuttaIntegrator\RKIntegrator\RKIntegrator.h"



std::map<std::string, std::string> settings = {
	{"ode", "LorenceAttractor" },
	{"rkmethod", "Verner_5_6_8"},
	{"iteration_number", "1000"}
};



int main()
{
	ode_t* ode = OdeLibrary::get(settings);
	const rkmethod_t* rkmethod = RKMethodLibrary::get(settings);

	_RKIntegrator integrator(ode, rkmethod);
	std::ofstream file("fileout.txt");

	integrator.setIntegratorSettings(settings);
	integrator.integrateInFile(std::ref(file));

	system("pause");
	return 0;
}