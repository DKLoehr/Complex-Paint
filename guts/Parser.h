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

extern std::unordered_map<const char*, cx (* const)(cx,cx)> parseops;
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

cx var(cx a, cx b = 0);

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
void init(); 

class Node { 
	friend class Tree;
private:

	Node* m_parent;
	Node* m_left;
	Node* m_right;
	string m_val; //should always be char (for std. fcts, operators) or Variable
public:
	Node(Node *parent = NULL, string val = "");
	~Node();
	string toString();
	void spawn(string childVal = "", int side = -1);
	void prune(int side = 0); 

};

class Tree { //Hold decomposed expr
private:
	string toString(Node *n, string path);
	int parse(Node *root);
	Node *m_root;
	string delim[9] = {"+", "-", "*", "/", "^", "sin;cos;tan;log;abs;", "sqrt;asin;acos;atan;", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ln;"}; 
public:
	Tree(string expr="");
	~Tree();

	string toString();	
	int parse();
	bool checkParenthesis(string s);
	
};

class Fct{
public:
	string m_fct;
	void makefct(std::string s); //Just passes string to Tree, which parses and stores it
	cx eval(cx n); //create temporary Tree, copy of m_tree, and evaluate
	Fct();
	Fct(std::string s);
	string toString();
private:
	Tree *m_tree;

};
}
#endif

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

