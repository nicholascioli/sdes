#ifndef __H_A_H_B_CONVERTER_H__
#define __H_A_H_B_CONVERTER_H__

#include "includes.hpp"

class converter
{
private:

	char hexchars[5] = {'A', 'B', 'C', 'D', 'E'};

public:

	converter() {}

	// Converts from binary to integer
	unsigned int b2i(std::vector<unsigned int> conv)
	{
		unsigned int result = 0;
		unsigned int temp;

		for (int i = 0; i < conv.size(); ++i)
		{
			temp = 2 << (i - 1);
			result += temp * conv[i];
		}

		return result;
	}

	// Converts from unsigned int to binary equivalent in a vector
	std::vector<unsigned int> i2b(unsigned int conv, unsigned int vector_size)
	{
		std::vector<unsigned int> result;
		unsigned int n = conv;
		unsigned int x = conv;
		int a;
		int c;
		unsigned int k;

		for (a = 1; n != 0; ++a)
		    n = n/2;

		a = a - 2;
		
		for (c = a; c >= 0; --c)
		{
		    k = x >> c;

		    if (k & 1)
		      result.push_back(1);
		    else
		      result.push_back(0);
		}

		while (result.size() < vector_size)
			result.emplace(result.begin(), 0);

		return result;
	}

	// Converts from ascii to a vector of ints
	std::vector<unsigned int> s2v(std::string conv)
	{
		std::vector<unsigned int> result;
		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			result.push_back(conv[i] - 0);
		}

		return result;
	}

	// Converts from ascii representation of hex to a vector of binary ints
	std::vector<unsigned int> a2v(std::string conv)
	{
		std::vector<unsigned int> result;
		std::vector<unsigned int> temp_v;
		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			if (isdigit(conv[i]) || conv[i] == 'A' || conv[i] == 'B' 
				|| conv[i] == 'C'  || conv[i] == 'D' || conv[i] == 'E')
			{
				int temp = conv[i] - '0';
				if (temp > 10)
				{
					temp_v = i2b(10 + conv[i] - 'A', 4);
				}
				else
				{
					temp_v = i2b(temp, 4);
				}

				result.insert(result.end(), temp_v.begin(), temp_v.end());
			}
		}

		return result;
	}

	// Converts from vector of hex numbers to ascii representation
	std::string h2a(std::vector<unsigned int> conv)
	{
		std::string result = "";

		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			if (conv[i] > 9)
				result += hexchars[conv[i] - 10];
			else
				result += std::to_string(conv[i]);
		}

		return result;
	}

	// Converts from string to a binary vector of block_size size
	std::vector<std::vector<unsigned int>> s2bv(std::string conv, unsigned int block_size)
	{
		std::vector<std::vector<unsigned int>> result;

		// First get integer representations of the individual characters
		std::vector<unsigned int> temp = s2v(conv);

		// Then get each individual number's binary equivalent
		for (unsigned int i = 0; i < temp.size(); ++i)
		{
			result.push_back(i2b(temp[i], block_size));
		}

		return result;
	}
};

#endif // __H_A_H_B_CONVERTER_H__