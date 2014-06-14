#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func;
	//std::cout << "Input function: \n";
	//std::cin >> func;
	//func = "(X + 1-1) ^ (2 *2 -2)";
	func  = "(i+1)^X";
	Tree* fct = new Tree(func);
	fct->setVar("X","2");
	std::cout << "Tree:  " << fct->toString() << "\n";

	std::cout << "ANSWER: " << fct->eval() << "\n";

	return 0;
}
