# cjk-stack
another stack machine

# building and dependencies
The program requires a reasonably up-to-date version of `g++` to build, such that it supports the commandline option `-std=gnu++20`. Development is done on WSL Ubuntu 22.04 with GCC 11.4.0, and it should work on anything newer as well. It requires GNU extensions, specifically ranged switch-case (e.g. in `case 0x20 ... 0x2e79:`), to avoid having to use convoluted if-statements; this can easily be changed if necessary. 

It has only one external dependency in the form of Nemanja Trifunović's [utfcpp](https://github.com/nemtrif/utfcpp) library, which is bundled with the code.

To build, execute `./build.sh`; the interpreter can be built in release (default or `-r`) or debug (`-d`) mode, which enables logging of individual commands and dumping the whole stack at end of execution.

# usage

This is currently a command-line utility consisting of the calling shell-code and the interpreter. To use it, either simply invoke the filename (for example, `a.out`) in which case it will execute a hardcoded demo fibonacci program, or additionally supply it with an input file (for example, `a.out ./samples/fib.cjk`) to interpret it as source.

It currently doesn't support any option flags.

# operation

The shellcode sets up the program source and handles invocation of the interpreter machine object.

The interpreter takes a string (`cstr` or `std::string`, or `std::string_view`) containing instructions, then lexes it into a vector of commands, and then executes commands from that array. The command tape wraps around: executing past the final instruction returns the pointer to the start of the vector, providing mechanics for an infinite loop.  
