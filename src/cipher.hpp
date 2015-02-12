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

	// Prints to output
	template <typename T>
	void print(std::vector<T> out)
	{
		if (to_std)
		{
			std::cout << "Printing to stdout:" << std::endl;
			for (unsigned int i = 0; i < out.size(); ++i)
				std::cout << out[i];

			std::cout << std::endl;
		}
		else
		{
			std::cout << "Printing to file." << std::endl;

			for (unsigned int i = 0; i < out.size(); ++i)
				output << out[i];

			output << std::endl;
		}
	}

	// Permutates the first vector by the second
	template <typename T>
	std::vector<T> perm(std::vector<T> data, std::vector<unsigned int> pm)
	{
		std::vector<T> result;

		if (debug) std::cout << "--- DEBUG: Permutation of ";
		for (unsigned int i = 0; i < pm.size(); ++i)
		{
			result.push_back(data[pm[i]-1]);
			if (debug) std::cout << result[i];
		}
		if (debug) std::cout << std::endl;

		return result;
	}

	// SBox & PBox function
	template <typename T>
	std::vector<T> boxes(std::vector<T> in)
	{
		std::vector<T> result;
		std::vector<std::vector<T>> sbox = pp.get_params().sbox;
		unsigned int number_sbox = pp.get_params().n_sbox;
		unsigned int rnd_key_size = pp.get_params().s_rkey;
		unsigned int block_size = pp.get_params().s_blk;
		std::vector<T> select_row = pp.get_params().row_b;
		std::vector<T> select_col = pp.get_params().col_b;
		std::vector<T> rows;
		std::vector<T> cols;

		for (unsigned int i = 0; i < in.size(); ++i)
			std::cout << in[i] << " ~ ";
		std::cout << std::endl;

		// Get the selected rows
		for (unsigned int j = 0; j < select_row.size(); ++j)
		{
			rows.push_back(in[select_row[j]-1]);
			cols.push_back(in[select_col[j]-1]);

			std::cout << rows[j] << " # ";
		}
		std::cout << std::endl;

		/*

		// Get decimal equivalent of row and pass that to result
		for (unsigned int i = 0; i < number_sbox; ++i)
		{
			if (debug) std::cout << "--- DEBUG: " << conv.b2i(rows) << " | " << conv.b2i(cols) << " ---" << std::endl;
			result.insert(result.end(), 
				conv.i2b(sbox[(conv.b2i(rows)) 
					+ (i * ((rnd_key_size / number_sbox) - (block_size / (2 * number_sbox))))]
					[(conv.b2i(cols))], number_sbox).begin(),
				conv.i2b(sbox[(conv.b2i(rows)) 
					+ (i * ((rnd_key_size / number_sbox) - (block_size / (2 * number_sbox))))]
					[(conv.b2i(cols))], number_sbox).end());
		}



		if (debug)
		{ 
			std::cout << "--- DEBUG: boxes() shows ";
			for (unsigned int i = 0; i < result.size(); ++i)
				std::cout << result[i];
			std::cout << " ---" << std::endl;
		}

		*/
		return result;
	}

	// Mangler function
	template <typename T> 
	std::vector<T> mangler(unsigned int rnd, std::vector<T> right)
	{
		std::vector<T> result;
		std::vector<T> box;
		std::vector<unsigned int> rnd_key = kg.get_rnd_key(rnd);
		std::vector<T> permutated = perm<T>(right, pp.get_params().e_perm);

		// XOR the permutated right side with the round key
		for (unsigned int i = 0; i < permutated.size(); ++i)
		{
			box.push_back(permutated[i] ^ rnd_key[i]);
		}

		// Run it through both the sbox and pbox
		result = boxes(box);

		return result;
	}

	// Function to do the SBox stuff
	template <typename T>
	std::vector<T> stage3(unsigned int rnd, std::vector<T> left, std::vector<T> right)
	{
		std::vector<T> result;
		
		// First mangle the right side
		std::vector<T> mangled = mangler<T>(rnd, right);

		// Then XOR the right and left sides together to get the input for sboxes
		for (unsigned int i = 0; i < left.size(); ++i)
		{
			result.push_back(left[i] ^ mangled[i]);
		}

		if (debug) 
		{
			std::cout << "--- DEBUG: Stage 3 completed with ";
			for (unsigned int i = 0; i < result.size(); ++i)
				std::cout << result[i];
			std::cout << " ---" << std::endl;
		}

		return result;
	}

	// Function to carry out the second half of the encryption process
	template <typename T>
	std::vector<T> stage2(std::vector<T> vec, unsigned int rnd)
	{
		unsigned int block_size = pp.get_params().s_blk;
		std::vector<T> result;

		// Split up permutated data into left and right halves
		std::vector<T> left(vec.begin(), vec.begin() + (block_size / 2));
		std::vector<T> right(vec.begin() + (block_size / 2), vec.end());

		// Do Feistall thing
		result.insert(result.begin(), right.begin(), right.end()); 			// Left side gets original right side
		
		left = stage3<T>(rnd, left, right);

		result.insert(result.end(), left.begin(), left.end());				// Right side gets L XOR mangler(K, R)

		if (debug)
		{
			std::cout << "--- DEBUG: Stage 2 ended with ";
			for (unsigned int i = 0; i < result.size(); ++i)
				std::cout << result[i];
			std::cout << " ---" << std::endl;
		}	

		return result;
	}

	// Function to encrypt 
	void en()
	{
		unsigned int rnds = pp.get_params().rnds;
		std::vector<std::vector<unsigned int>> rounds;
		std::vector<std::vector<unsigned int>> init_perm;
		std::vector<std::vector<unsigned int>> vec;

		// Encrypt from the stdin
		if (from_std)
		{
			// Convert string to binary equivalent
			vec = conv.s2bv(str_input, pp.get_params().s_blk);
		}
		// Encrypt from a file
		else
		{
			// Get the input
			std::string in = "";
			char c;
			while (input.get(c))
				in += c;

			std::vector<std::vector<unsigned int>> vec = conv.s2bv(in, pp.get_params().s_blk);
		}

		for (unsigned int r = 0; r < rnds; ++r)
		{
			if (debug) std::cout << "\n--- DEBUG: Starting Round " << r << " ---" << std::endl;

			for (unsigned int i = 0; i < vec.size(); ++i)
			{
				// Initial Permutation on first round
				if (rnds == 0)
					init_perm.push_back(perm<unsigned int>(vec[i], pp.get_params().i_perm));

				// Do round
				rounds.push_back(stage2<unsigned int>(init_perm[i], r));
			}

			if (debug) std::cout << "--- DEBUG: End of Round " << r << " ---\n" << std::endl;
		}

		if (debug) std::cout << "--- DEBUG: Finished encrypting " << str_input << " ---" << std::endl;

		// Permutate the scrambled result with the inverse initial permutation
		// & print out the result;
		print<unsigned int>(perm<unsigned int>(rounds.back(), pp.get_params().ii_perm));
	}

	// Function to decrypt
	void de()
	{
		if (from_std)
		{

		}
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