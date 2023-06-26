#include "RPNtree.hpp"

void	TermNode::debugPrint() const
{
	std::cout << "Term(" << m_coefficient << " * " << m_variable;
	std::cout << " ^ " << m_exponent << ")" << std::endl;
}

void	printNode(const RPNNode* node)
{
	if (node)
		node->print();
	std::cout << std::endl;
}

void	printTreeRightview(const std::string& prefix, const RPNNode* node, bool is_left)
{
    if( node != nullptr )
    {
        std::cout << prefix;
        std::cout << (is_left ? "└─" : "├─" );

		node->traverse();
    	std::cout << std::endl;
    	if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
		{
        	printTreeRightview(prefix + (is_left ? "   " : "│  "), binary_node->getRight(), false);
        	printTreeRightview(prefix + (is_left ? "   " : "│  "), binary_node->getLeft(), true);
    	}
    }
}

void	printTreeLeftview(const std::string& prefix, const RPNNode* node, bool is_left)
{
    if( node != nullptr )
    {
        std::cout << prefix;
        std::cout << (is_left ? "├─" : "└─" );

		node->traverse();
    	std::cout << std::endl;
    	if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
		{
        	printTreeLeftview(prefix + (is_left ? "│  " : "   "), binary_node->getLeft(), true);
        	printTreeLeftview(prefix + (is_left ? "│  " : "   "), binary_node->getRight(), false);
    	}
    }
}

void	printTree(const RPNNode* node, bool leftview)
{
	if (leftview)
    	printTreeRightview("", node, true);
	else
    	printTreeLeftview("", node, false);
}

void	reverseTraversal(const RPNNode* node)
{
    if (node != nullptr)
	{
		node->traverse();
    	if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
    	{
    	    reverseTraversal(binary_node->getRight());
    	    reverseTraversal(binary_node->getLeft());
    	}
	}
}
