/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lem_in.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_LEM_IN_H
# define TEST_LEM_IN_H

# include "../include/data.h"
# include "../include/lem-in.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <unistd.h>
# include <fcntl.h>

// Test result macros
# define TEST_PASS "\033[0;32m[PASS]\033[0m"
# define TEST_FAIL "\033[0;31m[FAIL]\033[0m"
# define TEST_INFO "\033[0;34m[INFO]\033[0m"
# define TEST_WARN "\033[0;33m[WARN]\033[0m"

// Test categories
void run_graph_tests(void);
void run_parsing_tests(void);
void run_algorithm_tests(void);
void run_integration_tests(void);
void run_edge_management_tests(void);
void run_search_navigation_tests(void);
void run_validation_tests(void);
void run_input_file_tests(void);

// Graph tests
void test_createGraph_basic(void);
void test_createGraph_edge_cases(void);
void test_addNode_basic(void);
void test_addNode_overflow(void);
void test_displayGraph_basic(void);
void test_displayGraph_edge_cases(void);
void test_graph_integration(void);

// Parsing tests (for when you implement parsing)
void test_parse_rooms(void);
void test_parse_links(void);
void test_parse_ants(void);
void test_parse_invalid_input(void);

// Algorithm tests (for when you implement pathfinding)
void test_find_path_basic(void);
void test_find_multiple_paths(void);
void test_no_path_exists(void);
void test_path_optimization(void);

// Integration tests
void test_complete_simple_map(void);
void test_complete_complex_map(void);
void test_error_handling(void);

// Utility functions
void print_test_header(const char *test_name);
void print_test_result(const char *test_case, int passed);
void cleanup_test_files(void);
void print_final_results(void);

// Test file helpers
int create_test_map_file(const char *filename, const char *content);
int compare_output_files(const char *expected, const char *actual);

#endif
