#include "Parser.hpp"

std::vector<Token> Parser::convertToRPN(const std::vector<Token>& tokens)
{
    std::vector<Token>	output_queue;
    std::stack<Token>	operator_stack;

    for (const Token& token : tokens)
	{
        switch (token.m_type)
		{
            case TokenType::IDENTIFIER:
                output_queue.push_back(token);
                break;
            case TokenType::OPERATOR:
                while (!operator_stack.empty() && hasHigherPrecedence(operator_stack.top(), token))
				{
                    output_queue.push_back(operator_stack.top());
                    operator_stack.pop();
                }
                operator_stack.push(token);
                break;
            default:
				runtimeException("Invalid token type encountered");
        }
    }
    while (!operator_stack.empty())
	{
        if (operator_stack.top().m_type == TokenType::WHITESPACE)
			runtimeException("Invalid equation syntax");
        output_queue.push_back(operator_stack.top());
        operator_stack.pop();
    }
    return output_queue;
}

void	Parser::extractTermTokens(const RPNNode* node, std::vector<Token>& tokens)
{
	Token	token;

    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
	{
    	extractTermTokens(binary_node->getLeft(), tokens);
    	extractTermTokens(binary_node->getRight(), tokens);
		token.m_type = TokenType::OPERATOR;
		token.m_value = node->getValue();
		token.m_token_num = 0;
		tokens.push_back(token);
    }
    if (const IdentifierNode* identifier_node = dynamic_cast<const IdentifierNode*>(node))
	{
		token.m_type = TokenType::IDENTIFIER;
		token.m_value = node->getValue();
		token.m_token_num = 0;
		tokens.push_back(token);
	}
}

bool	Parser::isValidTerm(std::vector<Token>& tokens)
{
	std::vector<Token>::iterator	current_token = tokens.begin();
	std::vector<Token>::iterator	end_token = tokens.end();

	if (current_token->m_value == "+" || current_token->m_value == "-")
		return false;
	if (tokens.size() > 5 || tokens.size() % 2 == 0)
		runtimeException("Invalid term");
	if (tokens.size() == 1 && !isPotentialVariable(current_token->m_value) && !isNumber(current_token->m_value))
		runtimeException("Invalid variable", current_token->m_value);
	if (tokens.size() == 3)
	{
		if ((end_token - 1)->m_value == "^")
		{
			if (isNumber(current_token->m_value))
				runtimeException("Invalid variable 1", current_token->m_value);
			if (!isInteger((current_token + 1)->m_value))
				runtimeException("Invalid exponent 1", (current_token + 1)->m_value);
		}
		else if ((end_token - 1)->m_value == "*")
		{
			if (!isNumber(current_token->m_value))
				runtimeException("Invalid coefficient 2", current_token->m_value);
			if (isNumber((current_token + 1)->m_value))
				runtimeException("Invalid variable 2", (current_token + 1)->m_value);
		}
		else
			runtimeException("Invalid operator", (end_token - 1)->m_value);
	}
	if (tokens.size() == 5)
	{
		if ((end_token - 1)->m_value != "*")
			runtimeException("Invalid order");
		if (!isNumber(current_token->m_value))
			runtimeException("Invalid coefficient 3", current_token->m_value);
		if (isNumber((current_token + 1)->m_value))
			runtimeException("Invalid variable 3", (current_token + 1)->m_value);
		if (!isInteger((current_token + 2)->m_value))
			runtimeException("Invalid exponent 3", (current_token + 2)->m_value);
	}
	return true;
}


std::string	Parser::extractTerm(std::unique_ptr<RPNNode>& node, std::vector<Token>& tokens, bool is_neg)
{
    std::unique_ptr<TermNode>		term = std::make_unique<TermNode>();
	std::vector<Token>::iterator	current_token = tokens.begin();
	std::vector<Token>::iterator	end_token = tokens.end();
	std::string						variable;

	if (!isValidTerm(tokens))
		return variable;
	if (tokens.size() == 1)
	{
		if (isNumber(current_token->m_value))
		{
			term->setCoefficient(current_token->m_value);
			term->setExponent("0");
		}
		else
		{
			if (current_token->m_value[0] == '-')
			{
				term->setCoefficient(std::string("-1", 2));
				variable = current_token->m_value[1];
				term->setVariable(variable);
			}
			else
			{
				variable = current_token->m_value;
				term->setVariable(variable);
			}
		}
	}
	if (tokens.size() == 3)
	{
		if ((end_token - 1)->m_value == "^")
		{
			if (current_token->m_value[0] == '-')
			{
				term->setCoefficient(std::string("-1", 2));
				variable = current_token->m_value[1];
				term->setVariable(variable);
			}
			else
			{
				variable = current_token->m_value;
				term->setVariable(variable);
			}
			term->setExponent((current_token + 1)->m_value);
		}
		else if ((end_token - 1)->m_value == "*")
		{
			term->setCoefficient(current_token->m_value);
			variable = (current_token + 1)->m_value;
			term->setVariable(variable);
		}
	}
	if (tokens.size() == 5)
	{
		term->setCoefficient(current_token->m_value);
		variable = (current_token + 1)->m_value;
		term->setVariable(variable);
		term->setExponent((current_token + 2)->m_value);
	}
	if (is_neg)
		term->setCoefficient("-" + term->getCoefficient());
	node = std::move(term);
	return variable;
}

