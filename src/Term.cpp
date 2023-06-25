#include "Term.hpp"

void	Term::debugPrint() const
{
	std::cout << "Term(" << (m_sign ? "+" : "-") << m_coefficient << " * " << m_variable;
	std::cout << " ^ " << m_exponent << ")" << std::endl;
}

void	Operator::debugPrint() const
{
	std::cout << "Operator(" << m_operator << std::endl;
}
