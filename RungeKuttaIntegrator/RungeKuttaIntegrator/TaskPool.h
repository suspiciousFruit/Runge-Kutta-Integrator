#pragma once
#include "Task.h"



// Класс-интерфейс конкретного представителя делегатора задач
class TaskPool
{
public:
	virtual Task* getTask() = 0; // Получить следующую задачу
	virtual void setCompleted(Task*) = 0; // Сообщить TaskPool о том что задача выполнена
	// Все представители исполнитей должны по окончанию выполнения задачи вызывать этот метод
	virtual ~TaskPool()
	{ }
};
