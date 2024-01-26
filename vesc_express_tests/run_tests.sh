#!/bin/bash

echo "*** Making tests"
make clean
make

#file="/dev/ttyUSB0"

success_count=0
fail_count=0
failing_tests=()

vesc_tool="vesc_tool_6.05"

echo "*** Running tests"
exec <>3
for f in *.plisp; do
    if [ -p "test_pipe" ]; then
        rm test_pipe
    fi
    echo "*** Performing test $f"
    mkfifo test_pipe
    
    $vesc_tool --bridgeAppData --vescPort /dev/ttyACM0 --uploadLisp $f >>  test_pipe &
    vesc_pid=$!
    
    while read -r line
    do
        echo $line
        
        if  [[ "$line" == "CHECK: SUCCESS" ]] ;
        then
            success_count=$((success_count+1))
            echo "Test successful!"
        fi
        if  [[ "$line" == "CHECK: FAILURE" ]] ;
        then
            fail_count=$((fail_count+1))
            failing_tests+=("$f")
            echo "Test failed!"
        fi
        if  [[ "$line" == "TEST END"* ]] ;
        then
            echo "Test finished"
            break
        fi
    done  < test_pipe

    echo "VESC_PID: " $vesc_pid
    
    proc=$(ps --pid $vesc_pid -o command=)
    proc=$(echo $proc | cut -d " " -f 1)
    echo "Trying to kill: " $proc
    if [[ $proc == $vesc_tool ]]
    then
        kill -9 $vesc_pid 
        if [ $? -eq 0 ] 
        then
            echo "KILL: VESC_TOOL"
        else
            echo "Error: Could not kill VESC_TOOL"
        fi
    else
        echo "VESC_TOOL died by itself"
    fi    
done

echo ""
echo "Failed tests"
echo "=================================================="
for ((i = 0; i < ${#failing_tests[@]}; i ++ ))
do
    echo "(FAILURE)" ${failing_tests[$i]}
done

echo ""
echo "Stats"
echo "=================================================="
echo Tests passed: $success_count
echo Tests failed: $fail_count
