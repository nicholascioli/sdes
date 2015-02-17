./" Use groff -man -Tascii man.1
./" Use groff -man -Tascii man.1

.TH mycommand 1 "FEBRUARY 2015" "CIS4362" "University of Florida"
.SH NAME
mycommand \- encrypt or decrypt input using S-DES
.SH SYNOPSIS
.B mycommand [-e] [-s] [-i
<input_file>
.B ] [-o
<file_name>
.B ] [-k
<key>
.B ] [-p
<param_file>
.B ] [-x] [-d] ...
.SH DESCRIPTION
A generalizable implementation of S-DES/DES-like ciphers for encrypting and decrypting files with full cipher customization at run-time. 

.SS Parameter file param.txt description:
.PP 
Any line may have comments, which start with // and continue to the end of the line
.PP 
Any line may be left blank or only have a comment on it; however, the parameters must appear in the following order, as described. 
.PP 
B: Block size – a single positive integer in decimal
.PP 
Q: Key size – number of bits in key – a single positive decimal integer
.PP 
QE: Effective key size – number of key bits after PC-1 – a single positive decimal integer
.PP 
R: Round key size – a single positive decimal integer
.PP 
N: Number of Rounds – a single positive decimal integer
.PP 
PC-1: Initial Permuted Choice for Round Key Generation – QE decimal integers on one line separated by spaces, with each integer between 1 and Q
.PP 
PC-2: Permuted Choice to set bits for Round Key – R decimal integers on one line separated by spaces with each integer between 1 and QE
.PP 
RS: Left Rotation Schedule: List of N integers, indicates number of logical left shifts before each round – N decimal integers separated by spaces all on one line with each between -QE/2+1 and QE/2-1
.PP 
IP: Initial Permutation – the B decimal integers between 1 and B (one of each) all on one line separated by spaces
.PP 
E: Expansion permutation – lists where each of R bits comes from in input of length B/2 – R decimal integers, each between 1 and B/2, all on one line, separated by spaces
.PP 
P: P-box transposition permutation – the decimal integers between 1 and B/2 (one of each) all on one line separated by spaces
.PP 
T: Number of S-Boxes (each takes in R/T bits, outputs B/2T bits) – single positive decimal integer, which must evenly divide R
.PP 
Row: Permuted choice to select the bits from the input to an S-box that are used to select the row – R/T-B/2T distinct decimal integers between 1 and R/T, all on one line separated by spaces
.PP 
Col: Permuted choice to select the bits from the input to an S-box that are used to select the column – B/2T distinct decimal integers betweend 1 and R/T that do not appear in Row, all on one line separated by spaces
.PP 
Si: ith S-box substitution – array of 2x rows and 2y columns, where x = R/T – B/2T, and y = B/2T – one blank line is recommended but not required, followed by  2x lines, each with 2y integers between 1 and 2y 
.SH OPTIONS
.IP -i<file_name>
Read from file_name(with no space before the file name) or from stdin if “-” is used for the file name. 
.IP -o<file-name>
Specifiy a different output file, or to write output to stdout if “-” is the file name. 
.IP -e
Encrypt the input
.IP -d
Decrypt the input
.IP -k<key>
Used to enter the key in hexadecimal by commandline rather than through key.txt
.IP -p<param_file>
Specify a different param file than params.txt
.IP -s
Show intermediate steps including round keys, left and right side input to each round
.IP -x
Indicates that hexidecimal representations should be used instead of binary for the input and output.
.IP -D
Enables diagnostic output
.SH FILES
.I ~/params.txt
.RS
File to read parameter values from, unless overriden by command-line param -p
.RE
.I ~/ptxt
.RS
File to read plain text from, unless overriden by command-line param -i
.RE
.I ~/ctxt
.RS
File to read cipher text from, unless overriden by command-line param -c
.RE

.\" .SH ENVIRONMENT
.\" .SH BUGS
.SH EXAMPLE
Standard Ecnryption
.RS
mycommand -e
.RE
Standard Decryption
.RS
mycommand -d 
.RE
Encryption with File Selections
.RS
mycommand -e -i<input_file> -o<output_file> -p<param_file>
.RE
Encryption with diagnostic output, intermediate steps, and hex input/output
.RS
mycommand -e -d -s -x
.RE

.SH AUTHOR
Nicholas Cioli, primary programmer
.PP
Collin Irwin