#ifndef __H_SDES_CIPHER_H__
#define __H_SDES_CIPHER_H__

#include "includes.hpp"

class cipher
{
private:

	std::string str_input;
	std::string str_output;

	std::ifstream input;
	std::ofstream output;

	keygen kg;
	pparser pp;
	converter conv;

	bool debug;
	bool encrypt;
	bool inter;
	bool hex;

	bool from_std;
	bool to_std;

	// Permutates the first vector by the second
	template <typename T>
	std::vector<T> perm(std::vector<T> data, std::vector<unsigned int> pm)
	{
		std::vector<T> result;

		if (debug) std::cout << "--- DEBUG: Permutation of ";
		for (unsigned int i = 0; i < pm.size(); ++i)
		{
			result.push_back(data[pm[i]-1]);

			if (debug) std::cout << result.back();
		}
		std::cout << " ---" << std::endl;

		return result;
	}

	// Mangler function
	void mangler()
	{

	}

	// Function to carry out the second half of the encryption process
	template <typename T>
	std::vector<T> stage2(std::vector<T> vec)
	{
		unsigned int block_size = pp.get_params().s_blk;
		std::vector<T> result(block_size, 0);

		// Split up permutated data into left and right halves
		std::vector<T> left(vec.begin(), vec.begin() + (block_size / 2));
		std::vector<T> right(vec.begin() + (block_size / 2) + 1, vec.end());

		// Do Feistall thing
		result.insert(result.begin(), right.begin(), right.end()); 				// Left side gets original right side
		
		

		result.insert(result.begin() + (block_size / 2) + 1,
			left.begin(), left.end());		

		return result;
	}

	// Function to encrypt 
	void en()
	{
		std::vector<std::vector<unsigned short>> result;

		// Encrypt from the stdin
		if (from_std)
		{
			std::vector<std::vector<unsigned short>> vec = conv.s2bv(str_input, pp.get_params().s_blk);

			if (debug) std::cout << "--- DEBUG: Converting \'" << str_input << "\' to binary ---" << std::endl;
			for (unsigned int i = 0; i < vec.size(); ++i)
			{
				if (debug) 
				{
					for (unsigned int j = 0; j < vec[i].size(); ++j)
						std::cout << vec[i][j];
					std::cout << std::endl;

					std::cout << "--- DEBUG: Permutating \'" << str_input[i] << "\' ---" << std::endl;
				}

				result.push_back(
					stage2<unsigned short>(
						perm<unsigned short>(
							vec[i], pp.get_params().i_perm
						)
					)
				);
			}
			if (debug) std::cout << "--- DEBUG: Finished converting " << str_input << " ---" << std::endl;
		}
		// Encrypt from a file
		else
		{

		}
	}

	// Function to decrypt
	void de()
	{

	}

public:

	cipher(std::string input_path, keygen KG, pparser PP, std::string output_path,
			bool ecrypt=true, bool intm=false, bool hx=false, bool dbg=false)
	{
		// Set up arguments and defaults
		kg = KG;
		pp = PP;
		encrypt = ecrypt;
		inter = intm;
		hex = hx;
		debug = dbg;
		from_std = false;
		to_std = false;

		// Get input (From file or from stdin)
		if (input_path == "-")
		{
			if (debug) std::cout << "--- DEBUG: Setting INPUT to STDIN ---" << std::endl;
			std::cout << "Enter text to cipher:" << std::endl << "> ";
			std::getline(std::cin, str_input);
			from_std = true;
		}
		else
		{
			if (debug) std::cout << "--- DEBUG: Setting INPUT to file: " << input_path << " ---" << std::endl;
			input.open(input_path.c_str(), std::ifstream::in | std::ifstream::binary);
		}

		// Set output
		if (output_path == "-")
		{
			if (debug) std::cout << "--- DEBUG: Setting OUTPUT to STDOUT ---" << std::endl;
			to_std = true;
		}
		else
		{
			if (debug) std::cout << "--- DEBUG: Setting OUTPUT to file: " << output_path << " ---" << std::endl;
			output.open(output_path.c_str(), std::ifstream::out | std::ifstream::binary);
		}

		// Encrypt / Decrypt the input
		if (encrypt)
		{
			if (debug) std::cout << "--- DEBUG: Beginning the encryption process ---" << std::endl;
			en();
			if (debug) std::cout << "--- DEBUG: Encryption process finished ---" << std::endl;
		}
		else
		{
			if (debug) std::cout << "--- DEBUG: Beginning the decryption process ---" << std::endl;
			de();
			if (debug) std::cout << "--- DEBUG: Decryption process finished ---" << std::endl;
		}
	}

	~cipher()
	{
		if (input.is_open())
			input.close();
		if (output.is_open())
			output.close();
	}
};

#endif // __H_SDES_CIPHER_H__