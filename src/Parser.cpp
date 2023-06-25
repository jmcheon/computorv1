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
	// Regular expression pattern for matching integer or double numbers
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


void	Parser::extractTerm(std::unique_ptr<RPNNode>& node, std::vector<Token>& tokens)
{
    std::unique_ptr<TermNode>		term = std::make_unique<TermNode>();
	std::vector<Token>::iterator	current_token = tokens.begin();
	std::vector<Token>::iterator	end_token = tokens.end();

	if (!isValidTerm(tokens))
		return ;
	if (tokens.size() == 1)
	{
		if (isNumber(current_token->m_value))
			term->setCoefficient(current_token->m_value);
		else
		{
			if (current_token->m_value[0] == '-')
			{
				std::string	variable;
				term->setCoefficient(std::string("-1", 2));
				variable = current_token->m_value[1];
				term->setVariable(variable);
			}
			else
				term->setVariable(current_token->m_value);
		}
	}
	if (tokens.size() == 3)
	{
		if ((end_token - 1)->m_value == "^")
		{
			term->setVariable(current_token->m_value);
			term->setExponent((current_token + 1)->m_value);
		}
		else if ((end_token - 1)->m_value == "*")
		{
			term->setCoefficient(current_token->m_value);
			term->setVariable((current_token + 1)->m_value);
		}
	}
	if (tokens.size() == 5)
	{
		term->setCoefficient(current_token->m_value);
		term->setVariable((current_token + 1)->m_value);
		term->setExponent((current_token + 2)->m_value);
	}
	node = std::move(term);
}


