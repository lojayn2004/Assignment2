#include "VoleMachine.h"
using namespace NumberSysAndBitManipulation;

VoleMachine::VoleMachine() {

    // Set the program counter to default value
    programCounter.load_to_register("00");

    // Set the instruction Register to default value
    instructionRegister.load_to_register("00");

    // Initializes the 16 register for vole machine
    initialize_registers();
}

VoleMachine::~VoleMachine() {

    // Free the memory Allocated dynamically for the registers
    for (int i = 0; i < (int)reg.size(); i++) {
        Register* tmp = reg[i];
        delete tmp;
        reg[i] = nullptr;
    }
}

// Function that takes the instructions set from a file
void VoleMachine::load_instructions(string fileName) {

    fstream instructionsFile(fileName, ios::in);
    if (!instructionsFile.is_open()) {
        throw "Cannot open Instruction file";
    }
    string line;
    // delete Old Instructions To Be able to excute New One
    if (!instructions.empty()) {
        instructions.clear();
    }
   
    while (getline(instructionsFile, line)) {
        stringstream cn(line);
        string address, content;

        
        if (line.empty()) {
            instructionsFile.close();
            break;
        }
        // Read the address and the content and push them to the instructions vector
        while (cn >> address, cn >> content) {
            instructions.push_back({ address, content });
            // Update the memory also with instruction set 
            mem.store_to_memory(address, content);
        }
    }
    instructionsFile.close();
    

    // Instruction Register points at the instruction that will currently be executed
    instructionRegister.load_to_register(instructions[0].first);

    // Program Counter will point to the next instruction to be executed 
    // Which is the current instruction address + 2
    increase_program_counter();
  
}

void VoleMachine::initialize_registers() {

    // Creates the 16 Registers of vole Machine
    // And Assigns each of them to 00

    string registerNumbers = "0123456789ABCDEF";
    for (int i = 0; i < 17; i++) {
        Register* newReg = new Register(registerNumbers[i], "00");
        reg.push_back(newReg);
    }
}

string VoleMachine::fetch()
{
    string instruction = "";
    for (int i = 0; i < (int)instructions.size(); i++) {
        
        // Gets the instruction pointed to by the instruction register
        if (instructionRegister.get_register_content() == instructions[i].first) {
            instruction += instructions[i].second;
            instruction += instructions[i + 1].second;
            break;
        }
    }
    return instruction;
}

void VoleMachine::decode(string currentInstruction) {

    // Check if the instruction is valid or no
    if (!is_valid_instruction(currentInstruction)) {
        throw "Invalid Instruction\n";
    }

    // Splits the instruction to the opcode and the operand
    char opcode = currentInstruction[0];
    string operand = currentInstruction.substr(1);

    // Get the number of the register 
    int index = hex_char_to_int(operand[0]);

    if (opcode == '1') {
        // Get the content of the specified memory address
        string memoryContent = mem.load_from_memory(operand.substr(1));

        // Set the content of the register to this value
        reg[index]->load_to_register(memoryContent);
    }
    else if (opcode == '2') {

        // load the register with the value XY
        reg[index]->load_to_register(operand.substr(1));
    }
    else if (opcode == '3') {
        
        if (operand.substr(1) == "00") {
            cout << reg[index] << endl;
        }
        else {
            mem.store_to_memory(operand.substr(1), reg[index]->get_register_content());
        }
    }
    else if (opcode == '4')
    {
        int index1 = hex_char_to_int(operand[1]);
        int index2 = hex_char_to_int(operand[2]);
        string result = reg[index1]->get_register_content();
       
        if (result.size() < 2) {
            result = '0' + result;
            reg[index1]->load_to_register(result);
        }
        reg[index2]->load_to_register(result);
    }
    else if (opcode == '5')
    {
        int index1 = hex_char_to_int(operand[1]);
        int index2 = hex_char_to_int(operand[2]);

        // Get the two's complement representations of the numbers in registers S and T
        string S = reg[index1]->get_register_content();
        string T = reg[index2]->get_register_content();

        // Perform  addition
        string result = add_bit_patterns(S, T);
       
        // Store the result in register R
        int indexR = hex_char_to_int(operand[0]);
        while (result.size() < 2) {
            result ='0'+ result;
        }

        reg[indexR]->load_to_register(result);
    }
  
   
    else if (opcode == 'B') {

        // If the value in Register (R) equals the value in Register (0)
        // Jump to the specified Memory Address
        if (reg[0]->get_register_content() == reg[index]->get_register_content()) {
            programCounter.load_to_register(operand.substr(1));
        }
    }
    
}

string VoleMachine::execute() {

    // Fetch New Instruction
    string currentInstruction = fetch();
  
    // If Halt or Line is empty End the program
    if (currentInstruction.empty() || currentInstruction == "C000") {
       return "C000";
    }

    try {
        
        // Decode the instruction and execute the operation
        decode(currentInstruction);

        // Update the instruction register with current instruction to be executed
        instructionRegister.load_to_register(programCounter.get_register_content());

        // Update the program Counter with the address of the next instruction to be executed
        increase_program_counter();
    }
    catch (const char* error) {
        cout << error;
    }
    
    return currentInstruction;
}

// Allows user to execute all Instruction Set At a time
void VoleMachine::run_all() {
    string instruction = execute();

    while (instruction != "C000") {
        instruction = execute();
    }

    show_status();
}


// Allows user to execute Instruction  By instruction then see The results
void VoleMachine::run_instruction_by_instruction() {
    string instruction = execute();
    
    while (instruction != "C000") {
        
        cout << "Instruction:  " << instruction << endl;
        show_status();

        
        instruction = execute();
    }
    cout << "Instruction:  " << instruction << endl;
    show_status();
}


// Function that allows the user to see the values of the
// Program Counter, Instruction Register , Memory & Registers
void VoleMachine::show_status() {
    cout << "Instruction Register: " << instructionRegister.get_register_content() << endl;
    cout << "Program Counter: " << programCounter.get_register_content() << endl;
   
    registers_status();
    mem.show_status();
}


// Print the Register Number and Content
void VoleMachine::registers_status() {

    cout << "Register Number    Register Content\n";
    for (int i = 0; i < 16; ++i)
    {
        if (i <= 9) {
            cout << i << "                  " << reg[i]->get_register_content() << endl;
        }
        else {
            cout << char('A' + i - 10) << "                  " << reg[i]->get_register_content() << endl;
        }

    }

}

// Validate the instruction Taken from the user
bool VoleMachine::is_valid_instruction(string instruction) {
    string s = "012356789ABCDEF";
    string opcode = "12345B";
    if (s.find(instruction[1]) == -1 || 
        s.find(instruction[2]) == -1 || s.find(instruction[3]) == -1) {
        return false;
    }
    
    if (opcode.find(instruction[0]) == -1) {
        return false;

    }
    return true;
}


// Increases Program Counter By 2
void VoleMachine::increase_program_counter()
{
    string a = to_string(hexadecimal_to_decimal(instructionRegister.get_register_content()));
    string answer = decimal_to_hexadecimal(Add(a, "2"));
    programCounter.load_to_register(answer);
}
