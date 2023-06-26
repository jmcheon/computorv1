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
			std::cout << "Reverse polish format:\n";
			for (Token& token : rpn_tokens)
				token.printRPN();
			std::cout << std::endl << std::endl;

			std::cout << "Term based format:\n";
			tree = parser.buildTree(rpn_tokens);
			printNode(tree.get());
			std::cout << std::endl << std::endl;

			std::cout << "Tree:\n";
			printTree(tree.get());
			std::cout << std::endl << std::endl;

			std::cout << "Equation: ";
			tree->printNormal();
			std::cout << std::endl;

			terms = parser.getTerms(tree.get());
			printReducedFormat(terms);
			printIrreducibleFormat(terms);
			//printTree(tree.get());
			solveEquation(terms);
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception caught: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " [equation]" << std::endl;
		std::cout << "\tExample: " << argv[0] << " \"2*X^2 + 1*X^1 - 3*X^0 = 1*X^2\"" << std::endl;
	}
	return (0);
}
