#pragma once
#include "FileToFileTask.h"
#include <map>
#include <string>



template <typename Integrator, typename SettingsParser>
class IntegrateTask : public FileToFileTask
{
private:
	Integrator integrator_;
public:
	IntegrateTask(std::ifstream&& a, std::ofstream&& b) : FileToFileTask(std::move(a), std::move(b))
	{ }

	virtual void complete() override
	{
		std::map<std::string, std::string> settings = SettingsParser::parse(std::ref(filein_)); // Парсим начальные данные

		integrator_.setSettings(settings); // Устанавливаем настройки

		integrator_.integrateInFile(std::ref(fileout_)); // Интегрируем в файл
	}

	virtual ~IntegrateTask()
	{ }
};
