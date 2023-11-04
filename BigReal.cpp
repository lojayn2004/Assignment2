#include "BigReal.h"


// Constructor that takes a double ,It splits whole and float Part
// Then turns Them to string to be stored in the class
BigReal::BigReal(double number) {

    // Get the Whole part
    // First separate the whole part by taking int()
    int beforeFloatPoint = int(number);
    // Turn the whole part into a string
    wholePart = to_string(beforeFloatPoint);

    // Get the float part
    // find how many digits after floating point
    // number length - (whole part length + the float point)
    int nOfdigitsAfterFloat = (int)(to_string(number).size() - (wholePart.size() + 1));

    // Get the float part only
    number -= beforeFloatPoint;
    number *= pow(10, nOfdigitsAfterFloat);

    // Turn the float part into string
    floatPart = to_string(int(number));

}

// Constructor that takes a string
// It splits whole and float Part
BigReal::BigReal(string number) {
    // Check if the input number is a valid floating-point number.
    if (!is_valid_real(number)) {
        throw ("Float Number is not in proper Format\n");
    }
    else {
        // By default, the sign is positive.
        sign = '+';

        // Find the position of the decimal point (if it exists).
        int floatPoint = (int)number.find('.');

        if (floatPoint != -1) {
            // Split the number into whole and fractional parts based on the decimal point.
            wholePart = number.substr(0, floatPoint);
            floatPart = number.substr(floatPoint + 1, number.size());
        }
        else {
            if (number[0] == '+' || number[0] == '-') {
                // If the number starts with a sign, set the sign accordingly and consider the rest as the whole part.
                sign = number[0];
                wholePart = number.substr(1, number.size());
            }
            else {
                // If there's no sign, consider the entire number as the whole part.
                wholePart = number;
            }
        }

        if (wholePart[0] == '-' || wholePart[0] == '+') {
            // Handle cases where the sign is specified within the whole part.
            sign = wholePart[0];
            wholePart.erase(wholePart.begin()); // Remove the sign from the whole part.
        }

        if (floatPart.size() == 0) {
            // If there is no fractional part, set it to "0."
            floatPart = "0";
        }

        if (wholePart.size() == 0) {
            // If there is no whole part, set it to "0."
            wholePart = "0";
        }
    }
}


// Copy Constructor that copies on Object to another
BigReal::BigReal(const BigReal& otherReal) {
    wholePart = otherReal.wholePart;
    floatPart = otherReal.floatPart;
    sign = otherReal.sign;
}

// Assignment Operator that assigns one object to another
BigReal& BigReal::operator=(const BigReal& otherReal) {
    wholePart = otherReal.wholePart;
    floatPart = otherReal.floatPart;
    sign = otherReal.sign;
    return *this;
}


// Addition function
string BigReal::plus(BigReal& otherNumber, bool flag) {
    // Create a copy of the other number to avoid modifying it directly.
    BigReal number = otherNumber;

    // Ensure that the numbers have the correct format (e.g., same number of decimal places).
    adjust_correct_format(number);

    int carry = 0;
    string result = "";

    // Add the decimal (float) parts of the numbers, from right to left.
    for (int i = floatPart.size() - 1; i >= 0; --i) {
        // Convert the characters to integers, add them, and account for any carry from the previous digit.
        int sum = (floatPart[i] - '0') + (number.floatPart[i] - '0') + carry;
        carry = sum / 10;     // Calculate the carry (if exist).
        sum %= 10;           // Calculate the current digit of the result.
        result = to_string(sum) + result;  // Add the current digit to the result string.
    }
    result = '.' + result;   // Add the decimal point to the result.

    // Add the whole parts of the numbers with carry, from right to left.
    for (int i = wholePart.size() - 1; i >= 0; --i) {
        // Convert the characters to integers, add them, and account for any carry from the previous digit.
        int sum = (wholePart[i] - '0') + (number.wholePart[i] - '0') + carry;
        carry = sum / 10;  // Calculate the carry (if exist).
        sum %= 10;  // Calculate the current digit of the result.
        result = to_string(sum) + result;  // Add the current digit to the result string.
    }

    // Add any remaining carry if it exists and the "flag" is set.
    if (carry && flag) {
        result = to_string(carry) + result;  // Add the remaining carry to the result string.
    }

    return result;  // Return the result of the addition.
}

