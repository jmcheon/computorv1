#include "Lexer.hpp"

std::vector<Token>	Lexer::tokenize(const std::string& texts)
{
	std::vector<Token>			tokens;
	Token						current_token;
	bool						is_equation = false;
	std::string::const_iterator	it;
	
	current_token.m_type = TokenType::WHITESPACE;
	current_token.m_token_num = 0;
	for (it = texts.begin(); it != texts.end(); ++it)
	{
		switch (*it)
		{
			case ' ':
			case '\t':
			case '\n':
				endToken(current_token, tokens);
				break ;
			case '+':
			case '-':
				if (current_token.m_token_num == 0 || is_equation) 
				{
					is_equation = false;
					endToken(current_token, tokens);
					current_token.m_type = TokenType::IDENTIFIER;
					current_token.m_value.append(1, *it);
				}
				else
				{
					endToken(current_token, tokens);
					current_token.m_type = TokenType::OPERATOR;
					current_token.m_value.append(1, *it);
					endToken(current_token, tokens);
				}
				break ;
			case '*':
			case '^':
				endToken(current_token, tokens);
				current_token.m_type = TokenType::OPERATOR;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			case '=':
				is_equation = true;
				endToken(current_token, tokens);
				current_token.m_type = TokenType::OPERATOR;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			default:
				if (current_token.m_type == TokenType::WHITESPACE)
				{
					endToken(current_token, tokens);
					current_token.m_type = TokenType::IDENTIFIER;
					current_token.m_value.append(1, *it);
				}
				else 
					current_token.m_value.append(1, *it);
				break ;
		}
	}
	endToken(current_token, tokens);
	return (tokens);
}

void	Lexer::endToken(Token& token, std::vector<Token>& tokens)
{
	if (token.m_type != TokenType::WHITESPACE)
	{
		token.m_token_num += 1;
		tokens.push_back(token);
	}
	token.m_type = TokenType::WHITESPACE;
	token.m_value.erase();
}
