#ifndef __H_A_H_B_CONVERTER_H__
#define __H_A_H_B_CONVERTER_H__

#include "includes.hpp"

class converter
{
private:

	char hexchars[5] = {'A', 'B', 'C', 'D', 'E'};

public:

	converter() {}

	std::vector<unsigned int> a2v(std::string conv)
	{
		std::vector<unsigned int> result;
		for (unsigned int i = 0; i < conv.size(); ++i)
		{
			if (isdigit(conv[i]) || conv[i] == 'A' || conv[i] == 'B' 
				|| conv[i] == 'C'  || conv[i] == 'D' || conv[i] == 'E')
			{
				int temp = conv[i] - '0';
				if (temp > 10)
					result.push_back((10 + conv[i] - 'A'));
				else
					result.push_back(temp);
			}
		}

		return result;
	}

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
};

#endif // __H_A_H_B_CONVERTER_H__