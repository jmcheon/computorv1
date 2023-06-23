#include "Parser.hpp"

std::vector<Term>	Parser::extract(const std::vector<Token>& tokens)
{
    std::vector<Term>	terms;
    Term 				current_term;
    bool				expecting_coef = true;
    bool				expecting_variable = true;
	Token				possible_token;
	Token				result;

    for (const Token& token : tokens)
	{
        if (token.type == TokenType::COEFFICIENT)
		{
            // Coefficient found at an unexpected position, invalid syntax
            if (!expecting_coef)
                throw std::runtime_error("Invalid equation syntax coef");
            current_term.coefficient = token.value;//std::stod(token.value);
            expecting_coef = false;
        }
		else if (token.type == TokenType::MULTIPLICATION)
		{
            // Multiplication operator found before coefficient, invalid syntax
            if (expecting_coef)
                throw std::runtime_error("Invalid equation syntax *");
            expecting_coef = true;
            expecting_variable = true;
        }
		else if (token.type == TokenType::VARIABLE)
		{
            // Variable found before coefficient, invalid syntax
            if (!expecting_variable)
                throw std::runtime_error("Invalid equation syntax variable");
            current_term.variable = token.value;
            expecting_variable = false;
			std::cout << possible_token.value << std::endl;
			/*
			result = expectToken(TokenType::DEGREE);
			if (result == possible_token)
				throw std::runtime_error("no degree");
				*/
        }
		else if (token.type == TokenType::DEGREE)
		{
            // Degree operator found before coefficient, invalid syntax
            if (expecting_variable)
                throw std::runtime_error("Invalid equation syntax ^");
            expecting_variable = false;
        }
		else if (token.type == TokenType::EXPONENT)
		{
            // Exponent found before coefficient, invalid syntax
            if (!expecting_coef)
                throw std::runtime_error("Invalid equation syntax exponent");
            current_term.exponent = token.value;//std::stoi(token.value);
            expecting_coef = true;
            terms.push_back(current_term); // Add the term to the list
        }
    }
	/*
    // If the last token was an exponent, add the current term
    if (expecting_coef)
        throw std::runtime_error("Invalid equation syntax");
	*/
    //terms.push_back(current_term);

    return terms;
}

std::vector<Term>	Parser::extractTerms(const std::vector<Token>& tokens)
{
    std::vector<Term>	terms;
    Term 				current_term;
    bool				expecting_coef = true;
    bool				expecting_variable = true;
	Token				possible_token;

    for (const Token& token : tokens)
	{
        if (token.type == TokenType::COEFFICIENT)
		{
            // Coefficient found at an unexpected position, invalid syntax
            if (!expecting_coef)
                throw std::runtime_error("Invalid equation syntax coef");
            current_term.coefficient = token.value;//std::stod(token.value);
            expecting_coef = false;
        }
		else if (token.type == TokenType::MULTIPLICATION)
		{
            // Multiplication operator found before coefficient, invalid syntax
            if (expecting_coef)
                throw std::runtime_error("Invalid equation syntax *");
            expecting_coef = true;
            expecting_variable = true;
        }
		else if (token.type == TokenType::VARIABLE)
		{
            // Variable found before coefficient, invalid syntax
            if (!expecting_variable)
                throw std::runtime_error("Invalid equation syntax variable");
            current_term.variable = token.value;
            expecting_variable = false;
        }
		else if (token.type == TokenType::DEGREE)
		{
            // Degree operator found before coefficient, invalid syntax
            if (expecting_variable)
                throw std::runtime_error("Invalid equation syntax ^");
            expecting_variable = false;
			std::cout << "pt:" << possible_token.value << std::endl;
        }
		else if (token.type == TokenType::EXPONENT)
		{
            // Exponent found before coefficient, invalid syntax
            if (!expecting_coef)
                throw std::runtime_error("Invalid equation syntax exponent");
            current_term.exponent = token.value;//std::stoi(token.value);
            expecting_coef = true;
            terms.push_back(current_term); // Add the term to the list
        }
    }
	/*
    // If the last token was an exponent, add the current term
    if (expecting_coef)
        throw std::runtime_error("Invalid equation syntax");
	*/
    //terms.push_back(current_term);

    return terms;
}

