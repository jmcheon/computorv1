#include "Token.hpp"

void	Token::debugPrint() const
{
	std::cout << "Token(" << sTokenTypeStrings[static_cast<int>(m_type)];
	std::cout << ", \"" << m_value << "\", " << m_token_num << ")" << std::endl;
}
void	Token::printRPN() const
{
	std::cout << m_value;
}
