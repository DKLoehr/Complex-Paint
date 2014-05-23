#include "Parser.h"
namespace parser{
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

std::string Variable::toString() {
	return parser::toString(val);
}

Node::Node(string val, Node* parent) {
	m_val = val;
	m_parent = parent;
}
}
