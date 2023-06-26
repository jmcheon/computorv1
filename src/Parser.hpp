#ifndef PARSER_HPP
#define PARSER_HPP
#include <stack>
#include "Lexer.hpp"
#include "RPNtree.hpp"
#include "utils.hpp"
#include "computorv.hpp"

class Parser
{
	public:
		std::vector<Token>			convertToRPN(const std::vector<Token>& tokens);
		std::unique_ptr<RPNNode>	buildTree(std::vector<Token>& rpn_tokens);
		std::vector<TermNode>		getTerms(RPNNode* node);
	private:
		void			extractTermFromStack(std::unique_ptr<RPNNode>& node, std::string* variable, bool is_neg = false);
		void			extractTermTokens(const RPNNode* node, std::vector<Token>& tokens);
		std::string		extractTerm(std::unique_ptr<RPNNode>& node, std::vector<Token>& tokens, bool is_neg);
		bool			isValidTerm(std::vector<Token>& tokens);

		void			getTermNodes(RPNNode* node, std::vector<TermNode>& terms, bool equal_sign);
};

void		solveEquation(std::vector<TermNode> terms);
#endif
