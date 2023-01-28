#!/bin/bash -ex
# a running script for SeamFuzz artifact accpeted in ICSE'23
# As all experiments were evaluated on FuzzBench framework, 
# this python file will generate the table. 
# TIME: the seconds for the running experiments. The default value is 86400, which is 24 hours
# [REPORT]: the path for storing the report of FuzzBench. must be given as an absolute path. The default path is the working directory/report/
# [EXP_NAME]: the name of the experiments which will be stored in [EXP_PATH]. The default name is maineval
# [RESULTS] : the path to results
# [TYPE] : 0 -> draw table with number of crashes and coverage only
#          1 -> draw table with number of crashes, coverage, and the ratio like Table 2 in our paper
# python3.9 generate_table.py [REPORT] [EXP_PATH] [TIME] [TRIALS] [EXP_NAME] [RESULTS] [TYPE]

import pandas as pd
import sys
from matplotlib import pyplot as plt
from matplotlib_venn import venn3
import os

def draw_table_total_result_with_ratio(col, result_path, total_cov_dict, total_crash_dict, total_ratio_cov_dict, total_ratio_crash_dict):
    f = open(result_path + "/result_table.txt", 'a+')
    f.write(f'{"Total":30s}|')
    line_str="------------------------------" * col
    table_line = line_str + "-" * len(fuzzers) + "\n"

    i = 0
    for fuzzer in fuzzers:
        if i == 0:
            f.write(f'{total_cov_dict[fuzzer]:>15d}' + f'{total_crash_dict[fuzzer]:>15d}|')
            i += 1
        else:
            f.write(f'{total_cov_dict[fuzzer]:>15d}' + f'{total_crash_dict[fuzzer]:>15d}' + f'{total_ratio_cov_dict[fuzzer]:>15s}' + f'{total_ratio_crash_dict[fuzzer]:>15s}|')            

    f.write("\n")        
    f.write(table_line + "\n\n")

    return
    
def draw_table_total_result_with_no_ratio(col, result_path, total_cov_dict, total_crash_dict):
    f = open(result_path + "/result_table.txt", 'a+')
    f.write(f'{"Total":30s}|')
    line_str="------------------------------" * col
    table_line = line_str + "-" * len(fuzzers) + "\n"

    for fuzzer in fuzzers:
        f.write(f'{total_cov_dict[fuzzer]:>15d}' + f'{total_crash_dict[fuzzer]:>15d}|')

    f.write("\n")        
    f.write(table_line + "\n\n")

    return


def draw_table_venn(result_path, fuzzers, benchmarks, unique_crashes_dict):
    f = open(result_path + "/result_table.txt", 'a+')

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

    return
    
def draw_table_with_no_ratio(col, result_path, exp_name, given_time, given_trials, benchmarks, fuzzers, mean_coverage_dict, mean_crash_dict):
    f = open(result_path + "/result_table.txt", 'w')

    # print the result table...
    f.write("Experiment  : " + exp_name + "\n")
    f.write("Total Time  : " + given_time + " seconds\n")
    f.write("Total trials: " + given_trials + " trials\n")
    line_str="------------------------------" * col
    table_line = line_str  + "-" * len(fuzzers) + "\n"
    f.write(table_line)
    #f.write(f'{"program":^30s}|' + f'{"AFL++":^30s}|' + f'{"AFL++_MOpt":^60s}|' + f'{"SeamFuzz":^60s}|' + "\n")

    i = 0
    f.write(f'{"program":^30s}|')

    for fuzz in fuzzers:
        if i == 0:
            f.write(f'{fuzz:^30s}|')
            i+=1
        else:
            f.write(f'{fuzz:^30s}|')

    f.write("\n")

    f.write(f'{"":^30s}|')

    for fuzzer in fuzzers:
        f.write(f'{"Cover":^15s}' + f'{"Crashes":^15s}|')

    f.write("\n")

    f.write(table_line)

    for b in benchmarks:
        f.write(f'{b:30s}|')
        
        for fuzzer in fuzzers:
            f.write(f'{mean_coverage_dict[fuzzer][b]:>15d}' + f'{mean_crash_dict[fuzzer][b]:>15d}|')


        f.write("\n")

    f.write(table_line)

    f.close()


