#ifndef RPNTREE_HPP
#define RPNTREE_HPP
#include <iostream>
#include <sstream>
#include <utility>
#include <memory>
#include <map>
#include <stack>
#include <cstdlib>
#include "Lexer.hpp"
#include "Token.hpp"

struct Term1
{
	std::string	variable;
	double		coefficient;
	int			exponent;
};

class Term
{
	private:
		std::string	m_variable;
		double		m_coefficient;
		size_t		m_exponent;
	public:
		Term() : m_coefficient(1.0), m_exponent(0) {};
		std::string getVariable() const { return m_variable; }
		double		getCoefficient() const { return m_coefficient; }
		size_t		getExponent() const { return m_exponent; }
		void 		setVariable(const std::string& variable) { m_variable = variable; }
		void		setCoefficient(const std::string& coef) { m_coefficient = std::stod(coef); }
		void		setExponent(const std::string& exponent) { m_exponent = std::stoi(exponent); }
		//virtual			~Term() = default;
		//virtual void	print() const = 0;
		//virtual void	traverse() const = 0;
		void		debugPrint() const;
};

class RPNNode
{
	public:
		virtual			~RPNNode() = default;
		virtual void	print() const = 0;
		virtual void	traverse() const = 0;
};

class IdentifierNode : public RPNNode
{
	private:
		std::string		m_value;

	public:
		explicit		IdentifierNode(std::string value) : m_value(value) {}
		std::string		getValue() const { return m_value; }
		void			print() const override { std::cout << m_value; }
		void			traverse() const override { std::cout << m_value; }
};

class BinaryOperatorNode : public RPNNode
{
	private:
		std::string					m_operator;
		std::unique_ptr<RPNNode>	m_left;
		std::unique_ptr<RPNNode>	m_right;

	public:
		BinaryOperatorNode(std::string op, std::unique_ptr<RPNNode> left, std::unique_ptr<RPNNode> right) : m_operator(op), m_left(std::move(left)), m_right(std::move(right)) {}
		std::string		getOperator() const { return m_operator; }
		const RPNNode*	getLeft() const { return m_left.get(); }
		const RPNNode*	getRight() const { return m_right.get(); }
		void			print() const override { m_left->print(); m_right->print(); std::cout << m_operator; }
		void			traverse() const override { std::cout << m_operator; }
		void			swapChildNodes() { std::swap(m_left, m_right); }
};

std::unique_ptr<RPNNode>	buildTree(const std::vector<Token>& rpn_tokens);
void	printNode(const RPNNode* node);
void	printTree(const RPNNode* node, bool leftview=false);
void	printTreeRightview(const std::string& prefix, const RPNNode* node, bool is_left);
void	printTreeLeftview(const std::string& prefix, const RPNNode* node, bool is_left);
void	reverseTraversal(const RPNNode* node);

#endif
