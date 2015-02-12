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

		// Get input (From file or from stdin)
		if (input_path == "-")
		{
			std::cout << "Enter text to cipher:" << std::endl << "> ";
			std::getline(std::cin, str_input);

		}
		else
		{
			input.open(input_path.c_str(), std::ifstream::in | std::ifstream::binary);
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