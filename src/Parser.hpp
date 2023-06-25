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
		void						parse(const RPNNode* node);
		std::vector<Token>			convertToRPN(const std::vector<Token>& tokens);
		std::unique_ptr<RPNNode>	buildTree(const std::vector<Token>& rpn_tokens);
	private:
		void			extractTermFromStack(std::unique_ptr<RPNNode>& node, std::string* variable);
		void			extractTermTokens(const RPNNode* node, std::vector<Token>& tokens);
		std::string		extractTerm(std::unique_ptr<RPNNode>& node, std::vector<Token>& tokens);
		bool			isValidTerm(std::vector<Token>& tokens);
};

double		calculateSquareRoot(double x, double precision);
//void		soloveEquation(const std::vector<Term>& terms);
#endif
