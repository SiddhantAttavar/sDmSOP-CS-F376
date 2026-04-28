python3 bin/reset_results.py
for i in {0..9}; do
	nextflow run main.nf -with-report -with-trace -with-timeline --workflow_type=full
	cp -r results ../results-bak-$i
	cp -r instance_results ../instance_results-bak-$i
	python3 bin/write_results.py
done

