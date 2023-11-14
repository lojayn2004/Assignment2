#ifndef NUMBERSYS_H
#define NUMBERSYS_H


#include <iostream>
#include <string>
#include <map>
using namespace std;

namespace NumberSysAndBitManipulation
{
	// Changes a hexaDecimal to other Number Systems
	int hexadecimal_to_decimal(string hexaDec);

	string hexadecimal_to_binary(string& hex);
	// Functions to convert from Binary to other Number Systems
	string binary_to_hexadecimal(string number);

	int binary_to_decimal(string bin);

	// Functions That Changes between twos complemnt And decimal
	// To be used for addition
	int twos_complement_to_decimal(string& binary);

	string decimal_to_twosComplement(int decimal);

	// Turns A hexadecimal digit to a number
	int hex_char_to_int(char hexChar);

	// Add the bit Patterns stored in Hexa decimal Format
	string add_bit_patterns(string s1, string s2);

	// Add to numbers stored as a string
	int Add(string a, string b);

	string decimal_to_hexadecimal(int number);
};

#endif // !NUMBERSYS_H
