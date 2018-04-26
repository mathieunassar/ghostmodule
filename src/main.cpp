#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>

int main()
{
	std::cout << "start" << std::endl;
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
	std::stringstream strCout;
	std::cout.rdbuf(strCout.rdbuf());

	std::cout << "Hello, World!" << std::endl;
	std::cout << "john " << std::flush;
	std::cout << "is not here" << std::endl;
	std::cout << "not here either!";

	// Restore old cout.
	//std::cout.rdbuf(oldCoutStreamBuf);

	std::string str;
	while (std::getline(strCout, str))
	{
		if (!strCout.eof())
		{
			str += "\n";
		}
		printf(str.c_str());
	}
}
