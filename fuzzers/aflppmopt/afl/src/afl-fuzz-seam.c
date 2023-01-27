#include "afl-fuzz.h"
#include <string.h>
#include <limits.h>
#include "cmplog.h"


/* count the number of set bits in a byte target */
u8 count_bits_in_byte(u8 target) {
  target = (target & 0x55) + (target >> 1 & 0x55);
  target = (target & 0x33) + (target >> 2 & 0x33);
  target = (target & 0x0f) + (target >> 4 & 0x0f);

  return target;
}

void setup_seamfuzz_dirs(afl_state_t *afl) {

  ACTF("Setting up output directories for seamfuzz...");

  afl->seam_prob_dir = alloc_printf("%s/seam_prob", afl->out_dir);

  if (access(afl->seam_prob_dir, 0) == -1)
    if (mkdir(afl->seam_prob_dir,0700)) PFATAL("Unable to create '%s'", afl->seam_prob_dir);

  afl->covered_path_dir = alloc_printf("%s/covered_path", afl->seam_prob_dir);

  if (access(afl->covered_path_dir, 0) == -1)
    if (mkdir(afl->covered_path_dir, 0700)) PFATAL("Unable to create '%s'", afl->covered_path_dir);

  afl->prob_dir = alloc_printf("%s/prob", afl->seam_prob_dir);

  if (access(afl->prob_dir, 0) == -1)
    if (mkdir(afl->prob_dir, 0700)) PFATAL("Unable to create '%s'", afl->prob_dir);

  afl->swarm_dir = alloc_printf("%s/swarm", afl->seam_prob_dir);

  if (access(afl->swarm_dir, 0) == -1)
    if (mkdir(afl->swarm_dir, 0700)) PFATAL("Unable to create '%s'", afl->swarm_dir);

  afl->group_covered_path_dir = alloc_printf("%s/group_covered_path", afl->seam_prob_dir);

  if (access(afl->group_covered_path_dir, 0) == -1)
    if (mkdir(afl->group_covered_path_dir, 0700)) PFATAL("Unable to create '%s'", afl->group_covered_path_dir);

}

void update_tmp_succ_group_prob_info(afl_state_t *afl, u32 id_num) {
  u32 tmp, i, j;
  int universe = 0;
  u32 univ_prob[afl->max_op_num];

  for (i = 0; i < afl->max_op_num; i++) {
    if (afl->success_op_prob[id_num][i] == 0) universe += 1;
    else universe += afl->success_op_prob[id_num][i];
    univ_prob[i] = universe;        
    afl->tmp_succ_op_prob[i] = 0;  
  }

  for (i = 0; i < 1000; i++) {
    tmp = rand_below(afl, universe);

    for (j = 0; j < afl->max_op_num; j++) {
      if (tmp < univ_prob[j]) {
        afl->tmp_succ_op_prob[j]++;
        break;
      }
    }
  }

  universe = 0;
  for (i = 0; i < sp_num; i++) univ_prob[i] = 0;
  
  for (i = 0; i < sp_num; i++) {
    if (afl->success_sp_prob[id_num][i] == 0) universe += 1;
    else universe += afl->success_sp_prob[id_num][i];
    univ_prob[i] = universe;    
    afl->tmp_succ_sp_prob[i] = 0;  
  }

  for (i = 0; i < 1000; i++) {
    tmp = rand_below(afl, universe);

    for (j = 0; j < sp_num; j++) {
      if (tmp < univ_prob[j]) {
        afl->tmp_succ_sp_prob[j]++;
        break;
      }
    }
  }

  return;
}

void update_tmp_fail_group_prob_info(afl_state_t *afl, u32 id_num) {  
  u32 tmp, i, j;
  int universe = 0;
  u32 univ_prob[afl->max_op_num];

  for (i = 0; i < afl->max_op_num; i++) {
    if (afl->fail_op_prob[id_num][i] == 0) universe += 1;
    else universe += afl->fail_op_prob[id_num][i];
    univ_prob[i] = universe;   
    afl->tmp_fail_op_prob[i] = 0;   
  }

  for (i = 0; i < 1000; i++) {
    tmp = rand_below(afl, universe);

    for (j = 0; j < afl->max_op_num; j++) {
      if (tmp < univ_prob[j]) {
        afl->tmp_fail_op_prob[j]++;
        break;
      }
    }
  }

  universe = 0;
  for (i = 0; i < sp_num; i++) univ_prob[i] = 0;
  
  for (i = 0; i < sp_num; i++) {
    if (afl->fail_sp_prob[id_num][i] == 0) universe += 1;
    else universe += afl->fail_sp_prob[id_num][i];
    univ_prob[i] = universe;
    afl->tmp_fail_sp_prob[i] = 0;
  }

  for (i = 0; i < 1000; i++) {
    tmp = rand_below(afl, universe);

    for (int j = 0; j < sp_num; j++) {
      if (tmp < univ_prob[j]) {
        afl->tmp_fail_sp_prob[j]++;
        break;
      }
    }
  }

  return;
}

