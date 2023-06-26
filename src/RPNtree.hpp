#ifndef RPNTREE_HPP
#define RPNTREE_HPP
#include <iostream>
#include <memory>

class BinaryOperatorNode;

class RPNNode
{
	protected:
	//	std::unique_ptr<RPNNode>	m_parent;
	public:
		virtual			~RPNNode() = default;
		virtual void	printNormal() const = 0;
		virtual void	print() const = 0;
		virtual void	traverse() const = 0;
		virtual std::string getValue() const = 0;
	//	RPNNode*	getParent() const { return m_parent.get(); }
	//	void		setParent(std::unique_ptr<RPNNode> parent) { m_parent = std::move(parent); }
		virtual std::unique_ptr<RPNNode>	copy() = 0; 
};

class TermNode : public RPNNode
{
	private:
		std::string	m_variable;
		std::string	m_coefficient;
		size_t		m_exponent;

	public:
		TermNode() : m_coefficient("1"), m_variable("1"), m_exponent(1) {};
		TermNode(std::string coef, std::string variable, size_t exponent) : m_coefficient(coef), m_variable(variable), m_exponent(exponent) {};

		std::string getVariable() const { return m_variable; }
		std::string	getCoefficient() const { return m_coefficient; }
		size_t		getExponent() const { return m_exponent; }
		std::string getValue() const override { return m_variable;}

		void 		setVariable(const std::string& variable) { m_variable = variable; }
		void		setCoefficient(const std::string& coef) { m_coefficient = coef; }
		void		setExponent(const std::string& exponent) { m_exponent = std::stoi(exponent); }

		void		printNormal() const override
		{
			if (getVariable() == "1")
				std::cout << m_coefficient << " ";
			else
				std::cout << m_coefficient << " * " << m_variable << "^" << m_exponent << " ";
		}
		void		print() const override { std::cout << m_coefficient << "*" << m_variable << "^" << m_exponent; }
		void		traverse() const override { std::cout << m_coefficient << "*" << m_variable << "^" << m_exponent; }
		void		debugPrint() const;
		std::unique_ptr<RPNNode>	copy() { std::make_unique<TermNode>(getCoefficient(), getVariable(), getExponent()); }
};

class IdentifierNode : public RPNNode
{
	private:
		std::string		m_value;

	public:
		explicit		IdentifierNode(std::string value) : m_value(value) {}

		std::string		getValue() const { return m_value; }
		void			printNormal() const override { std::cout << m_value << " "; }
		void			print() const override { std::cout << m_value; }
		void			traverse() const override { std::cout << m_value; }
		std::unique_ptr<RPNNode>	copy() { std::make_unique<IdentifierNode>(getValue()); }
};

class BinaryOperatorNode : public RPNNode
{
	private:
		std::string							m_operator;
		std::unique_ptr<RPNNode>			m_left;
		std::unique_ptr<RPNNode>			m_right;


	public:
		BinaryOperatorNode(std::string op, std::unique_ptr<RPNNode> left, std::unique_ptr<RPNNode> right) : m_operator(op), m_left(std::move(left)), m_right(std::move(right)) {}

		std::string			getOperator() const { return m_operator; }
		RPNNode*			getLeft() const { return m_left.get(); }
		RPNNode*			getRight() const { return m_right.get(); }
		std::string			getValue() const override { return m_operator; }

		void				setOperator(std::string op) { m_operator = op; }
		void				setLeft(std::unique_ptr<RPNNode> node) { m_left = std::move(node); }
		void				setRight(std::unique_ptr<RPNNode> node) { m_right = std::move(node); }

		void			printNormal() const override 
		{
			if (getRight())
				getRight()->printNormal();
			std::cout << getOperator() << " ";
			if (getLeft())
				getLeft()->printNormal();
			
		}
		void			print() const override { m_left->print(); m_right->print(); std::cout << m_operator; }
		void			traverse() const override { std::cout << m_operator; }
		void			swapChildNodes() { std::swap(m_left, m_right); }
		//std::unique_ptr<RPNNode>	copy(RPNNode* node) { std::make_unique<BinaryOperatorNode>(getOperator(), std::make_unique<RPNNode>(getLeft()), std::make_unique<RPNNode>(getRight())); }
		std::unique_ptr<RPNNode>	copy() { return nullptr; }
};

void	printNode(const RPNNode* node);
void	printTree(const RPNNode* node, bool leftview=false);
void	printTreeRightview(const std::string& prefix, const RPNNode* node, bool is_left);
void	printTreeLeftview(const std::string& prefix, const RPNNode* node, bool is_left);
void	reverseTraversal(const RPNNode* node);
#endif
