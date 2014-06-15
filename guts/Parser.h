#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <complex>
#include <unordered_map>

//Constants to 20 decimal places
#define PARSER_SPI "3.14159265358979323846"
#define PARSER_SE "2.71828182845904523536"
#define PARSER_PI 3.14159265358979323846
#define PARSER_E 2.71828182845904523536
#define PARSER_SI "1i"
using namespace std;

typedef std::complex<double> cx;

namespace parser {
template <typename T> std::string toString(T t); //To convert numeric types to strings
template <> std::string toString(cx c); //Specialized version for cx
std::string toStringRounded(cx c); //Specialized version for cx, which rounds to 2 decimal places
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
};

enum DelimInd { ADD, SUB, MUL, DIV, POW, FT3, FT4,  FT2, VAR, CON};
class Tree { //Hold decomposed expr
private:
	static string delim[];
	static bool initd;
	static std::unordered_map<std::string, Tree*> variables;

	string toString(Node *n, string path);
	string m_fct;
	int parse(Node *root);
	cx value(Node *root);
	static std::unordered_map<std::string, cx (* const)(cx,cx)> parseops;
	Node *m_root;
	void init();
	int isToken(string s);
	int checkParenthesis(string s);
public:
	Tree(string expr="");
	~Tree();

	string toString();
	bool isInitd();
	int parse();
	cx eval();

	string getVar(string var);
	void setVar(string var, string a);
	void setVar(string var, cx a);
	cx evalVar(string var);
};

}
#endif
