#include "guts/Parser.h"
#include <iostream>

int main() {
	using namespace parser;
	string func = "x+5";
	Fct *f = new Fct(func);
	std::cout << f->toString();
	return 0;
}