std::unique_ptr<RPNNode>	Parser::buildTree(const std::vector<Token>& rpn_tokens)
{
	std::stack<std::unique_ptr<RPNNode> > stack;
	bool	valid_equation = false;

	for (const Token& token : rpn_tokens)
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

			if (token.m_value == "-" || token.m_value == "+" || token.m_value == "=")
			{
				std::vector<Token> tokens;
				std::unique_ptr<RPNNode> right = std::move(stack.top());
				stack.pop();
				//std::cout << "right" << std::endl;
				extractTermTokens(right.get(), tokens);
				//for (auto& token : tokens)
					//token.debugPrint();
				//std::cout << std::endl;
				extractTerm(right, tokens);

				tokens.clear();
				std::unique_ptr<RPNNode> left = std::move(stack.top());
				stack.pop();
				//std::cout << "left" << std::endl;
				extractTermTokens(left.get(), tokens);
				//for (auto& token : tokens)
					//token.debugPrint();
				//std::cout << std::endl;
				extractTerm(left, tokens);

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

bool	Parser::isValidTerm(std::vector<Token>::iterator current_token)
{
    size_t	star_count = 0;
    size_t	degree_count = 0;

	for (; current_token != m_op_token; ++current_token)
	{
		current_token->debugPrint();
		if (current_token->m_type == TokenType::OPERATOR)
		{
			if (star_count <  degree_count)
				runtimeException("Invalid order", current_token->m_value);
			else if (current_token->m_value == "*")
				star_count++;
			else if (current_token->m_value == "^")
				degree_count++;
		}
		else if (current_token->m_type == TokenType::IDENTIFIER)
		{
			if (!star_count && (current_token + 1)->m_value != "^" && (current_token + 1) != m_op_token)
			{
				if (!isNumber(current_token->m_value))
					runtimeException("Invalid coefficient", current_token->m_value);
			}
			if (star_count && !degree_count || (!star_count && (current_token + 1)->m_value == "^"))
			{
				if (!isPotentialVariable(current_token->m_value))
					runtimeException("Invalid variable", current_token->m_value);
			}
			if (degree_count)
			{
				if (!isInteger(current_token->m_value))
					runtimeException("Invalid exponent", current_token->m_value);
			}
		}

		if (star_count > 1)
			runtimeException("Invalid multiple '*' in a term");
		if (degree_count > 1)
			runtimeException("Invalid multiple '^' in a term");
	}
}
void	Parser::extractTerm(std::vector<Term>& terms)
{
    Term	term;
    bool	expecting_coef = true;
    bool	expecting_variable = true;
    bool	expecting_exponent = true;

	//std::cout << m_op_token->m_value << m_op_token->m_token_num << std::endl;
	isValidTerm(m_current_token);
	for (; m_current_token != m_op_token; ++m_current_token)
	{
		//m_current_token->debugPrint();
		if (m_current_token->m_type == TokenType::IDENTIFIER)
        {
			if (isNumber(m_current_token->m_value))
			{
            	if (expecting_coef)
            	{
            	    term.setCoefficient(m_current_token->m_value);
            	    expecting_coef = false;
            	}
            	else if (expecting_exponent)
            	{
            	    term.setExponent(m_current_token->m_value);
            	    expecting_exponent = false;
            	}
				/*
				//current_token->m_type == TokenType::COEFFICIENT;
				if ((m_current_token - 1)->m_value == "^")
					term.setExponent(m_current_token->m_value);
				else if ((m_current_token + 1)->m_value == "*")
					term.setCoefficient(m_current_token->m_value);
				else if (term.getVariable().empty())
					term.setCoefficient(m_current_token->m_value);
					*/
			}
 			else if (isPotentialVariable(m_current_token->m_value))
			{
            	if (expecting_variable)
            	{
					if (term.getVariable().empty())
					{
						//term.getVariable() = current_token->m_value;
						term.setVariable(m_current_token->m_value);
						term.setExponent(std::string("1", 1));
						expecting_exponent = true;
					}
					else if (term.getVariable() != m_current_token->m_value)
					{
						if (!expecting_exponent)
						{
							std::cout << term.getVariable() << "!=" << m_current_token->m_value << std::endl;
							throw std::runtime_error("Invalid variable");
						}
					}
					//expecting_exponent = true;
            	    //term.setVariable(m_current_token->m_value);
            	    expecting_variable = false;
					expecting_coef = false;
            	}
			}
			else if (m_current_token->m_value != "*" && m_current_token->m_value != "^")
				throw std::runtime_error("Invalid variable o");
        }
        else if (m_current_token->m_type == TokenType::OPERATOR)
        {
            if (m_current_token->m_value == "*")
            {
                expecting_variable = true;
            }
            else if (m_current_token->m_value == "^")
            {
                expecting_exponent = true;
            }
        }
    }
	terms.push_back(term);
	if (m_op_token == m_end_token)
		m_current_token--;
}






/*
		if (isNumber(m_current_token->m_value))
		{
			//current_token->m_type == TokenType::COEFFICIENT;
			if ((m_current_token - 1)->m_value == "^")
				term.setExponent(m_current_token->m_value);
			else if ((m_current_token + 1)->m_value == "*")
				term.setCoefficient(m_current_token->m_value);
			else if (term.getVariable().empty())
				term.setCoefficient(m_current_token->m_value);
		}
		else
		{
 			if (isPotentialVariable(m_current_token->m_value))
			{
				if (term.getVariable().empty())
				{
					//term.getVariable() = current_token->m_value;
					term.setVariable(m_current_token->m_value);
					term.setExponent(std::string("1", 1));
					expecting_exponent = true;
				}
				else if (term.getVariable() != m_current_token->m_value)
				{
					if (!expecting_exponent)
					{
						std::cout << term.getVariable() << "!=" << m_current_token->m_value << std::endl;
						throw std::runtime_error("Invalid variable");
					}
				}
				expecting_exponent = true;
			}
			else if (m_current_token->m_value != "*" && m_current_token->m_value != "^")
				throw std::runtime_error("Invalid variable o");
		}
	}
	return *term;
}
*/

void		Parser::expectOperator(std::vector<Token>::iterator current_token)
{
	for (; current_token != m_end_token; ++current_token)
	{
		if (current_token->m_value == "+" || current_token->m_value == "-"
			|| current_token->m_value == "=")
		{
			m_op_token = current_token;
			std::cout << m_op_token->m_value << std::endl;
			return ;
		}
		if (current_token == m_end_token - 1)
		{
			m_op_token = m_end_token;
			return ;
		}
	}
}
/*
std::unique_ptr<RPNNode> traverseAndModify(RPNNode* node)
{
    if (node == nullptr)
        return nullptr;

    if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
	{
    	// Traverse and modify the left subtree
    	std::unique_ptr<RPNNode> left = traverseAndModify(binary_node->getLeft());

    	// Traverse and modify the right subtree
    	std::unique_ptr<RPNNode> right = traverseAndModify(binary_node->getRight());
	}

    // Perform modifications on the current node
    if (Condition for creating a TermNode) {
        // Extract values from the nodes to create a TermNode
        std::string variable;
        double coefficient;
        size_t exponent;

        // Extract values from the left and right nodes as needed
        if (left != nullptr) {
            // Extract values from the left node
            // Example: variable = dynamic_cast<TermNode*>(left.get())->getVariable();
        }

        if (right != nullptr) {
            // Extract values from the right node
            // Example: coefficient = dynamic_cast<TermNode*>(right.get())->getCoefficient();
        }

        // Create a new TermNode using the extracted values
        std::unique_ptr<RPNNode> newTermNode = std::make_unique<TermNode>(variable, coefficient, exponent);
        return newTermNode;
    }

    // Return the original node if no modifications were made
    return std::make_unique<RPNNode>(*node);
}
*/


void		Parser::parse(const RPNNode* node)
{
	std::vector<Term>	terms;
	std::stack<RPNNode> stack;
			//stack.push(std::make_unique<IdentifierNode>(token.m_value));

	//traverseAndModify(node);
	//printNode(static_cast<const RPNNode *>(&tree[0]));
    //const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node);
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
}

std::vector<Term>		Parser::parse(std::vector<Token>& tokens)
{
	std::vector<Term>	terms;

	m_current_token = tokens.begin();
	m_op_token = tokens.end();
	m_end_token = tokens.end();

	for (; m_current_token != m_end_token; ++m_current_token)
	{
		//m_current_token->debugPrint();
		if (m_current_token->m_type == TokenType::IDENTIFIER)
		{
			expectOperator(m_current_token);
			extractTerm(terms);
			for (auto& term : terms)
				term.debugPrint();
			//if (term.getVariable().empty() && term.getExponent() != 0)
			//	throw std::runtime_error("Invalid syntax");
		}
		else if (m_current_token->m_type == TokenType::OPERATOR)
		{
			std::cout << "operator: " <<  m_current_token->m_value << std::endl;
			terms.push_back(Operator(m_current_token->m_value));
		}
	}
	return terms;
}

bool	Parser::expectToken(Token& token, TokenType m_type)
{
	if (token.m_type != m_type)
		return (false);
	return (true);
}

Token		Parser::expectToken(TokenType m_type, const std::string& name)
{
	Token return_token;

	if (m_current_token== m_end_token)
		return return_token;
	if (m_current_token->m_type != m_type)
		return return_token;
	if (!name.empty() && m_current_token->m_value != name)
		return return_token;

	return_token = *m_current_token;
	++m_current_token;
	return return_token;
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
/*
void	soloveEquation(const std::vector<Term>& terms)
{
	int	degree = 0;
	for (const Term& term : terms)
	{
		if (term.exponent > degree)
			degree = term.exponent;
	}
	std::cout << "Polynomial degree: " << degree << std::endl;

	if (degree > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl; 
		return ;
	}

	if (degree == 0)
	{
		double constant_term = 0.0;

		for (const Term& term : terms)
		{
			if (term.exponent == 0)
			{
				constant_term = term.coefficient;
				break ;
			}
		}
		if (constant_term != 0.0)
			std::cout << "no solution" << std::endl;
		else
			std::cout << "infinte number of soluntions" << std::endl;
	}
	else if (degree == 1)
	{
		double constant_term = 0.0;
		double coef = 0.0;
		double solution;

		for (const Term& term : terms)
		{
			if (term.exponent == 0)
				constant_term = term.coefficient;
			else if (term.exponent == 1)
				coef = term.coefficient;
		}
		solution = -constant_term / coef;
		std::cout << "The solution is:\n" << solution << std::endl;
	}
	else if (degree == 2)
	{
		double constant_term = 0.0;
		double coef = 0.0;
		double coef_squre = 0.0;
		double discriminant;
		for (const Term& term : terms)
		{
			if (term.exponent == 0)
				constant_term = term.coefficient;
			else if (term.exponent == 1)
				coef = term.coefficient;
			else if (term.exponent == 2)
				coef_squre = term.coefficient;
		}
		// ax^2 + bx + c = 0, the discriminant is calculated as b^2 - 4ac
		discriminant = coef * coef - 4 * coef_squre * constant_term;
		if (discriminant < 0.0)
			std::cout << "Discriminant is negative, no real solution." << std::endl;
		else if (discriminant == 0.0)
		{
			double solution = -coef / (2 * coef_squre);
			std::cout << "Discriminant is zero, the one solutions is:" << std::endl;;
			std::cout << solution << std::endl;
		}
		else
		{
			double sqrt_discriminant = calculateSquareRoot(discriminant, 0.001);
			double solution1 = -coef / (2 * coef_squre);
			double solution2 = -coef / (2 * coef_squre);

			std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;;
			std::cout << solution1 << std::endl;
			std::cout << solution2 << std::endl;
		}
	}
};
*/
