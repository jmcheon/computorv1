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
		size_t		m_sign;
		std::string	m_variable;
		double		m_coefficient;
		size_t		m_exponent;
	public:
		Term() : m_sign(1), m_coefficient(1.0), m_exponent(0) {};

		size_t		getSign() const { return m_sign; }
		std::string getVariable() const { return m_variable; }
		double		getCoefficient() const { return m_coefficient; }
		size_t		getExponent() const { return m_exponent; }

		void		setSign(const std::string& sign) {
				if (sign == "+")
					m_sign = 1; 
				else if (sign == "-")
					m_sign = 0;
			}
		void 		setVariable(const std::string& variable) { m_variable = variable; }
		void		setCoefficient(const std::string& coef) { m_coefficient = std::stod(coef); }
		void		setExponent(const std::string& exponent) { m_exponent = std::stoi(exponent); }
		void		debugPrint() const;
};

class Operator : public Term
{
	private:
		std::string					m_operator;
	public:
		Operator(std::string op) : m_operator(op) {}

		std::string		getOperator() const { return m_operator; }
		void			print() const { std::cout << m_operator; }
		void			traverse() const { std::cout << m_operator; }
		void			debugPrint() const;
};
#endif
