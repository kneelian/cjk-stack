#include <cmath>

#include "machine.h"
#include "types.h"
#include "to_hanzi.h"

#include "debug.h"

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
            case 0x4f4e: // 低 - down/decrement = dai1
            	destination.push_back({DECREMENT, {0,0}});
            	break;
            case 0x52a0: // 加 - add = gaa1
            	destination.push_back({ADD, {0,0}});
            	break;
            case 0x6e1b: // 減 - sub = gam2
            	destination.push_back({SUB, {0,0}});
            	break;
            case 0x9664: // 除 - div = cui3
            	destination.push_back({DIV, {0,0}});
            	break;
            case 0x4e58: // 乘 - mul = sing4
            	destination.push_back({MUL, {0,0}});
            	break;
            case 0x9918: // 餘 - remainder = jyu4
            	destination.push_back({MOD, {0,0}});
            	break;
            case 0x57FA: // 基 - base = gei1
            	destination.push_back({LOG_E, {0,0}});
            	break;
            case 0x6839: // 根 - root = gan1
            	destination.push_back({ROOT, {0,0}});
            	break;
            case 0x51aa: // 冪 - exponent = mik6
            	destination.push_back({POW, {0,0}});
            	break;
            case 0x5638: // 嘸 - nothing = m4
                destination.push_back({NOTHING,{0,0}});
                break;
            case 0x5831: // 報 - report = bou3
            	destination.push_back({REPORT,{0,0}});
            	break;
            case 0x589e: // 增 - add/expand = zang1
            	destination.push_back({INCREMENT,{0,0}});
            	break;
            case 0x5b56: // 孖 - twin = maa1
            	destination.push_back({TWIN, {0,0}});
            	break;
            case 0x5b89: // 安 - where[literary] = on1
            	destination.push_back({WHERE,  {0,0}});
            	break;
            case 0x63db: // 換 - swap = wun6
            	destination.push_back({SWAP, {0,0}});
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
            case 0x7d21: // 紡 - spin/swap3 = fong2
            	destination.push_back({SWAP3, {0,0}});
            	break;
            case 0x843d: // 落 - fall = lok6
            	destination.push_back({FALL, {0,0}});
            	break;
            case 0x8df3: // 跳 - jump = tiu3
            	destination.push_back({JUMP, {0,0}});
            	break;
            case 0x96f6: // 零 - zero = leng4
            	destination.push_back({PSH_ZERO,{0, 0}});
            	break;
			case 0x58f9: // 壹 - one = jat1
				destination.push_back({PSH_ONE ,{0,0}});
				break;
			case 0x8cb3: // 貳 - two = ji6
				destination.push_back({PSH_TWO ,{0,0}});
				break;
			case 0x53c3: // 參 - three = saam1 
				destination.push_back({PSH_THREE,{0,0}});
				break;
			case 0x8086: // 肆 - four = sei3
				destination.push_back({PSH_FOUR,{0,0}});
				break;
			case 0x4f0d: // 伍 - five = ng5  
				destination.push_back({PSH_FIVE,{0,0}});
				break;
			case 0x9678: // 陸 - six = luk6
				destination.push_back({PSH_SIX ,{0,0}});
				break;
			case 0x67d2: // 柒 - seven = cat1
				destination.push_back({PSH_SEVEN,{0,0}});
				break;
			case 0x634c: // 捌 - eight = baat3
				destination.push_back({PSH_EIGHT,{0,0}});
				break;
			case 0x7396: // 玖 - nine = gau2
				destination.push_back({PSH_NINE,{0,0}});
				break;
			case 0x62fe: // 拾 - ten = sap6
				destination.push_back({PSH_TEN ,{0,0}});
				break;
			case 0x53DB: // 叛 - betray = bun6
				destination.push_back({BETRAY, {0,0}});
				break;
			case 0x985e: // 類 - type = leoi6
				destination.push_back({TYPE, {0,0}});
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
				typesystem-related ops
			*/
			case TYPE: // 類 (?) -> (?) (typeid)
			{
				temp_vmt = peek_main();
				push_side({TYPE_T, temp_vmt.type});
				break;
			}

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
			case PSH_ONE:
				if(__DEBUG) { std::printf("debug: 壹 PSH_ONE   @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x01});
				break;
			case PSH_TWO:
				if(__DEBUG) { std::printf("debug: 貳 PSH_TWO   @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x02});
				break;
			case PSH_THREE:
				if(__DEBUG) { std::printf("debug: 參 PSH_THREE @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x03});
				break;
			case PSH_FOUR:
				if(__DEBUG) { std::printf("debug: 肆 PSH_FOUR  @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x04});
				break;
			case PSH_FIVE:
				if(__DEBUG) { std::printf("debug: 伍 PSH_FIVE  @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x05});
				break;
			case PSH_SIX:
				if(__DEBUG) { std::printf("debug: 陸 PSH_SIX   @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x06});
				break;
			case PSH_SEVEN:
				if(__DEBUG) { std::printf("debug: 柒 PSH_SEVEN @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x07});
				break;
			case PSH_EIGHT:
				if(__DEBUG) { std::printf("debug: 捌 PSH_EIGHT @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x08});
				break;
			case PSH_NINE:
				if(__DEBUG) { std::printf("debug: 玖 PSH_NINE  @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x09});
				break;
			case PSH_TEN:
				if(__DEBUG) { std::printf("debug: 拾 PSH_TEN  @ %d\n", int(command_ptr));}
				push_main({INT24_T, 0x0a});
				break;

			case TWIN: // 孖 (x) -> (x)(x) double the top of stack
				if(__DEBUG) { std::printf("debug: 孖 TWIN      @ %d\n", int(command_ptr));}
				temp_vmt = peek_main();
				push_main(temp_vmt);
				break;

			case SWAP: // 換 - swap = wun6 (a)(b) -> (b)(a)
            	if(__DEBUG) { std::printf("debug: 換 SWAP      @ %d\n", int(command_ptr));} 
            	temp_vmt = pop_main();
            	push_side(pop_main());
            	push_main(temp_vmt);
            	push_main(pop_side());
            	break;

            case SWAP3: // 紡 (a)(b)(c) -> (c)(a)(b)
            	if(__DEBUG) { std::printf("debug: 紡 SWAP3     @ %d\n", int(command_ptr));}
            	temp_vmt = pop_main();
            	push_side(pop_main());
            	push_side(pop_main());
            	push_main(temp_vmt);
            	push_main(pop_side());
            	push_main(pop_side());
            	break;

			/* ---------------------------
				arithmetic
			*/
			case INCREMENT:
			/* 
				增 (i24) -> (i24); 
				   (u24) -> (u24); 
				   (f24) -> (f24); 
	        (i48h)(i48l) -> (i48h)(i48l) 
	                    add 1
			*/ // TODO : fix i48
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

			case DECREMENT:
			/*
				低 (i24) -> (i24); 
				   (u24) -> (u24); 
				   (f24) -> (f24); 
	        (i48h)(i48l) -> (i48h)(i48l) 
	                    sub 1
			*/ // TODO : fix i48
			{
				if(__DEBUG) { std::printf("debug: 低 DECREMENT @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT DECREMENT AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_vmt.value -= 1;
						break;

					case F24_T:
						temp_f32 
							= std::bit_cast<float>(temp_vmt.value << 8);
						temp_f32 -= 1.0;
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
						temp_i64 -= 1;
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
			*/ // TODO : i48
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
			case 0x9664: // 除 - div = cui3
            	destination.push_back({DIV, {0,0}});
            	break;
            case 0x4e58: // 乘 - mul = sing4
            	destination.push_back({MUL, {0,0}});
            	break;
            case 0x9918: // 餘 - remainder = jyu4
            	destination.push_back({MOD, {0,0}});
            	break;
			*/

			case SUB:
			/*
				減 (i24)(i24) -> (i24); 
				   (u24)(u24) -> (u24); 
				   (f24)(f24) -> (f24); 
	        (i48h)(i48l)
				 -   -----> (i48h)(i48l);
	        (i48h)(i48l) 
	                    sub top two
			*/ // TODO : i48
			{
				if(__DEBUG) { std::printf("debug: 減 SUB       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT SUB AN ERROR!\n");
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
							std::printf("type error: cannot sub non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						temp_i32 -= temp_vmt.value;
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
							std::printf("type error: cannot sub non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						else if(temp_vmt.type < 0x05)
						{
							temp_f32 -= float(temp_vmt.value);
						} else
						{ temp_f32 -= std::bit_cast<float>(temp_vmt.value << 8); }
						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
						temp_vmt.type = F24_T;
						break;
				}
				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}
			case DIV:
			/*
				除 (i24)(i24) -> (i24); 
				   (u24)(u24) -> (u24); 
				   (f24)(f24) -> (f24); 
	        (i48h)(i48l)
				 /   -----> (i48h)(i48l);
	        (i48h)(i48l) 
	                    div top with top-1
			*/ // TODO : i48
			{
				if(__DEBUG) { std::printf("debug: 除 DIV       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT DIV AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_type = temp_vmt.type;
						temp_i32  = temp_vmt.value;
						temp_vmt  = pop_main();
						if(temp_vmt.type == ERROR_T)
						{
							std::printf("insufficent arguments or unhandled error on stack\n");
							break;
						}
						else if(temp_vmt.type > 0x06)
						{
							std::printf("type error: cannot div non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						if(temp_vmt.value)
							temp_i32 /= temp_vmt.value;
						else
							temp_i32 =  0xff'ff'ff;
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
							std::printf("type error: cannot div non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						else if(temp_vmt.type < 0x05)
						{
							if(temp_vmt.value)
								temp_f32 /= float(temp_vmt.value);
							else
								temp_f32  = std::bit_cast<float>(0x7F'80'00'00);
						} else
						{
							if(temp_vmt.value)
								temp_f32 /= std::bit_cast<float>(temp_vmt.value << 8); 
							else
								temp_f32  = std::bit_cast<float>(0x7F'80'00'00);
								
						}
						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
						temp_vmt.type = F24_T;
						break;
				}
				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}
			case MUL:
			/*
				乘 (i24)(i24) -> (i24); 
				   (u24)(u24) -> (u24); 
				   (f24)(f24) -> (f24); 
	        (i48h)(i48l)
				 *   -----> (i48h)(i48l);
	        (i48h)(i48l) 
	                    mul top with top-1
			*/ // TODO : i48
			{
				if(__DEBUG) { std::printf("debug: 乘 MUL       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();

				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT MUL AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_type = temp_vmt.type;
						temp_i32  = temp_vmt.value;
						temp_vmt  = pop_main();
						if(temp_vmt.type == ERROR_T)
						{
							std::printf("insufficent arguments or unhandled error on stack\n");
							break;
						}
						else if(temp_vmt.type > 0x06)
						{
							std::printf("type error: cannot mul non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						temp_i32 *= temp_vmt.value;
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
							std::printf("type error: cannot div non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						else if(temp_vmt.type < 0x05)
						{ temp_f32 *= float(temp_vmt.value); } else
						{ temp_f32 *= std::bit_cast<float>(temp_vmt.value << 8); }

						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
						temp_vmt.type = F24_T;
						break;
				}

				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}
			case MOD:
			/*
				餘 (i24)(i24) -> (i24); 
				   (u24)(u24) -> (u24); 
				   (f24)(f24) -> (f24); 
	        (i48h)(i48l)
				 %   -----> (i48h)(i48l);
	        (i48h)(i48l) 
	                    mod top with top-1
			*/ // TODO : i48
			{ 
				if(__DEBUG) { std::printf("debug: 餘 MOD       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();

				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT MOD AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_type = temp_vmt.type;
						temp_i32  = temp_vmt.value;
						temp_vmt  = pop_main();
						if(temp_vmt.type == ERROR_T)
						{
							std::printf("insufficent arguments or unhandled error on stack\n");
							break;
						}
						else if(temp_vmt.type > 0x06)
						{
							std::printf("type error: cannot mod non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						temp_i32 %= temp_vmt.value;
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
							std::printf("type error: cannot div non-numeric typeid %#x", temp_vmt.type);
							push_main(temp_vmt);
							temp_vmt={temp_type, temp_i32};
							break;
						}
						else if(temp_vmt.type < 0x05)
						{
							temp_i32 =  int(temp_f32 / float(temp_vmt.value));
							temp_f32 =  (temp_f32 / float(temp_vmt.value)) - temp_i32;
						} else
						{
							temp_i32 =  int(temp_f32 / std::bit_cast<float>(temp_vmt.value << 8));
							temp_f32 =  (temp_f32 / std::bit_cast<float>(temp_vmt.value << 8)) - temp_i32; 
						}

						temp_vmt.value 
							= std::bit_cast<uint32_t>(temp_f32) >> 8;
						temp_vmt.type = F24_T;
						break;
				}

				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}

			case LOG_E: // top log_e (numeric)->(f24)
			// TODO : i48
			{
				if(__DEBUG) { std::printf("debug: 基 LOG_E     @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();

				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT LOG_E AN ERROR!\n");
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_f32 = float(temp_vmt.value);
						temp_f32 = std::log(temp_f32);
						temp_vmt = {F24_T, std::bit_cast<uint32_t>(temp_f32) >> 8};
						break;

					case F24_T:
						temp_f32 = std::bit_cast<float>(temp_vmt.value << 8);
						temp_f32 = std::log(temp_f32);
						temp_vmt = {F24_T, std::bit_cast<uint32_t>(temp_f32) >> 8};
						break;

					default:
						std::printf("type error: cannot log_e non-numeric typeid %#x", temp_vmt.type);
						break;
				}

				push_main(temp_vmt);
				temp_vmt = {0, 0};
				break;
			}

			case ROOT:
			/*
				(num)(num) -> (f24)
				base  root
			*/
			// TODO: i48
			{
				if(__DEBUG) { std::printf("debug: 根 ROOT      @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				temp_type = temp_vmt.type;

				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT ROOT AN ERROR!\n");
						push_main(temp_vmt);
						goto end_root;
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_i32 = temp_vmt.value;
						temp_f32 = float(temp_vmt.value);
						break;

					case F24_T:
						temp_f32 = std::bit_cast<float>(temp_vmt.value << 8);
						break;

					default:
						std::printf("type error: cannot root non-numeric typeid %#x", temp_vmt.type);
						break;
				}
				temp_vmt = pop_main();

				switch(temp_vmt.type)
				{
					case ERROR_T:
					{
						std::printf("insufficent arguments or unhandled error on stack\n");
						push_main(temp_vmt);
						if(temp_type < 0x05)
							{ push_main({temp_type, temp_i32}); } 
						else
							{ push_main({temp_type, std::bit_cast<uint32_t>(temp_f32) >> 8}); }
						goto end_root;
					}

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_f32 = std::pow(temp_f32, 1.0 / float(temp_vmt.value));
						break;

					case  F24_T:
						temp_f32 = std::pow(temp_f32, 1.0 / std::bit_cast<float>(temp_vmt.value << 8));
						break;
				}

				push_main({F24_T, std::bit_cast<uint32_t>(temp_f32) >> 8});

				end_root:
				break;
			}

			case POW: 
			/*
				(num)(num) -> (f24)
				base  root
			*/
			// TODO: i48
			{
				if(__DEBUG) { std::printf("debug: 冪 POW       @ %d\n", int(command_ptr));}
				temp_vmt = pop_main();
				temp_type = temp_vmt.type;

				switch(temp_vmt.type)
				{
					case ERROR_T:
						std::printf("CANNOT POW AN ERROR!\n");
						push_main(temp_vmt);
						goto end_pow;
						break;

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_i32 = temp_vmt.value;
						temp_f32 = float(temp_vmt.value);
						break;

					case F24_T:
						temp_f32 = std::bit_cast<float>(temp_vmt.value << 8);
						break;

					default:
						std::printf("type error: cannot pow non-numeric typeid %#x", temp_vmt.type);
						break;
				}
				temp_vmt = pop_main();

				switch(temp_vmt.type)
				{
					case ERROR_T:
					{
						std::printf("insufficent arguments or unhandled error on stack\n");
						push_main(temp_vmt);
						if(temp_type < 0x05)
							{ push_main({temp_type, temp_i32}); } 
						else
							{ push_main({temp_type, std::bit_cast<uint32_t>(temp_f32) >> 8}); }
						goto end_pow;
					}

					case  INT24_T:
					case UINT24_T:
					case   ADDR_T:
						temp_f32 = std::pow(temp_f32, float(temp_vmt.value));
						break;

					case  F24_T:
						temp_f32 = std::pow(temp_f32, std::bit_cast<float>(temp_vmt.value << 8));
						break;
				}

				push_main({F24_T, std::bit_cast<uint32_t>(temp_f32) >> 8});

				end_pow:
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
				push_side({ADDR_T, command_ptr-1});
				break;
			}

			case JUMP:
			/*
				(addr) -> ()
				pops address off side stack
				jumps to address
			*/
			{
				if(__DEBUG) { std::printf("debug: 跳 JUMP      @ %d\n", int(command_ptr));}
				temp_vmt = pop_side();
				if(temp_vmt.type != ADDR_T)
				{
					std::printf("type error: cannot jump to non-ADDR_T (%#x) @ %d\n", temp_vmt.type, int(command_ptr));
					push_side(temp_vmt);
				}
				command_ptr = temp_vmt.value;
				temp_vmt = {0,0};
				break;
			}

			case FALL:
			/*
				(cond)(addr) -> ()
				pops address off side stack
				peeks at condition
				if nonzero, jumps to address
				if zero: pops address and condition
			*/
			{
				if(__DEBUG) { std::printf("debug: 落 FALL      @ %d\n", int(command_ptr));}
				temp_vmt = pop_side();
				if(peek_side().value == 0x0)
				{
					temp_vmt = {0,0};
					remv_side();
					break;
				}

				if(temp_vmt.type != ADDR_T)
				{
					std::printf("type error: cannot jump to non-ADDR_T (%#x) @ %d\n", temp_vmt.type, int(command_ptr));
					push_side(temp_vmt);
				}

				command_ptr = temp_vmt.value;
				temp_vmt = {0,0};

				break;
			}

			/*
				print instructions
			*/

			case REPORT: // 報 peek top of stack and print to console log as i24 in chinese numerals
			{
				if(__DEBUG) { std::printf("debug: 報 REPORT    @ %d\n", int(command_ptr)); }
				temp_i32 = peek_main().value;

				bool negative = false; 

				if(temp_i32 & 0x00'80'00'00) 
				{ 
					negative = true;
					temp_i32 |= 0xff'00'00'00;
					temp_i32 = -temp_i32;
				}

				std::string temp_string;
				to_hanzi(temp_string, temp_i32);

				std::printf(
					"[%s%s]\n",
					(negative?"負":""), 
					temp_string.data()
				);

				break;
			}

			/*
				special instructions
			*/

			case BETRAY: // 叛 toggle debug mode
				if(__DEBUG) { std::printf("debug: 叛 BETRAY    @ %d - toggling to release\n", int(command_ptr));}
				__DEBUG = !__DEBUG;
				break;


			case DIE: // 死 is halt and catch fire
				if(__DEBUG) { std::printf("debug: 死 DIE       @ %d\n", int(command_ptr)); }
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