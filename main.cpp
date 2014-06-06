#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func = "X^2";
	Tree* fct = new Tree(func);
	fct->setVar("X",5);
	fct->setVar("A",2);
	std::cout << "Tree:  " << fct->toString() << "\n";
	std::cout << "ANSWER: " << fct->eval() << "\n";


	return 0;
}
