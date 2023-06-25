#include <iostream>
#include <string>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"


void	printTokens(std::vector<Token>& tokens)
{
	std::cout << "\t\t <Print tokenized tokens>\n";
	std::cout << "example: Token(token type, text, token number)\n\n";
	std::vector<Token>::iterator	current_token_iter = tokens.begin();
	std::vector<Token>::iterator	end_token_iter = tokens.end();
	for (;current_token_iter != end_token_iter; ++current_token_iter)
		current_token_iter->debugPrint();
}

int main(int argc, char** argv)
{
	Lexer	lexer;
	Parser	parser;
	std::vector<Token> tokens;
	std::vector<Token> rpn_tokens;
	std::unique_ptr<RPNNode> tree;

	if (argc == 2)
	{
		tokens = lexer.tokenize(argv[1]);
		try
		{
			rpn_tokens = parser.convertToRPN(tokens);
			for (Token& token : rpn_tokens)
				token.printRPN();
			std::cout << std::endl;
			tree = parser.buildTree(rpn_tokens);
			//printNode(tree.get());
			printTree(tree.get());
			//parser.parse(tree.get());
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception caught: " << e.what() << std::endl;
		}
		//printTokens(tokens);
		//solveEquation(terms);
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " [equation]" << std::endl;
		std::cout << "\tExample: " << argv[0] << " \"2*X^2 + 1*X^1 - 3*X^0 = 1*X^2\"" << std::endl;
	}
	return (0);
}
