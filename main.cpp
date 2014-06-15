#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func;
	std::cout << "Please input function: \n";
	std::getline(cin, func);
	Tree* fct = new Tree(func);
	string xval;
	std::cout << "Please input expression for X \n";
	std::getline(cin, xval);
	fct->setVar("X",xval);
	string yval;
	std::cout << "Please input expression for Y \n";
	std::getline(cin, yval);
	fct->setVar("Y",yval);

	std::cout << "Tree:  " << fct->toString() << "\n";

	std::cout << "ANSWER: " << fct->eval() << "\n";

	return 0;
}
