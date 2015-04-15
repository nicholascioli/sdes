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
	void print2d(std::vector<std::vector<T>> out)
	{
		if (to_std)
		{
			std::cout << "Printing to stdout:" << std::endl;
			for (unsigned int i = 0; i < out.size(); ++i)
				for (unsigned int j = 0; j < out[i].size(); ++j)
					std::cout << out[i][j];

			std::cout << std::endl;
		}
		else
		{
			std::cout << "Printing to file." << std::endl;

			for (unsigned int i = 0; i < out.size(); ++i)
				for (unsigned int j = 0; j < out[i].size(); ++j)
					output.write((char*)&(out[i][j]), sizeof(short));

			output << '\0';
		}
	}

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
				output.write((char*)&(out[i]), sizeof(short));

			output << '\0';
		}
	}

	// Permutates the first vector by the second
	template <typename T>
	std::vector<T> perm(std::vector<T> data, std::vector<unsigned int> pm, bool d=true)
	{
		std::vector<T> result;

		if (debug && d) 
		{
			std::cout << "--- DEBUG: Permutation of ";
			for (unsigned int i = 0; i < data.size(); ++i)
				std::cout << data[i];
			std::cout << " with ";
			for (unsigned int i = 0; i < pm.size(); ++i)
				std::cout << pm[i];
			std::cout << " gives ";
		}

		for (unsigned int i = 0; i < pm.size(); ++i)
		{
			result.push_back(data[pm[i]-1]);
			if (debug && d) std::cout << result[i];
		}

		if (debug && d) std::cout << " ---" << std::endl;

		return result;
	}

	// Return a vector containing the values selected from in through sel
	template<typename T>
	std::vector<T> select(std::vector<T> in, std::vector<T> sel)
	{
		if (debug) std::cout << "--- DEBUG: Starting select ---" << std::endl;
		std::vector<T> result;

		for (unsigned int i = 0; i < sel.size(); ++i)
		{
			result.push_back(in[sel[i]-1]);
		}

		if (debug) std::cout << "--- DEBUG: End of select ---" << std::endl;
		return result;
	}

	// Swaps left and right side
	template <typename T>
	std::vector<T> swap(std::vector<T> vec)
	{
		std::vector<T> result;

		std::vector<T> left(vec.begin(), vec.begin() + (vec.size() / 2));
		std::vector<T> right(vec.begin() + (vec.size() / 2), vec.end());

		// Swap the sides
		result.insert(result.begin(), right.begin(), right.end());
		result.insert(result.end(), left.begin(), left.end());

		return result;
	}

	// SBox & PBox function
	template <typename T>
	std::vector<T> boxes(std::vector<T> in)
	{
		if (debug) std::cout << "--- DEBUG: Starting boxes ---" << std::endl;
		std::vector<T> result;
		std::vector<std::vector<T>> sbox = pp.get_params().sbox;
		std::vector<T> pbox = pp.get_params().p_perm;
		unsigned int number_sbox = pp.get_params().n_sbox;
		unsigned int rnd_key_size = pp.get_params().s_rkey;
		unsigned int block_size = pp.get_params().s_blk;
		std::vector<T> select_row = pp.get_params().row_b;
		std::vector<T> select_col = pp.get_params().col_b;
		std::vector<T> rows;
		std::vector<T> cols;

		if (debug)
		{
			std::cout << "--- DEBUG: Input of ";
			for (unsigned int i = 0; i < in.size(); ++i)
				std::cout << in[i] << " ";
			std::cout << " received ---" << std::endl;
		}

		// Split the entries into number_of_round equal bits
		std::string temp;
		for (unsigned int i = 0; i < number_sbox; ++i)
		{
			temp = "";
			for (unsigned int j = 0; j < (rnd_key_size / number_sbox); ++j)
			{
				temp += std::to_string(in[i*(rnd_key_size / number_sbox) + j]);
			}

			// Get the selected rows / columns from the input (in integer format)
			rows.push_back(
				conv.b2i(
					select<unsigned int>(conv.s2v(temp, '0'), select_row)
				)
			);
			cols.push_back(
				conv.b2i(
					select<unsigned int>(conv.s2v(temp, '0'), select_col)
				)
			);
			if (debug) std::cout << "--- DEBUG: Gathered the following row / column for SBox " 
				<< i+1 << " ---" << std::endl;
			if (debug) std::cout << rows[i] << "|" << cols[i] << std::endl;
		}

		// Get the corresponding entries in the sboxes (in integer format)
		temp = "";
		for (unsigned int i = 0; i < number_sbox; ++i)
		{
			temp += std::to_string(
				sbox[
				(2<<(((rnd_key_size/number_sbox)-(block_size/(2*number_sbox)))*i)-1)+rows[i]	// Height of each Sbox is 
				][																				// R/T - B/2T
				cols[i]
				]
			);
		}
		if (debug) std::cout << "--- DEBUG: Got output of " << temp << " from sbox(es) ---" << std::endl;

		// Convert the integer representations to binary 
		// & Permutate that with PBox permutation
		result = perm<unsigned int>(
			conv.s2v(
				conv.h2a(conv.s2bv(temp, (rnd_key_size / number_sbox)))
			, '0')
		, pbox);
		
		if (debug)
		{ 
			std::cout << "--- DEBUG: boxes() shows ";
			for (unsigned int i = 0; i < result.size(); ++i)
				std::cout << result[i];
			std::cout << " ---" << std::endl;
		}

		return result;
	}

	// Mangler function
	template <typename T> 
	std::vector<T> mangler(unsigned int rnd, std::vector<T> right, 
		std::vector<std::vector<unsigned int>> rnd_keys)
	{
		if (debug) std::cout << "--- DEBUG: Starting Mangler with round key of ";
		if (debug)
		{
			for (unsigned int i = 0; i < rnd_keys[rnd].size(); ++i)
				std::cout << rnd_keys[rnd][i];
			std::cout << std::endl;
		}

		std::vector<T> result;
		std::vector<T> box;
		std::vector<T> permutated = perm<T>(right, pp.get_params().e_perm);

		// XOR the permutated right side with the round key
		if (debug) std::cout << "--- DEBUG: Mangled output: ";
		for (unsigned int i = 0; i < permutated.size(); ++i)
		{
			box.push_back(permutated[i] ^ rnd_keys[rnd][i]);
			if (debug) std::cout << box[i];
		}
		if (debug) std::cout << " ---" << std::endl;

		// Run it through both the sbox and pbox
		result = boxes(box);

		return result;
	}

	// Function to do the SBox stuff
	template <typename T>
	std::vector<T> stage3(unsigned int rnd, std::vector<T> left, std::vector<T> right, 
		std::vector<std::vector<unsigned int>> rnd_keys)
	{
		if (debug) std::cout << "--- DEBUG: Starting Stage 3 ---" << std::endl;
		std::vector<T> result;
		
		// First mangle the right side
		std::vector<T> mangled = mangler<T>(rnd, right, rnd_keys);

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
	std::vector<T> stage2(std::vector<T> vec, unsigned int rnd,
		std::vector<std::vector<unsigned int>> rnd_keys)
	{
		if (debug) std::cout << "--- DEBUG: Starting Stage 2 ---" << std::endl;
		unsigned int block_size = pp.get_params().s_blk;
		std::vector<T> result;

		// Split up permutated data into left and right halves
		std::vector<T> left(vec.begin(), vec.begin() + (block_size / 2));
		std::vector<T> right(vec.begin() + (block_size / 2), vec.end());

		// Do Feistall thing
		result.insert(result.begin(), right.begin(), right.end()); 			// Left side gets original right side
		
		left = stage3<T>(rnd, left, right, rnd_keys);

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

	// Cipher, in all of its glory 
	std::vector<std::vector<unsigned int>> do_cipher(std::vector<std::vector<unsigned int>> vec, 
		std::vector<std::vector<unsigned int>> rnd_keys)
	{
		unsigned int rnds = pp.get_params().rnds;
		std::vector<std::vector<unsigned int>> rounds;
		std::vector<std::vector<unsigned int>> init_perm;
		std::vector<std::vector<unsigned int>> result;

		// Run on all possible blocks
		for (unsigned int b = 0; b < vec.size(); ++b)
		{
			rounds.clear();
			init_perm.clear();

			if (debug) std::cout << "--- DEBUG: Permutating initially ---" << std::endl;
			init_perm.push_back(perm<unsigned int>(vec[b], pp.get_params().i_perm));

			if (debug) 
			{
				std::cout << "--- DEBUG: Starting cipher on input ";
				for (unsigned int i = 0; i < vec[b].size(); ++i)
					std::cout << vec[b][i];
				std::cout << " ---" << std::endl;
			}

			for (unsigned int r = 0; r < rnds; ++r)
			{
				if (debug) std::cout << "\n--- DEBUG: Starting Round " << (r + 1) << " ---" << std::endl;

				for (unsigned int i = 0; i < vec.size(); ++i)
				{
					// Initial Permutation on first round
					if (rounds.size() != 0)
						init_perm.push_back(rounds.back());

					// Do round
					rounds.push_back(stage2<unsigned int>(init_perm[r], r, rnd_keys));
				}

				if (debug) std::cout << "--- DEBUG: End of Round " << (r + 1) << " ---\n" << std::endl;
			}

			if (debug) std::cout << "--- DEBUG: Finished encrypting " << str_input << " ---" << std::endl;

			// Swap the left and right sides
			// & Permutate the scrambled result with the inverse initial permutation
			result.push_back(perm<unsigned int>(swap<unsigned int>(rounds.back()), pp.get_params().ii_perm, false));

			if (debug)
			{
				std::cout << "--- DEBUG: End of cipher with the following ---";
				for (unsigned int i = 0; i < result[b].size(); ++i)
					std::cout << result[b][i];
				std::cout << std::endl;
			}
		}

		return result;
	}

	// Function to encrypt
	void en()
	{
		std::vector<std::vector<unsigned int>> vec;
		std::vector<std::vector<unsigned int>> rnd_keys = kg.get_rnd_keys();

		// Encrypt from the stdin
		if (from_std)
		{
			// Convert string to hex or binary equivalent
			if (hex)
			{
				if (debug) std::cout << "--- DEBUG: Getting hex equivalent of \'" << str_input << "\' ---" << std::endl;
				vec = conv.a2vv(str_input, pp.get_params().s_blk);
			}
			else
			{
				if (debug) std::cout << "--- DEBUG: Getting binary equivalent of \'" << str_input << "\' ---" << std::endl;
				vec = conv.s2bv(str_input, pp.get_params().s_blk);
			}
		}
		// Encrypt from a file
		else
		{
			// Get the input
			std::string in = "";
			char *c = new char[sizeof(short)];	// Grab by sizeof(short)
			while (input.read(c, sizeof(short)))
			{
				in += std::to_string(*c);
			}
			delete[] c;

			if (debug) std::cout << "--- DEBUG: Got input of " << in << " ---" << std::endl;

			if (hex)
				vec = conv.a2vv(in, pp.get_params().s_blk);
			else
				vec = conv.bs2bv(in, pp.get_params().s_blk);
		}

		print2d<unsigned int>(do_cipher(vec, rnd_keys));
	}

	// Function to decrypt
	void de()
	{
		std::vector<std::vector<unsigned int>> vec;
		std::vector<std::vector<unsigned int>> rnd_keys = kg.get_rnd_keys_reverse();

		// Decrypt from the stdin
		if (from_std)
		{
			if (hex)
			{
				if (debug) std::cout << "--- DEBUG: Getting hex equivalent of \'" << str_input << "\' ---" << std::endl;
				vec = conv.a2vv(str_input, pp.get_params().s_blk);
			}
			else
			{
				// Convert string to binary equivalent
				if (debug) std::cout << "--- DEBUG: Getting binary equivalent of \'" << str_input << "\' ---" << std::endl;
				vec = conv.bs2bv(str_input, pp.get_params().s_blk);
			}
		}
		// Decrypt from a file
		else
		{
			// Get the input
			std::string in = "";
			char *c = new char[sizeof(short)];	// Grab by sizeof(short)
			while (input.read(c, sizeof(short)))
			{
				in += std::to_string(*c);
			}
			delete[] c;

			if (debug) std::cout << "--- DEBUG: Got input of " << in << " ---" << std::endl;

			if (hex)
				vec = conv.a2vv(in, pp.get_params().s_blk);
			else
				vec = conv.bs2bv(in, pp.get_params().s_blk);
		}

		print<unsigned int>(conv.bv2iv(do_cipher(vec, rnd_keys)));
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