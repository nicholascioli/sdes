#ifndef __H_PPARSER_H__
#define __H_PPARSER_H__

#include "includes.hpp"

class pparser
{
public:
	struct parameters
	{
		unsigned int s_blk;								// Block size
		unsigned int s_key;								// Key size
		unsigned int s_ekey;							// Effective key size
		unsigned int s_rkey;							// Round key size
		unsigned int rnds;								// Number of rounds

		std::vector<unsigned int> pc1;					// PC1 permutations
		std::vector<unsigned int> pc2;					// PC2 permutations
		std::vector<int> l_rot;							// Left rotation schedule
		std::vector<unsigned int> i_perm;				// Initial permutation
		std::vector<unsigned int> ii_perm;				// Inverse of initial permutation
		std::vector<unsigned int> e_perm;				// Expansion permutation
		std::vector<unsigned int> p_perm;				// P-box transposition permutation

		unsigned int n_sbox;							// Number of S-boxes

		std::vector<unsigned int> row_b;				// Row selection bits
		std::vector<unsigned int> col_b;				// Column selection bits

		std::vector<std::vector<unsigned int>> sbox;	// S-box substitutions

		parameters() {}
	};

private:

	std::ifstream params;
	bool debug;
	unsigned int n_params;

	// Parameters
	parameters p;

	template <typename T>
	std::vector<T> str2vec(const std::string str)
	{
		char temp;
		std::vector<T> result;

		for (T i = 0; i < str.length(); ++i)
		{
			temp = str[i];

			if (temp != ' ' && temp != '\t')
			{
				if ((temp - '0') > 9)		// Check if hexadecimal input
					result.push_back((10 + (temp - 'A')));
				else
					result.push_back((temp - '0'));
			}
		}

		return result;
	}

	template <typename T>
	std::string vec2str(std::vector<T> vec) const
	{
		std::string result = "[ ";
		for (T i = 0; i < vec.size(); ++i)
		{
			result += std::to_string(vec[i]);
			result += " ";
		}

		result += "]";

		return result;
	}

	template <typename T>
	std::string twoDvec2str(std::vector<std::vector<T>> vec) const
	{
		std::string result = "{ ";

		for (T i = 0; i < vec.size(); ++i)
		{
			result += vec2str(vec[i]);
			if (i + 1 != vec.size()) result += ", ";
		}

		result += " }";

		return result;
	}

	std::vector<unsigned int> inverse_perm(std::vector<unsigned int> orig)
	{
		std::vector<unsigned int> result(orig.size(),0);

		for (unsigned int i = 0; i < orig.size(); ++i)
			result[orig[i]-1] = (i + 1);

		return result;
	}

	void add_sbox(std::string new_row)
	{
		std::vector<unsigned int> row = str2vec<unsigned int>(new_row);
		p.sbox.push_back(row);
	}

