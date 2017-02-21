#!/bin/bash

set -e
for f in test/in*.txt; do
	fout=${f/in/out}
	echo Checking $f $fout

	actual=$(mktemp)
	./prog < $f > $actual
	if ! [ -r $fout ]; then
		echo "===  input   ==="
		cat $f
		echo "===  actual  ==="
		cat $actual
		echo "================"
		echo "No $fout found"
		echo "================"
	elif ! diff $actual $fout; then
		echo "===  input   ==="
		cat $f
		echo "===  actual  ==="
		cat $actual
		echo "=== expected ==="
		cat $fout
		echo "================"
	fi
	rm $actual
done
