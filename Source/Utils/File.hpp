#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace TM
{
	namespace Utils
	{
		class File
		{
		public:
			static std::string Read(const std::string& filePath)
			{
				std::ifstream file(filePath);
				if (!file.is_open())
				{
					std::cerr << "Could not open file: " << filePath << std::endl;
					return "";
				}
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				return content;
			}
		};;
	}
}