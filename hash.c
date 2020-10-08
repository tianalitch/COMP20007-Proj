/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Tiana Litchfield
 */

#include "hash.h"

/* This function gets the parameters from stdin (e.g. N, M) */
void get_num_params_from_stdin(int *params_array, int num_of_params) {
  int line_cursor = 0, param_index = 0;
  char input_char, temp_array[257];
  *temp_array = '\0';
  // checks current number of parameters
  while (param_index < num_of_params) {
    input_char = getc(stdin);
    // checks if input chars in line are all numbers 
    if (isalnum(input_char)) {
      temp_array[line_cursor++] = input_char;
    } else {
      // gets the parameters and converts them to ints
      if (strlen(temp_array) > 0) {
        temp_array[line_cursor] = '\0';
        params_array[param_index++] = atoi(temp_array);
        line_cursor = 0;
        *temp_array = '\0';
      }
      if ((input_char == EOF) || (input_char == '\n')) {
        break;
      }
    }
  }

  // moves the pointer to new line or EOF
  while ((input_char != EOF) && (input_char != '\n')) {
    input_char = getc(stdin);
  }
}

/* This function gets a string from stdin */
void get_string_from_stdin(char *output_str) {
  int line_cursor = 0;
  char input_char;
  *output_str = '\0';
  // loops through getting each char from stdin
  while (!feof(stdin) && !isspace(input_char)) {
    input_char = getc(stdin);
    if (isalnum(input_char)) {
      output_str[line_cursor++] = input_char;
    }
  } 
  output_str[line_cursor] = '\0';

  // moves the pointer to new line or EOF
  while ((input_char != EOF) && (input_char != '\n')) {
    input_char = getc(stdin);
  }
}

/* This function processes hash value from string input */
void process_string_to_hash_value(char *input_str, int hash_M, 
  int *hash_value) {
  char *char_cursor;
  int code_offset;
  coeff_list_ptr head = NULL, curr = NULL, to_delete = NULL;
  
  *hash_value = 0;

  char_cursor = input_str;
  while(*char_cursor != '\0') {
#ifdef HASH_DBG
    printf("Processing character %c ... ", *char_cursor);
#endif
    // decodes the character to their value 
    // by using offset from their ASCII value
    if (isalpha(*char_cursor)) {
      if (isupper(*char_cursor)) {
        code_offset = ALPHAUPCASEOFFSET;
      } else {
        code_offset = ALPHALOWCASEOFFSET;
      }
    } else {
      if (isdigit(*char_cursor)) {
        code_offset = DIGITOFFSET;
      } else {
        printf("Invalid character!\n");
      }
    }
#ifdef HASH_DBG
    printf("value is %d\n", (int)*char_cursor+code_offset);
#endif
    // stores character value (coefficent) to linked list
    // left to right/higher order to lower order
    if (head != NULL) {
      curr->next = (coeff_list_ptr) malloc(sizeof(coeff_list));
      curr = curr->next;
    } else {
      head = (coeff_list_ptr) malloc(sizeof(coeff_list));
      curr = head;
    }
    curr->coeff = (int)*char_cursor+code_offset;
    char_cursor++;
  }
  curr->next = NULL;
  curr = head;

  // computes hash value applying Horner's Rule and applying distributive 
  // property of modulo
  while(curr != NULL) {
    if (curr == head) {
      *hash_value = (curr->coeff) % hash_M;
    } else {
      *hash_value = (((*hash_value * (HASH_BASE % hash_M)) % hash_M) + 
        ((curr->coeff) % hash_M)) % hash_M;
    }
    to_delete = curr;
    curr = curr->next;
    free(to_delete);
  }
  free(curr);
}

/* This function completes Problem 1 (a) as per comments, using previous helper 
  functions */
// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line
void problem_1_a() {
  char line_input[1024];
  int N_M_params[2];
  int string_count = 0, hash_M = 0, hash_N = 0, hash_value = 0;

  // processes N M from input
  get_num_params_from_stdin(N_M_params, 2);
  hash_N = N_M_params[0];
  hash_M = N_M_params[1];

  // iterates through strings 
  for (string_count = 0; string_count < hash_N; string_count++) {
    get_string_from_stdin(line_input);
    process_string_to_hash_value(line_input, hash_M, &hash_value);
#ifdef HASH_DBG
    printf("String = %s ...", line_input);
#endif
    // prints out values/answers for Problem 1(a)
    printf("%d\n", hash_value);
  }
}

