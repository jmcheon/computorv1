#include "utils.hpp"

void	runtimeException(const std::string& str, const std::string& value)
{
	std::stringstream	error_message;
	if (!value.empty())
		error_message << RED << str << " '" << value << "'" << FIN;
	else
		error_message << RED << str << FIN;
	throw std::runtime_error(error_message.str());
}

bool	isNumber(const std::string& str)
{
	// Regular expression pattern for matching integer or double numbers, fraction, sign
	std::regex pattern("^[-+]?\\d+(\\.\\d+)?(\\/([1-9]\\d*)+)?$");
	// Match the pattern against the input string
	return std::regex_match(str, pattern);
}

bool		isInteger(const std::string& str)
{
	std::regex pattern("^\\d+?$");
	return std::regex_match(str, pattern);
}

bool		isDouble(const std::string& str)
{
	std::regex pattern("^(\\.\\d+)?$");
	return std::regex_match(str, pattern);
}

bool	isPotentialVariable(const std::string& str)
{
	char	variable;
	if (str.length() > 2)
		return false;
	if (str.length() == 1)
		variable = str[0];
	else
	{
		if (str[0] == '-' || str[0] == '+')
			variable = str[1];
		else
			return false;
	}
	return std::isalpha(variable);
}

int getOperatorPrecedence(const Token& token)
{
    if (token.m_value == "*" || token.m_value == "/" || token.m_value == "^")
		return 2;
    else if (token.m_value == "+" || token.m_value == "-")
		return 1;
	return 0;
}

bool hasHigherPrecedence(const Token& op1, const Token& op2)
{
    int precedence_op1 = getOperatorPrecedence(op1);
    int precedence_op2 = getOperatorPrecedence(op2);
    return (precedence_op1 > precedence_op2);
}

double calculateSquareRoot(double x, double precision)
{
	double low = 0.0;
	double high = x;
	double mid = (low + high) / 2.0;
	double diff = mid * mid - x;

	while (diff * diff > precision * precision)
	{
		if (diff > 0.0)
			high = mid;
		else
			low = mid;
		mid = (low + high) / 2.0;
		diff = mid * mid - x;
    }
    return mid;
};

// greatest common divisor (GCD)
int calculateGCD(int a, int b)
{
    if (b == 0)
        return a;
    return calculateGCD(b, a % b);
}

void convertToIrreducibleFraction(double number)
{
    // Convert the decimal number to a fraction by multiplying it by a power of 10
    int numerator = static_cast<int>(number * 100000);
    int denominator = 100000;

    int gcd = calculateGCD(numerator, denominator);

    numerator /= gcd;
    denominator /= gcd;
	if (denominator < 0)
	{
		denominator *= -1;
		numerator *= -1;
	}
	if (denominator == 1)
    	std::cout << numerator;
	else
    	std::cout << numerator << "/" << denominator;
}

double	extractCoefficient(std::string str)
{
	size_t	pos = str.find('/');

	if (pos == std::string::npos)
		return std::stod(str);
	int numerator = std::stoi(str.substr(0, pos));
	int denominator = std::stoi(str.substr(pos + 1));
	return static_cast<double>(numerator) / denominator;
}

