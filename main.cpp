#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <cstdio>

#include "utf8.h"

#include "types.h"
#include "machine.h"

int main()
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

	std::string example_string = "嘸嘸零嘸嘸零嘸死 a嘸c 死";

	machine.load(example_string);
	uint32_t lexed_count = machine.lex();
	//std::vector<command_t> recipient;
	/*
	std::printf("number of lexed items: %d\n", lexed_count); // machine.lex());
	for(auto i : machine.commands)
	{
		std::printf("%#x\n", i.instruction);
	}*/
	std::printf("-----------\n");

	machine.run(8);

	if(__DEBUG)
	{
		std::printf("program: %.*s\n-----\n", int(machine.program.length()), machine.program.data());
		std::printf("stack dump:\n - stack A: ");
		for(auto i : machine.A)
		{
			std::printf("[%#x,%#x] ", i.type, i.value);
		}
		std::printf("\n - stack B: ");
		for(auto i : machine.B)
		{
			std::printf("[%#x,%#x] ", i.type, i.value);
		}
		std::printf("\n-----\n");
	}

	return 0;
}