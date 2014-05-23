#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <complex>
#include <unordered_map>

using namespace std;

typedef std::complex<double> cx;

namespace parser {
template <typename T> std::string toString(T t); //To convert numeric types to strings
template <> std::string toString(cx c); //Specialized version for cx

std::unordered_map<const char*, cx (* const)(cx,cx)> ops;
//Functions and pointers to call actual operators
cx add(cx a, cx b);
cx sub(cx a, cx b);
cx mul(cx a, cx b);
cx div(cx a, cx b);

cx pow(cx base, cx a);
cx log(cx base, cx a);

cx sin(cx a, cx b);
cx cos(cx a, cx b);
cx tan(cx a, cx b);
cx asin(cx a, cx b);
cx acos(cx a, cx b);
cx atan(cx a, cx b);
cx sqrt(cx a, cx b);
cx abs(cx a, cx b);

cx pi(cx a, cx b);
cx e(cx a, cx b);

cx var(cx a, cx b);

cx (* const padd)(cx, cx) = parser::add;
cx (* const psub)(cx, cx) = parser::sub;
cx (* const pmul)(cx, cx) = parser::mul;
cx (* const pdiv)(cx, cx) = parser::div;

cx (* const ppow)(cx, cx) = parser::pow;
cx (* const plog)(cx, cx) = parser::log;

cx (* const psin)(cx, cx) = parser::sin;
cx (* const pcos)(cx, cx) = parser::cos;
cx (* const ptan)(cx, cx) = parser::tan;
cx (* const pasin)(cx, cx) = parser::asin;
cx (* const pacos)(cx, cx) = parser::acos;
cx (* const patan)(cx, cx) = parser::atan;
cx (* const psqrt)(cx, cx) = parser::sqrt;
cx (* const pabs)(cx, cx) = parser::abs;

cx (* const pe)(cx, cx) = parser::e;
cx (* const ppi)(cx, cx) = parser::pi;

cx (* const pvar)(cx,cx) = parser::var;

//initialize function pointers, put into map
void init() {
	ops.emplace("+", padd);	
	ops.emplace("-", psub);
	ops.emplace("*",pmul);
	ops.emplace("/",pdiv);
	
	ops.emplace("^",ppow);
	ops.emplace("ln",plog);

	ops.emplace("sin",psin);
	ops.emplace("cos",pcos);
	ops.emplace("tan",ptan);
	ops.emplace("asin",pasin);
	ops.emplace("acos",pacos);
	ops.emplace("atan",patan);
	ops.emplace("sqrt",psqrt);
	ops.emplace("abs", parser::abs);

	ops.emplace("PI",ppi);
	ops.emplace("E",pe);
}
	



struct Variable {
public:
	string name;
	cx val;
	std::string toString();
};


class Node { 
private:
	Node(Node *parent);
	Node(parser::Variable value, Node* parent);
	Node(std::string value, Node* parent);

	bool reduced; //whether it has been reduced to smallest form or not
	void spawn(std::string childVal); //Add child node
	void prune(parser::Variable childVal);
	void prune(); //remove all children

	Node* m_parent;
	Node* left;
	Node* right;
	string m_val; //should always be char (for std. fcts, operators) or Variable
	string toString();

};

class Tree { //Hold decomposed expr
	void parse();
	bool simplified();
	parser::Node root;
	std::string toString();	
	friend class Node;
};

class Fct{
public:
	std::string name;
	std::string fct;
	void makefct(std::string s); //Just passes string to Tree, which parses and stores it
	cx eval(cx n); //create temporary Tree, copy of m_tree, and evaluate
	Fct();
	Fct(std::string s);
	std::string toString();
private:
	parser::Tree m_tree;

};
}

/**
 * ___THE NOTES BELOW ARE FOR CORE FUNCTIONALITY: CLASS CAN BE EXTENDED LATER____ 
 * DESIGN NOTES
 * 1a. Create list of delimiters (look up in hashmap)
 * 1b. ***USE Alternate to 1a: have list of delimiters in order of operations, each level of tree is a level down the order
 * 2. Make tree from decomposed expressions. No node has more than 2 children.
 * 3. Go through tree from bottom up to evalutate expr. 
 * Use hashmap (unordered_map)  for operator and std function chars; map char values to function pointers. Variables use variable class.
 * Evaluate on a node-by node basis, going up tree.
 *
 * ON PARENTHESIS
 * Keep a counter, when encounter "(", counter++, when hit ")" counter--
 * If counter != 0 at the end, throw error
 *
 * ON PARSING GENERALLY
 * Legal fcts: sin, cos, tan, asin, acos, atan, , ln [calls complex::log()], logBaseN(base, num) [use change of base], abs, sqrt
 * Operators: (), +, -, *, /, ^ [complex::pow]
 * PI = pi, E = e
 * Maybe specify that variables must be put in as capital letters => very easy to parse everything else. If this is the case, make pi and e lowercase.
 *
 * */

