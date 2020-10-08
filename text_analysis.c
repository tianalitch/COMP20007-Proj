/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Tiana Litchfield
 */

#include "text_analysis.h"

/* This function gets the parameters from stdin (e.g. N, M) */
void get_num_params_from_stdin_txt(int *params_array, int num_of_params) {
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
void get_string_from_stdin_txt(char *output_str) {
  int line_cursor = 0;
  char input_char;
  *output_str = '\0';
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

/* This function places string to the trie */
void process_string_to_trie(char *line_input, simple_trie_ptr head) {
  char *char_cursor;
  simple_trie_ptr prev = NULL, curr = NULL, temp = NULL, parent = NULL;
  bool already_inserted;

  char_cursor = line_input;
  // always start with the root so point current pointer to its oldest child
  prev = NULL;
  head->num_occurrences++;
  curr = head->oldest_child;
  parent = head;

  while(*char_cursor != '\0') {
    // insert character to trie
#ifdef TEXT_ANALYSIS_DBG
    printf("Processing %c of %s...\n", *char_cursor, line_input);
#endif
    already_inserted = false;
    while (curr != NULL) {
      if (curr->val == *char_cursor) {
        curr->num_occurrences++;
        already_inserted = true;
#ifdef TEXT_ANALYSIS_DBG
        printf("Already exists ... incrementing...\n");
#endif
        break;
      }
      if (*char_cursor < curr->val) {
        // inserts the current character in between the siblings
        temp = (simple_trie_ptr) malloc(sizeof(simple_trie));
        temp->val = *char_cursor;
        temp->num_occurrences = 1;
        temp->oldest_child = NULL;

        if (prev == NULL) {
          parent->oldest_child = temp;
#ifdef TEXT_ANALYSIS_DBG
          printf("New oldest child\n");
#endif
        } else {
          prev->next_sibling = temp;
#ifdef TEXT_ANALYSIS_DBG
          printf("In between\n");
#endif
        }
        temp->next_sibling = curr;
        curr = temp;

        already_inserted = true;
#ifdef TEXT_ANALYSIS_DBG
        printf("Inserting %c ...\n", *char_cursor);
#endif
        break;
      }

      prev = curr;
      curr = curr->next_sibling;
    }
    if (!already_inserted) {
      // inserts the current character in the end of the siblings
      curr = (simple_trie_ptr) malloc(sizeof(simple_trie));
      curr->val = *char_cursor;
      curr->num_occurrences = 1;
      curr->oldest_child = NULL;
      curr->next_sibling = NULL;
      
      if (prev != NULL) {
        prev->next_sibling = curr;
      } else {
        parent->oldest_child = curr;
      }
    }

    parent = curr;
    curr = curr->oldest_child;
    prev = NULL;
    char_cursor++;
  }
  // adds '$' as the string has ended
  if (curr == NULL || curr->val != '$') {
    // inserts $ as the oldest child among siblings
    temp = (simple_trie_ptr) malloc(sizeof(simple_trie));
    temp->val = '$';
    temp->num_occurrences = 1;
    temp->oldest_child = NULL;

    temp->next_sibling = curr;
    parent->oldest_child = temp;
    curr = temp;
#ifdef TEXT_ANALYSIS_DBG
    printf("ending new string %s\n", line_input);
#endif
  } else {
    curr->num_occurrences++;
#ifdef TEXT_ANALYSIS_DBG
    printf("incrementing %s\n", line_input);
#endif        
  }
}

/* This function traverses and prints trie to show answer to Problem 2(a) */
void traverse_trie(simple_trie_ptr trie_ptr) {
  simple_trie_ptr child;

  if (trie_ptr != NULL) {
    // prints the current val
    printf("%c\n",trie_ptr->val);
    // prints the children
    child = trie_ptr->oldest_child;
    while (child != NULL) {
      traverse_trie(child);
      child = child->next_sibling;
    }  
  }
}

/* This function completes Problem 2(a) as per comments, using previous helper
   functions */
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
void problem_2_a() {
  char line_input[1024];
  int string_count = 0, trie_N = 0;
  simple_trie my_trie;

  my_trie.num_occurrences = 0;
  my_trie.val = '^';
  my_trie.oldest_child = NULL;
  my_trie.next_sibling = NULL;

  // gets the parameters needed (N) from stdin
  get_num_params_from_stdin_txt(&trie_N, 1);
  // gest N lines for strings to be hashed 
  for (string_count = 0; string_count < trie_N; string_count++) {
    get_string_from_stdin_txt(line_input);
    process_string_to_trie(line_input, &my_trie);
  }
  traverse_trie(&my_trie);
}

/* This function traverses the trie to get the prefies for Problem 2(b) */
void traverse_trie_for_prefixes(simple_trie_ptr trie_ptr, int curr_level, 
    int depth, char *out_str) {
  simple_trie_ptr child;

  if (trie_ptr != NULL) {
    // prints the current val
    if (curr_level > 0) {
      if (trie_ptr->val != '$') {
#ifdef TEXT_ANALYSIS_DBG
        printf("Adding %c to str %s\n", trie_ptr->val, out_str);
#endif
        out_str[curr_level-1] = trie_ptr->val;
        out_str[curr_level] = '\0';
      } else {
        return;
      }
    }
    // outputs answer for Problem 2(b)
    if (strlen(out_str) == depth) {
      printf("%s %d\n", out_str, trie_ptr->num_occurrences);
      return;
    }
    // prints the children
    child = trie_ptr->oldest_child;
    while (child != NULL) {
      traverse_trie_for_prefixes(child, curr_level+1, depth, out_str);
      child = child->next_sibling;
    }  
  }
}

/* This function completes Problem 2 (b) as per comments, using previous 
  helper functions */
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
void problem_2_b() {
  char line_input[1024], out_str[257];
  int string_count = 0, input_N_depth[2];
  simple_trie my_trie;

  my_trie.num_occurrences = 0;
  my_trie.val = '^';
  my_trie.oldest_child = NULL;
  my_trie.next_sibling = NULL;

  *out_str = '\0';

  // gets the required parameters (N and K)
  get_num_params_from_stdin_txt(input_N_depth, 2);
  // gets N lines for strings to be hashed 
  for (string_count = 0; string_count < input_N_depth[0]; string_count++) {
    get_string_from_stdin_txt(line_input);
    process_string_to_trie(line_input, &my_trie);
  }
  traverse_trie_for_prefixes(&my_trie, 0, input_N_depth[1], out_str);
}

/* This function gets the node ("root") containing the strings that 
  "auto complete" the prefix */
void traverse_trie_to_find_node(simple_trie_ptr head, char *substr, 
  simple_trie_ptr *ans_node) {
  char *char_cursor;
  simple_trie_ptr curr = NULL, parent = NULL;
  
  char_cursor = substr;
  // always start with the root so point current pointer to its oldest child
  parent = head;
  curr = head->oldest_child;
  while (*char_cursor != '\0' && curr != NULL) {
    if (curr->val == *char_cursor) {
      parent = curr;
      curr = curr->oldest_child;
      char_cursor++;
    } else {
      curr = curr->next_sibling;
    }
  }
  *ans_node = parent;
}

/* This function gets all strings in trie with that prefix */
void traverse_trie_from_prefix_to_strings(simple_trie_ptr ans_node, 
  char *substr, substr_list_ptr sublist, int *index) {
  simple_trie_ptr child;
  char temp[2], init_string[257];

  strcpy(init_string, substr);

  if (ans_node != NULL) {
    // prints the current val
    if(ans_node->val == '$') {
#ifdef TEXT_ANALYSIS_DBG
      printf("Adding str %s to list to index %d\n",init_string, *index);
#endif
      strcpy(sublist[*index].substr, init_string);
      sublist[*index].num_occurrences = ans_node->num_occurrences;
      (*index)++;
      return;
    } else {
      temp[0] = ans_node->val;
      temp[1] = '\0';
      strcat(init_string, temp);
    }
    // prints the children
    child = ans_node->oldest_child;
    while (child != NULL) {
      traverse_trie_from_prefix_to_strings(child, init_string, sublist, index);
      child = child->next_sibling;
    }  
  }
}

/* This function is a comparator for Quicksort */
int substr_list_comparator(const void *p1, const void *p2) {
  substr_list_ptr node1, node2;
  int diff;

  node1 = (substr_list_ptr) p1;
  node2 = (substr_list_ptr) p2;

  diff = node2->num_occurrences - node1->num_occurrences;

  if (diff != 0) {
    return diff;
  } else {
    return strcmp(node1->substr, node2->substr);
  }
}

/* This function completes Problem 2(c) as per comments, using previous helper 
  functions */
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
void problem_2_c() {
  char line_input[1024], substr[257];
  int string_count = 0, trie_N = 0, index = 0;
  simple_trie my_trie;
  simple_trie_ptr ans_node;
  substr_list_ptr substr_head;

  my_trie.num_occurrences = 0;
  my_trie.val = '^';
  my_trie.oldest_child = NULL;
  my_trie.next_sibling = NULL;

  *substr = '\0';
 
  // gets the parameters from stdin (N)
  get_num_params_from_stdin_txt(&trie_N, 1);

  get_string_from_stdin_txt(substr);
  fflush(stdout);

  // gets N lines for strings to be hashed 
  for (string_count = 0; string_count < trie_N; string_count++) {
    get_string_from_stdin_txt(line_input);
    process_string_to_trie(line_input, &my_trie);
  }
  traverse_trie_to_find_node(&my_trie, substr, &ans_node);
  fflush(stdout);

  // removes the last character to avoid duplicate
  substr[strlen(substr)-1] = '\0';

  substr_head = (substr_list_ptr) malloc(ans_node->num_occurrences 
    * sizeof(substr_list));
  memset(substr_head, 0, ans_node->num_occurrences * sizeof(substr_list));

  traverse_trie_from_prefix_to_strings(ans_node, substr, substr_head, &index);
#ifdef TEXT_ANALYSIS_DBG
  printf("Printing the list ...\n");
  string_count = 0;
  while (strlen(substr_head[string_count].substr) > 0) {
    printf("Index %d: Occurrences: %d String: %s\n", string_count, 
      substr_head[string_count].num_occurrences, substr_head[string_count].substr);
    string_count++;
  }
#endif
  // sorts based on number of occurrences; if the same, sorts alphabetically
  qsort(substr_head, index, sizeof(substr_list), substr_list_comparator);
  string_count = 0;

  // prints answer to Problem 2(c)
  while ((strlen(substr_head[string_count].substr) > 0) && (string_count < 5)) {
    printf("%0.2f %s\n", (float) substr_head[string_count].num_occurrences / 
      ans_node->num_occurrences, substr_head[string_count].substr);
    string_count++;
  }
}
