#include "Memory.h"

Memory::Memory() {

 // Initialize 256 Memory Address Starting from 00 to FF
 string address = "0123456789ABCDEF";
 for (int i = 0; i < 16; i++) {
  for (int j = 0; j < 16; j++) {
   string tmp = "";
   tmp += address[i];
   tmp += address[j];
   memoryAddress[tmp] = "00";
  }
 }

}

void Memory::store_to_memory(string address, string memoryContent) {
 memoryAddress[address] = memoryContent;
}


string Memory::load_from_memory(string address) {
 return memoryAddress[address];
}

void Memory::show_status() {

 auto itr = memoryAddress.begin();
 cout << "------------------ Memory Status -----------------\n";
 string s = "0123456789ABCDEF";
 
 cout << "  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  |\n";
 for (int index = 0 ;index<16;index++)
 {
  cout << s[index] << " ";
  for (int i = 0; i < 16 && itr != memoryAddress.end(); i++) {
   cout << itr->second << " ";
   itr++;
  }
  cout <<"|"<<endl;
 }
 cout << "--------------------------------------------------\n";
}
