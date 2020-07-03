#pragma once
#include <filesystem>
#include <iostream>
#include "Vector.h"
#include "FixThreadPool.h"
#include "FolderTaskPool.h"
#include <chrono>



class TestSystem
{
private:
	const std::filesystem::path source_folder_;
	const std::filesystem::path destination_folder_;
public:
	TestSystem(const std::filesystem::path& source_folder, const std::filesystem::path& destination_folder) :
		source_folder_(source_folder), destination_folder_(destination_folder)
	{ }

	void makeTest(const Vector<size_t>& range = { 1, 2, 4, 8, 16 })
	{
		if (std::filesystem::is_directory(source_folder_) &&
			std::filesystem::is_directory(destination_folder_) &&
			source_folder_ != destination_folder_)
		{
			for (size_t i = 0; i < range.size(); ++i)
			{
				FolderTaskPool task_pool(source_folder_, destination_folder_);
				FixThreadPool thread_pool(range[i]);
				std::chrono::time_point starting_time = std::chrono::high_resolution_clock::now();
				thread_pool.processTasks(&task_pool);
				std::chrono::time_point ending_time = std::chrono::high_resolution_clock::now();
				std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(ending_time - starting_time);
				std::cout << "threads: " << range[i] << " time(millisec): " << time.count() << std::endl;
			}
		}
		else
		{
			std::cout << "Error: Wrong path!" << std::endl;
		}
	}
};
