#!/bin/bash

make Makefile

PASSED=0
FAILED=0

for i in compiler_tests/*; do
    for j in ${i}/*; do 
    

    nj=${j%.c}
    

    #Skips driver . c files
    if [[ ${j} == *"driver"* ]]; then
        continue;
    fi

    echo "==========================="
    echo "Input file : ${j}"


    ./bin/c_compiler -S ${j} -o test_program.s 
    mips-linux-gnu-gcc -mfp32 -o test_program.o -c test_program.s
    mips-linux-gnu-gcc -mfp32 -static -o test_program test_program.o ${nj}_driver.c

    if qemu-mips test_program;
    then 
        echo "PASS"
        PASSED=$(( ${PASSED}+1 ));
    else 
        echo "FAIL"
        FAILED=$(( ${FAILED}+1 ));


    fi 

    done 
done

echo "########################################"
echo "Passed ${PASSED} , Failed ${FAILED}".
echo ""
