# cjk-stack
another stack machine

# building and dependencies
The program requires a reasonably up-to-date version of `g++` to build, such that it supports the commandline option `-std=gnu++20`. Development is done on WSL Ubuntu 22.04 with GCC 11.4.0, and it should work on anything newer as well.

It has only one external dependency in the form of Nemanja Trifunović's [utfcpp](https://github.com/nemtrif/utfcpp) library, which is bundled with the code.

To build, execute `./build.sh`; the interpreter can be built in release (default or `-r`) or debug (`-d`) mode, which enables logging of individual commands and dumping the whole stack at end of execution.

# operation

The interpreter takes a string (`cstr` or `std::string`, or `std::string_view`) containing instructions, then lexes it into a vector of commands, and then executes commands from that array. The command tape wraps around: executing past the final instruction returns the pointer to the start of the vector, providing mechanics for an infinite loop.  