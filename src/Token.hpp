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
	"IDENTIFIER",
	"VARIABLE",
	"COEFFICIENT",
	"EXPONENT"
};

class Token
{
	public:
		TokenType 	type;
		std::string	value;
		size_t		token_num;

	public:
		void		debugPrint() const;
		Token		copy() const;
};
#endif
