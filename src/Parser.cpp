#include "Parser.hpp"

void	runtimeException(const std::string& str, const std::string& value = std::string())
{
	std::stringstream	error_message;
	if (!value.empty())
		error_message << RED << str << " '" << value << "'" << FIN;
	else
		error_message << RED << str << FIN;
	throw std::runtime_error(error_message.str());
}

static bool	isNumber(const std::string& str)
{
	// Regular expression pattern for matching integer or double numbers, fraction, sign
	std::regex pattern("^[-+]?\\d+(\\.\\d+)?(\\/([1-9]\\d*)+)?$");
	// Match the pattern against the input string
	return std::regex_match(str, pattern);
}

static bool		isInteger(const std::string& str)
{
	std::regex pattern("^\\d+?$");
	return std::regex_match(str, pattern);
}

static bool		isDouble(const std::string& str)
{
	std::regex pattern("^(\\.\\d+)?$");
	return std::regex_match(str, pattern);
}

static bool	isPotentialVariable(const std::string& str)
{
	char	variable;
	if (str.length() > 2)
		return false;
	if (str.length() == 1)
		variable = str[0];
	else
	{
		if (str[0] == '-' || str[0] == '+')
			variable = str[1];
		else
			return false;
	}
	return std::isalpha(variable);
}

static int getOperatorPrecedence(const Token& token)
{
    if (token.m_value == "*" || token.m_value == "/" || token.m_value == "^")
		return 2;
    else if (token.m_value == "+" || token.m_value == "-")
		return 1;
	return 0;
}

static bool hasHigherPrecedence(const Token& op1, const Token& op2)
{
    int precedence_op1 = getOperatorPrecedence(op1);
    int precedence_op2 = getOperatorPrecedence(op2);
    return (precedence_op1 > precedence_op2);
}

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
	//std::cout << std::endl;
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
	//for (auto& token : tokens)
		//token.debugPrint();
	//std::cout << std::endl;
	current_variable = extractTerm(node, tokens, is_neg);

	//std::cout << "current var:" << current_variable << std::endl;

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
			stack.push(std::make_unique<BinaryOperatorNode>(token.m_value, std::move(left), std::move(right)));
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
	{
		//std::cout << stack.size() << std::endl;
		runtimeException("Invalid equation: Too many operands");
	}
	if (!valid_equation)
		runtimeException("Invalid syntax: equation not found");
	return std::move(stack.top());
}

void		Parser::parse(const RPNNode* node)
{
	std::stack<RPNNode> stack;
	//stack.push(std::make_unique<IdentifierNode>(token.m_value));

/*
	//printNode(static_cast<const RPNNode *>(&tree[0]));
    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
	{
    	parse(binary_node->getLeft());
    	parse(binary_node->getRight());
		if (binary_node->getOperator() == "-" || binary_node->getOperator() == "+" || binary_node->getOperator() == "=")
			node->traverse();
    }
    if (const IdentifierNode* identifier_node = dynamic_cast<const IdentifierNode*>(node))
	{
		node->traverse();
	}
	//node->traverse();
	std::cout << std::endl;
	*/
}

double calculateSquareRoot(double x, double precision)
{
	double low = 0.0;
	double high = x;
	double mid = (low + high) / 2.0;
	double diff = mid * mid - x;

	while (diff * diff > precision * precision)
	{
		if (diff > 0.0)
			high = mid;
		else
			low = mid;
		mid = (low + high) / 2.0;
		diff = mid * mid - x;
    }
    return mid;
};

void getMaxDegree(const RPNNode* node, size_t& degree)
{
    if (const TermNode* term_node = dynamic_cast<const TermNode*>(node))
    {
        if (term_node->getExponent() > degree)
            degree = term_node->getExponent();
    }
    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
    {
        getMaxDegree(binary_node->getLeft(), degree);
        getMaxDegree(binary_node->getRight(), degree);
    }
}

void getCoefficients(const RPNNode* node, double& constant_term, double& coef, double& coef_square)
{
	if (const TermNode* term_node = dynamic_cast<const TermNode*>(node))
	{
		std::cout << term_node->getExponent() << std::endl;
		if (term_node->getExponent() == 0)
			constant_term = std::stod(term_node->getCoefficient());
		else if (term_node->getExponent() == 1)
			coef = std::stod(term_node->getCoefficient());
		else if (term_node->getExponent() == 2)
			coef_square = std::stod(term_node->getCoefficient());
	}
    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
    {
        getCoefficients(binary_node->getLeft(), constant_term, coef, coef_square);
        getCoefficients(binary_node->getRight(), constant_term, coef, coef_square);
    }
}

void	printEquation(const RPNNode* node)
{
    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
    {
        printEquation(binary_node->getLeft());
        printEquation(binary_node->getRight());
    }
	//node->printNormal();
}

void	reduceEquation(const RPNNode* root)
{
	//for (const RPNNode* node = root; node != nullptr; node = node->getNextNode())
}

void	solveEquation(const RPNNode* root)
{
	size_t	degree = 0;
	double	constant_term = 0.0;
	double	coef = 0.0;
	double	coef_square = 0.0;

	//printEquation(root);
	root->printNormal();
	getMaxDegree(root, degree);
	std::cout << "Polynomial degree: " << degree << std::endl;

	if (degree > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl; 
		return ;
	}

	if (degree == 0)
	{
		getCoefficients(root, constant_term, coef, coef_square);
		if (constant_term != 0.0)
			std::cout << "no solution" << std::endl;
		else
			std::cout << "infinte number of soluntions" << std::endl;
	}
	else if (degree == 1)
	{
		double solution;

		getCoefficients(root, constant_term, coef, coef_square);
		solution = -constant_term / coef;
		std::cout << "The solution is:\n" << solution << std::endl;
	}
	else if (degree == 2)
	{
		double discriminant;

		getCoefficients(root, constant_term, coef, coef_square);
		std::cout << "constant_term : " << constant_term << std::endl;
		std::cout << "coef : " << coef << std::endl;
		std::cout << "coef_square : " << coef_square << std::endl;
		// ax^2 + bx + c = 0, the discriminant is calculated as b^2 - 4ac
		discriminant = coef * coef - 4 * coef_square * constant_term;
		std::cout << "discriminant: " << discriminant << std::endl;
		if (discriminant < 0.0)
			std::cout << "Discriminant is negative, no real solution." << std::endl;
		else if (discriminant == 0.0)
		{
			double solution = -coef / (2 * coef_square);
			std::cout << "Discriminant is zero, the one solutions is:" << std::endl;;
			std::cout << solution << std::endl;
		}
		else
		{
			double sqrt_discriminant = calculateSquareRoot(discriminant, 0.001);
			//double sqrt_discriminant = std::sqrt(discriminant);
			double solution1 = (-coef + sqrt_discriminant) / (2 * coef_square);
			double solution2 = (-coef - sqrt_discriminant) / (2 * coef_square);

			std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;;
			std::cout << solution1 << std::endl;
			std::cout << solution2 << std::endl;
		}
	}
};

