#pragma once
#include <map>
#include <fstream>
#include <string>



class KeyValueParser
{
public:
	static std::map<std::string, std::string> parse(std::ifstream& file);
};


std::map<std::string, std::string> KeyValueParser::parse(std::ifstream& file)
{
	std::string line;
	std::map<std::string, std::string> map;

	while (1)
	{
		std::getline(file, line);

		size_t sep_pos = line.find(':');

		if (sep_pos != std::string::npos && sep_pos != 0)
		{
			std::string key;
			std::string value;

			key = line.substr(0, sep_pos); // Выделяем ключ
			
			size_t value_pos = line.find_first_not_of(' ', sep_pos + 1); // пропускаем пробелы при считывании
			if (value_pos != std::string::npos)
				value = line.substr(value_pos);
			else
				value = "";

			map[key] = value;
		}

		if (file.eof()) break;
	}

	return map;
}
