#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

enum State{
 S0 = 0,
 S1,
 S2
};

namespace  fs = std::filesystem;


void print_quotes(std::vector<std::string> quotes){
  for(std::string& quote : quotes){
     std::cout << quote << '\n';	  
     std::cout << "----------------\n";
  }	
}

int main(int argc, char **argv) { 
 
 State currentState = State::S0;	

 char* program = argv[0];
 if(argc != 2){
   std::cerr << "Error:Invalid Argument\n";
   std::cerr << "Usage: " << program << " <input_file>\n";
   std::exit(1);
 }

 char* input_file_path = argv[1];
 if (!fs::exists(input_file_path)){
   std::cerr << "Error: "<< input_file_path << " No such file exists\n";	 
   std::exit(1);
 }
 std::ifstream input_file(input_file_path);

 if(input_file.is_open()){
    char temp_byte;
    std::vector<std::string> quotes;
    std::string quote;
     int line_break = 0;
    while(input_file.good()){
       temp_byte = input_file.get();
       if(temp_byte == '"' && currentState == State::S0){
	  currentState = State::S1;
       } else if (temp_byte == EOF && currentState == State::S1){
	  // check for line breaks in quote
          std::cerr << "Error: No matching \" found for " << "\""<< quote << "at line " << quotes.size() + line_break << '\n';     
	  std::exit(1);
       } else if(temp_byte != '"' && currentState == State::S1){
	   if (temp_byte == '\n') line_break +=1;  
	   quote.push_back(temp_byte);
       } else if (temp_byte == '"' && currentState == State::S1){
	  quotes.emplace_back(quote);
	  currentState = State::S2;
	  quote.erase();
       }else if(temp_byte == '"' && currentState == State::S2){
           currentState = State::S1;
       }  else if (temp_byte == EOF && currentState == State::S2) {
	   std::cout << "Found these quotes for you:\n";
	   print_quotes(quotes);
	   break;
       } else{
	  continue;     
       }

    }
 }else{
   std::cerr << "Error " << input_file_path << " not open\n";
   std::exit(1);
 }
 return 0; // my happy compiler
}
