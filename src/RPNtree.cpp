#include "RPNtree.hpp"
const RPNNode*	TermNode::getNextNode() const
{ 
	/*
	if (parent)
	{
		std::cout << "yes" << std::endl;
    	return parent->getNextNode();
	}
	return nullptr;
	*/
}

void	TermNode::debugPrint() const
{
	std::cout << "Term(" << m_coefficient << " * " << m_variable;
	std::cout << " ^ " << m_exponent << ")" << std::endl;
}

const RPNNode*	BinaryOperatorNode::getNextNode() const
{
    m_right->getNextNode();
    m_left->getNextNode();
	/*
	// If this node has a parent and this node is the left child of its parent,
    // return the parent as the next node
    if (parent && parent->getLeft() == this)
        return parent;

    // If this node has a parent and this node is the right child of its parent,
    // return the next node of the parent
    if (parent && parent->getRight() == this)
        return parent->getNextNode();

    // This node is not part of a larger expression
    return nullptr;
	//return m_right.get();
	*/
}

void	printNode(const RPNNode* node)
{
	if (node)
		node->print();
	std::cout << std::endl;
}

void	printTreeLeftview(const std::string& prefix, const RPNNode* node, bool is_left)
{
    if( node != nullptr )
    {
        std::cout << prefix;
        std::cout << (is_left ? "└─" : "├─" );

		node->traverse();
    	std::cout << std::endl;
    	if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
		{
        	printTreeLeftview(prefix + (is_left ? "   " : "│  "), binary_node->getRight(), false);
        	printTreeLeftview(prefix + (is_left ? "   " : "│  "), binary_node->getLeft(), true);
    	}
    }
}

void	printTreeRightview(const std::string& prefix, const RPNNode* node, bool is_left)
{
    if( node != nullptr )
    {
        std::cout << prefix;
        std::cout << (is_left ? "├─" : "└─" );

		node->traverse();
    	std::cout << std::endl;
    	if (const BinaryOperatorNode* binary_node = dynamic_cast<const BinaryOperatorNode*>(node))
		{
        	printTreeRightview(prefix + (is_left ? "│  " : "   "), binary_node->getLeft(), true);
        	printTreeRightview(prefix + (is_left ? "│  " : "   "), binary_node->getRight(), false);
    	}
    }
}

void	printTree(const RPNNode* node, bool leftview)
{
	if (leftview)
    	printTreeLeftview("", node, true);
	else
    	printTreeRightview("", node, false);
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
