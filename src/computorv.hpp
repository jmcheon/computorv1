#ifndef COMPUTORV_HPP
#define COMPUTORV_HPP
#include <complex>
#include "Lexer.hpp"
#include "RPNtree.hpp"
#include "utils.hpp"

void		solveEquation(std::vector<TermNode> terms);
double		reduce(std::vector<TermNode>& terms, size_t degree);
void		printReducedFormat(std::vector<TermNode>& terms);
void		printIrreducibleFormat(std::vector<TermNode>& terms);
#endif
