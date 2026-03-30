#!/usr/bin/env python3
from pathlib import Path
from glob import glob

def calculate_avg_time():
	"""
	Calculate the average time taken by the program.
	Time (in seconds) is given by the 3rd line of every file in ./instance_results/
	"""
	instance_files = list(glob('instance_results/*.txt'))
	
	if not instance_files:
		print("No instance result files found in ./instance_results/")
		return
	
	total_time = 0.0
	valid_files = 0
	
	for file_path in instance_files:
		try:
			with open(file_path, 'r') as f:
				lines = [line.strip() for line in f if line.strip()]
			
			if len(lines) >= 3:
				time_seconds = float(lines[2])
				total_time += time_seconds
				valid_files += 1
		except (ValueError, IndexError) as e:
			print(f"Warning: Could not parse time from {file_path}: {e}")
			continue
	
	if valid_files == 0:
		print("No valid time data found.")
		return
	
	avg_time = total_time / valid_files
	
	print("=" * 60)
	print("AVERAGE TIME CALCULATION")
	print("=" * 60)
	print(f"Total files processed: {valid_files}")
	print(f"Total time: {total_time:.2f} seconds")
	print(f"Average time: {avg_time:.2f} seconds")
	print("=" * 60)
	
	return avg_time

if __name__ == "__main__":
	calculate_avg_time()
