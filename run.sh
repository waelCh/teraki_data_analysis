#!/usr/bin/env bash
# it can only be executed, do not allow to source
# and crash in a controlled way if some unbound variable is used

[[ $- == *i* ]] && echo "Shell Interactive" || echo "Shell Not interactive"
if [[ $0 != "$BASH_SOURCE" ]]; then
    echo "ERROR: I'm a script forcing you to execute. Don't source me!" >&2
    return 1
else
    # if I am OK to execute, force that the script stops if variables are not defined
    # this catches bugs in the code when you think a variable has value, but it is empty
    set -eu
fi

#############################################################################################
### Configuration
#############################################################################################

DEBUG="0"
VERBOSE="0"

DO_CMAKE_AND_MAKE="1"
DO_ONLY_MAKE="0"
DO_EXIT_AFTER_COMPILATION="0"

DO_RUN="1"

#############################################################################################
### Compilation
#############################################################################################

# cmake + make
if [[ ${DO_CMAKE_AND_MAKE} == "1" ]]; then
    COMMAND="cd code && cd build && rm -rf * && cmake ../source && make && cd .. && cd .."
    echo "COMMAND=${COMMAND}"
    eval ${COMMAND}    
fi

# only make
if [[ ${DO_ONLY_MAKE} == "1" ]]; then
    COMMAND="make -C code/build"
    echo "COMMAND=${COMMAND}"
    eval ${COMMAND}
fi

# exit, safer to be on while we are compiling for the first time, to move to running only once we expect compilation to work
if [[ ${DO_EXIT_AFTER_COMPILATION} == "1" ]]; then
    COMMAND="exit"
    echo "COMMAND=${COMMAND}"
    eval ${COMMAND}
fi

#############################################################################################
### Running
#############################################################################################

if [[ ${DO_RUN} == "0" ]]; then
    exit 0
fi

# if here, you also want to run

COMMAND="./code/build/test.exe" # for example, assuming you create an executable test.exe
echo "COMMAND=${COMMAND}"
eval ${COMMAND}

#############################################################################################
### Done
#############################################################################################

echo "Done ./run.sh!"
