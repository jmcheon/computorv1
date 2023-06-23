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
		TokenType 	type;
		std::string	value;
		size_t		token_num;
		//bool		is_error;

	public:
		void		debugPrint() const;
		void		printRPN() const;
		Token		copy() const;
};
#endif
