#include "Parser.h"
namespace parser{
 std::unordered_map<const char*, cx (* const)(cx,cx)> parseops;

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

void init() {
	parseops = std::unordered_map<const char*, cx (* const)(cx,cx)>();
	parseops.emplace("+", padd);	
	parseops.emplace("-", psub);
	parseops.emplace("*",pmul);
	parseops.emplace("/",pdiv);
	
	parseops.emplace("^",ppow);
	parseops.emplace("ln",plog);

	parseops.emplace("sin",psin);
	parseops.emplace("cos",pcos);
	parseops.emplace("tan",ptan);
	parseops.emplace("asin",pasin);
	parseops.emplace("acos",pacos);
	parseops.emplace("atan",patan);
	parseops.emplace("sqrt",psqrt);
	parseops.emplace("abs", parser::abs);

	parseops.emplace("pi",ppi);
	parseops.emplace("e",pe);
}

cx add(cx a, cx b) {
	return a + b;
}
cx sub(cx a, cx b) {
	return a - b;
}
cx mul(cx a, cx b) {
	return a * b;
}
cx div(cx a, cx b) {
	return a / b;
}

cx pow(cx base, cx a) {
	return std::pow(base, a);	
}
cx log(cx base, cx a) {
	if(base == M_E)	return std::log(a);
	else return std::log(a) / std::log(base);
}

cx sin(cx a, cx b) {
	return std::sin(a);
}
cx cos(cx a, cx b) {
	return std::cos(a);
}
cx tan(cx a, cx b) {
	return std::tan(a);
}
cx asin(cx a, cx b) {
	return std::asin(a);
}
cx acos(cx a, cx b) {
	return std::acos(a);
}
cx atan(cx a, cx b) {
	return std::atan(a);
}
cx sqrt(cx a, cx b) {
	return std::sqrt(a);
}
cx abs(cx a, cx b) {
	return std::abs(a);
}

cx pi(cx a, cx b) {
	return M_PI;
}
cx e(cx a, cx b) {
	return M_E;
}

cx var(cx a, cx b) {
	return a;
}





Node::Node(Node * parent, string val) {
	m_parent = parent;
	m_val = val;
	m_left = NULL;
	m_right = NULL;
}

Node::~Node() {
	delete m_left;
	delete m_right;
}

string Node::toString() {
	return m_val;
}

//side == -1 => new left node
//side == 1 => new right node
void Node::spawn(string childVal, int side) {
	if(side == -1)
		m_left = new Node(this, childVal);
	else if(side == 1) 
		m_right = new Node(this, childVal);
}

//side == -1 => delete left node
//side == 1 => delete right node
//side == 0 => delete all children
void Node::prune(int side) {
	if(side ==  -1)
		delete m_left;
	else if(side ==  1)
		delete m_right;
	else if(side == 0) {
		delete m_right;
		delete m_left;
	}
}

Tree::Tree(string expr) {
	m_root = new Node(NULL, expr);
	parse();
}


Tree::~Tree() {
	delete m_root;
}

string Tree::toString(Node* n, string path) {
	if(n->m_left != NULL && n->m_right != NULL)
		return n->toString() + "; " + path + "L"+ " " + toString(n->m_left, path+"L") + "; " + path +"R" + " "+ toString(n->m_right, path+"R") + ";  ";
	else if(n->m_left != NULL)
		return n->toString() + ";  " +path + "L" + toString(n->m_left, path+"L") + ";  ";
	else if(n->m_right != NULL)
		return n->toString() + ";  " + path + "R" + toString(n->m_right, path+"R") + ";  ";
	else return n->toString();
}

string Tree::toString() {
	return toString(m_root, "O");
}

bool Tree::checkParenthesis(string s) {
	int i = 0;
	int count = 0;
	while(s[i] != NULL) {
		if(s[i] == '(') count++;
		else if(s[i] == ')') count--;
		++i;
	}
	return count == 0;
}

int Tree::parse() {
	return parse(m_root);
}

int Tree::parse(Node *root) {
	string& s = root->m_val;
	if(!checkParenthesis(s)) return 1;	
	int length = s.length();
	if(s[0] == '(' && s[length-1] == ')') {
		s.erase(length-1, 1);
		s.erase(0, 1);
		length -= 2;
	}

	int delimIndex = -1;
	//Loop through delimiters
	for(int j = 0; j < 5 && delimIndex == -1; j++) {
		for(int i = 0; i < length && delimIndex == -1; i++) {
			//skip paren
			if(s[i] == '(') { 
				int parenthCount = 1;
				while(parenthCount!= 0) {
					++i;
					if(s[i] == '(') ++parenthCount;
					else if(s[i] == ')') --parenthCount;
				}
			}
			//check if is current delimiter
			else if(s[i]== (delim[j])[0]) {
				delimIndex = i;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+1));
				root->m_val = s[i];
				parse(root->m_left);
				parse(root->m_right);
			}
		}
	}
	return 0;
}

Fct::Fct() {
	m_fct = "";
	m_tree=NULL;
}		

Fct::Fct(string s) {
	m_fct = s;
	makefct(m_fct);
}

void Fct::makefct(string s) {
	m_tree = new Tree(s);
}

string Fct::toString() {
	return m_fct + "\n" + m_tree->toString();
}
}
