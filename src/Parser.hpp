#ifndef PARSER_HPP
#define PARSER_HPP
#include <stack>
#include <regex>
#include "Lexer.hpp"
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
	public:
		std::vector<Token>			convertToRPN(const std::vector<Token>& tokens);
		std::unique_ptr<RPNNode>	buildTree(std::vector<Token>& rpn_tokens);
	private:
		void			extractTermFromStack(std::unique_ptr<RPNNode>& node, std::string* variable, bool is_neg = false);
		void			extractTermTokens(const RPNNode* node, std::vector<Token>& tokens);
		std::string		extractTerm(std::unique_ptr<RPNNode>& node, std::vector<Token>& tokens, bool is_neg);
		bool			isValidTerm(std::vector<Token>& tokens);
};

std::vector<TermNode>	getTerms(RPNNode* node);
void		reduceRightHandSide(RPNNode* node);
double		calculateSquareRoot(double x, double precision);
void		solveEquation(std::vector<TermNode> terms);
void		solveEquation2(const RPNNode* root);
#endif