BigReal BigReal::operator-(BigReal& otherNum) {

    if(otherNum.isZero()){
        return BigReal(*this);
    }
    else if(this->isZero()){
        BigReal tmp(otherNum);
        // Change sign
        if(otherNum.sign == '+')
             tmp.sign ='-';
        else
            tmp.sign = '+';
        return tmp;
    }


    // If two sign are not equal
    // That means its addition
    if (sign != otherNum.sign) {
        BigReal tmp = (*this).plus(otherNum, true);

        // The sign follows the sign of the first Number
        if (sign == '+') {
            tmp.sign = '+';
        }
        else {
            tmp.sign = '-';
        }
        return tmp;
    }
    else {
        // If Two numbers have the same sign we must compare the absolute value
        // of them and send to subtract the greater first
        // Get the absolute of two numbers

        BigReal* absNum1 = new BigReal(wholePart+ '.'+ floatPart);
        absNum1->sign = '+';

        BigReal* absNum2 = new BigReal(otherNum);
        absNum2->sign = '+';

        BigReal tmp ("0.0");

        // call the subtract function with the greater absolute number first
        if (*absNum1 > *absNum2) {
            tmp = (absNum1)->subtract(*absNum2);
        }
        else if (*absNum1 < *absNum2) {
            tmp = (absNum2)->subtract(*absNum1);
        }
        else {
            return BigReal("0.0");
        }

        // if the first number is grater set the sign to positive
        // otherwise set it to negative
        if (*this > otherNum ) {
            tmp.sign = '+';
        }
        else{
            tmp.sign = '-';
        }
        // Delete the memory located to avoid memory leak
        delete absNum1;
        delete absNum2;
        return tmp;
    }
}

// operator overloading to determine i make addition or subtraction according to numbers' sign
BigReal BigReal::operator+(BigReal& otherNumber) {
    BigReal result;  // Create a new BigReal object to store the result of the addition.

    if (sign == otherNumber.sign) {
        // Check if both numbers have the same sign.
        // If they do, proceed to addition.
        result = plus(otherNumber);  // Call the plus function to add the two numbers.
        result.sign = sign;  // Set the sign of the result based on the input numbers' sign.
    }
    else {
        // If the signs are different, do subtract.

        // Create copies of the current and other BigReal objects with positive signs.
        BigReal absNum1(*this);  // Create a copy of the current BigReal object.
        absNum1.sign = '+';

        BigReal absNum2(otherNumber);  // Create a copy of the other BigReal object.
        absNum2.sign = '+';

        if (absNum1 > absNum2) {
            // Check if the absolute value of the current number is greater than the absolute value of the other number.
            result = absNum1.subtract(absNum2);  // Subtract the smaller absolute value from the larger one.
        } else if (absNum1 < absNum2) {
            // Check if the absolute value of the current number is smaller than the absolute value of the other number.
            result = absNum2.subtract(absNum1);  // Subtract the smaller absolute value from the larger one.
        } else {
            // If the absolute values are equal, return a BigReal with a value of "0.0".
            return BigReal("0.0");
        }

        // Set the sign of the result based on the original numbers.
        result.sign = (absNum1 > absNum2) ? this->sign : otherNumber.sign;
    }

    return result;  // Return the result of the addition
}

// function check negtive sign exist or not
bool BigReal::isNegative() {
    // Check if the sign of the BigReal is negative.
    if (sign == '-')
        return true;  // If the sign is '-', the number is negative, so return true.
    else
        return false; // If the sign is not '-', the number is non-negative, so return false.
}


bool BigReal::operator>(BigReal& otherReal) {
    // Check if the current instance is negative and the other is positive.
    if (isNegative() && !otherReal.isNegative()) {
        return false; // The current instance is negative, and the other is positive, so it's not greater.
    }
        // Check if the current instance is positive and the other is negative.
    else if (!isNegative() && otherReal.isNegative()) {
        return true; // The current instance is positive, and the other is negative, so it's greater.
    }
        // Both are negative, so compare them in reverse.
    else if (isNegative() && otherReal.isNegative()) {
        if (wholePart.size() > otherReal.wholePart.size()) {
            return false; // If the current whole part has more digits, it's smaller in magnitude.
        }
        else if (wholePart.size() < otherReal.wholePart.size()) {
            return true; // If the other whole part has more digits, it's smaller in magnitude.
        }

        for (int i = 0; i < wholePart.size(); ++i) {
            if (wholePart[i] > otherReal.wholePart[i]) {
                return false; // Compare digits in the whole part and return false if the current is greater.
            }
            else if (wholePart[i] < otherReal.wholePart[i]) {
                return true; // Compare digits in the whole part and return true if the current is smaller.
            }
        }

        for (int i = 0; i < floatPart.size(); ++i) {
            if (floatPart[i] > otherReal.floatPart[i]) {
                return false; // Compare digits in the float part and return false if the current is greater.
            }
            else if (floatPart[i] < otherReal.floatPart[i]) {
                return true; // Compare digits in the float part and return true if the current is smaller.
            }
        }
    } else {
        // Both are positive, so compare them normally.
        if (wholePart.size() > otherReal.wholePart.size()) {
            return true; // If the current whole part has more digits, it's greater in magnitude.
        } else if (wholePart.size() < otherReal.wholePart.size()) {
            return false; // If the other whole part has more digits, it's smaller in magnitude.
        }

        for (int i = 0; i < wholePart.size(); ++i) {
            if (wholePart[i] > otherReal.wholePart[i]) {
                return true; // Compare digits in the whole part and return true if the current is greater.
            }
            else if (wholePart[i] < otherReal.wholePart[i]) {
                return false; // Compare digits in the whole part and return false if the current is smaller.
            }
        }

        for (int i = 0; i < floatPart.size(); ++i) {
            if (floatPart[i] > otherReal.floatPart[i]) {
                return true; // Compare digits in the float part and return true if the current is greater.
            }
            else if (floatPart[i] < otherReal.floatPart[i]) {
                return false; // Compare digits in the float part and return false if the current is smaller.
            }
        }

        return false; // If we reach this point, the numbers are equal.
    }
}


