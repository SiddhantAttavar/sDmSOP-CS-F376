#!/bin/sh

for file in results/*/*.csv; do
	echo $file
	python3 bin/parse_csv.py 1 $file gap 1 | tail -n 1
done