/* This function updates the table if M needs to be increased */
void update_hash_table(hash_table *table_ptr) {
  int new_table_size, hash_value, index;
  char **new_hash_table;

  // initializes new hash table and size
  new_table_size = 2 * table_ptr->table_size_M;

  new_hash_table = (char**)malloc(new_table_size * sizeof(char*));
  for (index = 0; index < new_table_size; index++) {
    new_hash_table[index] = (char*) malloc(256 * sizeof(char));
    *new_hash_table[index] = '\0';
  }

  // updates the strings to the new table
  for (index = 0; index < table_ptr->table_size_M; index++) {
    if (strlen(table_ptr->hash_table[index]) > 0) {
      process_string_to_hash_value(table_ptr->hash_table[index], 
        new_table_size, &hash_value);
      while (strlen(new_hash_table[hash_value]) > 0) {
        hash_value = (hash_value + table_ptr->step_size_K) % new_table_size;
      }
      // adds string to the new hash table
      strcpy(new_hash_table[hash_value], table_ptr->hash_table[index]); 
    }
  }
  
  // cleans up old hash table
  for (index = 0; index < table_ptr->table_size_M; index++) {
    free(table_ptr->hash_table[index]);
  }
  free(table_ptr->hash_table);

  // updates table size
  table_ptr->table_size_M = new_table_size;

  // updates the hash table in structure
  table_ptr->hash_table = new_hash_table;
}

/* This function processes each string and place it to table */
void process_string_to_hash_table(char *line_input, hash_table *table_ptr) {
  int hash_value, first_hash_value;
  bool first_hash_val_checked = false;
  
  // processes hash values 
  process_string_to_hash_value(line_input, table_ptr->table_size_M, 
    &hash_value);
  first_hash_value = hash_value;
  while (strlen(table_ptr->hash_table[hash_value]) > 0) {
    if ((hash_value == first_hash_value) && (first_hash_val_checked)) {
      // steps have been exhausted and all have collisions,
      // so increase table size
      update_hash_table(table_ptr);
      process_string_to_hash_value(line_input, table_ptr->table_size_M, 
        &hash_value); 
      first_hash_value = hash_value;
      first_hash_val_checked = false;
    } else {
      if (!first_hash_val_checked) {
        first_hash_val_checked = true;
      }
      // linear probing because of collision
      hash_value = (hash_value + table_ptr->step_size_K) % 
        table_ptr->table_size_M;
    }
  }
  // adds string to hash table
  strcpy(table_ptr->hash_table[hash_value], line_input);
}

/* This function completes Problem 1 (b) as per comments, using previous 
  helper functions */
// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b() {
  hash_table my_table = {};
  char line_input[1024];
  int index = 0, N_M_K_params[3];
  // gets line containing N M K
  get_num_params_from_stdin(N_M_K_params, 3);
  my_table.num_of_strings_N = N_M_K_params[0];
  my_table.table_size_M = N_M_K_params[1];
  my_table.step_size_K = N_M_K_params[2];

  //  creates hash table
  my_table.hash_table = (char**)malloc(my_table.table_size_M * sizeof(char*));
  for (index = 0; index < my_table.table_size_M; index++) {
    my_table.hash_table[index] = (char*) malloc(257 * sizeof(char));
    *(my_table.hash_table[index]) = '\0';
  }

  // gets N lines for strings to be hashed 
  for (index = 0; index < my_table.num_of_strings_N; index++) {
    get_string_from_stdin(line_input);
    process_string_to_hash_table(line_input, &my_table);
  }

  // print out values/answers for Problem 1(b)
  for (index = 0; index < my_table.table_size_M; index++) {
    printf("%d:", index);
    if (strlen(my_table.hash_table[index]) > 0) {
      printf(" %s", my_table.hash_table[index]);
    }
    printf("\n");
  }
  
  // cleans up the hash table before exiting
  for (index = 0; index < my_table.table_size_M; index++) {
    free(my_table.hash_table[index]);
  }
  free(my_table.hash_table);
}