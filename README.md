# S-DES
This is an attempt at implementing simple-des in C++

## Building
After cloning the repository, simply run <code>make</code>. The executable should be built in the current working directory as <code>sdes</code>.

## Usage
By default, when invoked this program will read all of the configurable options from a file in the current directory labelled 'params.txt', will read plaintext from 'ptxt', will output to file 'ctxt', and will read the key from 'key.txt'

For an in-depth explanation of the usage and parameters, run
<code>man sdes.man</code>
in the root directory.

## Bugs
As of writing this, bit zero for some reason gets translated as integer 36.