#pragma once
#include "TaskPool.h"



// Класс-интерфейс для представителя класса который сможет выполнять задачи
class ThreadPool
{
public:
	virtual void processTasks(TaskPool*) = 0; // Обработать все задачи из пула задач
	// TaskPool считыется выполненным если метод .getTask() вернул nullptr
	// Пока функция .processTasks() не завершена, переданный ей TaskPool должен существовать
	virtual ~ThreadPool()
	{ }
};
