#!/usr/bin/env python3
import csv
import shutil
from pathlib import Path
from glob import glob

def reset_alns_values():
	"""
	Reset all ALNS_T* values to 0 in CSV files and update GAP columns accordingly.
	GAP is calculated as (ALNS - STOP) / STOP
	"""
	csv_files = list(glob('results/*/*.csv'))
	
	for csv_path in csv_files:
		csv_path = Path(csv_path)
		print(f"Processing {csv_path}...")
		
		# Read the CSV file
		with open(csv_path, 'r', newline='') as f:
			reader = csv.DictReader(f)
			header = reader.fieldnames
			rows = list(reader)
		
		# Find all ALNS and GAP columns
		alns_columns = [col for col in header if col.startswith('ALNS_T')]
		gap_columns = [col for col in header if col.startswith('GAP_T')]
		
		# Update rows
		for row in rows:
			for alns_col in alns_columns:
				# Reset ALNS to 0
				if row.get(alns_col, '') != '' and row.get(alns_col, '') != '-':
					row[alns_col] = '0'
			
			for gap_col in gap_columns:
				# Extract T value from GAP column name (e.g., GAP_T40 -> 40)
				t_value = gap_col.split('_T')[1]
				stop_col = f'STOP_T{t_value}'
				alns_col = f'ALNS_T{t_value}'
				
				# Calculate new GAP: (0 - STOP) / STOP = -1
				if row.get(stop_col, '') not in ['', '-']:
					stop_val = int(row[stop_col])
					if stop_val != 0:
						row[gap_col] = str(-1.0)
					else:
						row[gap_col] = '0'
				else:
					row[gap_col] = '-'
		
		# Write back to CSV
		with open(csv_path, 'w', newline='') as f:
			writer = csv.DictWriter(f, fieldnames=header)
			writer.writeheader()
			writer.writerows(rows)
		
		print(f"  ✓ Reset ALNS values and updated GAP columns")

def clear_instances_directory():
	"""
	Remove all files from results/instances directory.
	"""
	instances_dir = Path('results/instances')
	
	if not instances_dir.exists():
		print(f"Directory {instances_dir} does not exist. Nothing to clear.")
		return
	
	# Remove all files in the directory
	file_count = 0
	for file_path in instances_dir.iterdir():
		if file_path.is_file():
			file_path.unlink()
			file_count += 1
	
	print(f"Removed {file_count} file(s) from {instances_dir}")

def main():
	print("=" * 60)
	print("RESET RESULTS SCRIPT")
	print("=" * 60)
	print()
	
	print("Step 1: Resetting ALNS values in CSV files...")
	reset_alns_values()
	print()
	
	print("Step 2: Clearing results/instances directory...")
	clear_instances_directory()
	print()
	
	print("=" * 60)
	print("RESET COMPLETE")
	print("=" * 60)

if __name__ == "__main__":
	main()
