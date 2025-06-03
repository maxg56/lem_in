/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_parse_rooms(void)
{
    print_test_header("Parsing - Room parsing");
    
    // Create test map with rooms
    const char *test_map = "3\n##start\nstart 0 0\nroom1 5 5\nroom2 10 10\n##end\nend 15 15\n";
    
    print_test_result("Test map created", create_test_map_file("test_rooms.txt", test_map));
    
    // TODO: When you implement parsing functions, add tests here
    // Example:
    // Graph* graph = parse_map_file("test_rooms.txt");
    // print_test_result("Map parsed successfully", graph != NULL);
    // print_test_result("Correct number of rooms", graph && graph->size == 4);
    
    printf("    " TEST_INFO " Room parsing tests will be implemented when parsing functions are ready\n");
    
    unlink("test_rooms.txt");
    ft_arna_free();
}

void test_parse_links(void)
{
    print_test_header("Parsing - Link parsing");
    
    const char *test_map = "3\n##start\nstart 0 0\nmiddle 5 5\n##end\nend 10 10\nstart-middle\nmiddle-end\n";
    
    print_test_result("Test map with links created", create_test_map_file("test_links.txt", test_map));
    
    // TODO: Add link parsing tests when implemented
    printf("    " TEST_INFO " Link parsing tests will be implemented when parsing functions are ready\n");
    
    unlink("test_links.txt");
    ft_arna_free();
}

void test_parse_ants(void)
{
    print_test_header("Parsing - Ant count parsing");
    
    const char *valid_ant_counts[] = {"1", "10", "100", "1000"};
    const char *invalid_ant_counts[] = {"0", "-1", "abc", "", "1.5"};
    
    printf("    " TEST_INFO " Testing valid ant counts:\n");
    for (int i = 0; i < 4; i++) {
        printf("        Valid: %s\n", valid_ant_counts[i]);
    }
    
    printf("    " TEST_INFO " Testing invalid ant counts:\n");
    for (int i = 0; i < 5; i++) {
        printf("        Invalid: %s\n", invalid_ant_counts[i]);
    }
    
    // TODO: Add actual parsing tests when implemented
    printf("    " TEST_INFO " Ant parsing tests will be implemented when parsing functions are ready\n");
    
    ft_arna_free();
}

void test_parse_invalid_input(void)
{
    print_test_header("Parsing - Invalid input handling");
    
    // Test various invalid inputs
    const char *invalid_maps[] = {
        "",  // Empty file
        "0\n",  // Zero ants
        "-5\n",  // Negative ants
        "abc\n",  // Non-numeric ants
        "5\n##start\n##start\n",  // Multiple start rooms
        "5\n##end\n##end\n",  // Multiple end rooms
        "5\nroom1 0 0\n",  // No start or end
        "5\n##start\nstart\n",  // Invalid room format
        "5\n##start\nstart 0 0\n##end\nend 0 0\n"  // Same coordinates
    };
    
    for (int i = 0; i < 9; i++) {
        char filename[50];
        sprintf(filename, "test_invalid_%d.txt", i);
        
        print_test_result("Invalid map file created", 
            create_test_map_file(filename, invalid_maps[i]));
        
        // TODO: Test that parsing correctly rejects these
        printf("        Testing invalid case %d\n", i + 1);
        
        unlink(filename);
    }
    
    printf("    " TEST_INFO " Invalid input tests will be implemented when parsing functions are ready\n");
    
    ft_arna_free();
}

void run_parsing_tests(void)
{
    printf("\n🔍 PARSING TESTS\n");
    printf("==================================\n");
    printf("Testing input parsing for lem-in...\n\n");
    
    test_parse_rooms();
    test_parse_links();
    test_parse_ants();
    test_parse_invalid_input();
    
    printf("\n✅ Parsing Tests Complete\n\n");
}
