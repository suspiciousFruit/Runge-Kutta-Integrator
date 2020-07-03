#pragma once
#include "ThreadPool.h"
#include <thread>
#include <mutex>
#include <vector>



// Конкретный представитель выполнителя с фиксированным количеством потоков выполнения
class FixThreadPool : public ThreadPool
{
private:
	static const size_t DEFAULT_THREADS_NUMBER = 2;
	const size_t threads_number_; // Количество потоков
	std::vector<std::thread> threads_; // Массив с потоками
	std::mutex pool_access_; // Доступ к пулу задач
	// Позже будет переписано с std::atomic
public:
	FixThreadPool(size_t threads_number); // Конструктор с одним аргументом
	// Удаляем все возможности перемещения и копирования объекта
	FixThreadPool(const FixThreadPool&) = delete;
	FixThreadPool(FixThreadPool&&) = delete;
	FixThreadPool& operator= (const FixThreadPool&) = delete;
	FixThreadPool&& operator= (FixThreadPool&&) = delete;

	virtual void processTasks(TaskPool*) override;
	virtual ~FixThreadPool() override;
private:
	static void thread_function_(TaskPool*, std::mutex*);
	// Позже будет переписано с std::atomic
};



FixThreadPool::FixThreadPool(size_t threads_number) :
	threads_number_(threads_number ? threads_number : DEFAULT_THREADS_NUMBER), threads_(threads_number_)
{ }

void FixThreadPool::processTasks(TaskPool* task_pool)
{
	if (task_pool != nullptr)
	{
		for (size_t i = 0; i < threads_number_; ++i) // В цикле инициализируем потоки
			threads_[i] = std::thread(&FixThreadPool::thread_function_, task_pool, &pool_access_);
			// Копирование потоков

		for (size_t i = 0; i < threads_number_; ++i)
			threads_[i].join();
			// Дожидаемся пока все потоки закончат выполнение задания
	}
}

FixThreadPool::~FixThreadPool()
{ }

// Функция которую будет выполнять каждый из созданных потоков
void FixThreadPool::thread_function_(TaskPool* task_pool, std::mutex* pool_access)
{
	Task* current_task;

	while (true)
	{
		{
			std::lock_guard<std::mutex> guard(*pool_access); // Будет переписано через std::atomic
			current_task = task_pool->getTask();
		}
		if (current_task == nullptr) break; // Если задач больше нет выходим из цикла

		current_task->complete(); // Выполняем задачу
		task_pool->setCompleted(current_task); // Выполнение контракта с TaskPool
		// Сообщаем пулу задач о том что задача выполнена
	}
}
