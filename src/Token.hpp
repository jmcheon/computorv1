#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <iostream>
#include <string>
#include <vector>

enum class TokenType
{
	WHITESPACE,
	PLUS, // +
	MINUS, // -
	MULTIPLICATION, // *
	EQUATION, // =
	DEGREE, // ^
	OPERATOR,
	IDENTIFIER,
	VARIABLE,
	COEFFICIENT,
	EXPONENT
};

static const char* sTokenTypeStrings[] = {
	"WHITESPACE",
	"PLUS", // +
	"MINUS", // -
	"MULTIPLICATION", // *
	"EQUATION", // =
	"DEGREE", // ^
	"OPERATOR",
	"IDENTIFIER",
	"VARIABLE",
	"COEFFICIENT",
	"EXPONENT"
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
		Token		copy() const;
};
#endif
