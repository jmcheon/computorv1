#include "Token.hpp"

void	Token::debugPrint() const
{
	std::cout << "Token(" << sTokenTypeStrings[static_cast<int>(type)] << ", \"" << value << "\", " << token_num << ")" << std::endl;
}

Token	Token::copy() const
{
	Token copied;

	copied.type = type;
    copied.value = value;
    copied.token_num = token_num;

	return copied;
}
