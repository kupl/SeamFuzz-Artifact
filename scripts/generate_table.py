#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this python file will generate the table. 
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [REPORT]: the path for storing the report of FuzzBench. must be given as an absolute path. The default path is the working directory/report/
# [EXP_NAME]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maineval
# python3.9 generate_table.py [REPORT] [EXP_PATH] [TIME] [TRIALS] [EXP_NAME] [RESULTS]

import pandas as pd
import sys
from matplotlib import pyplot as plt
from matplotlib_venn import venn3
import os

report_path = sys.argv[1]
exp_path = sys.argv[2]
given_time = sys.argv[3]
given_trials = sys.argv[4]
exp_name = sys.argv[5]
result_path = sys.argv[6] + "/" + exp_name

data = pd.read_csv(report_path + "/experimental/" + exp_name + "/data.csv.gz")
df = pd.DataFrame(data, columns=data.keys())

grouped_by_time = df[df['time'] == int(given_time)]

deduplicated_data = grouped_by_time.drop_duplicates(['trial_id'])

fuzzers = df['fuzzer'].unique()
benchmarks = df['benchmark'].unique()

os.system("./scripts/count_crash_inputs.sh " + exp_path + " " + exp_name)

f = open(result_path + "/coverage.csv", 'w')

f.write("experiment, fuzzer, benchmark, average_coverage\n")

unique_crashes_dict=dict()

mean_coverage_dict = dict()

for fuzzer in fuzzers:
    coverage_selected_fuzzer = deduplicated_data[df['fuzzer'] == fuzzer]
    coverage_grouped = coverage_selected_fuzzer['edges_covered'].groupby(coverage_selected_fuzzer['benchmark'])

    crash_selected_fuzzer = df[df['fuzzer'] == fuzzer]

    unique_crashes_dict[fuzzer] = dict()
    mean_coverage_dict[fuzzer] = dict()

    for b in benchmarks:
        f.write(exp_name + ", " + fuzzer + ", " + b + ", " + str(coverage_grouped.mean()[b]) + "\n")
        
        mean_coverage_dict[fuzzer][b] = int(coverage_grouped.mean()[b])

        unique_crashes = crash_selected_fuzzer[df['benchmark'] == b]['crash_key'].dropna().unique()
        unique_crashes_dict[fuzzer][b] = set(unique_crashes)
        
f.close()


crash_input_data = pd.read_csv(result_path + "/crashes.csv")
crash_input_df = pd.DataFrame(crash_input_data, columns=crash_input_data.keys())

mean_crash_dict = dict()

for fuzzer in fuzzers:
    crash_input_selected_fuzzer = crash_input_df[crash_input_df['fuzzer'] == fuzzer]

    input_grouped = crash_input_selected_fuzzer['crashes'].groupby(crash_input_selected_fuzzer['benchmark'])

    mean_crash_dict[fuzzer] = dict()

    for b in benchmarks:
        mean_crash_dict[fuzzer][b] = int(input_grouped.mean()[b])

f = open(result_path + "/result_table.txt", 'w')


ratio_crash_dict = dict()
ratio_coverage_dict = dict()


for fuzzer in fuzzers:
    if ("mopt" in fuzzer) or ("seam" in fuzzer):
        ratio_crash_dict[fuzzer] = dict()
        ratio_coverage_dict[fuzzer] = dict()

        for b in benchmarks:
            if (mean_coverage_dict['aflpp'][b] != 0):
                tmp_val = (float(mean_coverage_dict[fuzzer][b]) - float(mean_coverage_dict['aflpp'][b])) / float(mean_coverage_dict['aflpp'][b]) * 100
                ratio_coverage_dict[fuzzer][b] = str(round(tmp_val, 1))
            elif (mean_coverage_dict[fuzzer][b] == 0):
                ratio_coverage_dict[fuzzer][b] = "0%"
            else:
                ratio_coverage_dict[fuzzer][b] = "NaN"

                
            if (mean_crash_dict['aflpp'][b] != 0):
                tmp_val = (float(mean_crash_dict[fuzzer][b]) - float(mean_crash_dict['aflpp'][b])) / float(mean_crash_dict['aflpp'][b]) * 100
                ratio_crash_dict[fuzzer][b] = str(round(tmp_val, 1))
            elif (mean_crash_dict[fuzzer][b] == 0):
                ratio_crash_dict[fuzzer][b] = "0%"
            else:
                ratio_crash_dict[fuzzer][b] = "NaN"