void update_prob_using_group(afl_state_t *afl, s32 id_num, u8 flag) {
  u32 i;
  if (flag == 0) {
    for (i = 0; i < afl->max_op_num; i++) {
      afl->op_prob[i] = 1;
      
      if (i < sp_num) afl->sp_prob[i] = 1;
    }
  } else {
    update_tmp_succ_group_prob_info(afl, id_num);
    update_tmp_fail_group_prob_info(afl, id_num);

    for (i = 0; i < afl->max_op_num; i++) {
      if (afl->tmp_succ_op_prob[i] <= afl->tmp_fail_op_prob[i]) afl->op_prob[i] = 1;
      else afl->op_prob[i] = afl->tmp_succ_op_prob[i] - afl->tmp_fail_op_prob[i];

      if (i < sp_num) {
        if (afl->tmp_succ_sp_prob[i] <= afl->tmp_fail_sp_prob[i]) afl->sp_prob[i] = 1;
        else afl->sp_prob[i] = afl->tmp_succ_sp_prob[i] - afl->tmp_fail_sp_prob[i];

      }
    }  
  }

  return;
}

/* returns 1 when any rare path is covered.
   returns 2 when all covered paths are common ones. (80%)
   o.w. retunrs 0.
*/
u8 check_hit_table (afl_state_t *afl) {
  int i;
  u8 result = 2;
  u8 tmp, tmp_bit;
  u32 common_check = 0;

  for (i = 0; i < afl->working_index; i++) {
    u32 tmp_index = afl->rare_hit_table[i].index;
    if (tmp_index != MAP_SIZE) {
      tmp = afl->fsrv.trace_bits[tmp_index];
      if (unlikely(tmp)) {
        tmp_bit = tmp & (1 << (7 - afl->rare_hit_table[i].bin_index));
        if (tmp_bit != 0) return 1;
      }
    }

    tmp_index = afl->common_hit_table[i].index;
    
    if (tmp_index != MAP_SIZE) {
      tmp = afl->fsrv.trace_bits[tmp_index];
      if (unlikely(tmp)) {
        tmp_bit = tmp & (1 << (7 - afl->common_hit_table[i].bin_index));
        if (tmp_bit == 1) common_check++;
      }
    }
  }
  
  if ((common_check * 100) / afl->current_hit_cnt > 80) return 2;
  
  return result;                              
}

void sorting_hit_table(afl_state_t *afl) {
  u32 limit, tmp_index;
  int i, j;
  struct hit_table tmp_key;
  struct hit_table *tmp_hit_table = ck_alloc(sizeof(struct hit_table) * afl->working_index);

  tmp_index = 0;

  afl->rareness_score = 0.0;

  for (u32 k = 0; k < (MAP_SIZE << 3); k++) {
    if ((signed) tmp_index == afl->working_index) break;
    if (afl->real_hit_table[k].hit_cnt == 0) continue;
    
    tmp_hit_table[tmp_index] = afl->real_hit_table[k];
    
    // Initiate information table
    afl->rare_hit_table[tmp_index].index = MAP_SIZE;
    afl->rare_hit_table[tmp_index].bin_index = 8;
    afl->common_hit_table[tmp_index].index = MAP_SIZE;
    afl->common_hit_table[tmp_index].bin_index = 8;

    tmp_index++;
  }


  // sorting hit table
  for (i = 1; i < afl->working_index; i++) {
    tmp_key = tmp_hit_table[i];

    for (j = i - 1; j >= 0 && tmp_hit_table[j].hit_cnt < tmp_key.hit_cnt; j--) {
      tmp_hit_table[j + 1] = tmp_hit_table[j];
    }

    tmp_hit_table[j + 1] = tmp_key; 
  }

  // update rare path info
  limit = tmp_hit_table[afl->working_index - (afl->working_index / 10)].hit_cnt;

  i = afl->working_index - 1;
  u32 k = 0;
  while(tmp_hit_table[i].hit_cnt <= limit) {
    afl->rare_hit_table[k].index = tmp_hit_table[i].index;
    afl->rare_hit_table[k].bin_index = tmp_hit_table[i].bin_index;

    afl->rareness_score += (1.0 / (double)tmp_hit_table[i].hit_cnt);

    i--;
    k++;
  }

  limit = tmp_hit_table[(afl->working_index / 10) * 3].hit_cnt;

  i = 0;
  while(tmp_hit_table[i].hit_cnt >= limit) {
    afl->common_hit_table[i].index = tmp_hit_table[i].index;
    afl->common_hit_table[i].bin_index = tmp_hit_table[i].bin_index;
    i++;
  }

  ck_free(tmp_hit_table);

  afl->current_working_index = afl->working_index;

  return;
}