bool BigReal::operator<(BigReal& otherReal) {
    // Check if the current instance is negative and the other is positive.
    if (isNegative() && !otherReal.isNegative()) {
        return true; // The current instance is negative, and the other is positive.
    }
        // Check if the current instance is positive and the other is negative.
    else if (!isNegative() && otherReal.isNegative()) {
        return false; // The current instance is positive, and the other is negative.
    }
        // Both are negative, so compare them in reverse.
    else if (isNegative() && otherReal.isNegative()) {
        if (wholePart.size() > otherReal.wholePart.size()) {
            return true; // If the current whole part has more digits, it's smaller in magnitude.
        }
        else if (wholePart.size() < otherReal.wholePart.size()) {
            return false; // If the other whole part has more digits, it's smaller in magnitude.
        }

        for (int i = 0; i < wholePart.size(); ++i) {
            if (wholePart[i] > otherReal.wholePart[i]) {
                return true; // Compare digits in the whole part and return true if the current is larger.
            }
            else if (wholePart[i] < otherReal.wholePart[i]) {
                return false; // Compare digits in the whole part and return true if the current is smaller.
            }
        }

        for (int i = 0; i < floatPart.size(); ++i) {
            if (floatPart[i] > otherReal.floatPart[i]) {
                return true; // Compare digits in the float part and return true if the current is larger.
            }
            else if (floatPart[i] < otherReal.floatPart[i]) {
                return false; // Compare digits in the float part and return true if the current is smaller.
            }
        }
    }
        // Both are positive, so compare them normally.
    else {
        if (wholePart.size() < otherReal.wholePart.size()) {
            return true; // If the other whole part has more digits, it's larger in magnitude.
        }
        else if (wholePart.size() > otherReal.wholePart.size()) {
            return false; // If the current whole part has more digits, it's larger in magnitude.
        }

        for (int i = 0; i < wholePart.size(); ++i) {
            if (wholePart[i] < otherReal.wholePart[i]) {
                return true; // Compare digits in the whole part and return true if the current is smaller.
            }
            else if (wholePart[i] > otherReal.wholePart[i]) {
                return false; // Compare digits in the whole part and return true if the current is larger.
            }
        }

        for (int i = 0; i < floatPart.size(); ++i) {
            if (floatPart[i] < otherReal.floatPart[i]) {
                return true; // Compare digits in the float part and return true if the current is smaller.
            }
            else if (floatPart[i] > otherReal.floatPart[i]) {
                return false; // Compare digits in the float part and return true if the current is larger.
            }
        }
    }

    return false; // If we reach this point, the numbers are equal.
}


bool BigReal::operator== (BigReal& otherReal) {
    adjust_correct_format(otherReal);
    for (int i = 0; i < wholePart.size(); ++i) {
        if (wholePart[i] != otherReal.wholePart[i]) {
            //checks if whole part not equal to the other part
            //break
            return false;
        }

    }
    //if first loop return true , it will enter second loop of float part
    for (int i = 0; i < floatPart.size(); ++i) {
        if (floatPart[i] != otherReal.floatPart[i]) {
            return false;
        }
    }
    return true;
}


void BigReal::correct_number(BigReal& number) {
    while (number.wholePart.size()>1 && number.wholePart[0] == '0') {
        number.wholePart.erase(number.wholePart.begin());
    }
}

