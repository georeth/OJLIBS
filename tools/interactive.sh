#!/bin/bash
set -e

Q=test/Q-chan
A=test/A-chan

for question in test/q-*; do
    logfile=$(mktemp)
    rm -f $Q
    rm -f $A

    mkfifo $Q
    mkfifo $A

    # buffered IO may caused corrupt logfile
    sed 's/^/= /' $question >> $logfile;
    cat $A | tee >(sed 's/^/< /' >> $logfile) | ./prog | tee $Q | sed 's/^/> /' >> $logfile &
    if cat $question $Q | ./tester > $A; then
	echo "$question: PASSED"
    else
	echo "$question: FAILED"
	cat $logfile
    fi
    rm $logfile
done