	void add_param(std::string new_param)
	{
		if (new_param != "")
		{
			if (n_params < 14)
			{
				switch(n_params)
				{
					case 0:		// BLOCK SIZE
						if (debug) std::cout << "--- DEBUG: Setting BLOCK SIZE to " << new_param << std::endl;
						p.s_blk = atoi(new_param.c_str());
						break;
					case 1:		// KEY SIZE
						if (debug) std::cout << "--- DEBUG: Setting KEY SIZE to " << new_param << std::endl;
						p.s_key = atoi(new_param.c_str());
						break;
					case 2:		// EFFECTIVE KEY SIZE
						if (debug) std::cout << "--- DEBUG: Setting EFFECTIVE KEY SIZE to " << new_param << std::endl;
						p.s_ekey = atoi(new_param.c_str());
						break;
					case 3:		// ROUND KEY SIZE
						if (debug) std::cout << "--- DEBUG: Setting ROUND KEY SIZE to " << new_param << std::endl;
						p.s_rkey = atoi(new_param.c_str());
						break;
					case 4:		// ROUNDS
						if (debug) std::cout << "--- DEBUG: Setting ROUNDS to " << new_param << std::endl;
						p.rnds = atoi(new_param.c_str());
						break;
					case 5:		// PC1
						if (debug) std::cout << "--- DEBUG: Setting PC1 to " << new_param << std::endl;
						p.pc1 = str2vec<unsigned int>(new_param);
						break;
					case 6:		// PC2
						if (debug) std::cout << "--- DEBUG: Setting PC2 to " << new_param << std::endl;
						p.pc2 = str2vec<unsigned int>(new_param);
						break;
					case 7:		// Left rotation
						if (debug) std::cout << "--- DEBUG: Setting LEFT ROTATION to " << new_param << std::endl;
						p.l_rot = str2vec<int>(new_param);
						break;
					case 8:		// Initial Permutation
						if (debug) std::cout << "--- DEBUG: Setting INITIAL PERMUTATION to " << new_param << std::endl;
						p.i_perm = str2vec<unsigned int>(new_param);
						break;
					case 9:		// Expansion permutation
						if (debug) std::cout << "--- DEBUG: Setting EXPANSION PERMUTATION to " << new_param << std::endl;
						p.e_perm = str2vec<unsigned int>(new_param);
						break;
					case 10:	// P-box transposition
						if (debug) std::cout << "--- DEBUG: Setting P-BOX TRANSPOSITION to " << new_param << std::endl;
						p.p_perm = str2vec<unsigned int>(new_param);
						break;
					case 11:	// Number of S-Boxes
						if (debug) std::cout << "--- DEBUG: Setting NUMBER OF S-BOXES to " << new_param << std::endl;
						p.n_sbox = atoi(new_param.c_str());
						break;
					case 12:	// Row selection bits
						if (debug) std::cout << "--- DEBUG: Setting ROW SELECTION BITS to " << new_param << std::endl;
						p.row_b = str2vec<unsigned int>(new_param);
						break;
					case 13:	// Column selection bits
						if (debug) std::cout << "--- DEBUG: Setting COLUMN SELECTION BITS to " << new_param << std::endl;
						p.col_b = str2vec<unsigned int>(new_param);
						break;
				}
			}
			else
			{
				unsigned int t = (p.rnds * (2 << ((p.s_rkey / p.n_sbox) - (p.s_blk / (2 * p.n_sbox))) - 1));
				if (n_params - 13 <= t)
				{
					if (debug) std::cout << "--- DEBUG: Adding row \'" << new_param << "\' to sbox ---" << std::endl;
					add_sbox(new_param);
				}
				else
					std::cerr << "ERROR: Too many parameters detected! Please remove \'" 
						<< new_param << "\'" << std::endl;
			}

			++n_params;
		}
	}

	// Some helper functions
	template <typename T>
	void below(bool& result, std::string err, T compare, std::vector<T> vec)
	{
		for (typename std::vector<T>::iterator first = vec.begin(), 
			last = vec.end(); first != last; ++first)
		{
			if (*first > compare)
			{
				result = false;
				std::cerr << "ERROR: Entry \'" << *first << "\' for " << err << " is invalid."
					<< " Entry cannot be greater than \'" << compare << "\'" << std::endl;
			}
		}
	}

	template <typename T>
	void between(bool& result, std::string err, T upper, T lower, std::vector<T> vec)
	{
		for (typename std::vector<T>::iterator first = vec.begin(), 
			last = vec.end(); first != last; ++first)
		{
			if (*first > upper || *first < lower)
			{
				result = false;
				std::cerr << "ERROR: Entry \'" << *first << "\' for " << err << " is invalid."
					<< " Entry must be between \'" << upper << "\' & \'" << lower << "\'" << std::endl;
			}
		}
	}

	template <typename T>
	void equal(bool& result, std::string err, T c1, T c2)
	{
		if (c1 != c2)
		{
			result = false;
			std::cerr << "ERROR: Invalid amount of entires for " << err << ". Found \'" << c2 
				<< "\' expected \'" << c1 << "\'" << std::endl;
		}
	}

