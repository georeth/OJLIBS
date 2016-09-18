#!/bin/bash

set -e
for f in test/out*.txt; do
	fin=${f/out/in}
	echo Checking $fin $f

	actual=$(mktemp)
	./prog < $fin > $actual
	if ! diff $actual $f; then
		echo "===  input   ==="
		cat $fin
		echo "===  actual  ==="
		cat $actual
		echo "=== expected ==="
		cat $f
		echo "================"
	fi
	rm $actual
done