int getOperatorPrecedence(const Token& token)
{
    if (token.value == "*" || token.value == "/" || token.value == "^")
		return 2;
    else if (token.value == "+" || token.value == "-")
		return 1;
	return 0;
}

bool hasHigherPrecedence(const Token& op1, const Token& op2)
{
    int op1 = getOperatorPrecedence(op1);
    int op2 = getOperatorPrecedence(op2);
    return (op1 > op2);
}


std::vector<Token> Parser::convertToRPN(const std::vector<Token>& tokens)
{
    std::vector<Token>	output_queue;
    std::stack<Token>	operator_stack;

    for (const Token& token : tokens)
	{
        switch (token.type)
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
                throw std::runtime_error("Invalid token type encountered");
        }
    }

    while (!operator_stack.empty())
	{
        if (operator_stack.top().type == TokenType::WHITESPACE)
		{
            throw std::runtime_error("Invalid equation syntax");
        }
        output_queue.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return output_queue;
}

std::unique_ptr<RPNNode>	Parser::buildTree(const std::vector<Token>& rpn_tokens)
{
	std::stack<std::unique_ptr<RPNNode> > stack;

	for (const Token& token : rpn_tokens)
	{
		//token.debugPrint();
		if (token.type == TokenType::IDENTIFIER)
			stack.push(std::make_unique<IdentifierNode>(token.value));
		else if (token.type == TokenType::OPERATOR)
		{
			if (stack.size() < 2)
				throw std::runtime_error("Invalid formula: Not enough operands");
			std::unique_ptr<RPNNode> right = std::move(stack.top());
			stack.pop();
			std::unique_ptr<RPNNode> left = std::move(stack.top());
			stack.pop();
			stack.push(std::make_unique<BinaryOperatorNode>(token.value, std::move(left), std::move(right)));
		}
		else
			throw std::runtime_error("Invalid formula: Unexpected symbol");
	}
	if (stack.size() != 1)
		throw std::runtime_error("Invalid formula: Too many operands");
	return std::move(stack.top());
}

void	Term::debugPrint() const
{
	std::cout << "Term(" << coefficient << " * " << variable << " ^ " << exponent << ")" << std::endl;
}

std::vector<Term>		Parser::parse(std::vector<Token>& tokens)
{
	std::vector<Term>	terms;

	for (Token& token : tokens)
	{
		token.debugPrint();
		if (token.type == TokenType::COEFFICIENT)
		{
			
		}
	}
//	terms = extractTerms(tokens);
//	for (Term& term : terms)
//		term.debugPrint();


	return terms;
}

bool					Parser::checkValidSyntax(const std::vector<Token>& tokens)
{
	int	variable_count = 0;
	bool	equal_sign = false;
	std::string variable;

	for (const Token& token : tokens)
	{
		if (token.type == TokenType::VARIABLE)
		{
			if (!variable.empty() && token.value != variable)
			{
				std::cout << "Invalid syntax: unknown variable " << token.value << std::endl;
				return false;
			}
			variable = token.value;
			variable_count++;
		}
		else if (token.type == TokenType::EQUATION)
		{
			if (equal_sign)
			{
				std::cout << "Invalid syntax: Multiple equations found." << std::endl;
				return false;
			}
			equal_sign = true;
		}
		else if (token.type == TokenType::COEFFICIENT)
		{
			if (variable.empty())
			{
				std::cout << "Invalid syntax" << token.value << std::endl;
				return false;
			}
		}
	}
}

bool	Parser::expectToken(Token& token, TokenType type)
{
	if (token.type != type)
		return (false);
	return (true);
}

Token		Parser::expectToken(TokenType type, const std::string& name)
{
	Token return_token;

	if (m_current_token== m_end_token)
		return return_token;
	if (m_current_token->type != type)
		return return_token;
	if (!name.empty() && m_current_token->value != name)
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
