#include "Parser.hpp"

int main(int argc, char** argv)
{
	Lexer	lexer;
	Parser	parser;
	std::vector<TermNode> terms;
	std::vector<Token> tokens;
	std::vector<Token> rpn_tokens;
	std::unique_ptr<RPNNode> tree;

	if (argc == 2)
	{
		tokens = lexer.tokenize(argv[1]);
		try
		{
			rpn_tokens = parser.convertToRPN(tokens);
			tree = parser.buildTree(rpn_tokens);
			terms = parser.getTerms(tree.get());
			//printTree(tree.get());
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception caught: " << e.what() << std::endl;
			return (1);
		}
		printReducedFormat(terms);
		solveEquation(terms);
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " [equation]" << std::endl;
		std::cout << "\tExample: " << argv[0] << " \"2*X^2 + 1*X^1 - 3*X^0 = 1*X^2\"" << std::endl;
	}
	return (0);
}
