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
	int32_t elapsed = 0;

	return elapsed;
}