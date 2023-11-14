#ifndef MEMORY_H
#define MEMORY_H


#include <map>
#include <iostream>
using namespace std;


class Memory
{
private:
	map<string, string> memoryAddress;
	
public:
	Memory();
	void store_to_memory(string,string);
	string load_from_memory(string);
	void show_status();
	
};

#endif // !MEMORY_H
