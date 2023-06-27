#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <regex>
#include "Token.hpp"

# define YELLOW "\033[0;38;5;220m"
# define GREEN "\033[0;38;5;42m"
# define RED "\033[0;38;5;196m"
# define BLUE "\033[0;38;5;21m"
# define PINK "\033[0;38;5;205m"
# define PURPLE "\033[0;38;5;93m"
# define ORANGE "\033[38;5;202m"
# define FIN "\033[0m"

void	runtimeException(const std::string& str, const std::string& value = std::string());

bool	isNumber(const std::string& str);
bool	isInteger(const std::string& str);
bool	isDouble(const std::string& str);
bool	isPotentialVariable(const std::string& str);

int		getOperatorPrecedence(const Token& token);
bool 	hasHigherPrecedence(const Token& op1, const Token& op2);

double	calculateSquareRoot(double x, double precision);
int		calculateGCD(int a, int b);
void 	convertToIrreducibleFraction(double number, bool first_term = false);
double	extractCoefficient(std::string str);

#endif
