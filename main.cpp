#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func = "X^2";
	Tree* fct = new Tree(func);
	fct->setVar("X",5);
	fct->setVar("A",10);
	std::cout << "Tree:  " << fct->toString() << "\n";
	std::cout << fct->getVar("X") << "\n";


	std::cout << fct->eval() << "\n";

	return 0;
}

//		TODO
//
//		LOG
// 	X
// GOOD
//	X+5
//BAD: not removing parenth correctly. Need give length parameter to string.erase(position, length);
//BAD: going out of bounds on many loops later on. Fix: add check. 
//BAD: assigning X to origin, giving empty to others. Fix: Have to let variables fall through, just like numbers. Just add in a map entry.
//GOOD
//	X*sin(5)
//GOOD
//	X*sin(5)+pi
//BAD: segfault @ 196 Fix: rearranged vars to avoid segfault
//BAD: parsing into wrong order: reading as X*sin(5+pi)