def draw_table_with_ratio(col, result_path, exp_name, given_time, given_trials, benchmarks, fuzzers, mean_coverage_dict, mean_crash_dict, ratio_coverage_dict, ratio_crash_dict):
    f = open(result_path + "/result_table.txt", 'w')

    # print the result table...
    f.write("Experiment  : " + exp_name + "\n")
    f.write("Total Time  : " + given_time + " seconds\n")
    f.write("Total trials: " + given_trials + " trials\n")
    line_str="------------------------------" * col
    table_line = line_str  + "-" * len(fuzzers) + "\n"
    f.write(table_line)
    #f.write(f'{"program":^30s}|' + f'{"AFL++":^30s}|' + f'{"AFL++_MOpt":^60s}|' + f'{"SeamFuzz":^60s}|' + "\n")

    i = 0
    f.write(f'{"program":^30s}|')

    for fuzz in fuzzers:
        if i == 0:
            f.write(f'{fuzz:^30s}|')
            i+=1
        else:
            f.write(f'{fuzz:^60s}|')

    f.write("\n")

    
    i = 0
    f.write(f'{"":^30s}|')
    for fuzzer in fuzzers:
        if i == 0:
            f.write(f'{"Cover":^15s}' + f'{"Crashes":^15s}|')
            i += 1
        else:
            f.write(f'{"Cover":^15s}' + f'{"Crashes":^15s}' + f'{"R_Cov":^15s}' + f'{"R_Crashes":^15s}|')

    f.write("\n")

    f.write(table_line)

    for b in benchmarks:
        f.write(f'{b:30s}|')
        
        for fuzzer in fuzzers:
            if ("mopt" in fuzzer) or ("seam" in fuzzer):
                f.write(f'{mean_coverage_dict[fuzzer][b]:>15d}' + f'{mean_crash_dict[fuzzer][b]:>15d}' + f'{ratio_coverage_dict[fuzzer][b]:>15s}' + f'{ratio_crash_dict[fuzzer][b]:>15s}|')
            else:
                f.write(f'{mean_coverage_dict[fuzzer][b]:>15d}' + f'{mean_crash_dict[fuzzer][b]:>15d}|')


        f.write("\n")

    f.write(table_line)

    f.close()


report_path = sys.argv[1]
exp_path = sys.argv[2]
given_time = sys.argv[3]
given_trials = sys.argv[4]
exp_name = sys.argv[5]
result_path = sys.argv[6] + "/" + exp_name
use_ratio = sys.argv[7]

data = pd.read_csv(report_path + "/experimental/" + exp_name + "/data.csv.gz")
df = pd.DataFrame(data, columns=data.keys())

grouped_by_time = df[df['time'] == int(given_time)]

deduplicated_data = grouped_by_time.drop_duplicates(['trial_id'])

fuzzers = df['fuzzer'].unique()
benchmarks = df['benchmark'].unique()

f = open(result_path + "/coverage.csv", 'w')

f.write("experiment, fuzzer, benchmark, average_coverage\n")

unique_crashes_dict=dict()

mean_coverage_dict = dict()

total_cov_dict = dict()
total_crash_dict = dict()

