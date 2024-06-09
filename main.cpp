#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "debug.h"

#include "utf8.h"

#include "types.h"
#include "machine.h"

const char* type_as_string(uint32_t in);

int main(int argc, char** argv)
{
	#ifdef __DBG
		__DEBUG = true;
	#else
		__DEBUG = false;
	#endif

	std::printf("mode: %s\n", __DEBUG?"debug":"release");

	machine_c machine;

	std::string example_string;

	if(argc > 1)
	{
		std::FILE* input = std::fopen(argv[1], "r");

		int tmp = 0;
		while ((tmp = std::fgetc(input)) != EOF) 
		{
			example_string.push_back(tmp);
		}

		std::fclose(input);
	}
	else
	{
		example_string = //argc>1?argv[1]:
		"死"
		//"擰伍玖加擰 零壹fib(安換孖紡換孖紡加報 擰換低換擰 落)end死" // 叛
		//"零增孖增孖紡換孖紡死";
		// 換孖紡換孖紡加
		;
	}
	machine.load(example_string);
	uint32_t lexed_count = machine.lex();

	if(__DEBUG) { std::printf("-----------\n"); }

	int runtime = machine.run(1<<16);

	std::printf("-----------\n");

	std::printf("command runtime: %d\n=========\n", runtime);

	if(__DEBUG)
	{
		std::printf("program: \n%.*s\n", int(machine.program.length()), machine.program.data());
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

        case TYPE_T:  return "typ"; break;

        case LABL_T:  return "lbl"; break;
        case PRED_T:  return "prd"; break;

        case CUSTOM_T:return "usr"; break;

        case COMM_T:  return "com"; break;

        case ERROR_T: return "err"; break;

        default:
            return "????";
    }
    return "????";
}