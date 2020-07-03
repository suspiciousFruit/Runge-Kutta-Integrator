#pragma once
#include "Task.h"
#include <fstream>



// Класс задач которые работают с двумя файлами
class FileToFileTask : public Task
{
protected:
	std::ifstream filein_; // Файл с заданием
	std::ofstream fileout_; // Файл куда необходимо записать ответ
public:
	FileToFileTask(std::ifstream&& filein, std::ofstream&& fileout) : // Конструктор с двумя аргументами
		filein_(std::move(filein)), fileout_(std::move(fileout))
	{ }

	virtual ~FileToFileTask()
	{ }
};
