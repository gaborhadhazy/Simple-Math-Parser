#include <iostream>
#include <cctype>
#include <stdexcept>
#include <memory>

class Parser {
public:
    Parser(const std::string& expression) : expression(expression), index(0) {}

    double parse() {
        return parseExpression();
    }

private:
    double parseExpression() {
        double term = parseTerm();

        while (index < expression.length() && (expression[index] == '+' || expression[index] == '-')) {
            char op = expression[index++];
            double nextTerm = parseTerm();
            term = applyOperator(op, term, nextTerm);
        }

        return term;
    }

    double parseTerm() {
        double factor = parseFactor();

        while (index < expression.length() && (expression[index] == '*' || expression[index] == '/')) {
            char op = expression[index++];
            double nextFactor = parseFactor();
            factor = applyOperator(op, factor, nextFactor);
        }

        return factor;
    }

    double parseFactor() {
        if (index < expression.length() && expression[index] == '(') {
            index++;
            double result = parseExpression();
            if (index >= expression.length() || expression[index] != ')') {
                throw std::invalid_argument("Mismatched parentheses");
            }
            index++;
            return result;
        } else {
            size_t start = index;
            while (index < expression.length() && (isdigit(expression[index]) || expression[index] == '.')) {
                index++;
            }
            if (start == index) {
                throw std::invalid_argument("Invalid character in expression");
            }
            return stod(expression.substr(start, index - start));
        }
    }

    double applyOperator(char op, double left, double right) {
        switch (op) {
            case '+':
                return left + right;
            case '-':
                return left - right;
            case '*':
                return left * right;
            case '/':
                if (right == 0) {
                    throw std::invalid_argument("Division by zero");
                }
                return left / right;
            default:
                throw std::invalid_argument("Invalid operator");
        }
    }

    std::string expression;
    size_t index;
};

int main() {
    std::string input; // input buffer
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    try {
        Parser parser(input);
        double result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
