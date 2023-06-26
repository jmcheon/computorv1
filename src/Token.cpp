#include "Token.hpp"

const char* sTokenTypeStrings[] = {
	"WHITESPACE",
	"OPERATOR",
	"IDENTIFIER",
};

void	Token::debugPrint() const
{
	std::cout << "Token(" << sTokenTypeStrings[static_cast<int>(m_type)];
	std::cout << ", \"" << m_value << "\", " << m_token_num << ")" << std::endl;
}
void	Token::printRPN() const
{
	std::cout << m_value;
}

void	printTokens(std::vector<Token>& tokens)
{
	std::cout << "\t\t <Print tokenized tokens>\n";
	std::cout << "example: Token(token type, text, token number)\n\n";
	for (auto it = tokens.begin(); it != tokens.end() ; ++it)
		it->debugPrint();
	std::cout << std::endl;
}
