#include "computorv.hpp"

double	reduceEquation(std::vector<TermNode>& terms, size_t degree)
{
	std::vector<TermNode>::iterator	 current_term;
	std::vector<TermNode>::iterator	 end_term;
	double	coef = 0.0;
	bool	first_term_found = false;

	current_term = terms.begin();
	end_term = terms.end();
	for (; current_term != end_term; ++current_term)
	{
		if (current_term->getExponent() == degree)
		{
			if (first_term_found)
            {
				//current_term->print();
                coef += extractCoefficient(current_term->getCoefficient());
                current_term = terms.erase(current_term);
               	--current_term;
				end_term = terms.end();
            }
            else
            {
                coef += extractCoefficient(current_term->getCoefficient());
                first_term_found = true;
            }
		}
	}
	//printTerms(terms);
	return coef;
}

void	printReducedFormat(std::vector<TermNode>& terms)
{
	std::cout << "Reduced form: ";
	for (auto it = terms.begin(); it != terms.end(); ++it)
	{
		if (it->getCoefficient()[0] != '-')
			std::cout << " + ";
		else
			std::cout << " ";
		it->print();
	}
	std::cout << " = 0" <<  std::endl;
}

void	printIrreducibleFormat(std::vector<TermNode>& terms)
{
	std::cout << "Irreducible form: ";
	for (auto it = terms.begin(); it != terms.end(); ++it)
	{
		if (it->getCoefficient()[0] != '-')
			std::cout << " + ";
		else
			std::cout << " ";
    	convertToIrreducibleFraction(extractCoefficient(it->getCoefficient()));
		std::cout << " * " << it->getVariable() << "^" << it->getExponent();
	}
	std::cout << " = 0" << std::endl << std::endl;
}

void	solveEquation(std::vector<TermNode> terms)
{
	size_t	degree = 0;
	double	constant_term = 0.0;
	double	coef = 0.0;
	double	coef_square = 0.0;

	for (auto it = terms.begin(); it != terms.end(); ++it)
	{
        if (it->getExponent() > degree)
            degree = it->getExponent();
	}
	std::cout << "Polynomial degree: " << degree << std::endl;

	if (degree > 2)
	{
		std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl; 
		return ;
	}

	if (degree == 0)
	{
		for (auto it = terms.begin(); it != terms.end(); ++it)
		{
			if (it->getExponent() == 0)
				constant_term = std::stod(it->getCoefficient());
			else if (it->getExponent() == 1)
				coef = std::stod(it->getCoefficient());
			else if (it->getExponent() == 2)
				coef_square = std::stod(it->getCoefficient());
		}
		if (constant_term != 0.0)
			std::cout << "no solution" << std::endl;
		else
			std::cout << "infinte number of soluntions" << std::endl;
	}
	else if (degree == 1)
	{
		double solution;

		for (auto it = terms.begin(); it != terms.end(); ++it)
		{
			if (it->getExponent() == 0)
				constant_term = std::stod(it->getCoefficient());
			else if (it->getExponent() == 1)
				coef = std::stod(it->getCoefficient());
			else if (it->getExponent() == 2)
				coef_square = std::stod(it->getCoefficient());
		}
		solution = -constant_term / coef;
		std::cout << "The solution is:\n" << solution << std::endl;
	}
	else if (degree == 2)
	{
		double discriminant;

		for (auto it = terms.begin(); it != terms.end(); ++it)
		{
			if (it->getExponent() == 0)
				constant_term = std::stod(it->getCoefficient());
			else if (it->getExponent() == 1)
				coef = std::stod(it->getCoefficient());
			else if (it->getExponent() == 2)
				coef_square = std::stod(it->getCoefficient());
		}
		// ax^2 + bx + c = 0, the discriminant is calculated as b^2 - 4ac
		discriminant = coef * coef - 4 * coef_square * constant_term;
		if (discriminant < 0.0)
		{
			std::complex<double> sqrt_discriminant = std::sqrt(std::complex<double>(discriminant, 0.0));
			std::complex<double> solution1 = (-coef + sqrt_discriminant) / (2 * coef_square);
			std::complex<double> solution2 = (-coef - sqrt_discriminant) / (2 * coef_square);

			std::cout << "Discriminant is negative, no real solution." << std::endl;
			if (solution2.imag() < 0)
			{
				std::cout << solution1.real() << "+" << solution1.imag() << "i" << std::endl;
        		std::cout << solution2.real() << solution2.imag() << "i" << std::endl;
			}
			else if (solution1.imag() < 0)
			{
        		std::cout << solution1.real() << solution1.imag() << "i" << std::endl;
				std::cout << solution2.real() << "+" << solution2.imag() << "i" << std::endl;
			}
			//std::cout << solution1 << std::endl;
			//std::cout << solution2 << std::endl;
		}
		else if (discriminant == 0.0)
		{
			double solution = -coef / (2 * coef_square);
			std::cout << "Discriminant is zero, the one solutions is:" << std::endl;;
			std::cout << solution << std::endl;
		}
		else
		{
			double sqrt_discriminant = calculateSquareRoot(discriminant, 0.001);
			double solution1 = (-coef + sqrt_discriminant) / (2 * coef_square);
			double solution2 = (-coef - sqrt_discriminant) / (2 * coef_square);

			std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;;
			std::cout << solution1 << std::endl;
			std::cout << solution2 << std::endl;
		}
	}
};
