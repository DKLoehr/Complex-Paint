#include "Parser.h"

template <typename T> string toString(T t) {
	stringstream s;
	s << t;
	return s.str();
}

template<> string toString(cx c) {
	stringstream s;
	s << c.real() << "+" << c.imag() << "i";
	return s.str();
}

Variable::toString() {
	return toString(val);
}

Node::Node(auto val) {
	m_val = val
}