void	Parser::extractTermFromStack(std::unique_ptr<RPNNode>& node, std::string* variable, bool is_neg)
{
	std::vector<Token>	tokens;
	std::string			current_variable;

	tokens.clear();
	extractTermTokens(node.get(), tokens);
	//printTokens(tokens);
	current_variable = extractTerm(node, tokens, is_neg);
	if (*variable != current_variable)
	{
		if (variable->empty())
			*variable = current_variable;
		else if (!variable->empty() && !current_variable.empty())
			runtimeException("Invalid variable", current_variable);
	}
}

std::unique_ptr<RPNNode>	Parser::buildTree(std::vector<Token>& rpn_tokens)
{
	std::stack<std::unique_ptr<RPNNode> > stack;
	std::string			variable;
	bool				valid_equation = false;

	for (Token& token : rpn_tokens)
	{
		//token.debugPrint();
		if (token.m_type == TokenType::IDENTIFIER)
		{
			if (!isNumber(token.m_value) && !isPotentialVariable(token.m_value))
				runtimeException("Invalid identifier", token.m_value);
			stack.push(std::make_unique<IdentifierNode>(token.m_value));
		}
		else if (token.m_type == TokenType::OPERATOR)
		{
			if (stack.size() < 2)
				runtimeException("Invalid equation: Not enough operands");

			if (token.m_value == "-")
			{
				std::unique_ptr<RPNNode> right = std::move(stack.top());
				stack.pop();
				extractTermFromStack(right, &variable, true);
				token.m_value = "+";

				std::unique_ptr<RPNNode> left = std::move(stack.top());
				stack.pop();
				extractTermFromStack(left, &variable);

				stack.push(std::move(right));
				stack.push(std::move(left));
			}
			else if (token.m_value == "+" || token.m_value == "=")
			{
				std::unique_ptr<RPNNode> right = std::move(stack.top());
				stack.pop();
				extractTermFromStack(right, &variable);

				std::unique_ptr<RPNNode> left = std::move(stack.top());
				stack.pop();
				extractTermFromStack(left, &variable);

				stack.push(std::move(right));
				stack.push(std::move(left));
			}
			std::unique_ptr<RPNNode> right = std::move(stack.top());
			stack.pop();
			std::unique_ptr<RPNNode> left = std::move(stack.top());
			stack.pop();
			std::unique_ptr<RPNNode> parent = std::make_unique<BinaryOperatorNode>(token.m_value, std::move(left), std::move(right));
			stack.push(std::move(parent));
			if (token.m_value == "=")
			{
				if (!valid_equation)
					valid_equation = true;
				else
					runtimeException("Invalid syntax: Multiple equations found");
			}
		}
		else
			runtimeException("Invalid equation: Unexpected symbol", token.m_value);
	}
	if (stack.size() != 1)
		runtimeException("Invalid equation: Too many operands");
	if (!valid_equation)
		runtimeException("Invalid syntax: equation not found");
	return std::move(stack.top());
}

void	Parser::getTermNodes(RPNNode* node, std::vector<TermNode>& terms, bool equal_sign)
{
	if (TermNode* term_node = dynamic_cast<TermNode*>(node))
	{
		//term_node->print();
		//std::cout << ", " << equal_sign << std::endl;
		if (equal_sign == true)
		{
			if (term_node->getCoefficient()[0] != '-')
				term_node->setCoefficient("-" + term_node->getCoefficient());
			else
				term_node->setCoefficient(term_node->getCoefficient().substr(1, term_node->getCoefficient().length()));
		}
		terms.push_back(*term_node);
	}
    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
    {
		if (binary_node->getOperator() == "=")
		{

			equal_sign = true;
        	getTermNodes(binary_node->getLeft(), terms, equal_sign);
			equal_sign = false;
			//std::cout << "(All nodes to the right of the equals sign)" << std::endl;
        	getTermNodes(binary_node->getRight(), terms, equal_sign);
		}
		else
		{
        	getTermNodes(binary_node->getLeft(), terms, equal_sign);
        	getTermNodes(binary_node->getRight(), terms, equal_sign);
		}
    }
}

std::vector<TermNode>	Parser::getTerms(RPNNode* node)
{
	std::vector<TermNode> terms;
	std::vector<TermNode>::iterator	 current_term;
	std::vector<TermNode>::iterator	 end_term;
	double	coef = 0.0;

	getTermNodes(node, terms, false);
	current_term = terms.begin();
	end_term = terms.end();
	//printTerms(terms);

	for (; current_term != end_term; ++current_term)
		current_term->setCoefficient(std::to_string(reduce(terms, current_term->getExponent())));
	//printTerms(terms);
	for (auto it = terms.begin(); it != terms.end(); ++it)
	{
		if (std::stod(it->getCoefficient()) == 0)
		{
                it = terms.erase(it);
				--it;
		}
	}
	return terms;
}