for fuzzer in fuzzers:
    coverage_selected_fuzzer = deduplicated_data[df['fuzzer'] == fuzzer]
    coverage_grouped = coverage_selected_fuzzer['edges_covered'].groupby(coverage_selected_fuzzer['benchmark'])

    crash_selected_fuzzer = df[df['fuzzer'] == fuzzer]

    unique_crashes_dict[fuzzer] = dict()
    mean_coverage_dict[fuzzer] = dict()
    total_cov_dict[fuzzer] = 0

    for b in benchmarks:
        f.write(exp_name + ", " + fuzzer + ", " + b + ", " + str(coverage_grouped.mean()[b]) + "\n")
        
        mean_coverage_dict[fuzzer][b] = int(coverage_grouped.mean()[b])
        total_cov_dict[fuzzer] += int(coverage_grouped.mean()[b])

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
    total_crash_dict[fuzzer] = 0

    for b in benchmarks:
        mean_crash_dict[fuzzer][b] = int(input_grouped.mean()[b])
        total_crash_dict[fuzzer] += int(input_grouped.mean()[b])




ratio_crash_dict = dict()
ratio_coverage_dict = dict()

total_ratio_crash_dict = dict()
total_ratio_cov_dict = dict()

i = 0
base_fuzz=""
for fuzzer in fuzzers:
    if i == 0:
        base_fuzz=fuzzer
        i+=1
    else:
    #if ("mopt" in fuzzer) or ("seam" in fuzzer):
        ratio_crash_dict[fuzzer] = dict()
        ratio_coverage_dict[fuzzer] = dict()

        for b in benchmarks:
            if (mean_coverage_dict[base_fuzz][b] != 0):
                tmp_val = (float(mean_coverage_dict[fuzzer][b]) - float(mean_coverage_dict[base_fuzz][b])) / float(mean_coverage_dict[base_fuzz][b]) * 100
                ratio_coverage_dict[fuzzer][b] = str(round(tmp_val, 1))
            elif (mean_coverage_dict[fuzzer][b] == 0):
                ratio_coverage_dict[fuzzer][b] = "0%"
            else:
                ratio_coverage_dict[fuzzer][b] = "NaN"

                
            if (mean_crash_dict[base_fuzz][b] != 0):
                tmp_val = (float(mean_crash_dict[fuzzer][b]) - float(mean_crash_dict[base_fuzz][b])) / float(mean_crash_dict[base_fuzz][b]) * 100
                ratio_crash_dict[fuzzer][b] = str(round(tmp_val, 1))
            elif (mean_crash_dict[fuzzer][b] == 0):
                ratio_crash_dict[fuzzer][b] = "0%"
            else:
                ratio_crash_dict[fuzzer][b] = "NaN"

        tmp_val = (float(total_cov_dict[fuzzer]) - float(total_cov_dict[base_fuzz])) / float(total_cov_dict[base_fuzz]) * 100
        total_ratio_cov_dict[fuzzer] = str(round(tmp_val, 1))
        
        if (total_crash_dict[base_fuzz] != 0):
            tmp_val = (float(total_crash_dict[fuzzer]) - float(total_crash_dict[base_fuzz])) / float(total_crash_dict[base_fuzz]) * 100
            total_ratio_crash_dict[fuzzer] = str(round(tmp_val, 1))
        elif (mean_coverage_dict[fuzzer][b] == 0):
            total_ratio_crash_dict[fuzzer] = "0%"
        else:
            total_ratio_crash_dict[fuzzer] = "NaN"

col = 1 + len(fuzzers)
if use_ratio == "1":
    col = col + len(ratio_crash_dict)
    draw_table_with_ratio(col, result_path, exp_name, given_time, given_trials, benchmarks, fuzzers, mean_coverage_dict, mean_crash_dict, ratio_coverage_dict, ratio_crash_dict)
    draw_table_total_result_with_ratio(col, result_path, total_cov_dict, total_crash_dict, total_ratio_cov_dict, total_ratio_crash_dict)
else:
    draw_table_with_no_ratio(col, result_path, exp_name, given_time, given_trials, benchmarks, fuzzers, mean_coverage_dict, mean_crash_dict)
    draw_table_total_result_with_no_ratio(col, result_path, total_cov_dict, total_crash_dict)


draw_table_venn(result_path, fuzzers, benchmarks, unique_crashes_dict)
    



