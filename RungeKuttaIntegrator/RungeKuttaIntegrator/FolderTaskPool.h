#pragma once
#include <filesystem>
#include "TaskPool.h"
#include "TaskLibrary.h"



class FolderTaskPool : public TaskPool
{
private:
	std::filesystem::directory_iterator current_entry_;
	const std::filesystem::path folder_out_path_;
	size_t task_number_;
	
	TaskLibrary task_library_;
public:
	FolderTaskPool(const std::filesystem::path&, const std::filesystem::path&);
	virtual ~FolderTaskPool();
	virtual Task* getTask() override;
	virtual void setCompleted(Task*) override;
private:
	std::wstring generateName(size_t task_number);
};



FolderTaskPool::FolderTaskPool(const std::filesystem::path& source_folder, const std::filesystem::path& destination_folder) :
	current_entry_(std::filesystem::directory_iterator(source_folder)), // Считаем что переданная директория существует
	folder_out_path_(destination_folder), // Считаем что директория существует
	task_number_(1)
{ }

FolderTaskPool::~FolderTaskPool()
{ }

Task* FolderTaskPool::getTask()
{
	Task* task = nullptr;

	std::filesystem::path file_out_name = generateName(task_number_);

	for (; current_entry_ != std::filesystem::directory_iterator(); ++current_entry_)
	{
		if (current_entry_->is_regular_file())
		{
			task = task_library_.allocateTask(current_entry_->path(), file_out_name);

			if (task != nullptr)
			{
				++task_number_;
				++current_entry_;
				break;
			}
		}
	}

	return task;
}

void FolderTaskPool::setCompleted(Task* task)
{
	delete task;
}

std::wstring FolderTaskPool::generateName(size_t task_number)
{
	return std::filesystem::path(folder_out_path_) /= std::wstring(L"result_" + std::to_wstring(task_number) + L".txt");
}


class Folder
{
private:
	std::filesystem::directory_entry current_entry_;
public:
	Folder(std::filesystem::path&);
	std::filesystem::directory_entry* getNextFile()
	{
		return &current_entry_;
	}
};

