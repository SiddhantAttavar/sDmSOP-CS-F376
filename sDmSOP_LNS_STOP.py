import os
from glob import glob
from csv import writer

ITERATION_COUNT = 1
code = 'sDmSOP_LNS_STOP_new.cpp'
executable = './sdmi_STOP.out'
compile = f'g++ {code} -o {executable}'
os.system(compile)

results = 'STOP_supp/LNS_Solutions'
if not os.path.exists(results):
	os.mkdir(results)

data = {'stop_1veh': [], 'stop_2veh': [], 'stop_3veh': []}

def get_size(instance):
	instance_name = instance.split('/')[-1].split('.')[0]
	vehicles = instance.split('/')[-2]
	x = ''
	for c in instance_name:
		if not c.isdigit():
			break
		x += c
	return vehicles, int(x), instance_name


instances = list(glob('./STOP_supp/stop_instances/*/*.sop'))
instances.sort(key = get_size)
for instance in instances:
	instance_name = instance.split('/')[-1].split('.')[0]
	vehicles = instance.split('/')[-2]
	if 'T20' in instance_name or 'T100' in instance_name:
		continue

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
		# os.system(run)

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

for file, records in data.items():
	with open(f'{results}/{file}.csv', 'w') as out_file:
		csv_writer = writer(out_file)
		csv_writer.writerow([
			'Instance',
			'Vehicles',
			'Tmax',
			'Avg. time',
			'Avg. effective iters',
			'Avg. profit',
			'Best profit',
			'Total profit',
			'Comment'
		])
		csv_writer.writerows(records)
