#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <complex>
typedef complex<double> cx;
using namespace std;

template <typename T> string toString(T t); //To convert numeric types to strings
template <> string toString(cx c); //Specialized version for cx

static unordered_map<char*, cx (*T) (cx,cx)> ops;
//Functions and pointers to call actual operators
cx add(cx a, cx b);
cx sub(cx a, cx b);
cx mul(cx a, cx b);
cx div(cx a, cx b);

cx pow(cx base, cx a);
cx log(cx base, cx a);

cx sin(cx a);
cx cos(cx a);
cx tan(cx a);
cx asin(cx a);
cx acos(cx a);
cx atan(cx a);
cx sqrt(cx a);
cx abs(cx a);

cx pi();
cx e();

cx (* const static padd) (cx, cx);
cx (* const static psub) (cx, cx);
cx (* const static pmul) (cx, cx);
cx (* const static pdiv) (cx, cx);

cx (* const static ppow) (cx, cx);
cx (* const static plog) (cx, cx);

cx (* const static psin) (cx);
cx (* const static pcos) (cx);
cx (* const static ptan) (cx);
cx (* const static pasin) (cx);
cx (* const static pacos) (cx);
cx (* const static patan) (cx);
cx (* const static psqrt) (cx);
cx (* const static pabs) (cx);

cx (* const static pe) (cx);
cx (* const static ppi) (cx);

//initialize function pointers, put into map
void init() {
	padd = add;
	psub = sub;
	pmul = mul;
	pdiv = div;
	
	ppow = pow;
	plog = log;

	psin = sin;
	pcos = cos;
	ptan = tan;
	pasin = asin;
	pacos = acos;
	patan = atan;
	psqrt = sqrt;
	pabs = abs;

	ppi = pi;
	pe = e;

	ops.emplace("+", padd);	
	ops.emplace("-", psub);
	ops.emplace("*",pmul);
	ops.emplace("/",pdiv);
	
	ops.emplace("^",ppow);
	ops.emplace("log",plog);

	ops.emplace("sin",psin);
	ops.emplace("cos",pcos);
	ops.emplace("tan",ptan);
	ops.emplace("asin",pasin);
	ops.emplace("acos",pacos);
	ops.emplace("atan",patan);
	ops.emplace("sqrt",psqrt);
	ops.emplace("|",abs);

	ops.emplace("PI",ppi);
	ops.emplace("E",pe);
}
	
class Fct{
public:
	string name;
	string fct;
	makefct(string s); //Just passes string to Tree, which parses and stores it
	cx eval(cx n); //create temporary Tree, copy of m_tree, and evaluate
	Fct();
	Fct(string s);
	string toString();
private:
	Tree m_tree;

}

class Tree { //Hold decomposed expr
	const static char[] ooo = {"(",")","^","^","*","/","+","-"} //take in pairs. Double "^" is intentional.
	void parse();
	vector<Node> m_nodes;
	string toString();	
	friend class Node;
};

class Node { 
private:
	Node(auto val);
	bool reduced; //whether it has been reduced to smallest form or not
	void spawn(auto childVal); //Add child node
	void prune(auto childVal);
	void prune(); //remove all children
	vector<Node> m_child;
	Node* m_parent;
	auto m_val; //should always be char (for std. fcts, operators) or Variable
	string toString();

};

struct Variable {
public:
	string name;
	cx val;
	string toString();
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