	void unique(bool& result, std::string err, 
		std::vector<unsigned int> v1, std::vector<unsigned int> v2=std::vector<unsigned int>())
	{
		std::vector<unsigned int> vec;
		if (!v2.empty())
			vec.insert(vec.begin(), v2.begin(), v2.end());
		
		vec.insert(vec.begin(), v1.begin(), v1.end());

		for (unsigned int i = 0; i < vec.size(); ++i)
		{
			for (unsigned int j = i + 1; j < vec.size(); ++j)
			{
				if (vec[i] == vec[j])
				{
					result = false;
					std::cerr << "ERROR: Entry \'" << vec[i] << "\' in " << err
						<< " is repeated! (Must be unique)" << std::endl;
				}
			}
		}
	}

public:

	bool const get_debug_flag() const
	{
		return debug;
	}

	unsigned int const get_n_params() const
	{
		return n_params;
	}

	parameters const get_params() const
	{
		return p;
	}

	std::ostream& print(std::ostream& os) const
	{
		os 	<< "Block Size:\t\t" 				<< p.s_blk
			<< "\nKey Size:\t\t" 				<< p.s_key
			<< "\nEffective Key Size:\t"		<< p.s_ekey
			<< "\nRound Key Size:\t\t"			<< p.s_rkey
			<< "\nNumber of Rounds:\t"			<< p.rnds
			<< "\nPC-1:\t\t\t"					<< vec2str<unsigned int>(p.pc1)
			<< "\nPC-2:\t\t\t"					<< vec2str<unsigned int>(p.pc2)
			<< "\nLeft Rotation Schedule:\t"	<< vec2str<int>(p.l_rot)
			<< "\nInitial Permutation:\t"		<< vec2str<unsigned int>(p.i_perm)
			<< "\nInverse Permutation:\t"		<< vec2str<unsigned int>(p.ii_perm)
			<< "\nExpansion Permutation:\t"		<< vec2str<unsigned int>(p.e_perm)
			<< "\nP-box transposition:\t"		<< vec2str<unsigned int>(p.p_perm)
			<< "\nNumber of S-boxes:\t"			<< p.n_sbox
			<< "\nRow selection bits\t"			<< vec2str<unsigned int>(p.row_b)
			<< "\nColumn selection bits:\t"		<< vec2str<unsigned int>(p.col_b)	
			<< "\nS-box substitutions:\t"		<< twoDvec2str<unsigned int>(p.sbox)
			<< std::endl;

		return os;
	}

	pparser(std::string Params, bool dbg) 
	{
		// Initialize some things
		n_params = 0;
		debug = dbg;

		if (debug) std::cout << "--- DEBUG: Initializing pparser ---" << std::endl;

		params.open(Params.c_str(), std::ifstream::in);

		if (params)
		{
			if (debug) std::cout << "--- DEBUG: Parameter file opened ---" << std::endl;

			std::string temp;
			char t2;

			// Extract parameters from data
			while (params.get(t2))
			{
				temp += t2;

				if (t2 == '\n' || t2 == '/' || t2 == '\t')
				{
					temp.pop_back();
					if (debug) std::cout << "--- DEBUG: Extracted Values: ---\n" << temp << std::endl;

					while (t2 != '\n' && params.get(t2)) {}

					add_param(temp);

					temp = "";
				}
			}

			// Get inverse of initial permutation
			p.ii_perm = inverse_perm(p.i_perm);

			if (debug) {std::cout << "--- DEBUG: Printing contents of p ---" << std::endl; print(std::cout);}
			
			// Close file
			params.close();
		}
	}

	pparser()
	{
		n_params = 0;
		debug = false;
	}

	pparser(const pparser& other)
	{
		this->debug = other.get_debug_flag();
		this->n_params = other.get_n_params();
		this->p = other.get_params();
	}

	pparser& operator=(pparser rhs)
	{
		this->debug = rhs.get_debug_flag();
		this->n_params = rhs.get_n_params();
		this->p = rhs.get_params();

		return *this;
	}

