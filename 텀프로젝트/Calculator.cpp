#include <iostream>
#include <string>
#include <stack>
#include <vector>

class Strategy {
public:
	virtual int doOperation(int num1, int num2) = 0;
};

class OperationAdd : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubstract : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 - num2;
	}
};

class OperationMultiply : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};

class OperationDivide : public Strategy {
public:
	int doOperation(int num1, int num2) {
		return num1 / num2;
	}
};

class Context {
private:
	Strategy* strategy;

public:
	Context(Strategy* strategy) {
		this->strategy = strategy;
	}

	int executeStrategy(int num1, int num2) {
		return strategy->doOperation(num1, num2);
	}
};

int getPriority(std::string ch) {
	if (ch == "*" || ch == "/") {
		return 2;
	}
	else if (ch == "+" || ch == "-") {
		return 1;
	}
	else if (ch == "(" || ch == ")") {
		return 0;
	}
	else {
		return -1;
	}
}

int main() {
	std::string expression;

	std::cout << "Enter the expression : ";
	std::cin >> expression;

	std::stack<std::string> s;
	std::vector<std::string> v;
	int len = expression.length();
	int operationCount = 0;
	int numCount = 0;
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int bracketError = 0;
	int idx = 1;

	for (int i = 0; i < len; i++) {
		
		std::string ch = expression.substr(i, 1);
		int a = getPriority(ch);

		if (a < 0) {
			if (i != len - 1) {
				std::string s = expression.substr(i + 1, 1);
				int k = getPriority(s);
				if (k >= 0) {
					ch = expression.substr(i-idx+1, idx);
					a = getPriority(ch);
					idx = 1;
				}
				else {
					idx++;
					continue;
				}
			}
			else {
				ch = expression.substr(i - idx +1, idx);
				a = getPriority(ch);
			}
		}
		if (a >= 1) {
			while (!s.empty() && getPriority(s.top()) >= a) {
				v.push_back(s.top());
				s.pop();
			}
			s.push(ch);
			operationCount++;
		}
		else if (ch == "(") {
			s.push(ch);
			leftBracketCount++;
		}
		else if (ch == ")") {
			if (s.empty()) {
				bracketError = 1;
				break;
			}
			while (s.top() != "(") {
				if (s.empty()) {
					bracketError = 1;
					break;
				}
				v.push_back(s.top());
				s.pop();
			}
			s.pop();
			rightBracketCount++;
		}
		else {
			v.push_back(ch);
			numCount++;
		}
	}
	while (!s.empty()) {
		v.push_back(s.top());
		s.pop();
	}
	
	if (numCount - operationCount != 1) {
		std::cout << "Error: 잘못된 식입니다." << std::endl;
	}
	else if (leftBracketCount != rightBracketCount) {
		std::cout << "Error: 잘못된 식입니다." << std::endl;
	}
	else if (bracketError == 1) {
		std::cout << "Error: 잘못된 식입니다." << std::endl;
	}
	else {
		for (int i = 0; i < v.size(); i++) {
			if (v[i] == "/") {
				Context* contextDiv = new Context(new OperationDivide());
				int n1 = 0;
				int n2 = 0;
				if (v[i - 2].substr(0, 2) == "0x") {
					n1 = std::stoi(v[i - 2], nullptr, 16);
				}
				else if (v[i - 2].substr(0, 2) == "0b") {
					n1 = std::stoi(v[i - 2].substr(2, v[i - 2].length()-2), nullptr, 2);
				}
				else {
					n1 = std::stoi(v[i - 2]);
				}
				if (v[i - 1].substr(0, 2) == "0x") {
					n2 = std::stoi(v[i - 1], nullptr, 16);
				}
				else if (v[i - 1].substr(0, 2) == "0b") {
					n2 = std::stoi(v[i - 1].substr(2, v[i - 1].length()-2), nullptr, 2);
				}
				else {
					n2 = std::stoi(v[i - 1]);
				}
				
				int result = contextDiv->executeStrategy(n1, n2);
				v.erase(v.begin() + i);
				v.insert(v.begin() + i, std::to_string(result));
				v.erase(v.begin() + (i - 1));
				v.erase(v.begin() + (i - 2));
				i = 0;
			}
			else if (v[i] == "*") {
				Context* contextMul = new Context(new OperationMultiply());
				int n1 = 0;
				int n2 = 0;
				if (v[i - 2].substr(0, 2) == "0x") {
					n1 = std::stoi(v[i - 2], nullptr, 16);
				}
				else if (v[i - 2].substr(0, 2) == "0b") {
					n1 = std::stoi(v[i - 2].substr(2, v[i - 2].length()-2), nullptr, 2);
				}
				else {
					n1 = std::stoi(v[i - 2]);
				}
				if (v[i - 1].substr(0, 2) == "0x") {
					n2 = std::stoi(v[i - 1], nullptr, 16);
				}
				else if (v[i - 1].substr(0, 2) == "0b") {
					n2 = std::stoi(v[i - 1].substr(2, v[i - 1].length()-2), nullptr, 2);
				}
				else {
					n2 = std::stoi(v[i - 1]);
				}
				int result = contextMul->executeStrategy(n1, n2);
				v.erase(v.begin() + i);
				v.insert(v.begin() + i, std::to_string(result));
				v.erase(v.begin() + (i - 1));
				v.erase(v.begin() + (i - 2));
				i = 0;
			}
			else if (v[i] == "+") {
				Context* contextAdd = new Context(new OperationAdd());
				int n1 = 0;
				int n2 = 0;
				if (v[i - 2].substr(0, 2) == "0x") {
					n1 = std::stoi(v[i - 2], nullptr, 16);
				}
				else if (v[i - 2].substr(0, 2) == "0b") {
					n1 = std::stoi(v[i - 2].substr(2, v[i - 2].length()-2) , nullptr, 2);
				}
				else {
					n1 = std::stoi(v[i - 2]);
				}
				if (v[i - 1].substr(0, 2) == "0x") {
					n2 = std::stoi(v[i - 1], nullptr, 16);
				}
				else if (v[i - 1].substr(0, 2) == "0b") {
					n2 = std::stoi(v[i - 1].substr(2, v[i - 1].length()-2), nullptr, 2);
				}
				else {
					n2 = std::stoi(v[i - 1]);
				}

				int result = contextAdd->executeStrategy(n1, n2);
				v.erase(v.begin() + i);
				v.insert(v.begin() + i, std::to_string(result));
				v.erase(v.begin() + (i - 1));
				v.erase(v.begin() + (i - 2));
				i = 0;
			}
			else if (v[i] == "-") {
				Context* contextSub = new Context(new OperationSubstract());
				int n1 = 0;
				int n2 = 0;
				if (v[i - 2].substr(0, 2) == "0x") {
					n1 = std::stoi(v[i - 2], nullptr, 16);
				}
				else if (v[i - 2].substr(0, 2) == "0b") {
					n1 = std::stoi(v[i - 2].substr(2, v[i - 2].length()-2), nullptr, 2);
				}
				else {
					n1 = std::stoi(v[i - 2]);
				}
				if (v[i - 1].substr(0, 2) == "0x") {
					n2 = std::stoi(v[i - 1], nullptr, 16);
				}
				else if (v[i - 1].substr(0, 2) == "0b") {
					n2 = std::stoi(v[i - 1].substr(2, v[i - 1].length()-2), nullptr, 2);
				}
				else {
					n2 = std::stoi(v[i - 1]);
				}
				int result = contextSub->executeStrategy(n1, n2);
				v.erase(v.begin() + i);
				v.insert(v.begin() + i, std::to_string(result));
				v.erase(v.begin() + (i - 1));
				v.erase(v.begin() + (i - 2));
				i = 0;
			}
		}
		std::cout << "result : " << v[v.size() - 1] << std::endl;
	}
	
}


	
