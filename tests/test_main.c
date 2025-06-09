/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void print_welcome_message(void)
{
    printf("🧪 LEM-IN COMPREHENSIVE TEST SUITE\n");
    printf("==================================\n");
    printf("Testing all components of your lem-in implementation...\n");
    printf("\nThis test suite includes:\n");
    printf("  📊 Graph data structure tests\n");
    printf("  🔍 Input parsing tests\n");
    printf("  🧮 Pathfinding algorithm tests\n");
    printf("  🔗 Integration tests\n");
    printf("\n");
}

void print_test_usage(const char *program_name)
{
    printf("Usage: %s [test_category]\n\n", program_name);
    printf("Available test categories:\n");
    printf("  graph      - Test graph data structure functions\n");
    printf("  edge       - Test edge/connection management\n");
    printf("  search     - Test search and navigation functions\n");
    printf("  validation - Test graph validation functions\n");
    printf("  parsing    - Test input parsing functions\n");
    printf("  algorithms - Test pathfinding algorithms\n");
    printf("  integration- Test complete functionality\n");
    printf("  input      - Test input file validation\n");
    printf("  all        - Run all tests (default)\n");
    printf("\nExamples:\n");
    printf("  %s graph      # Run only graph tests\n", program_name);
    printf("  %s edge       # Run only edge management tests\n", program_name);
    printf("  %s            # Run all tests\n", program_name);
}

int main(int argc, char **argv)
{
    print_welcome_message();
    
    // Check if help is requested
    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_test_usage(argv[0]);
        return 0;
    }
    
    // Determine which tests to run
    if (argc > 1) {
        if (strcmp(argv[1], "graph") == 0) {
            run_graph_tests();
        } else if (strcmp(argv[1], "edge") == 0) {
            run_edge_management_tests();
        } else if (strcmp(argv[1], "search") == 0) {
            run_search_navigation_tests();
        } else if (strcmp(argv[1], "validation") == 0) {
            run_validation_tests();
        } else if (strcmp(argv[1], "parsing") == 0) {
            run_parsing_tests();
        } else if (strcmp(argv[1], "algorithms") == 0) {
            run_algorithm_tests();
        } else if (strcmp(argv[1], "integration") == 0) {
            run_integration_tests();
        } else if (strcmp(argv[1], "input") == 0) {
            run_input_file_tests();
        } else if (strcmp(argv[1], "all") == 0) {
            run_graph_tests();
            run_edge_management_tests();
            run_search_navigation_tests();
            run_validation_tests();
            run_parsing_tests();
            run_algorithm_tests();
            run_integration_tests();
            run_input_file_tests();
        } else {
            printf("❌ Unknown test category: %s\n\n", argv[1]);
            print_test_usage(argv[0]);
            return 1;
        }
    } else {
        // Run all tests by default
        run_graph_tests();
        run_edge_management_tests();
        run_search_navigation_tests();
        run_validation_tests();
        run_parsing_tests();
        run_algorithm_tests();
        run_integration_tests();
        run_input_file_tests();
    }
    
    // Clean up and show final results
    cleanup_test_files();
    print_final_results();
    
    return 0;
}
