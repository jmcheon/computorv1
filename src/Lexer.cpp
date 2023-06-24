#include "Lexer.hpp"

std::vector<Token>	Lexer::tokenize2(const std::string& texts)
{
	std::vector<Token>			tokens;
	Token						current_token;
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
			case '*':
			case '=':
			case '^':
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
	//modifyIdentifierToken(tokens);
	return (tokens);
}

std::vector<Token>	Lexer::tokenize(const std::string& texts)
{
	std::vector<Token>			tokens;
	Token						current_token;
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
				endToken(current_token, tokens);
				current_token.m_type = TokenType::PLUS;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			case '-':
				endToken(current_token, tokens);
				current_token.m_type = TokenType::MINUS;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			case '*':
				endToken(current_token, tokens);
				current_token.m_type = TokenType::MULTIPLICATION;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			case '=':
				endToken(current_token, tokens);
				current_token.m_type = TokenType::EQUATION;
				current_token.m_value.append(1, *it);
				endToken(current_token, tokens);
				break ;
			case '^':
				endToken(current_token, tokens);
				current_token.m_type = TokenType::DEGREE;
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
	modifyIdentifierToken(tokens);
	return (tokens);
}

void	Lexer::endToken(Token& token, std::vector<Token>& tokens)
{
	if (token.m_type != TokenType::WHITESPACE)
	{
		if (token.m_type == TokenType::IDENTIFIER && !token.m_value.empty() && std::isdigit(token.m_value[0]))
		{
			//token.debugPrint();
			// treat the token as the exponent of the previous variable token
			Token prev_token = tokens.empty() ? Token() : tokens.back();
			//prev_token.debugPrint();
			if (prev_token.m_type == TokenType::DEGREE)
				token.m_type = TokenType::EXPONENT;
		}
		token.m_token_num += 1;
		tokens.push_back(token);
	}
	token.m_type = TokenType::WHITESPACE;
	token.m_value.erase();
}

void	Lexer::modifyIdentifierToken(std::vector<Token>& tokens)
{
	m_current_token = tokens.begin();
	m_end_token = tokens.end();
	
	for (; m_current_token != m_end_token; ++m_current_token)
	{
		if (m_current_token->m_type == TokenType::IDENTIFIER)
		{
			//(*(m_current_token + 1)).debugPrint();
			if ((*(m_current_token + 1)).m_type == TokenType::MULTIPLICATION)
				m_current_token->m_type = TokenType::COEFFICIENT;
			else if ((*(m_current_token - 1)).m_type == TokenType::MULTIPLICATION
					&& (*(m_current_token + 1)).m_type == TokenType::DEGREE)
				m_current_token->m_type = TokenType::VARIABLE;
		}
	}
}
