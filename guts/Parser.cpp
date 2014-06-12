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

cx stringToCx(string s) {
	std::istringstream is('(' + s + ",0)");
	cx a;
	is >> a;
	return a;
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
	return 0;
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

//	TREE
string Tree::delim[] = {"+", "-", "*", "/", "^", "sin;cos;tan;log;abs;", "sqrt;asin;acos;atan;", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ln;"}; 
bool Tree::initd = false;
std::unordered_map<std::string, cx (* const)(cx,cx)> Tree::parseops = std::unordered_map<std::string, cx (* const)(cx,cx)>();
std::unordered_map<std::string, Tree*> Tree::variables = std::unordered_map<std::string, Tree*>();

Tree::Tree(string expr) {
	if(!initd) {
		initd = true;
		init();
	}
	m_fct = expr;
	for(int i = m_fct.length()-1; i >= 0; i--)
		if(m_fct[i] == 32) //Whitespace character
			m_fct.erase(i, 1);
	std::cout << m_fct << "\n";
	m_root = new Node(NULL, m_fct);
	parse();
}


Tree::~Tree() {
	delete m_root;
}

void Tree::init() {
	Tree::parseops.emplace("+", padd);	
	Tree::parseops.emplace("-", psub);
	Tree::parseops.emplace("*",pmul);
	Tree::parseops.emplace("/",pdiv);

	Tree::parseops.emplace("^",ppow);
	Tree::parseops.emplace("ln",plog);

	Tree::parseops.emplace("sin",psin);
	Tree::parseops.emplace("cos",pcos);
	Tree::parseops.emplace("tan",ptan);
	Tree::parseops.emplace("asin",pasin);
	Tree::parseops.emplace("acos",pacos);
	Tree::parseops.emplace("atan",patan);
	Tree::parseops.emplace("sqrt",psqrt);
	Tree::parseops.emplace("abs", parser::abs);

	Tree::parseops.emplace("pi",ppi);
	Tree::parseops.emplace("e",pe);
	std::cout << "initializing map" << "\n";	
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
	while(s[i] != 0) {
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

	bool foundDelim = false;
	//Loop through delimiters
	for(int j = 0; j < 9 && !foundDelim; j++) {
		for(int i = 0; i < length && !foundDelim; i++) {
			//skip paren
			if(s[i] == '(') { 
				int parenthCount = 1;
				while(parenthCount!= 0) {
					++i;
					if(s[i] == '(') ++parenthCount;
					else if(s[i] == ')') --parenthCount;
				}
			}
			//arithmetic
			else if(s[i]== (delim[j])[0] && j < 5) {
				foundDelim = true;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+1));
				root->m_val = s[i];
				parse(root->m_left);
				parse(root->m_right);
			}
			//Length 3 fct
			else if(i+3 < length && j == 5 && delim[j].find(s.substr(i, i+3) + ";") != -1) {
				std::cout << "length 3 fcts \n";
				foundDelim = true;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+3));
				root->m_val = s.substr(i, i+3);
				parse(root->m_left);
				parse(root->m_right);
			}
			//Length 4 fct
			else if(i+4 < length && j == 6 && delim[j].find(s.substr(i, i+4) + ";") != -1) {
				std::cout << "length 4 fcts \n";
				foundDelim = true;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+4));
				root->m_val = s.substr(i, i+4);
				parse(root->m_left);
				parse(root->m_right);
			}
			//Length 2 fct
			else if(i+2 < length && j == 8 && delim[j].find(s.substr(i, i+2) + ";") != -1) {
				std::cout << "finding ln \n";
				foundDelim = true;
				root->m_left = new Node(root, s.substr(0, i));
				root->m_right = new Node(root, s.substr(i+2));
				root->m_val = s.substr(i, i+2);
				parse(root->m_left);
				parse(root->m_right);
			}
			else if(j == 7 && delim[j].find(s[i]) != -1) {
				std::cout << "finding var \n";
				foundDelim = true;
				variables.emplace(&s[i], new Tree("0") );
				//root->m_left = new Node(root, s.substr(0, i));
				//root->m_right = new Node(root, s.substr(i+1));
				root->m_val = s[i];
			}
		}
	}
	return 0;
}

//Calculate the value of a tree
cx Tree::value(Node *root) {
	try {
		return parseops.at(root->m_val)( value(root->m_left), value(root->m_right));
	}
	catch(const out_of_range& err) {
		if(-1 != delim[7].find(root->m_val)) {
			return variables.at(root->m_val)->eval();
		}
		else {
			std::cout << "WARNING: variable not found \n";
			return stringToCx(root->m_val);
		}
	}
}

//Set a variable
void Tree::setVar(string var, string a) {
	try {
		variables[var] = new Tree(a);
	}
	catch(const std::out_of_range& err) {
		std::cout << "variables does not exist \n";
	}
}

//Get the toString of a variable's tree
string Tree::getVar(string var) {
	try {
		return variables[var]->toString();
	}
	catch(const std::out_of_range& err) {
		std::cout << "variable undefined or does not exist \n";
	}
}


cx Tree::eval() {
	return value(m_root);
}

bool Tree::isInitd() {
	return Tree::initd;
}


}
