// -- main.cpp --
// Written by Nicholas Cioli
// --------------
// By default, when invoked this program
// will read all of the configurable options
// from a file in the current directory labelled
// 'params.txt', will read plaintext from
// 'ptxt', will output to file 'ctxt', and will
// read the key from 'key.txt'

// Includes
#include "includes.hpp"
#include "pparser.hpp"
#include "keygen.hpp"
#include "cipher.hpp"

//	@params:
//		-i<filepath>	=> Path to alternate ptxt (- for stdin)
//		-o<filepath>	=> Path to alternate ctxt (- for stdout)
//		-e				=> Encrypt (default)
//		-d 				=> Decrypt
//		-k<filepath>	=> Path to alternate key.txt
//		-p<filepath>	=> Path to alternate pamas.txt
//		-s 				=> Show intermediate steps
//		-x				=> Use hexadecimal instead of binary
//		-D 				=> Enable debug mode
int main(int argc, char** argv)
{
	// Setup parameter defautls
	std::string input 	= "./ptxt";
	std::string output 	= "./ctxt";
	std::string key		= "./key.txt";
	std::string params	= "./params.txt";
	bool encrypt 	= true;
	bool inter 		= false;
	bool hex 		= false;
	bool debug		= false;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			// First check for flags without paths
			if (argv[i][1] == 'd')
				encrypt = false;

			if (argv[i][1] == 's')
				inter = true;

			if (argv[i][1] == 'x')
				hex = true;

			if (argv[i][1] == 'D')
				debug = true;

			// Now check for flags with paths
			if (argv[i][1] == 'i')
			{
				input = argv[i];
				input = input.substr(2);
			}

			if (argv[i][1] == 'o')
			{
				output = argv[i];
				output = output.substr(2);
			}

			if (argv[i][1] == 'k')
			{
				key = argv[i];
				key = key.substr(2);
			}

			if (argv[i][1] == 'p')
			{
				params = argv[i];
				params = params.substr(2);
			}
		}
		else
		{
			std::cerr << "ERROR: Invalid flag: " << argv[i] << std::endl;
		}
	}

	if (debug)
	{
		std::cout << "--- DEBUG: Showing Flags: ---\nintput: " << input << "\noutput: " 
			<< output << "\nkey: " << key << "\nparams: " << params
			<< "\nencrypt: " << encrypt << "\ninter: " << inter
			<< "\nhex: " << hex << "\ndebug: " << debug << std::endl;
		std::cout << "--- DEBUG: Showing Flags Completed ---\n" << std::endl;
	}

	// Import the parameters from parameter file
	if (debug) std::cout << "--- DEBUG: Gathering Parameters ---" << std::endl;
	pparser pp(params, debug);

	if (debug) std::cout << "--- DEBUG: Validating Parameters ---" << std::endl;
	if (!pp.valid())
	{
		return -1;
	}
	if (debug) std::cout << "--- DEBUG: Validation Complete ---" << std::endl;
	if (debug) std::cout << "--- DEBUG: Gathering Parameters Completed ---\n" << std::endl;

	// Initialize the key generator for the rounds
	if (debug) std::cout << "--- DEBUG: Gathering/Generating keys ---" << std::endl;
	keygen kg(key, pp, debug);

	if (kg.had_error())
		return -1;
	if (debug) std::cout << "--- DEBUG: Gathering/Generating keys Complete ---\n" << std::endl;

	// Initialize and carry out the cipher
	if (debug) std::cout << "--- DEBUG: Starting Cipher ---" << std::endl;
	cipher cp(input, kg, pp, output, encrypt, inter, hex, debug);
	if (debug) std::cout << "--- DEBUG: Cipher completed ---\n" << std::endl;

	if (debug) std::cout << "--- DEBUG: Closing program ---" << std::endl;

	return 0;
}

