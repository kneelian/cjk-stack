#! /bin/bash

RED='\033[1;31m'
YLW='\033[1;33m'
WHT='\033[0m' # white aka no colour

FILES='main.cpp machine.cpp'

if [ $# -eq 1 ]
	then 
		if [ "$1" = "-d" ]
			then g++ ${FILES} -o a.out -O2 -std=gnu++20 -D__DEBUG
		elif [ "$1" = "-h" ]
			then echo -e "${YLW}help${WHT}: run this script with '-d' for debug mode, and '-r' or no argument for release mode"
		elif [ "$1" = "-r" ]
			then g++ ${FILES} -o a.out -O2 -std=gnu++20
		else 
			echo -e "${RED}error${WHT}: unknown argument '${1}'; aborting"
		fi
	elif [ $# -eq 0 ]
		then 
			echo -e "${YLW}hint${WHT}: building in release mode; for debug, pass the flag '-d', or use '-h' for usage and help"
			g++ ${FILES} -o a.out -O2 -std=gnu++20
	else echo -e "${RED}error${WHT}: too many arguments! nr. of args: ${#}; aborting"
fi