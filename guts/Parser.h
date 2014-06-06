#ifndef _PARSER_H_
#define _PARSER_H_

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
cx stringToCx(string s);

void init();
//Functions and pointers to call actual operators
cx add(cx a, cx b);
cx sub(cx a, cx b);
cx mul(cx a, cx b);
cx div(cx a, cx b);

cx pow(cx base, cx a); //only raises e^a right now
cx log(cx base, cx a); //Only does nat log of a right now

cx sin(cx a, cx b = 0);
cx cos(cx a, cx b = 0);
cx tan(cx a, cx b = 0);
cx asin(cx a, cx b = 0);
cx acos(cx a, cx b = 0);
cx atan(cx a, cx b = 0);
cx sqrt(cx a, cx b = 0);
cx abs(cx a, cx b = 0);

cx pi(cx a = 0, cx b = 0);
cx e(cx a = 0, cx b = 0);

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

//initialize function pointers, put into map

class Node { 
	friend class Tree;
private:

	Node* m_parent;
	Node* m_left;
	Node* m_right;
	string m_val; //should always be char (for std. fcts, operators) or Variable
	Node(Node *parent = NULL, string val = "");
public:
	~Node();
	string toString();
	void spawn(string childVal = "", int side = -1);
	void prune(int side = 0); 

};

class Tree { //Hold decomposed expr
private:
	static string delim[9];
	static bool initd;
	static cx variables[26];	

	string toString(Node *n, string path);
	string m_fct;
	int parse(Node *root);
	cx value(Node *root);
	static std::unordered_map<std::string, cx (* const)(cx,cx)> parseops;
	Node *m_root;
	void init();
public:

	Tree(string expr="");
	~Tree();

	string toString();	
	int parse();
	cx eval();
	bool checkParenthesis(string s);
	void setVar(string var, cx a);
	cx getVar(string var);
	bool isInitd();
	void printMapVal(const char* key);
};

}
#endif
