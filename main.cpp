#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "utf8.h"

#include "types.h"
#include "machine.h"

const char* type_as_string(uint32_t in);

int main(int argc, char** argv)
{

	std::printf("mode: %s\n", __DEBUG?"debug":"release");

	/*
	std::string a = "\xe6\x97\xa5\xd1\x88";

	auto iter = a.begin();
	int  cp = utf8::next(iter, a.end());

	std::string b;

	std::printf("%d and %d <-- %s\n", cp, utf8::next(iter, a.end()), a.data());
	std::printf("%d\n", utf8::prior(iter, a.begin()) ==	0x448); // see if sha
	
	utf8::append(0x448, b);
	std::printf("%d <-- %s ?= %s\n", b == "\xd1\x88", b.data(), "\xd1\x88"); // prints both

	vm_t test; test.type = 0x50; test.value = 0x111111; std::printf("%d\n", *(uint32_t*)(&test));*/

	machine_c machine;

	std::string example_string = argc>1?argv[1]:"零增孖加孖加";

	machine.load(example_string);
	uint32_t lexed_count = machine.lex();
	//std::vector<command_t> recipient;
	/*
	std::printf("number of lexed items: %d\n", lexed_count); // machine.lex());
	for(auto i : machine.commands)
	{
		std::printf("%#x\n", i.instruction);
	}*/
	if(__DEBUG) { std::printf("-----------\n"); }

	int runtime = machine.run(32);

	std::printf("-----------\n");

	std::printf("command runtime: %d\n=========\n", runtime);

	if(__DEBUG)
	{
		std::printf("program: %.*s\n", int(machine.program.length()), machine.program.data());
		std::printf("-----\nstack dump:\n - stack A: ");
		for(auto i : machine.A)
		{
			std::printf("[%s,%#x] ", type_as_string(i.type), i.value);
		}
		std::printf("\n - stack B: ");
		for(auto i : machine.B)
		{
			std::printf("[%s,%#x] ", type_as_string(i.type), i.value);
		}
		std::printf("\n-----\n");
	}

	return 0;
}

const char* type_as_string(uint32_t in)
{
    switch(type_t(in))
    {
        case EMPTY_T: return "nul"; break;
        case INT24_T: return "i24"; break;
        case UINT24_T:return "u24"; break;

        case INT48H_T:return "48h"; break;
        case INT48L_T:return "48l"; break;

        case F24_T:   return "f24"; break;
        case ADDR_T:  return "adr"; break;

        case CUSTOM_T:return "usr"; break;

        case COMM_T:  return "com"; break;

        case ERROR_T: return "err"; break;

        default:
            return "????";
    }
    return "????";
}