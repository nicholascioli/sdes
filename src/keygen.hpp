#ifndef __H_KEYGEN_H__
#define __H_KEYGEN_H__

#include "includes.hpp"
#include "a_h_b_converter.hpp"

class keygen
{
private:

	std::string str_key;				// String representation of key
	std::vector<unsigned int> key;		// Master Key
	std::vector<unsigned int> ekey;		// Effective key
	unsigned int l_key;					// Length of key

	std::vector<std::vector<unsigned int>> round_keys;
	
	pparser pp;
	converter cnv;

	bool debug;
	bool error;

public:

	keygen(std::string key_path, pparser m_pp, bool dbg=false)
	{ 
		// Setup defaults
		error = false;
		debug = dbg;

		// Get the parameters
		pp = m_pp;

		// Open the file where the key is stored
		// & Get the key
		std::ifstream temp;
		temp.open(key_path, std::ifstream::in);
		if (temp.is_open())
		{
			l_key = pp.get_params().s_key;
			char buffer[l_key];
			temp.getline(buffer, l_key+1);
			str_key = buffer;
			temp.close();
		}
		else
		{
			std::cerr << "ERROR: Could not open key file" << std::endl;
			error = true;
		}

		// Get the master key
		key = cnv.a2v(str_key);

		if (debug) std::cout << "--- DEBUG: Got key of \'" << cnv.h2a(key) << "\'" << std::endl;

		// Get the round keys
		for (unsigned int i = 0 ; i < pp.get_params().rnds; ++i)
			round_keys.push_back(gen_round(pp.get_params().rnds));
	}

	std::vector<unsigned int> gen_round(unsigned int m_round)
	{
		// Get the appropriate permutation & round shifts
		std::vector<int> shifts = pp.get_params().l_rot;
		std::vector<unsigned int> perm = pp.get_params().pc[1];

		if (m_round == 0)
			for (unsigned int i = 0; i < pp.get_params().pc[0].size(); ++i)
				ekey.push_back(key[pp.get_params().pc[0][i]-1]);

		std::vector<unsigned int> shifted(ekey);

		// Perform the shifts
		unsigned int temp;
		for (unsigned int i = 0; i < (abs(shifts[m_round])); ++i)
		{
			unsigned int l_begin = shifted[0];
			unsigned int r_begin = shifted[shifted.size()/2];

			// Apply the shifts
			for (unsigned int j = 0; j < shifted.size(); ++j)
			{
				while (j + 1 < shifted.size() / 2)		// Shift left side
				{
					if (debug) std::cout << "--- DEBUG: Changing left character " 
						<< shifted[j] << " for " << shifted[j+1] << " ---" << std::endl;

					shifted[j] = shifted[j+1];
					++j;
				}
				shifted[j++] = l_begin;
				while (j + 1 < shifted.size())			// Shift right side
				{
					if (debug) std::cout << "--- DEBUG: Changing right character " 
						<< shifted[j] << " for " << shifted[j+1] << " ---" << std::endl;

					shifted[j] = shifted[j+1];
					++j;
				}
				shifted[j] = r_begin;
			}
		}

		if (debug) std::cout << "--- DEBUG: Generated shifted key of: " << cnv.h2a(shifted) << " ---" << std::endl;

		// Generate the round key by permutating through PC2 on the shifted values
		std::vector<unsigned int> rnd_key;

		for (unsigned int i = 0; i < perm.size(); ++i)
		{
			rnd_key.push_back(shifted[perm[i]-1]);
		}

		if (debug) std::cout << "--- DEBUG: Round key of \'" << cnv.h2a(rnd_key) << "\' ---" << std::endl;

		// Return the round key
		return rnd_key;
	}

	std::vector<unsigned int> get_key() const
	{
		return key;
	}

	std::vector<unsigned int> get_rnd_key(unsigned int rnd) const
	{
		return round_keys[rnd];
	}

	std::vector<std::vector<unsigned int>> get_rnd_keys() const
	{
		return round_keys;
	}

	std::string get_str_key() const
	{
		return str_key;
	}

	keygen(const keygen& other)
	{
		this->str_key = other.get_str_key();
		this->key = other.get_key();
		this->l_key = this->key.size();

		this->round_keys = other.get_rnd_keys();
	}

	keygen() {}

	// Check if error was reported during key generating process
	bool had_error()
	{
		return error;
	}
};


#endif //__H_KEYGEN_H__
