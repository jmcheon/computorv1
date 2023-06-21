#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <cstdlib>
#include "Lexer.hpp"
#include "Token.hpp"

struct Term
{
	double	coefficient;
	int		exponent;
};

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

void	soloveEquation(const std::vector<Term>& terms)
{
	int	degree = 0;
	for (const Term& term : terms)
	{
		if (term.exponent > degree)
			degree = term.exponent;
	}
	std::cout << "Polynomial degree: " << degree << std::endl;

	if (degree > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl; 
		return ;
	}

	if (degree == 0)
	{
		double constant_term = 0.0;

		for (const Term& term : terms)
		{
			if (term.exponent == 0)
			{
				constant_term = term.coefficient;
				break ;
			}
		}
		if (constant_term != 0.0)
			std::cout << "no solution" << std::endl;
		else
			std::cout << "infinte number of soluntions" << std::endl;
	}
	else if (degree == 1)
	{
		double constant_term = 0.0;
		double coef = 0.0;
		double solution;

		for (const Term& term : terms)
		{
			if (term.exponent == 0)
				constant_term = term.coefficient;
			else if (term.exponent == 1)
				coef = term.coefficient;
		}
		solution = -constant_term / coef;
		std::cout << "The solution is:\n" << solution << std::endl;
	}
	else if (degree == 2)
	{
		double constant_term = 0.0;
		double coef = 0.0;
		double coef_squre = 0.0;
		double discriminant;
		for (const Term& term : terms)
		{
			if (term.exponent == 0)
				constant_term = term.coefficient;
			else if (term.exponent == 1)
				coef = term.coefficient;
			else if (term.exponent == 2)
				coef_squre = term.coefficient;
		}
		// ax^2 + bx + c = 0, the discriminant is calculated as b^2 - 4ac
		discriminant = coef * coef - 4 * coef_squre * constant_term;
		if (discriminant < 0.0)
			std::cout << "Discriminant is negative, no real solution." << std::endl;
		else if (discriminant == 0.0)
		{
			double solution = -coef / (2 * coef_squre);
			std::cout << "Discriminant is zero, the one solutions is:" << std::endl;;
			std::cout << solution << std::endl;
		}
		else
		{
			double sqrt_discriminant = calculateSquareRoot(discriminant, 0.001);
			double solution1 = -coef / (2 * coef_squre);
			double solution2 = -coef / (2 * coef_squre);

			std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;;
			std::cout << solution1 << std::endl;
			std::cout << solution2 << std::endl;
		}
	}
};

class Parser
{
	private:
		std::vector<Token>::iterator	current_token_;
		std::vector<Token>::iterator	start_token_;
		std::vector<Token>::iterator	end_token_;
		size_t							max_token_num_;

	public:
		Parser();
		std::vector<Term>		parse(std::vector<Token>& tokens);
	private:
		void					printCurrentTokenInfo(std::string func_name);

		// Empty string means match any string.
		std::pair<bool, Token>	expectToken(enum TokenType type, const std::string& name = std::string());

};

#endif
