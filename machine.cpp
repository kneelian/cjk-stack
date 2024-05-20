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
    	uint32_t codepoint = utf8::next(index, end_itr);
        switch(codepoint)
        {
        	case 0x00 ... 0x1f:
        		destination.push_back({ERROR,{0,0}});
        		break;
            case 0x20 ... 0x2e79: // everything up to CJK is a comment for now
                if(__DEBUG) { destination.push_back({COMM, {COMM_T, codepoint}}); }
                break;
            case 0x52a0: // 加 - add = gaa1
            	destination.push_back({ADD, {0,0}});
            	break;
            case 0x5638: // 嘸 - nothing = m4
                destination.push_back({NOTHING,{0,0}});
                break;
            case 0x589e: // 增 - add/expand = zang1
            	destination.push_back({INCREMENT,{0,0}});
            	break;
            case 0x5b89: // 安 - where[literary] = on1
            	destination.push_back({WHERE,  {0,0}});
            	break;
            case 0x64f0: // 擰 - turn around/spin/rotate = ning6
            	destination.push_back({ROTATE, {0,0}});
            	break;
            case 0x68c4: // 棄 - discard/abandon = hei3
            	destination.push_back({ABANDON,{0,0}});
            	break;
            case 0x6b7b: // 死 - die = sei2
                destination.push_back({DIE, {0, 0}});
                break;
            case 0x6bb2: // 殲 - destroy = cim1
            	destination.push_back({DESTROY,{0,0}});  
            	break;
            case 0x96f6: // 零 - zero = leng4
            	destination.push_back({PSH_ZERO,{0, 0}});
            	break;
            default:
                if(__DEBUG) { destination.push_back({ZERO, {0, 0}}); }
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
		- int ticks := number of commands to run, default 1 << 23 - 1
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

	 uint8_t temp_type= 0;

	uint32_t temp_u32 = 0;
	 int32_t temp_i32 = 0;
	uint64_t temp_u64 = 0;
	 int64_t temp_i64 = 0;

	 float   temp_f32 = 0.0;
	 double  temp_f64 = 0.0;

	 vm_t    temp_vmt = {0,0};

	while((elapsed < ticks) and (elapsed < 0x80'00'00))
	{
		elapsed++;
		switch(commands[command_ptr].instruction)
		{
			/*
				junk "instructions"
			*/
			case ZERO:
				if(__DEBUG) { std::printf("debug: ZERO\n"); }
				elapsed--;
				break;
			case COMM:
				elapsed--;
				if(__DEBUG) { 
					unsigned char u[5] = {0,0,0,0,0};
					utf8::append(commands[command_ptr].value.value, u);
					std::printf("debug: --- %s - %#x @ %d\n", u, commands[command_ptr].value.value, int(command_ptr)); 
				}
				break;

			/*
				instructions that modify machine internals
			*/

			case ROTATE: // 擰 swaps around A and B
				if(__DEBUG) { std::printf("debug: 擰 ROTATE    @ %d\n", int(command_ptr));}
				if(swap_ab()) { break; }
				else
				{
					std::printf("FATAL ERROR: 擰 @ %d after %d failed; terminating", int(command_ptr), elapsed);
					goto exit_loop;
				}
				break;

			/*
				modifying stack contents without ops
			*/

			case ABANDON: // 棄 (i24) -> () pop without saving
				if(__DEBUG) { std::printf("debug: 棄 ABANDON   @ %d\n", int(command_ptr));}
				remv_main();
				break;

			case DESTROY: // 殲 clear the current main stack
				if(__DEBUG) { std::printf("debug: 殲 DESTROY   @ %d\n", int(command_ptr));}
				main -> clear();
				break;

			case PSH_ZERO: // 零 () -> (i24) push
				if(__DEBUG) { std::printf("debug: 零 PSH_ZERO  @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x00});
				break;

			/*
				arithmetic
			*/
			case INCREMENT:
			/* 
				增 (i24) -> (i24); 
				   (u24) -> (u24); 
				   (f24) -> (f24); 
	        (i48h)(i48l) -> (i48h)(i48l) 
	                    add 1
			*/
			{
				if(__DEBUG) { std::printf("debug: 增 INCREMENT @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT INCREMENT AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_vmt.value += 1;
						break;

					case F24_T:
						temp_f32 
							= std::bit_cast<float>(temp_vmt.value << 8);
						temp_f32 += 1.0;
						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
					
					case INT48L_T:
						temp_i64 = temp_vmt.value;
						temp_vmt = pop_main();
						if(temp_vmt.type != INT48H_T)
						{
							std::printf("MALFORMED I48!\n");
							temp_vmt = {INT48L_T, uint32_t(temp_i64)};
							break;
						}
						temp_i64 += uint64_t(temp_vmt.value) << 24;
						temp_i64 += 1;
						push_main({INT48H_T, uint32_t(temp_i64 >> 24)});
						temp_vmt = {INT48L_T, uint32_t(temp_i64 & 0x7f'ff'ff)};
						break;

					default:
						break;
				}
				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}
			case ADD:
			/*
				加 (i24)(i24) -> (i24); 
				   (u24)(u24) -> (u24); 
				   (f24)(f24) -> (f24); 
	        (i48h)(i48l)
				 +   -----> (i48h)(i48l);
	        (i48h)(i48l) 
	                    add top two
			*/
			{
				if(__DEBUG) { std::printf("debug: 加 ADD       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT ADD AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_type= temp_vmt.type;
						temp_i32 = temp_vmt.value;
						temp_vmt = pop_main();
						if(temp_vmt.type == ERROR_T)
						{
							std::printf("insufficent arguments or unhandled error on stack\n");
							break;
						} 
						else if(temp_vmt.type > 0x06)
						{
							std::printf("type error: cannot add non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						temp_i32 += temp_vmt.value;
						temp_vmt = {temp_type, temp_i32};
						break;

					case F24_T:
						temp_f32 
							= std::bit_cast<float>(temp_vmt.value << 8);
						temp_vmt = pop_main();
						if(temp_vmt.type == ERROR_T)
						{
							std::printf("insufficent arguments or unhandled error on stack\n");
							break;
						}
						else if(temp_vmt.type > 0x06)
						{
							std::printf("type error: cannot add non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						else if(temp_vmt.type < 0x05)
						{
							temp_f32 += float(temp_vmt.value);
						} else
						{ temp_f32 += std::bit_cast<float>(temp_vmt.value << 8); }
						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
						temp_vmt.type = F24_T;
						break;
				}
				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}

			/*
				branching and addressing
			*/
			case WHERE:
			/*
				() -> (addr)
				push current command pointer
				onto side stack
			*/
			{
				if(__DEBUG) { std::printf("debug: 安 WHERE     @ %d\n", int(command_ptr));}
				push_side({ADDR_T, command_ptr});
				break;
			}

			/*
				special instructions
			*/

			case DIE: // 死 is halt and catch fire
				if(__DEBUG) { std::printf("debug: 死 DIE   @ %d\n", int(command_ptr)); }
				std::printf("encountered 死 / sei3, terminating after %d at index #%ld\n", elapsed, command_ptr);
				goto exit_loop;
				break;
			case NOTHING: // 嘸 is a NOP
				if(__DEBUG) { std::printf("debug: 嘸 NOTHING   @ %d\n", int(command_ptr)); }
				break;

			//////////

			default:
				if(__DEBUG) { std::printf("unknown instruction @ %d\n", int(command_ptr));}
				break;
		}
		command_ptr = (command_ptr+1) % commands_sz;
	}

	exit_loop:

	return elapsed;
}