# print the result table...
f.write("Experiment  : " + exp_name + "\n")
f.write("Total Time  : " + given_time + " seconds\n")
f.write("Total trials: " + given_trials + " trials\n")
f.write("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")
f.write(f'{"program":^30s}|' + f'{"AFL++":^30s}|' + f'{"AFL++_MOpt":^60s}|' + f'{"SeamFuzz":^60s}|' + "\n")
f.write(f'{"":^30s}|' + f'{"Cover":^15s}' + f'{"Crashes":^15s}|' + f'{"Cover":^15s}' + f'{"Crashes":^15s}' + f'{"R_Cov":^15s}' + f'{"R_Crashes":^15s}|' + f'{"Cover":^15s}' + f'{"Crashes":^15s}' + f'{"R_Cov":^15s}' + f'{"R_Crashes":^15s}|' + "\n")
f.write("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")

for b in benchmarks:
    f.write(f'{b:30s}|')
    
    for fuzzer in fuzzers:
        if ("mopt" in fuzzer) or ("seam" in fuzzer):
            f.write(f'{mean_coverage_dict[fuzzer][b]:>15d}' + f'{mean_crash_dict[fuzzer][b]:>15d}' + f'{ratio_coverage_dict[fuzzer][b]:>15s}' + f'{ratio_crash_dict[fuzzer][b]:>15s}|')
        else:
            f.write(f'{mean_coverage_dict[fuzzer][b]:>15d}' + f'{mean_crash_dict[fuzzer][b]:>15d}|')


    f.write("\n")

f.write("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n")

f.write(f'{"program":^30s}|' + f'{"# Unique Vulnerabilities":^30s}' + "\n")
f.write("-----------------------------------------------------------\n")
tmp_str = v.get_label_by_id('111').get_text()
f.write(f'{"AFL++ & AFL++_MOpt & SeamFuzz":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('110').get_text()
f.write(f'{"Only AFL++ & AFL++_MOpt":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('101').get_text()
f.write(f'{"Only AFL++ & SeamFuzz":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('011').get_text()
f.write(f'{"Only AFL++_MOpt & SeamFuzz":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('100').get_text()
f.write(f'{"Only AFL++":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('010').get_text()
f.write(f'{"Only AFL++_MOpt":^30s}|' + f'{tmp_str:^30s}' + "\n")
tmp_str = v.get_label_by_id('001').get_text()
f.write(f'{"Only SeamFuzz":^30s}|' + f'{tmp_str:^30s}' + "\n")
f.write("-----------------------------------------------------------\n")

f.close()


# Generate Venn-diagram for the unique vulnerabilities found by each fuzzer.
tmp_crash_dict = dict()
crash_elements = []
fuzzer_elements = []
for fuzzer in fuzzers:
    tmp_set = set()
    for b in benchmarks:
        tmp_set |= unique_crashes_dict[fuzzer][b]

    tmp_crash_dict[fuzzer] = tmp_set
    crash_elements.append(tmp_set)
    fuzzer_elements.append(fuzzer)


v = venn3(crash_elements, tuple(fuzzer_elements))
#v=venn3([tmp_crash_dict['aflpp'], tmp_crash_dict['aflppmopt'], tmp_crash_dict['seam']], ('aflpp', 'aflppmopt', 'seam'))

plt.title("Unique Vulnerabilities")
plt.savefig(result_path + "/unique_vul_venn_diagram.png")
