#!/usr/bin/env python3
import csv
import sys

"""
Usage:
    python3 parse_csv.py num_veh input.csv

CSV columns:
Instances,STOP_T40,ALNS_T40,GAP_T40,
          STOP_T60,ALNS_T60,GAP_T60,
          STOP_T80,ALNS_T80,GAP_T80
"""

if len(sys.argv) != 4 and len(sys.argv) != 5:
    print("Usage: python3 parse_csv.py <num_veh> <csv_file> <workflow_type> <verbose>")
    sys.exit(1)

veh = sys.argv[1]
csv_file = sys.argv[2]
workflow_type = sys.argv[3]
verbose = len(sys.argv) == 5
p_value = 'p1' if 'p1' in csv_file else 'p2'

negatives = []   # store (instance, T) pairs
neg = 0
pos = 0

with open(csv_file, newline='') as f:
    reader = csv.DictReader(f)

    for row in reader:
        instance = row["Instances"]

        # Check GAP_T40, GAP_T60, GAP_T80
        for T in [40, 60, 80]:
            gap_col = f"GAP_T{T}"

            # Convert to float safely
            try:
                gap_val = float(row[gap_col])
            except ValueError:
                continue  # skip if missing or invalid

            if gap_val < 0:
                neg += 1
            elif gap_val > 0:
                pos += 1

            if workflow_type == 'gap':
                if gap_val < 0:
                    negatives.append((instance, T, gap_val))
            else:
                negatives.append((instance, T, gap_val))

# Print results
for inst, T, gap_val in negatives:
    file_name = f'stop_instances/{veh}/{inst}'
    if 'Set2' in csv_file:
        file_name += '_RND'
    file_name += f'_T{T}_{p_value}.sop'
    print(file_name)

if verbose:
    print(neg, pos)