/* It calculates the rareness_score between a given seed input and a group. */
double calculate_rareness(afl_state_t *afl) {  
  double result = 0.0, rare_hit_cnt;
  u32 tmp_index;
  u8 tmp_byte, tmp_bit;
  
  if (afl->rareness_score == 0.0) return 0.0;

  for (int i = 0; i < afl->current_working_index; i++) {
    tmp_index = afl->rare_hit_table[i].index;
    if (tmp_index == MAP_SIZE) break;

    tmp_byte = afl->current_seed_path[tmp_index] & afl->group_covered_paths[tmp_index];

    if (tmp_byte != 0) {
      tmp_bit = tmp_byte & (1 << (7 - afl->rare_hit_table[i].bin_index));

      if (tmp_bit != 0) {
        rare_hit_cnt = (double) afl->real_hit_table[(tmp_index << 3) + afl->rare_hit_table[i].bin_index].hit_cnt;
        result += (1.0 / rare_hit_cnt);
      }
    }
  }

  return (result / afl->rareness_score);
}

/* update hit table information only when the testcase is added in a queue.

return 1 if it covers rare path
   return 2 if it covers common paths
   o.w. return 0
   When flag is 1, we record the hit cnt as it is added in a queue; o.w., only checks the rarilty information.
*/
void update_hit_table(afl_state_t *afl) {
  u8 tmp, tmp_bit, j;

  for (u32 i = 0; i < MAP_SIZE; i++) {
    tmp = afl->fsrv.trace_bits[i];
    if (unlikely(tmp)) {
      for (j = 0; j < 8; j++) {
        tmp_bit = tmp & (1 << (7 - j));
        if(tmp_bit == 0) continue;
        else {
          if (afl->real_hit_table[(i << 3) + j].hit_cnt == 0) afl->working_index++;
          afl->real_hit_table[(i << 3) + j].index = i;
          afl->real_hit_table[(i << 3) + j].bin_index = j;

          if (afl->real_hit_table[(i << 3) + j].hit_cnt < UINT_MAX) afl->real_hit_table[(i << 3) + j].hit_cnt++;

          afl->current_hit_cnt++;
        }
      }
    }
  }

  return;
}

void read_covered_path(afl_state_t* afl, u8* fname) {
  s32 fd = open(fname, O_RDONLY);

  if (fd < 0) PFATAL("Unable to open '%s'", fname);

  memset(afl->src_file_covered_path, 0, MAP_SIZE);
  ck_read(fd, afl->src_file_covered_path, MAP_SIZE, fname);

  close(fd);
}

void read_group_covered_path(afl_state_t* afl, u8* fname) {
  s32 fd = open(fname, O_RDONLY);

  if (fd < 0) PFATAL("Unable to open '%s'", fname);

  memset(afl->group_covered_paths, 0, MAP_SIZE);
  ck_read(fd, afl->group_covered_paths, MAP_SIZE, fname);

  close(fd);
}

void write_covered_path(afl_state_t* afl, u8 *fn) {
  s32 fd;

  u8* fname = alloc_printf("%s", fn);
  fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  if (fd < 0) PFATAL("Unable to open '%s'", fname);

  ck_write(fd, afl->fsrv.trace_bits, MAP_SIZE, fname);

  close(fd);
  ck_free(fname);
}

