#include "machine.h"
#include "types.h"

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
        	case 0x00 ... 0x1f:
        		destination.push_back({ERROR,{0,0}});
        		break;
            case 0x20 ... 0x2e79: // everything up to CJK is a comment for now
                destination.push_back({COMM, {0, 0}});
                break;
            case 0x5638: // 嘸 - nothing = m4
                destination.push_back({NOTHN,{0,0}});
                break;
            case 0x6b7b: // 死 - die = sei2
                destination.push_back({DIE, {0, 0}});
                break;
            case 0x96f6: // 零 - zero = leng4
            	destination.push_back({PSH_ZERO,{0, 0}});
            	break;
            default:
                destination.push_back({ZERO, {0, 0}});
                break;
        }
        amount_written++;
    }

    return amount_written;
}

uint32_t machine_c::run(int ticks)
/*
	this method trawls the commands vector, executes the
	commands one by one, and returns the number of commands
	executed

	arguments:
		- int ticks := number of commands to run, default -1
			    	   means no stopping
				       when arg1 specified, a well-formed program should
				       return a number up to the number it was given, or less
				       when arg1 unspecified, it times out at 2^23 insns
				       to prevent infinite loops
	return:
		- int24_t   := number of commands that were run
					   negative value means the program timed out
					   (this means that bit 23 was set in the return value
					   since there is no native 24-bit integer type in cpp)
					   (val & 0x80_00_00)
*/
{
	if(commands.empty()) { return -1; } // naughty naughty!

	int32_t elapsed = 0;
	size_t  command_ptr = 0;
	size_t  commands_sz = commands.size();

	while((elapsed < ticks) and (elapsed < 0x80'00'00))
	{
		elapsed++;
		switch(commands[command_ptr].instruction)
		{
			case ZERO:
				if(__DEBUG) { std::printf("debug: ZERO\n"); }
				break;
			case COMM:
				if(__DEBUG) { std::printf("debug: COMM\n"); }
				break;
			case PSH_ZERO:
				if(__DEBUG) { std::printf("debug: PSH_ZERO 零 @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x00});
				break;
			case DIE:
				if(__DEBUG) { std::printf("debug: DIE 死 @ %d\n", int(command_ptr)); }
				std::printf("encountered 死 / sei3, terminating after %d at index #%ld\n", elapsed, command_ptr);
				goto exit_loop;
				break;
			case NOTHN:
				if(__DEBUG) { std::printf("debug: NOTHING 嘸 @ %d\n", int(command_ptr)); }
				break;
			default:
				break;
		}
		command_ptr = (command_ptr+1) % commands_sz;
	}

	exit_loop:

	return elapsed;
}