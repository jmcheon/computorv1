#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <string>
#include <vector>
#include "Token.hpp"

class Lexer 
{
	private:
		std::vector<Token>::iterator	m_current_token;
		std::vector<Token>::iterator	m_end_token;
	public:
		std::vector<Token>	 			tokenize(const std::string& texts);
	private:
		void			 				endToken(Token& token, std::vector<Token>& tokens);
};
#endif
