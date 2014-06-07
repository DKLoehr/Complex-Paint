#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func;
	std::cin >> func;
	Tree* fct = new Tree(func);
	std::cout << "Tree:  " << fct->toString() << "\n";

	fct->setVar("X","5");
	std::cout << "ANSWER: " << fct->eval() << "\n";


	return 0;
}
