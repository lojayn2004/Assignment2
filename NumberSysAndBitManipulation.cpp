// Determine the sign of the decimal number
    int sign = (decimal < 0) ? -1 : 1;

    // Take the absolute value of the decimal number
    decimal = abs(decimal);

    // Convert decimal to binary
    while (decimal > 0) {
        // Prepend the remainder (0 or 1) to the binary string
        binary = std::to_string(decimal % 2) + binary;

        // Divide the decimal number by 2
        decimal /= 2;
    }
    // Add leading zeros to make the binary string 8 bits long
    while (binary.size() < 8) {
        binary = "0" + binary;
    }

    // If the original number was negative, find the two's complement
    if (sign == -1) {
        // Invert all bits
        for (char& bit : binary) {
            bit = (bit == '0') ? '1' : '0';
        }

        // Add 1 to the result
        int carry = 1;
        for (int i = binary.size() - 1; i >= 0; i--) {
            int bitSum = (binary[i] - '0') + carry;
            binary[i] = char(bitSum % 2 + '0');
            carry = bitSum / 2;
        }
    }

    return binary;
}

string NumberSysAndBitManipulation::hexadecimal_to_binary(string& hex)
{
    // Hex to binary conversion table
    string hexChars = "0123456789ABCDEF";
    string binaryChars[] = { "0000", "0001", "0010", "0011",
                                 "0100", "0101", "0110", "0111",
                                 "1000", "1001", "1010", "1011",
                                 "1100", "1101", "1110", "1111" };

    string binaryResult = "";

    for (char c : hex)
    {
        // Convert each hex character to its binary equivalent
        if (isxdigit(c))
        {
            int index = hexChars.find(toupper(c));
            binaryResult += binaryChars[index];
        }
    }

    return binaryResult;
}

int NumberSysAndBitManipulation::Add(string a, string b) {
    int result = stoi(a) + stoi(b);
    return result;
}
