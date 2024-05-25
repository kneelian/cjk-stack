#pragma once

#include "debug.h"

#include "types.h"
#include <vector>
#include <string>
#include <string_view>

#include <bit>

#include "utf8.h"

class machine_c
{ public:
	std::vector<vm_t>  A, B;
	std::vector<vm_t> *main = nullptr, 
					  *side = nullptr;
    //std::vector<std::string*> registry;

    std::string_view program;
    std::vector<command_t> commands;

    std::string response;

	machine_c()
	{
		A.reserve(256);
		B.reserve(32);
		main = &A;
		side = &B;
        commands.reserve(32);
	//  registry.reserve(8);
	}

   ~machine_c()
    {
    	/*for(auto i : registry)
    		{ delete i; }*/
    }

    bool swap_ab()
    {
    	std::vector<vm_t>* temp = main;
    	main = side; side = temp;
    	return (main and side) and (main != side);
    }

    bool load(std::string& input)
    {
    	program = input;
    	return !(program.empty());
    }

    /*
		Stack operations
			- push: adds one to stack (vm_t -> void)
			- peek: returns top of stack
			- remv: removes top of stack
			- pop:  returns top of stack, then removes it
    */

    void push_main(vm_t n)
    { main -> push_back(n); }
    vm_t peek_main()
    { if(!main->empty()) return main -> back(); else return {0xff, 0xffff}; }
    void remv_main()
    { if(!main->empty()) main -> pop_back(); }
    vm_t  pop_main()
    {
        if(main->empty()) { return {0xff, 0xffff}; }
    	vm_t temp = peek_main();
    	remv_main();
    	return temp;
    }

    void push_side(vm_t n)
    { side-> push_back(n); }
    vm_t peek_side()
    { if(!side->empty()) return side -> back(); else return {0xff, 0xffff}; }
    void remv_side()
    { if(!side->empty()) side -> pop_back(); }
    vm_t  pop_side()
    {
        if(side->empty()) { return {0xff, 0xffff}; }
    	vm_t temp = peek_side();
    	remv_side();
    	return temp;
    }

    uint32_t lex(std::string_view, std::vector<command_t>&);
    uint32_t lex() { return lex(program, commands); }

    uint32_t run(int ticks = 0x7f'ff'ff);
};