#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <iostream>
#include <string>
#include <vector>

enum class TokenType
{
	WHITESPACE,
	OPERATOR,
	IDENTIFIER,
};

static const char* sTokenTypeStrings[] = {
	"WHITESPACE",
	"OPERATOR",
	"IDENTIFIER",
};

class Token
{
	public:
		TokenType 	m_type;
		std::string	m_value;
		size_t		m_token_num;

	public:
		void		debugPrint() const;
		void		printRPN() const;
};
#endif
