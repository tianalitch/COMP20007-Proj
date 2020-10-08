/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Tiana Litchfield
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/*****************************************************************************/
// Definitions and structures

#ifndef HASH_H
#define HASH_H

// #define HASH_DBG /* debug, commented out whilst not in use */

#define ALPHAUPCASEOFFSET -39
#define ALPHALOWCASEOFFSET -97
#define DIGITOFFSET 4
#define HASH_BASE 64

/* Coefficient list structure */
typedef struct coeff_list_struct {    
    int coeff;
    struct coeff_list_struct *next;
} coeff_list, *coeff_list_ptr;

/* Hash table structure */ 
typedef struct hash_table_struct {
    char **hash_table;
    int num_of_strings_N;
    int table_size_M;
    int step_size_K;
} hash_table;

/*****************************************************************************/
/* Function protoypes */

void get_num_params_from_stdin(int *params_array, int num_of_params);
void get_string_from_stdin(char *output_str);
void process_string_to_hash_value(char *input_str, int hash_M, int *hash_value);
void update_hash_table(hash_table *table_ptr);
void process_string_to_hash_table(char *line_input, hash_table *table_ptr);

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a();

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
void problem_1_b();

#endif
