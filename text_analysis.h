/* * * * * * *
 * Text Analysis module for Assignment 2.
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

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

// #define TEXT_ANALYSIS_DBG /* debug, commented out whilst not in use */

/* Trie structure */
typedef struct trie_struct {
  char val;
  int num_occurrences;
  struct trie_struct *next_sibling;
  struct trie_struct *oldest_child;
} simple_trie, *simple_trie_ptr;

/* Subtring list structure */
typedef struct substr_list_struct {
  char substr[257];
  int num_occurrences;
} substr_list, *substr_list_ptr;

/*****************************************************************************/
/* Function protoypes */

void get_num_params_from_stdin_txt(int *params_array, int num_of_params);
void get_string_from_stdin_txt(char *output_str);
void process_string_to_trie(char *line_input, simple_trie_ptr head);
void traverse_trie(simple_trie_ptr trie_ptr);
void process_2in_from_line_input(char *line_input, int *hash_N, int *hash_M);
void traverse_trie_for_prefixes(simple_trie_ptr trie_ptr, int curr_level, 
  int depth, char *out_str);
void traverse_trie_to_find_node(simple_trie_ptr head, char *substr, 
  simple_trie_ptr *ans_node);
void traverse_trie_from_prefix_to_strings(simple_trie_ptr ans_node, 
  char *substr, substr_list_ptr sublist, int *index);
int substr_list_comparator(const void *p1, const void *p2);

// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the pre-order traversal of the characters in
// the trie, on a single line.
void problem_2_a();

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b();

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c();

#endif
