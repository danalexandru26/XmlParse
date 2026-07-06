#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<regex>
#include<format>

#include<cstdio>
#include<Windows.h>

constexpr uint16_t utf_8{65001};
const std::string path{"questions.txt"};

int main()
{
	std::fstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cerr << "File cannot be opened\n";
		return 1;
	}

	SetConsoleOutputCP(utf_8);

	std::string line{};
	std::regex questionIdent("^\\d+\\.\\D",
	                         std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex optionIdent("^[a-zA-z][\\.\\)]\\s",
	                       std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex answerIdent(":.+\\(",
	                       std::regex_constants::ECMAScript | std::regex_constants::icase);
	while (std::getline(file, line))
	{
		if (std::regex_search(line, questionIdent))
		{
			std::cout << std::format("Question: {}\n", line);
		}
		else if (std::regex_search(line, answerIdent))
		{
			std::cout << std::format("Answer: {}\n", line);
		}
	}

	file.close();
	return 0;
}
