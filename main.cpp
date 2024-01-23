#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>

bool loadInstructions(char* arg, std::vector<unsigned char>& set);
void loadJumps(const std::vector<unsigned char>& set, std::unordered_map<uint32_t, uint32_t>& map);
void interpretProgram(const std::vector<unsigned char>& set, std::unordered_map<uint32_t, uint32_t>& map);

int main(int argc, char** argv){
	
	std::vector<unsigned char> instructions;
	std::unordered_map<uint32_t, uint32_t> bracket_map;
	
	if(!loadInstructions(argv[1], instructions)){
		std::cout << "Fajl nem letezik" << std::endl;
		return -1;
	}

	loadJumps(instructions, bracket_map);
	interpretProgram(instructions, bracket_map);
	return 0;
}

bool loadInstructions(char* arg, std::vector<unsigned char>& set){

	std::ifstream stream = std::ifstream(arg, std::ios::in);
	if(!stream.is_open()) return false;

	char instruction;
	while(stream.get(instruction)){
		set.emplace_back( (unsigned char)instruction );
	}
	set.shrink_to_fit();

	stream.close();
	return true;
}

void loadJumps(const std::vector<unsigned char>& set, std::unordered_map<uint32_t, uint32_t>& map) {

	uint32_t counter = 0;
	std::stack<uint32_t> bracket_stack;
	for(auto c : set){
		if(c == '[') bracket_stack.push(counter);

		else if(c == ']'){
			map[bracket_stack.top()] = counter;
			bracket_stack.pop();
		}
		++counter;
	}
}

void interpretProgram(const std::vector<unsigned char>& set, std::unordered_map<uint32_t, uint32_t>& map) {

	std::vector<unsigned char> cells = {0};
	int dataPointer = 0;
	uint32_t iterator = 0;
	std::stack<uint32_t> iteratorStack;

	while(iterator < set.size()){
		char c = set[iterator];
		switch(c){
			case '>':
				++dataPointer;
				if(dataPointer >= cells.size()) cells.push_back(0);
				break;

			case '<':
				--dataPointer;
				break;

			case '+':
				++cells[dataPointer];
				break;
			
			case '-':
				--cells[dataPointer];
				break;

			case ',':
				std::cin >> cells[dataPointer];
				break;

			case '.':
				std::cout << cells[dataPointer] << std::flush;
				break;

			case '[':
				if(cells[dataPointer] == 0)	iterator = map[iterator];
				else iteratorStack.push(iterator);
				break;

			case ']':
				if(cells[dataPointer] != 0) iterator = iteratorStack.top();
				else iteratorStack.pop();
				break;
			default:
				break;
		}
		++iterator;
	}
}