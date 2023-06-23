#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <sstream>
#include <utility>
#include <memory>
#include <map>
#include <stack>
#include <cstdlib>
#include "Lexer.hpp"
#include "Token.hpp"
#include "RPNtree.hpp"

class Parser
{
	private:
		std::vector<Token>::iterator	m_current_token;
		std::vector<Token>::iterator	m_start_token;
		std::vector<Token>::iterator	m_end_token;
		size_t							m_max_token_num;

	public:
		//Parser();
		std::vector<Term>		parse(std::vector<Token>& tokens);
		std::vector<Token>		convertToRPN(const std::vector<Token>& tokens);
		std::unique_ptr<RPNNode>	buildTree(const std::vector<Token>& rpn_tokens);
	private:
		void					printCurrentTokenInfo(std::string func_name);
		bool					checkValidSyntax(const std::vector<Token>& tokens);

		// Empty string means match any string.
		Token					expectToken(TokenType type, const std::string& name = std::string());
		bool					expectToken(Token& token, TokenType type);
		std::vector<Term> 		extractTerms(const std::vector<Token>& tokens);
		std::vector<Term> 		extract(const std::vector<Token>& tokens);
};

double		calculateSquareRoot(double x, double precision);
void		soloveEquation(const std::vector<Term>& terms);
#endif
