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

	bool debug;
	bool encrypt;
	bool inter;
	bool hex;

	bool from_std;
	bool to_std;

	// Mangler function
	void mangler()
	{

	}

	// Function to encrypt 
	void en()
	{
		// Encrypt from the stdin
		if (to_std)
		{
			//for (unsigned int i = 0; i < )
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
			en();
		else
			de();
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