#include "Term.hpp"

void	Term::debugPrint() const
{
	std::cout << "Term(" << m_coefficient << " * " << m_variable;
	std::cout << " ^ " << m_exponent << ")" << std::endl;
}

