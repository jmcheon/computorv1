#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <sstream>
#include <utility>
#include <memory>
#include <map>
#include <stack>
#include <cstdlib>
#include <regex>
#include "Lexer.hpp"
#include "Token.hpp"
#include "Term.hpp"
#include "RPNtree.hpp"

# define YELLOW "\033[0;38;5;220m"
# define GREEN "\033[0;38;5;42m"
# define RED "\033[0;38;5;196m"
# define BLUE "\033[0;38;5;21m"
# define PINK "\033[0;38;5;205m"
# define PURPLE "\033[0;38;5;93m"
# define ORANGE "\033[38;5;202m"
# define FIN "\033[0m"
class Parser
{
	private:
		std::vector<Token>::iterator	m_current_token;
		std::vector<Token>::iterator	m_op_token;
		std::vector<Token>::iterator	m_end_token;

	public:
		//Parser();
		std::vector<Term>		parse(std::vector<Token>& tokens);
		std::vector<Token>		convertToRPN(const std::vector<Token>& tokens);
		std::unique_ptr<RPNNode>	buildTree(const std::vector<Token>& rpn_tokens);
	private:
		void					expectOperator(std::vector<Token>::iterator current_token);
		Token					expectToken(TokenType type, const std::string& name = std::string());
		bool					expectToken(Token& token, TokenType type);
		void					extractTerm(std::vector<Term>& terms);
		bool					isValidTerm(std::vector<Token>::iterator current_token);
};

double		calculateSquareRoot(double x, double precision);
void		soloveEquation(const std::vector<Term>& terms);
#endif
