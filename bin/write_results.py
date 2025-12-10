#!/usr/bin/env python3
import re
import csv
import sys
from pathlib import Path
from glob import glob

def parse_run_instance_file(file_path):
	"""
	Parse a single run_instances output file.
	Returns a dictionary of values to update the CSV.
	Customize this depending on your output format.
	"""
	file_path = Path(file_path)

	with open(file_path, 'r') as f:
		lines = [line.strip() for line in f if line.strip()]

	return int(lines[4])

def get_csv_path(file_path):
	"""
	Translate a run_instances output file path into the corresponding CSV file path.

	Example:
	input:  'stop_instances/stop_2veh/115u574_RND_T60_p2.sop'
	output: 'results/stop_2veh/Set2_p2.csv'
	"""
	file_path = Path(file_path)
	# Extract vehicle type from parent folder: 'stop_2veh'
	# veh = file_path.parent.name  # e.g., 'stop_2veh'

	# Extract file name
	fname = file_path.stem  # e.g., '115u574_RND_T60_p2'
	veh = f"stop_{fname.split('_')[1]}"

	# Determine Set number
	set_number = 2 if '_RND' in fname else 1

	# Determine p_value from '_p1' or '_p2' at the end
	match = re.search(r'_p(\d+)$', fname)
	if match:
		p_value = match.group(1)
	else:
		raise ValueError(f"Cannot extract p_value from file: {file_path}")

	# Extract T value
	t_match = re.search(r'_T(\d+)', fname)
	if not t_match:
		raise ValueError(f"Cannot extract T value from file: {file_path}")
	t_value = int(t_match.group(1))

	# Extract base instance name (everything before optional _RND)
	base_name = fname.split('_RND')[0] if '_RND' in fname else fname.split('_T')[0]
	base_name = base_name.split('veh_')[1]

	# Build CSV path
	csv_path = Path(f"results/{veh}/Set{set_number}_p{p_value}.csv")
	return csv_path, base_name, t_value

def update_csv(file, profit, csv_data):
	"""
	Update the CSV entry corresponding to a run_instances output.

	- file: path to instance file like stop_instances/...sop
	- profit: int
	- csv_data: dictionary produced by load_csv_files()

	Updates:
		row['GAP_{t_value}'] = max(previous_value, profit)
	"""

	csv_path, instance, t_value = get_csv_path(file)
	veh_name = csv_path.parent.name
	csv_name = csv_path.name   # key inside csv_data
	alns_col = f"ALNS_T{t_value}"
	stop_col = f"STOP_T{t_value}"
	gap_col = f"GAP_T{t_value}"

	if csv_path not in csv_data:
		raise KeyError(f"CSV file {csv_path} not found in csv_data")

	data = csv_data[csv_path]
	rows = data["rows"]
	header = data["header"]

	# Ensure GAP column exists
	if alns_col not in header:
		raise KeyError(f"Column {alns_col} does not exist in {csv_name}")

	# Find row with matching instance name
	for row in rows:
		if row["Instances"] != instance:
			continue

		if row.get(stop_col, "") == '-':
			return

		# Extract previous value
		alns_val = int(row.get(alns_col, ""))
		stop_val = int(row.get(stop_col, ""))
		if profit <= alns_val:
			return

		print(
			f"Max profit for {veh_name}/{csv_name.split('.')[0]}/{instance}_T{t_value} "
			f"has improved from {alns_val} to {profit}. STOP value is {stop_val}"
		)

		# Update to max
		row[alns_col] = str(profit)
		row[gap_col] = str((profit - stop_val) / stop_val)
		return

	# If the instance is missing, raise error
	raise KeyError(f"Instance {instance} not found in {csv_name}")

def load_csv_files(csv_paths):
	"""
	Load CSV files into a dictionary where:
	  key   = CSV file name (e.g. 'Set1_p2.csv')
	  value = list of OrderedDict rows (editable)
	"""
	csv_dict = {}

	for path in csv_paths:
		path = Path(path)
		with open(path, newline='') as f:
			reader = csv.DictReader(f)
			rows = list(reader)   # editable rows
		csv_dict[path] = {
			"path": path,
			"header": reader.fieldnames,
			"rows": rows
		}

	return csv_dict

def save_csv(data):
	path = data["path"]
	header = data["header"]
	rows = data["rows"]

	with open(path, 'w', newline='') as f:
		writer = csv.DictWriter(f, fieldnames=header)
		writer.writeheader()
		writer.writerows(rows)

def main():
	instance_files = sys.argv[1:]
	if not instance_files:
		instance_files = list(glob('instance_results/*.txt'))

	csv_files = list(glob('results/*/*.csv'))
	csv_data = load_csv_files(csv_files)

	for file_path in instance_files:
		profit = parse_run_instance_file(file_path)
		update_csv(file_path, profit, csv_data)

	for file, data in csv_data.items():
		save_csv(data)

if __name__ == "__main__":
	main()
