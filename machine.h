#pragma once

#include "types.h"
#include <vector>
#include <string>
#include <string_view>
#include "utf8.h"

class machine_c
{ public:
	std::vector<vm_t>  A, B;
	std::vector<vm_t> *main = nullptr, 
					  *side = nullptr;
    //std::vector<std::string*> registry;

    std::string_view program;
    std::vector<command_t> commands;

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

    void push_main (vm_t n)
    { main -> push_back(n); }
    vm_t peek_main()
    { return main -> back(); }
    void remv_main()
    { main -> pop_back(); }
    vm_t  pop_main()
    {
    	vm_t temp = peek_main();
    	remv_main();
    	return temp;
    }

    void push_side(vm_t n)
    { side-> push_back(n); }
    vm_t peek_side()
    { return side -> back(); }
    void remv_side()
    { side -> pop_back(); }
    vm_t  pop_side()
    {
    	vm_t temp = peek_side();
    	remv_side();
    	return temp;
    }

    uint32_t lex(std::string_view, std::vector<command_t>&);
    uint32_t lex() { return lex(program, commands); }
};

uint32_t machine_c::lex(std::string_view cjk, std::vector<command_t>& destination)
/*
    takes a string view and trawls it codepoint by codepoint
    pushes back results into destination vector

    results are of the type command_t := struct { instruction, value }
*/
{
    uint32_t amount_written = 0;

    std::string_view::iterator beg_itr = cjk.begin();
    std::string_view::iterator end_itr = cjk.end();

    if(beg_itr == end_itr) { return -1; }
    if(!utf8::is_valid(cjk)) { return -1; }

    std::string_view::iterator index = beg_itr;

    while(index != end_itr)
    {
        switch(utf8::next(index, end_itr))
        {
            case 0x0 ... 0x2e79: // everything up to CJK is a comment for now
                destination.push_back({COMM, {0, 0}});
                break;
            case 0x6b7b: // 死 - die = sei2
                destination.push_back({DIE, {0, 0}});
                break;
            case 0x5638: // 嘸 - nothing = m4
                destination.push_back({NOTHN,{0,0}});
                break;
            default:
                destination.push_back({ZERO, {0, 0}});
                break;
        }
        amount_written++;
    }

    return amount_written; // placeholder
}