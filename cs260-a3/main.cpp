/*
	Author: Cody Potter
	Title: Infix -> Postfix Evalutator
	Purpose: CS260 Assignment 3
	Instructor: Jen Miller
*/

#include<climits>
#include<iostream>
#include<stack>
#include<string>

using namespace std;

bool isOperand(char testChar);
bool isOperator(char testChar);
bool isLeftParenthesis(char testChar);
bool isRightParenthesis(char testChar);
bool isRightAssociative(char testChar);
bool hasHigherPrecedence(char operator1, char operator2);
bool isValid(string testString);
string removeWhiteSpace(string stringToFormat);
string infixToPostfix(string expression);
int getOperatorWeight(char operatorChar);
int evaluatePostfix(string expression);
int performOperation(char theoperator, int operand1, int operand2);
int convertCharToInt(char theChar);

int main(void) {
	cout << "Author: Cody Potter" << endl;
	bool didContinue = true;
	while (didContinue) {
		string infixExpression = "";
		string postfixExpression = "";

		cout << "Please enter an infix expression to convert and evaluate (or quit to exit): ";
		getline(cin, infixExpression);
		if (infixExpression == "quit") {
			didContinue = false;
			continue;
		}
		
		cout << "Infix expression: " << infixExpression << endl;

		postfixExpression = infixToPostfix(infixExpression);
		if (!isValid(postfixExpression)) {
			cout << "ERROR: Not a valid infix expression. Please try again." << endl;
			continue;
		}
		cout << "Postfix expression: " << postfixExpression << endl;

		cout << "Result: " << evaluatePostfix(postfixExpression) << endl;
	}
	
	return 0;
}

string removeWhiteSpace(string stringToFormat) {
	string formattedString = "";
	for (unsigned int i = 0; i < stringToFormat.length(); i++) {
		if (stringToFormat[i] != ' ') {
			formattedString += stringToFormat[i];
		}
	}
	return formattedString;
}

string infixToPostfix(string expression) {
	stack<char> operatorStack;
	string postfix = "";
	string formattedExpression = removeWhiteSpace(expression);

	for (unsigned int i = 0; i < formattedExpression.length(); i++) {
		if (isOperator(formattedExpression[i])) {
			while (!operatorStack.empty() && !isLeftParenthesis(operatorStack.top()) && hasHigherPrecedence(operatorStack.top(), formattedExpression[i])) {
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(formattedExpression[i]);
		} else if (	isOperand(formattedExpression[i])) {
			postfix += formattedExpression[i];
		} else if (isLeftParenthesis(formattedExpression[i])) {
			operatorStack.push(formattedExpression[i]);
		} else if (isRightParenthesis(formattedExpression[i])) {
			while (!operatorStack.empty() && !isLeftParenthesis(operatorStack.top())) {
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
		}
	}

	while (!operatorStack.empty()) {
		postfix += operatorStack.top();
		operatorStack.pop();
	}

	return postfix;
}

bool hasHigherPrecedence(char operator1, char operator2) {
	int operator1Weight = getOperatorWeight(operator1);
	int operator2Weight = getOperatorWeight(operator2);

	if (operator1Weight == operator2Weight) {
		return !isRightAssociative(operator1); // returns false if operator is ^
	}
	return operator1Weight > operator2Weight;
}

int getOperatorWeight(char operatorChar) {
	int weight = 0;
	switch (operatorChar) {
	case '+':
	case '-':
		weight = 1;
		break;
	case '*':
	case '/':
		weight = 2;
		break;
	case '^':
		weight = 3;
		break;
	}
	return weight;
}

bool isOperand(char testChar) {
	return (testChar >= '0' && testChar <= '9');
}

bool isOperator(char testChar) {
	return (testChar == '+' || testChar == '-' || testChar == '*' || testChar == '/' || testChar == '^');
}

bool isLeftParenthesis(char testChar) {
	return testChar == '(';
}

bool isRightParenthesis(char testChar) {
	return testChar == ')';
}

bool isRightAssociative(char testChar) {
	return testChar == '^';
}

int evaluatePostfix(string expression) {
	stack<int> values;
	stack<char> operators;
	string formattedExpression = removeWhiteSpace(expression);

	for (unsigned int i = 0; i < formattedExpression.length(); i++) {
		if (isOperator(formattedExpression[i])) {
			operators.push(formattedExpression[i]);
		} else if (isOperand(formattedExpression[i])) {
			values.push(convertCharToInt(formattedExpression[i]));
		}
		while (!operators.empty() && values.size() == 2) {
			int operand2 = values.top();
			values.pop();
			int operand1 = values.top();
			values.pop();
			char theOperator = operators.top();
			operators.pop();

			int result = performOperation(theOperator, operand1, operand2);
			values.push(result);
		}
	}
	return values.top();
}

int performOperation(char theOperator, int operand1, int operand2) {
	switch (theOperator) {
	case '+': return operand1 + operand2;
	case '-': return operand1 - operand2;
	case '*': return operand1 * operand2;
	case '/': return operand1 / operand2;
	case '^': return operand1 ^ operand2;
	default: return INT_MAX;
	}
}

int convertCharToInt(char theChar) {
	int operand = theChar - '0';
	return operand;
}

bool isValid(string testString) {
	bool test = false;
	for (unsigned int i = 0; i < testString.length(); i++) {
		if (isOperand(testString[i]) || isOperator(testString[i])) {
			test = true;
		} else {
			return false;
		}
	}
	return test;
}