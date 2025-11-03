import os
import xlsxwriter
import csv
import math

ITERATION_COUNT = 5
path = os.path.dirname(os.path.abspath(__file__)) + '/VNS-Solutions-Individual/' 
columns = ["Instance", "w", "GTSP Solution", "B"]
budget_dict = {}
uniq_instances = set();
with open('Tmax_values.csv', mode='r') as csv_file:
	csv_reader = csv.DictReader(csv_file)
	line_count = 0
	for row in csv_reader:
		if row[columns[0]] in uniq_instances:
			continue;
		budget_dict[row[columns[0]]] = row
		uniq_instances.add(row[columns[0]])
		line_count += 1

#Create an excel file
workbook = xlsxwriter.Workbook('VNS_profit_individual.xlsx')
worksheet = workbook.add_worksheet()

worksheet.write(0, 0, 'Instance')
worksheet.write(0, 1, 'Travellers')
worksheet.write(0, 2, 'Pg')
worksheet.write(0, 3, 'w')
worksheet.write(0, 4, 'Max cost')
worksheet.write(0, 5, 'VNS time (in seconds)')
worksheet.write(0, 6, 'Solutions Improving Iterations')
worksheet.write(0, 7, 'Optimal Solution')
worksheet.write(0, 8, 'Max profit')
worksheet.write(0, 9, 'Comments')
row = 1
col = 0

compile_code = 'g++ sDmSOP_VNS_individual.cpp -o sdmi.out'
os.system(compile_code)
files_directory = "GTSP-symmetric/"
all_files = os.listdir(files_directory)
txt_files = list(filter(lambda x: x[-4:] == '.txt', all_files))

travellers = [2]
g_values = [1]
w_values = [0.1]

# SPEED UP GENERATION FOR TESTING ONLY
# travellers = [2]
# g_values = [1]
# w_values = [0.5] 
# SPEED UP GENERATION FOR TESTING ONLY ENDS

for i in range(len(txt_files)):
	file_name = txt_files[i]
	print("Processing " + file_name[:-4])
	if file_name[:-4] in uniq_instances:
		for t in travellers:
			for g in g_values:
				for w in w_values:
					budget = math.ceil(w * t * ((int)(budget_dict[file_name[:-4]]["GTSP Solution"])))
					cla = str(t) + ' ' + str(budget) + ' ' + str(g)
					inp_file = files_directory + file_name
					out_file = 'VNS-Solutions-Individual/VNS_' + file_name[:-4] + '_' + str(t) + '_g' + str(g) + '_' + str(w) +'.txt'
					run_code = './sdmi.out ' + inp_file + ' ' + cla  + ' ' + out_file
					best_case = [t, g, budget, 0, 0, 0, 0, ""]
					for iters in range(ITERATION_COUNT):
						print("Processing " + file_name[:-4] + " m = " + str(t) + ", Pg = " + str(g) + ", w = " + str(w) + " Iter No. " + str(iters+1))
						os.system(run_code)

						lines = []
						with open(path + 'VNS_' + file_name[:-4] + '_' + str(t) + '_g' + str(g) + '_' + str(w) +'.txt') as f:
							lines = f.readlines()
						if len(lines) < 8:
							#BAD FILES: STORE IN ANOTHER FILE WITH THE VALUES OF fileName, travellers, Pg, w, 
							continue
						lines = list(map(lambda x: x.strip(), lines))
						best_case[3] += float(lines[3])
						best_case[6] = int(lines[6])
						if int(lines[5]) > int(best_case[5]):
							best_case[4] = int(lines[4])
							best_case[5] = int(lines[5])
							best_case[7] = lines[7]
					best_case[3] /= ITERATION_COUNT
					worksheet.write(row, col, file_name[:-4])
					worksheet.write(row, col + 1, best_case[0])
					worksheet.write(row, col + 2, best_case[1])
					worksheet.write(row, col + 3, w)
					worksheet.write(row, col + 4, best_case[2])
					worksheet.write(row, col + 5, best_case[3])
					worksheet.write(row, col + 6, best_case[4])
					worksheet.write(row, col + 7, best_case[5])
					worksheet.write(row, col + 8, best_case[6])
					worksheet.write(row, col + 9, best_case[7])
					row += 1
		print("Processed " + file_name[:-4])
	else:
		print(file_name[:-4] + " not present in Tmax_values.csv")

workbook.close()
