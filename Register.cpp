#include "Register.h"

Register::Register() {
	regNum = 'X';
	regContent = "00";
}


Register::Register(char number, string content) {
	regNum = number;
	regContent = content;
}

void Register::load_to_register(string content) {
	regContent = content;
}

string Register::get_register_content() {
	return regContent;
}
