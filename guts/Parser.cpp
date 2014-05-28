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
	string retVal = m_val;
	if(m_right != NULL)
		retVal += ", right";
	if(m_left != NULL)
		retVal += ", left";
	return retVal;
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

string Tree::toString(Node* n) {
	if(n->m_left != NULL && n->m_right != NULL)
		return n->toString() + "; " + toString(n->m_left) + "; " + toString(n->m_right) + "; ";
	else if(n->m_left != NULL)
		return n->toString() + "; " + toString(n->m_left) + "; ";
	else if(n->m_right != NULL)
		return n->toString() + "; " + toString(n->m_right) + "; ";
	else return n->toString();
}

string Tree::toString() {
	return toString(m_root);
}

bool Tree::checkParenthesis(string s) {
	int i = 0;
	int count = 0;
	while(s[i] != 0)
		if(s[i] == '(') count++;
		else if(s[i] == ')') count--;
	return count == 0;
}

int Tree::parse() {
	return parse(m_root);
}

int Tree::parse(Node *root) {
	string& s = root->m_val;
	if(!checkParenthesis(s)) return 1;	
	int length = s.length();
	//remove overall enclosing parenthesis
	if(s[0] == '(' && s[length-1] == ')') {
		s.erase(length-1);
		s.erase(0);
		length -= 2;
	}

	int delimIndex = -1;
	for(int i = 0; i < length && delimIndex == -1; i++) {
		int pcount = 0; //parenthesis counter
		if(s[i] == '(') { //skips contents of parenthesis
			pcount++;
			while(pcount != 0) {
				i++;
				if(s[i] == ')') pcount++;
				else if(s[i] == '(') pcount--;
			}
		}
		//looking for delimiters
		//arithmetic operators
		for(int j = 0; j < 5 && delimIndex == -1; j++) 
			if(s[i]== delim[j][0]) {
				delimIndex = i;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+1));
				root->m_val = s[i];
				parse(root->m_left);
				parse(root->m_right);
			}
		//3 and 4 char func
		for(int k = 0; k < 2 && delimIndex == -1; k++) {
			if(delim[5+k].find(s.substr(i, i+3+k) + ";") != -1) {
				delimIndex = i;
				root->m_left = new Node(root, s.substr(i+3+k));
				root->m_right = new Node(root, "");
				root->m_val = s.substr(i, i+3+k);
				parse(root->m_left);
			}
		}
		if(delimIndex == -1) {
			// ln
			if(s.substr(i, i+2).compare(delim[8]) == 0) {
				delimIndex = i;
				root->m_left = new Node(root, s.substr(i+3));
				root->m_right = new Node(root, "");
				root->m_val = s.substr(i, i+4);
				parse(root->m_left);
			}
			else if(s[i] == 'e') {
				delimIndex = i;
				root->m_left = new Node(root, "");
				root->m_right = new Node(root, "");
				root->m_val = s[i];
			}
			else if(s.substr(i, i+2).compare("pi") == 0) {
				delimIndex = i;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+2));
				root->m_val = s.substr(i, i+2);
				parse(root->m_left);
				parse(root->m_right);
			}
			else if(delim[7].find(s[i]) != -1) {
				delimIndex == i;
				root->m_left = new Node(root, "");
				root->m_right = new Node(root, "");
				root->m_val = s[i];
				parseops.emplace("A",pvar);
				//parseops.emplace(s[i], pvar); //SHOULD ACTUALLY BE SOMETHING LIKE THIS
			}
		}
	}
	if(delimIndex == -1) return 3;
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