void init_group_path(afl_state_t* afl) {
  s32 fd;
  
  u8 *current_file_path = alloc_printf("%s/%d", afl->covered_path_dir, afl->current_working_int_id);
  
  read_covered_path(afl, current_file_path);        //src_file_covered_path

  u8 *tmp_fn = alloc_printf("%s/%d", afl->group_covered_path_dir, afl->prob_group_cnt);

  fd = open(tmp_fn, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  if (fd < 0) PFATAL("Unable to open '%s'", tmp_fn);

  ck_write(fd, afl->src_file_covered_path, MAP_SIZE, tmp_fn);

  close(fd);

  for (u32 i = 0; i < MAP_SIZE; i++) {
    if (afl->src_file_covered_path[i] != 0) {
      afl->group_path_len[afl->prob_group_cnt] += (u32)count_bits_in_byte(afl->src_file_covered_path[i]);
    }
  }

  ck_free(tmp_fn);
  ck_free(current_file_path);

  return;
}

/* Save group path information (paths covered by all seeds in the same group)
*/
void write_group_path(afl_state_t* afl, u32 group_id) {
  s32 fd;

  u8 *total_fn = alloc_printf("%s/%d", afl->group_covered_path_dir, group_id);

  fd = open(total_fn, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  if (fd < 0) PFATAL("Unable to open '%s'", total_fn);

  ck_write(fd, afl->group_covered_paths, MAP_SIZE, total_fn);

  close(fd);

  ck_free(total_fn);

  return;
}

/* read the covered path information of the selected seed */
void read_current_seed_path(afl_state_t *afl) {

  s32 current_fd;
  memset(afl->current_seed_path, 0, MAP_SIZE);
  u8 *current_fname_id = alloc_printf("%s/%d", afl->covered_path_dir, afl->current_working_int_id);
  
  current_fd = open(current_fname_id, O_RDONLY);

  if (current_fd < 0) PFATAL("Unable to open '%s' for the current file trace", current_fname_id);

  ck_read(current_fd, afl->current_seed_path, MAP_SIZE, current_fname_id);       // store the coverage info of the current seed input

  close(current_fd);
  ck_free(current_fname_id);

  return;
}

u32 select_starting_point(afl_state_t *afl, u32 limit) {
  u32 universe, tmp, i;
  u32 univ_prob[30];

  if (afl->use_existing_prob == 0) return rand_below(afl, limit);
  else if (afl->use_thompson == 1) {
    universe = 0;
    for (i = 0; i < sp_num; i++) {
      if(afl->thompson_sp[afl->prob_group_id][i] == 0) universe += 1;    // to give the lowest probability for 0 case
      else universe += afl->thompson_sp[afl->prob_group_id][i];
      univ_prob[i] = universe;
    }
    tmp = rand_below(afl, universe);

    for (i = 0; i < sp_num; i++) {
      if (tmp < univ_prob[i]) {        
        return i;
      } 
    } 
  } else {
    universe = 0;
    for (i = 0; i < sp_num; i++) {
      if(afl->sp_prob[i] == 0) universe += 1;    // to give the lowest probability for 0 case
      else universe += afl->sp_prob[i];
      univ_prob[i] = universe;
    }
    tmp = rand_below(afl, universe);

    for (i = 0; i < sp_num; i++) {
      if (tmp < univ_prob[i]) {        
        return i;
      } 
    }
  } 

  PFATAL("Invalid statring point is chosen %d %d", tmp, i);
  return 100;
}

/*
* randomly select the mutation operator if we don't use the existing/learned probability for selecting operator.
*/
u32 select_operator (afl_state_t *afl, u32 limit) {
  u32 universe, tmp, i;
  u32 univ_prob[limit];

  if (afl->use_existing_prob == 0) return rand_below(afl, limit);
  else if (afl->use_thompson == 1) {
    universe = 0;
    for (i = 0; i < limit; i++) {
      if (afl->thompson_op[afl->prob_group_id][i] == 0) universe += 1;
      else universe += afl->thompson_op[afl->prob_group_id][i];
      univ_prob[i] = universe;
    }

    tmp = rand_below(afl, universe);

    for (i = 0; i < limit; i++) {
      if (tmp < univ_prob[i]) {    
        return i;
      }
    }
  } else {
    universe = 0;
    for (i = 0; i < limit; i++) {
      if (afl->op_prob[i] == 0) universe += 1;
      else universe += afl->op_prob[i];
      univ_prob[i] = universe; 
     
    }

    tmp = rand_below(afl, universe);

    for (i = 0; i < limit; i++) {
      if (tmp < univ_prob[i]) {    
        return i;
      }
    }
  }

  PFATAL("Invalid operator is chosen - %d", tmp);
  return 100;
}

void perform_thompson_sampling_mut(afl_state_t *afl, u8 *file_path) {
  pid_t pid;

  pid = fork();
  
  switch(pid) {
    case -1:
      perror("fork fails");
      exit(EXIT_FAILURE);
      break;
    
    case 0:
      execl("/usr/local/bin/python3", "/usr/local/bin/python3", afl->thompson_path, file_path, NULL);
      break;

    default:
      waitpid(-1, NULL, 0);
  }

  
  return;
}

void read_calculated_prob(afl_state_t *afl) {
  u8 *param_file_path = alloc_printf("%s/thompson_info_next", afl->seam_prob_dir); 
  char *opt_index = ck_alloc(10 * sizeof(char));
  u32 i = 0;
  u32 sp_flag = 0;

  FILE *param_file = fopen(param_file_path, "r");

  while (fgets(opt_index, 10, param_file) != NULL) {
    if (sp_flag == 0 && strstr(opt_index, "-") != NULL) {
      sp_flag = 1;
      i = 0;
      continue;
    }

    if (sp_flag == 0) afl->thompson_op[afl->prob_group_id][i] = atoi(opt_index);
    else afl->thompson_sp[afl->prob_group_id][i] = atoi(opt_index);

    i++;
  }

  fclose(param_file);
  ck_free(opt_index);
  ck_free(param_file_path);
  return; 
}

void print_overall_thompson(afl_state_t *afl) {
  u32 file_id = afl->prob_group_id;
  u8 *thompson_group_dir = alloc_printf("%s/group_thompson", afl->prob_dir);
  
  if (access(thompson_group_dir, 0) == -1) 
    if (mkdir(thompson_group_dir,0700)) PFATAL("Unable to create '%s'", thompson_group_dir);
  
  u8 *prob_dir_file = alloc_printf("%s/%d", thompson_group_dir, file_id);
  
  if (access(prob_dir_file, 0) == -1) 
    if (mkdir(prob_dir_file,0700)) PFATAL("Unable to create '%s'", prob_dir_file);
  
  u8 *op_path = alloc_printf("%s/thompson_op", prob_dir_file);
  u8 *sp_path = alloc_printf("%s/thompson_sp", prob_dir_file);

  FILE *op_file = fopen(op_path, "a+");
  FILE *sp_file = fopen(sp_path, "a+");

  fprintf(op_file, "%d\n", file_id);
  fprintf(sp_file, "%d\n", file_id);

  for (u32 i = 0; i < afl->max_op_num; i++) {
    fprintf(op_file, "%d, ", afl->success_op_prob[file_id][i]);

    if (i < sp_num) {
      fprintf(sp_file, "%d, ", afl->success_sp_prob[file_id][i]);
    } 
  } 
  
  fprintf(op_file, "\n");
  fprintf(sp_file, "\n");
  
  for (u32 i = 0; i < afl->max_op_num; i++) {
    fprintf(op_file, "%d, ", afl->fail_op_prob[file_id][i]);

    if (i < sp_num) {
      if (afl->use_thompson == 1) fprintf(sp_file, "%d, ", afl->fail_sp_prob[file_id][i]);
    } 
  } 
  
  fprintf(op_file, "\n\n");
  fprintf(sp_file, "\n\n");

  fclose(op_file);
  fclose(sp_file);

  ck_free(op_path);
  ck_free(sp_path);

  ck_free(prob_dir_file);
  
  return;
}

// for thomposn information...
void print_thompson_info(afl_state_t *afl, u32 file_id, u8 flag) {
  u8 *prob_file_path = alloc_printf("%s/thompson_info", afl->seam_prob_dir);

  FILE *prob_file = fopen(prob_file_path, "w");

  for (u32 i = 0; i < afl->max_op_num; i++) {
    if (flag == 0) fprintf(prob_file, "1 1\n");
    else fprintf(prob_file, "%u %u\n", afl->success_op_prob[file_id][i], afl->fail_op_prob[file_id][i]);
  }
  fprintf(prob_file, "-\n");

  for (u32 i = 0; i < sp_num; i++) {
    if (flag == 0) fprintf(prob_file, "1 1\n");
    else fprintf(prob_file, "%u %u\n", afl->success_sp_prob[file_id][i], afl->fail_sp_prob[file_id][i]);
  }

  fclose(prob_file);

  print_overall_thompson(afl);
  
  perform_thompson_sampling_mut(afl, prob_file_path);
  read_calculated_prob(afl);

  ck_free(prob_file_path);

  return;
}

void print_seed_in_swarm_in_file(afl_state_t *afl) {
  FILE *seed_file;
  u8 *prob_dir_file = alloc_printf("%s/%d", afl->swarm_dir, afl->prob_group_id);

  if (access(prob_dir_file, F_OK) == -1) {    
    seed_file = fopen(prob_dir_file, "a+");
    fprintf(seed_file, "Starting id - %d\n", afl->current_working_int_id);
    fprintf(seed_file, "gamma_val, current_id, syn, sem, rareness_score, group_path_cnt\n");
  } else {
    seed_file = fopen(prob_dir_file, "a+");
    
    fprintf(seed_file, "%1.5f, %d, %1.5f, %1.5f, %1.5f, %d \n", afl->gamma_val, afl->current_working_int_id, afl->syn_sim, afl->sem_sim, afl->current_rareness, afl->group_path_len[afl->prob_group_id]);
  }


  fclose(seed_file);
  ck_free(prob_dir_file);

  afl->syn_sim = 0.0;
  afl->sem_sim = 0.0;
  afl->current_rareness = 0.0;

  return;
}

void print_group_perf(afl_state_t *afl) {
  u8 *prob_dir_file = alloc_printf("%s/group_perf", afl->seam_prob_dir);

  if (access(prob_dir_file, 0) == -1) 
    if (mkdir(prob_dir_file,0700)) PFATAL("Unable to create '%s'", prob_dir_file);
  
  u8 *op_path = alloc_printf("%s/%d", prob_dir_file, afl->prob_group_id);

  FILE *op_file = fopen(op_path, "w");

  fprintf(op_file, "group_cnt, perf, success, fail\n");
  fprintf(op_file, "%u, %u, %u, %u", afl->group_cnt[afl->prob_group_id], afl->group_perf[afl->prob_group_id], afl->group_success[afl->prob_group_id], afl->group_fail[afl->prob_group_id]);

  fprintf(op_file, "\n");

  fclose(op_file);

  ck_free(op_path);

  ck_free(prob_dir_file);
  
  return;
}

void print_updated_probability(afl_state_t *afl, u32 file_id) {

  u8 *prob_dir_file = alloc_printf("%s/%u", afl->prob_dir, file_id);
  
  if (access(prob_dir_file, 0) == -1) 
    if (mkdir(prob_dir_file,0700)) PFATAL("Unable to create '%s'", prob_dir_file);
  
  u8 *op_path = alloc_printf("%s/updated_op_prob", prob_dir_file);
  u8 *sp_path = alloc_printf("%s/updated_sp_prob", prob_dir_file);

  FILE *op_file = fopen(op_path, "a+");
  FILE *sp_file = fopen(sp_path, "a+");
  fprintf(op_file, "%u, ", afl->current_working_int_id);
  fprintf(sp_file, "%u, ", afl->current_working_int_id);

  for (u32 i = 0; i < afl->max_op_num; i++) {
    if (afl->use_thompson == 1) fprintf(op_file, "%d, ", afl->thompson_op[file_id][i]);
    else fprintf(op_file, "%d, ", afl->op_prob[i]);

    if (i < sp_num) {
      if (afl->use_thompson == 1) fprintf(sp_file, "%d, ", afl->thompson_sp[file_id][i]);
      else fprintf(sp_file, "%d, ", afl->sp_prob[i]);
    } 
  } 
  
  fprintf(op_file, "\n");
  fprintf(sp_file, "\n");

  fclose(op_file);
  fclose(sp_file);

  ck_free(op_path);
  ck_free(sp_path);

  ck_free(prob_dir_file);
  
  return;
}


/* syntactic similarity claculation */
double calculate_syntactic_sim(struct cluster_table *checking_group, u8* in_buf, u32 in_buf_len) {
  double syntactic_sim;
  u32 testing_len = in_buf_len > checking_group->len ? in_buf_len : checking_group->len;
  u32 i;
  s32 fd;

  u8* src_file_buf;
  syntactic_sim = 0.0;
  
  /* calculate syntactic similarities. */ 
    /* Map the test case into memory. */

  fd = open(checking_group->file_buf, O_RDONLY);

  if (fd < 0) PFATAL("Unable to open '%s' in seed cluster function", checking_group->file_buf);

  src_file_buf = mmap(0, checking_group->len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

  if (src_file_buf == MAP_FAILED) PFATAL("Unable to mmap '%s' - in seed cluster function", checking_group->file_buf);

  close(fd);

  for (i = 0; i < testing_len; i++) {
    u8 tmp_byte = in_buf[i] & src_file_buf[i];

    if (tmp_byte == 0) continue;

    syntactic_sim += (double)count_bits_in_byte(tmp_byte);
  }

  syntactic_sim = syntactic_sim / (double)(in_buf_len * 8);

  munmap(src_file_buf, checking_group->len);

  return syntactic_sim;
}

/* 
* returns the group which the selected seed input belongs to. (making no new group)
* if no group exists, returns UINT_MAX and makes a new group.
*/
u32 seed_cluster(afl_state_t *afl, u8* in_buf, u32 in_buf_len) {
  struct cluster_table *checking_group = afl->file_table_ori;
  struct cluster_table *update_group = afl->file_table_ori;
  u8 selected_seed_path[MAP_SIZE];
  u8 selected_group_path[MAP_SIZE];
  double semantic_sim, syntactic_sim, final_similarity, rareness_score;
  double max_sim_check = 0.0;
  u32 cand_group = UINT_MAX;
  u8* src_file_buf;
  u32 i;
  s32 fd;
  u8 tmp_byte;
  u32 tmp_index;
  u8 tmp_bit;
  u32 testing_len;


  // if the selected input already belongs to a certain group, skip the calculation process and that group.
  if (afl->group_exists[afl->current_working_int_id] != 0) {
    afl->make_new_group = 0;
    return afl->group_exists[afl->current_working_int_id];
  }

  afl->make_new_group = 1;

  read_current_seed_path(afl);
  
  while (checking_group) {
    syntactic_sim = 0.0;
    
    /* calculate syntactic similarities. */ 
      /* Map the test case into memory. */
    fd = open(checking_group->file_buf, O_RDONLY);

    if (fd < 0) PFATAL("Unable to open '%s' in seed cluster function", checking_group->file_buf);

    src_file_buf = mmap(0, checking_group->len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (src_file_buf == MAP_FAILED) PFATAL("Unable to mmap '%s' - in seed cluster function", checking_group->file_buf);

    close(fd);
    testing_len = in_buf_len > checking_group->len ? checking_group->len : in_buf_len;

    for (i = 0; i < testing_len; i++) {
      u8 tmp_byte = in_buf[i] & src_file_buf[i];

      if (tmp_byte == 0) continue;

      syntactic_sim += (double)count_bits_in_byte(tmp_byte);
    }

    syntactic_sim = in_buf_len > checking_group->len ? syntactic_sim / (double)(in_buf_len * 8) : syntactic_sim / (double)(checking_group->len * 8);

    munmap(src_file_buf, checking_group->len);

    read_group_covered_path(afl, checking_group->group_covered_path);
    semantic_sim = 0.0;
    
    for (u32 i = 0; i < MAP_SIZE; i++) {
      tmp_byte = afl->current_seed_path[i] & afl->group_covered_paths[i];

      if (tmp_byte == 0) continue;

      semantic_sim += (double)count_bits_in_byte(tmp_byte);
    }

    semantic_sim = semantic_sim / (double) afl->group_path_len[checking_group->group_id]; 

    // claculate rareness score
    double result = 0.0, rare_hit_cnt;
    
    if (afl->rareness_score == 0.0) return 0.0;

    for (int i = 0; i < afl->current_working_index; i++) {
      tmp_index = afl->rare_hit_table[i].index;
      if (tmp_index == MAP_SIZE) break;

      tmp_byte = afl->current_seed_path[i] & afl->group_covered_paths[i];

      if (tmp_byte != 0) {
        tmp_bit = tmp_byte & (1 << (7 - afl->rare_hit_table[i].bin_index));

        if (tmp_bit != 0) {
          rare_hit_cnt = (double) afl->real_hit_table[(tmp_index << 3) + afl->rare_hit_table[i].bin_index].hit_cnt;
          result += (1.0 / rare_hit_cnt);
        }
      }
    }
    

    rareness_score = result / afl->rareness_score;
    
    final_similarity = syntactic_sim * afl->alpha_val + semantic_sim * afl->beta_val + rareness_score;
 
    if (max_sim_check == 0.0 || final_similarity > afl->gamma_val) { 
      if (final_similarity > max_sim_check) {
        afl->syn_sim = syntactic_sim;
        afl->sem_sim = semantic_sim;
        afl->current_rareness = rareness_score;

        max_sim_check = final_similarity;
        cand_group = checking_group->group_id;
        update_group = checking_group;

        memcpy(selected_seed_path, afl->current_seed_path, MAP_SIZE);
        memcpy(selected_group_path, afl->group_covered_paths, MAP_SIZE);
      }
    }

    checking_group = checking_group->next_candidate;
  } 

  if (max_sim_check < afl->gamma_val) {
    afl->make_new_group = 1;
    cand_group = UINT_MAX;
  } else {  // update candidate seed input
    afl->make_new_group = 0;
      
    double current_rare_score = 0.0;
    double cand_rare_score = 0.0;
    double rare_hit_cnt = 0.0;
          
    for (int i = 0; i < afl->current_working_index; i++) {
      u32 tmp_index = afl->rare_hit_table[i].index;

      if (tmp_index == MAP_SIZE) break;
      
      u8 group_tmp = selected_seed_path[tmp_index];
      u8 current_tmp = afl->current_seed_path[tmp_index];
      
      u8 tmp_bin_index = afl->rare_hit_table[i].bin_index;

      if (group_tmp != 0) {
        u8 tmp_bit = group_tmp & (1 << (7 - tmp_bin_index));
        if (tmp_bit != 0) {
          rare_hit_cnt = (double) afl->real_hit_table[(tmp_index << 3) + afl->rare_hit_table[i].bin_index].hit_cnt;
          cand_rare_score += (1.0 / rare_hit_cnt);
        }
      }

      if (current_tmp != 0) {
        u8 tmp_bit = current_tmp & (1 << (7 - tmp_bin_index));
        if (tmp_bit != 0) {
          rare_hit_cnt = (double) afl->real_hit_table[(tmp_index << 3) + afl->rare_hit_table[i].bin_index].hit_cnt;
          current_rare_score += (1.0 / rare_hit_cnt);
        }
      }
    }

    if (current_rare_score >= cand_rare_score) {
      update_group->file_id = afl->current_working_int_id;
      update_group->file_buf = afl->queue_cur->fname;    
      update_group->len = in_buf_len;
    }
      
    afl->group_path_len[cand_group] = 0;
    
    for (u32 i = 0; i < MAP_SIZE; i++) {
      afl->group_covered_paths[i] = selected_group_path[i] | selected_seed_path[i];     // for total paths

      if (afl->group_covered_paths[i] != 0) {
        afl->group_path_len[cand_group] += (u32)count_bits_in_byte(afl->group_covered_paths[i]);
      }
    }

    write_group_path(afl, cand_group);

  }

  return cand_group;
}

/*
* Decide the mutating location based on the learned probability and the pre-defined sp_num(starting point number).
*/
u32 calculate_starting_point(afl_state_t *afl, u32 end_point, u32 index) {
  u32 starting_dist, current_place, starting_point;
  u32 tmp_dist = sp_num; 

  starting_dist = end_point / sp_num; 

  if (starting_dist == 0) {
    current_place = rand_below(afl, end_point);
  } else {
    u32 tmp_result = starting_dist * (sp_num - 1);
    if (end_point - tmp_result >= sp_num) starting_dist++;
    
    tmp_dist = select_starting_point(afl, sp_num);
    starting_point = starting_dist * tmp_dist;

    if (tmp_dist == sp_num - 1) 
      starting_dist = end_point - (starting_point * tmp_dist); 

    current_place = starting_point + rand_below(afl, starting_dist);
  }

  afl->sp_trace[index] = tmp_dist;

  if (current_place > end_point - 1) return end_point - 1;
  
  return current_place;
  
}

/* update success information */
void update_success_info(afl_state_t *afl) {
  u32 file_id;

  file_id = afl->prob_group_id;

  if (afl->use_thompson == 1 && get_cur_time() - afl->reward_update_time > 60 * 60 * 1000) {
    afl->reward_update_time = get_cur_time();
    afl->reward += afl->reward_update_value;
  }

  for (int i = 0; i < afl->havoc_trials; i++) {
    if (afl->op_trace[i] != 50) afl->success_op_prob[file_id][afl->op_trace[i]] += afl->reward;
    if (afl->sp_trace[i] != sp_num) afl->success_sp_prob[file_id][afl->sp_trace[i]] += afl->reward;
  }

  afl->group_success[afl->prob_group_id]++;
  return;
}

/* update fail information */
void update_fail_info(afl_state_t *afl) {
  u32 file_id;

  file_id = afl->prob_group_id;

  for (int i = 0; i < afl->havoc_trials; i++) {
    if (afl->op_trace[i] != 50) afl->fail_op_prob[file_id][afl->op_trace[i]] += 2;
    if (afl->sp_trace[i] != sp_num) afl->fail_sp_prob[file_id][afl->sp_trace[i]] += 2;
  }

  afl->group_fail[afl->prob_group_id]++;

  return;
}
