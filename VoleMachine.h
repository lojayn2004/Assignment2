#ifndef VOLEMACHINE_H
#define VOLEMACHINE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "Memory.h"
#include "Register.h"
#include "NumberSysAndBitManipulation.h"
using namespace std;

class VoleMachine
{
private:
	Memory mem;
	vector<Register*> reg;
	Register programCounter, instructionRegister; 
	vector<pair<string, string>> instructions;
	
private:
	string fetch();
	void decode(string);
	bool is_valid_instruction(string);
	void increase_program_counter();
	void initialize_registers();
	void registers_status();
	string execute();
	void show_status();
public:

	VoleMachine();
	
	void load_instructions(string);
	void run_all();
	void run_instruction_by_instruction();

	~VoleMachine();
   
};
#endif // VOLEMACHINE_H
