for i in {0..9}; do
	nextflow run main.nf -with-report -with-trace -with-timeline --workflow_type=full
	cp results ../results-bak-$i
	cp instance_results ../instance_results-bak-$i
	py bin/reset_results.py
done