	bool valid()
	{
		bool result = true;

		// Check if correct amount of parameters present
		equal<unsigned int>(result, "NUMBER OF PARAMETERS", 
			(14 + (p.rnds * (2 << ((p.s_rkey / p.n_sbox) - (p.s_blk / (2 * p.n_sbox))) - 1))), n_params);
		
		// Check that PC1 has s_ekey entries
		equal<unsigned int>(result, "PC1", p.s_ekey, p.pc1.size());

		// Check that each entry in PC1 is not greater than the key size
		below<unsigned int>(result, "PC1", p.s_key, p.pc1);

		// Check that PC2 has s_rkey entries
		equal<unsigned int>(result, "PC2", p.s_rkey, p.pc2.size());

		// Check that each entry in PC2 is not greater than the effective key size
		below<unsigned int>(result, "PC2", p.s_ekey, p.pc2);

		// Check that Left rotations has rnds entries
		equal<int>(result, "LEFT ROTATIONS", p.rnds, p.l_rot.size());

		// Check that each entry in Left rotations is between ((-QE / 2) + 1) & ((QE / 2) - 1) (QE = s_ekey)
		between<int>(result, "LEFT ROTATIONS", (((int) p.s_ekey / 2) - 1), ((-1 * (int) p.s_ekey / 2) + 1), p.l_rot);

		// Check that Initial permutations has Block size entries
		equal<unsigned int>(result, "INITIAL PERMUTATIONS", p.s_blk, p.i_perm.size());

		// Check that each entry in Initial permutations is unique
		unique(result, "INITIAL PERMUTATIONS", p.i_perm);

		// Check that Expansion permutations has s_rkey entries
		equal<unsigned int>(result, "EXPANSION PERMUTATIONS", p.s_rkey, p.e_perm.size());

		// Check that each entry in expansion permutations is between 1 and B/2
		between<unsigned int>(result, "EXPANSION PERMUTATIONS", (p.s_blk / 2), 1, p.e_perm);

		// Check that P-boxes permutations has Block size / 2 entries
		equal<unsigned int>(result, "P-BOXES PERMUTATIONS", (p.s_blk / 2), p.p_perm.size());

		// Check that each entry in P-boxes permutations is between 1 and B/2
		between<unsigned int>(result, "P-BOXES PERMUTATIONS", (p.s_blk / 2), 1, p.p_perm);

		// Check that each entry in P-boxes permutations is unique
		unique(result, "P-BOXES PERMUTATIONS", p.p_perm);

		// Check that number of S-boxes evenly divides round key size
		equal<unsigned int>(result, "NUMBER OF S-BOXES", 0, (p.s_rkey % p.n_sbox));

		// Check that Row selection bits has R/2 - B/2T entries
		equal<unsigned int>(result, "ROW SELECTION BITS", 
			((p.s_rkey / p.n_sbox) - (p.s_blk / (2 * p.n_sbox))), p.row_b.size());

		// Check that each entry in Row selection bits is between 1 and R/T
		between<unsigned int>(result, "ROW SELECTION BITS", (p.s_rkey / p.n_sbox), 1, p.row_b);

		// Check that each entry in Row selection bits is unique
		unique(result, "ROW SELECTION BITS", p.row_b);

		// Check that Column selection bits has B/2T entries
		equal<unsigned int>(result, "COLUMN SELECTION BITS", (p.s_blk / (2 * p.n_sbox)), p.col_b.size());

		// Check that each entry in column selection bits is between R/T and 1
		between<unsigned int>(result, "COLUMN SELECTION BITS", (p.s_rkey / p.n_sbox), 1, p.col_b);

		// Check that each entry in column selection bits is unique to itself and to row selection bits
		unique(result, "COLUMN SELECTION BITS", p.col_b, p.row_b);

		// Check that each row in SBox has 2^y entries, where y = B/2T
		// & Check that each entry in each row of SBox is between 1 & 2^y
		std::string temp;
		unsigned int y = (2 << (p.s_blk / (2 * p.n_sbox)) - 1);
		for (unsigned int i = 0; i < p.sbox.size(); ++i)
		{
			temp = "SBOX ROW ";
			temp += std::to_string(i+1);

			equal<unsigned int>(result, temp, y, p.sbox[i].size());
			between<unsigned int>(result, temp, y-1, 0, p.sbox[i]);
		}

		return result;
	}
};

#endif //__H_PPARSER_H__