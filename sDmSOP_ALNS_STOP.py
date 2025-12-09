import os
from glob import glob
from csv import writer, DictWriter
from collections import defaultdict
from multiprocessing import Pool
from sys import argv

ITERATION_COUNT = 1
code = 'sDmSOP-CS-F376/sDmSOP_ALNS_STOP.cpp'
executable = './sdmi_ALNS_STOP.out'
compile = f'g++ --std=c++17 {code} -o {executable}'
os.system(compile)

results = 'STOP_supp/ALNS_Solutions'
if not os.path.exists(results):
	os.mkdir(results)

data = {'stop_1veh': [], 'stop_2veh': [], 'stop_3veh': []}
instance_classes = {'stop_1veh': {'instance'}, 'stop_2veh': {'instance'}, 'stop_3veh': {'instance'}}
profit_data = {'stop_1veh': defaultdict(dict), 'stop_2veh': defaultdict(dict), 'stop_3veh': defaultdict(dict)}

def get_size(instance):
	instance_name = instance.split('/')[-1].split('.')[0]
	vehicles = instance.split('/')[-2]
	x = ''
	for c in instance_name:
		if not c.isdigit():
			break
		x += c
	return vehicles, int(x), instance_name

def process_instance(instance):
	instance_name = instance.split('/')[-1].split('.')[0]
	vehicles = instance.split('/')[-2]
	base_instance = instance_name.split('_')[0]
	instance_class = '_'.join(instance_name.split('_')[1:])
	if 'T20' in instance_name or 'T100' in instance_name or vehicles != argv[1]:
		return

	m = 0
	t_max = 0
	avg_time = 0
	avg_effective_iterations = 0
	avg_profit = 0
	best_profit = 0
	total_profit = 0
	comment = ''

	for _ in range(ITERATION_COUNT):
		print(f'Running instance {vehicles}/{instance_name}')

		out_file = f'{results}/{instance_name}_{vehicles}.txt'
		run = f'{executable} {instance} {out_file}'
		if instance_name == '201pr1002_T80_p1':
			os.system(run)

		with open(out_file, 'r') as file:
			lines = [s[:-1] for s in file.readlines()]
			m = int(lines[0])
			t_max = int(lines[1])
			avg_time += float(lines[2])
			avg_effective_iterations += int(lines[3])
			avg_profit += int(lines[4])
			best_profit = max(best_profit, int(lines[4]))
			total_profit = int(lines[5])
			comment = lines[6]

	avg_profit /= ITERATION_COUNT
	avg_time /= ITERATION_COUNT
	avg_effective_iterations /= ITERATION_COUNT
	data[vehicles].append([
		instance_name,
		m,
		t_max,
		avg_time,
		avg_effective_iterations,
		avg_profit,
		best_profit,
		total_profit,
		comment
	])
	profit_data[vehicles][base_instance]['instance'] = base_instance
	profit_data[vehicles][base_instance][instance_class] = best_profit
	instance_classes[vehicles].add(instance_class)
	print("---------------")

instances = list(glob('./STOP_supp/stop_instances/*/*.sop'))
instances.sort(key = get_size)
for instance in instances:
	process_instance(instance)

# for file, records in data.items():
# 	if not records:
# 		continue
# 	with open(f'{results}/{file}.csv', 'w') as out_file:
# 		csv_writer = writer(out_file)
# 		csv_writer.writerow([
# 			'Instance',
# 			'Vehicles',
# 			'Tmax',
# 			'Avg. time',
# 			'Avg. effective iters',
# 			'Avg. profit',
# 			'Best profit',
# 			'Total profit',
# 			'Comment'
# 		])
# 		csv_writer.writerows(records)
#
# for file, records in profit_data.items():
# 	if not records:
# 		continue
# 	with open(f'{results}/{file}_ALNS_results.csv', 'w') as out_file:
# 		csv_writer = DictWriter(out_file, fieldnames = list(instance_classes[file]))
# 		csv_writer.writeheader()
# 		csv_writer.writerows(records.values())
