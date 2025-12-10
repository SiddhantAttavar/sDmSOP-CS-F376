#!/usr/bin/env nextflow

params.veh_types = ['stop_1veh', 'stop_2veh', 'stop_3veh']
params.solution = 'sDmSOP_ALNS_STOP.out'
params.results_dir = "${projectDir}/results"

process process_csv {
    input:
    tuple val(veh), path(csv_file)

    output:
    path "*.txt"

    script:
    """
    parse_csv.py ${veh} ${csv_file} > ${veh}_${csv_file.simpleName}.txt
    """
}

process merge_results {
    input:
    path txt_files

    output:
    path "run_instances.txt"

    script:
    """
    cat ${txt_files.join(" ")} > run_instances.txt
    """
}

process run_instances {
    publishDir "instance_results", mode: 'copy'

    input:
    tuple val(veh), path(instance)

    output:
    path("*.txt")

    script:
    result = "${veh}_${instance.simpleName}.txt"
    """
    ${params.solution} ${instance} ${result}
    """
}

process write_results {
    input:
    path results

    script:
    """
    write_results.py ${params.results_dir} ${results}
    """
}

workflow {
    csv_files = Channel.fromPath(
            params.veh_types.collect { "${params.results_dir}/${it}/*.csv" },
        ).flatten()
        .map { f -> tuple(f.parent.name, f) }

    processed = process_csv(csv_files).collect()

    instance_files = merge_results(processed).flatMap { merged_file ->
        merged_file
            .toFile()
            .readLines()
            .findAll { it }
            .collect { file(it) }
    }

    sorted_instances = instance_files.collect()
        .map { list ->
            list.sort { file -> 
                def fname = file.getName()
                def match = fname =~ /^(\d+)/
                match ? match[0][1].toInteger() : 0
            }
        }
        .flatMap { it }
        .map { tuple(it.parent.name, it) }

    output = run_instances(sorted_instances)

    write_results(output.collect())
}
