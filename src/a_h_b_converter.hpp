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

		for (int i = 0, j = conv.size() - 1; i < conv.size(); ++i, --j)
		{
			if (i)
				temp = 2 << (i - 1);
			else
				temp = 1;

			result += temp * conv[j];
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
	std::vector<unsigned int> s2v(std::string conv, int sub=0)
	{
		std::vector<unsigned int> result;
		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			result.push_back(conv[i] - sub);
		}

		return result;
	}

	// Converts from ascii representation of hex to a vector of ints
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

	// Convert from ascii to a 2D vector of ints
	std::vector<std::vector<unsigned int>> a2vv(std::string conv, unsigned int block_size)
	{
		std::vector<std::vector<unsigned int>> result;
		std::string temp;

		for (unsigned int i = 0; i < conv.size(); i += block_size)
		{
			temp = conv.substr(i, block_size);
			result.push_back(a2v(temp));
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

	// Converts from 2dvector of hex numbers to ascii representation
	std::string h2a(std::vector<std::vector<unsigned int>> conv)
	{
		std::string result = "";

		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			result += h2a(conv[i]);
		}

		return result;
	}

	// Converts from string to a binary vector of block_size size
	std::vector<std::vector<unsigned int>> s2bv(std::string conv, unsigned int block_size)
	{
		std::vector<std::vector<unsigned int>> result;

		// First get integer representations of the individual characters
		std::vector<unsigned int> temp = s2v(conv, '0');

		// Then get each individual number's binary equivalent
		for (unsigned int i = 0; i < temp.size(); ++i)
		{
			result.push_back(i2b(temp[i], block_size));
		}

		return result;
	}

	// Converts from string representation of binary to a binary vector of block_size size
	std::vector<std::vector<unsigned int>> bs2bv(std::string conv, unsigned int block_size)
	{
		std::vector<std::vector<unsigned int>> result;
		std::vector<unsigned int> temp;
		std::string block;

		for (unsigned int i = 0; i < conv.size(); i += block_size)
		{
			block = conv.substr(i, block_size);
			temp = s2v(block, '0');
			result.push_back(temp);
		}

		return result;
	}

	// Converts from binary 2dvector to vector of ints
	std::vector<unsigned int> bv2iv(std::vector<std::vector<unsigned int>> conv)
	{
		std::vector<unsigned int> result;

		for (unsigned int i = 0; i < conv.size(); ++i)
			result.push_back(b2i(conv[i]));

		return result;
	}

};

#endif // __H_A_H_B_CONVERTER_H__