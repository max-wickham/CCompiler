#!/bin/bash
make clean
make -B

if [ $? -ne 0 ]; then
    echo "==========================="
    echo "Failed to build "
    make clean
    exit 
fi 

PASSED=0
FAILED=0

for i in compiler_tests/*; do
    for j in ${i}/*; do 
    

        nj=${j%.c}
        

        #Skips driver . c files
        if [[ ${j} == *"driver.c" ]]; then
            continue;
        fi

        echo "==========================="
        echo "Input file : ${j}"
        #pipe  = [./scope.o < ${j}]
        ./scope.o < ${j} | ./bin/c_compiler > bin/test_program.s 
        #./bin/c_compiler < ./scope.o < ${j} > bin/test_program.s 
        mips-linux-gnu-gcc -mfp32 -o bin/test_program.o -c bin/test_program.s
        mips-linux-gnu-gcc -mfp32 -static -o bin/test_program bin/test_program.o ${nj}_driver.c

        if qemu-mips bin/test_program;
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

make clean