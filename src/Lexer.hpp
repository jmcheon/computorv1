#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <string>
#include <vector>
#include "Token.hpp"

class Lexer 
{
	public:
		std::vector<Token>	 			tokenize(const std::string& texts);
	private:
		void			 				endToken(Token& token, std::vector<Token>& tokens);
};
#endif
