#ifndef TERM_HPP
#define TERM_HPP
#include <iostream>
#include <stack>
#include <cstdlib>
#include "Lexer.hpp"
#include "Token.hpp"

class Term
{
	private:
		std::string	m_variable;
		double		m_coefficient;
		size_t		m_exponent;
	public:
		Term() : m_coefficient(1.0), m_exponent(0) {};

		std::string getVariable() const { return m_variable; }
		double		getCoefficient() const { return m_coefficient; }
		size_t		getExponent() const { return m_exponent; }
		void 		setVariable(const std::string& variable) { m_variable = variable; }
		void		setCoefficient(const std::string& coef) { m_coefficient = std::stod(coef); }
		void		setExponent(const std::string& exponent) { m_exponent = std::stoi(exponent); }
		void		debugPrint() const;
};
#endif