ostream& operator<<(ostream& cout, const BigReal& number) {
    BigReal y = number;
    y.correct_number(y);

    if (number.sign == '-') {
        cout << '-';//if number less than zero
    }
    //print int part then decimal point then float part
    cout << y.wholePart << '.' << y.floatPart << endl;
    return cout;
}


// Private Helper Functions That are by public functions


// Function That puts the real number in correct format
// To prepare it for addition and subtraction
void BigReal::adjust_correct_format(BigReal& otherNum) {
    // Find the maximum length of whole parts between the current instance and the other number.
    int maxWholeLength = max(wholePart.length(), otherNum.wholePart.length());

    // Pad both whole parts with zeros to match the maximum length.
    PadZeros(wholePart, maxWholeLength, true);
    PadZeros(otherNum.wholePart, maxWholeLength, true);

    // Find the maximum length of decimal (float) parts between the current instance and the other number.
    int maxDecimalLength = max(floatPart.length(), otherNum.floatPart.length());

    // Pad both decimal parts with zeros to match the maximum length.
    PadZeros(floatPart, maxDecimalLength, false);
    PadZeros(otherNum.floatPart, maxDecimalLength, false);
}



//function make numbers at same length by adding zeros 
void BigReal::PadZeros(string& str, int length, bool leftPad) {
    int currentLength = str.length();

    // Check if the current length of the string is less than the desired length.
    if (currentLength < length) {
        if (leftPad) {
            // If leftPad is true, insert zeros at the beginning of the string to reach the desired length.
            str.insert(0, length - currentLength, '0');
        }
        else {
            // If leftPad is false, append zeros at the end of the string to reach the desired length.
            str.append(length - currentLength, '0');
        }
    }
}



// Function that generate the 10s complement of a number
// To be used in subtraction process
BigReal BigReal::generate_complement(BigReal n)
{
    // 10s complement  = 9s complement  + 1
    string complement = "";
    string number = n.wholePart + '.' + n.floatPart;
    int index = number.size() - 1;
    // If the last index is dot ignore it
    if (number[index] == '.') {
        index--;
    }
    // Get the 9s complement for the rest of the digits
    while (index >= 0)
    {
        if (number[index] == '.') {
            complement = '.' + complement;
            index--;
            continue;
        }
        complement = char(9 - (number[index] - '0') + '0') + complement;
        index--;
    }

    // Add one to the least significant bit of the number
    BigReal lsb = BigReal(".0");
    BigReal y = BigReal(complement);

    y.adjust_correct_format(lsb);
    lsb.floatPart.back() = '1';
    BigReal answer = y.plus(lsb);

    return answer;
}


// Function that subtracts two real numbers
BigReal BigReal::subtract(BigReal& otherReal) {

    // Get the 10 th complement of the second number
    BigReal secondNum = otherReal.wholePart + '.' + otherReal.floatPart;
    adjust_correct_format(secondNum);
    BigReal secondNumComplement = generate_complement(secondNum);


    // Add the 10 th complement of the second number to firs Number
    BigReal answer = (*this).plus(secondNumComplement, false);

    // if the first number is greater that means the
    // resulting number is positive otherwise its negative
    if ((*this) > otherReal) {
        answer.sign = '+';
    }
    else {
        answer.sign = '-';
    }

    return answer;

}


// Checks that the given number is in his correct format
bool BigReal::is_valid_real(string realNumber) {
    int floatingPoint = count(realNumber.begin(), realNumber.end(), '.');
    // Real number cannot have more than floating point
    if (floatingPoint > 1) {
        return false;
    }
    else
    {
        // first charachter can be digit or a sign only
        if (realNumber[0] != '-' && realNumber[0] != '+' &&
            !isdigit(realNumber[0]) && realNumber[0] != '.') {
            return false;
        }
        else
        {
            // All other charachters can be either digits or the floating point
            for (int i = 1; i < (int)realNumber.size(); i++) {
                if (!isdigit(realNumber[i]) && realNumber[i] != '.') {
                    return false;
                }
            }
            return true;
        }
    }
}
int BigReal::getSign() {
    // if sign is '-' return -1
    if(sign=='-'){
        return -1;
    }
        //else is number is zero return 0
    else if (sign=='+' &&wholePart=="0"&&floatPart=="0"){
        return 0;
    }
    else
        // else number is positive return 1
    {
        return 1;
    }
}


int BigReal::size(){
    return wholePart.size() + floatPart.size();
}


bool BigReal::isZero(){
    for(int i = 0; i <(int)wholePart.size();i++){
        if(wholePart[i] != '0'){
            return false;
        }
    }
    for(int i = 0; i <(int)floatPart.size();i++) {
        if (floatPart[i] != '0') {
            return false;
        }
    }
    return true;
}
