#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
using namespace std;

class Register
{
private:
	char regNum;
	string regContent;

public:
	Register();
	Register(char, string);
    void load_to_register(string);
	string get_register_content();
};


#endif // !REGISTER_H
