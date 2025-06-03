/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_integration.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_complete_simple_map(void)
{
    print_test_header("Integration - Simple complete map");
    
    // Create a simple valid lem-in map
    const char *simple_map = 
        "3\n"
        "##start\n"
        "start 0 0\n"
        "middle 5 5\n"
        "##end\n"
        "end 10 10\n"
        "start-middle\n"
        "middle-end\n";
    
    print_test_result("Simple map file created", 
        create_test_map_file("test_map_simple.txt", simple_map));
    
    printf("    " TEST_INFO " Map content:\n");
    printf("        3 ants\n");
    printf("        3 rooms: start, middle, end\n");
    printf("        2 links: start-middle, middle-end\n");
    
    // TODO: When you have a complete lem-in implementation
    // int result = system("./lem-in < test_map_simple.txt > test_output.txt");
    // print_test_result("Program executed successfully", result == 0);
    
    // Expected output format:
    // 3
    // ##start
    // start 0 0
    // middle 5 5
    // ##end  
    // end 10 10
    // start-middle
    // middle-end
    //
    // L1-middle
    // L2-middle L1-end
    // L3-middle L2-end
    // L3-end
    
    printf("    " TEST_INFO " Complete integration test will run when lem-in is implemented\n");
    
    ft_arna_free();
}

void test_complete_complex_map(void)
{
    print_test_header("Integration - Complex complete map");
    
    // Create a more complex map with multiple paths
    const char *complex_map = 
        "10\n"
        "##start\n"
        "start 0 0\n"
        "room1 1 1\n"
        "room2 2 2\n"
        "room3 1 3\n"
        "room4 3 1\n"
        "junction 4 4\n"
        "##end\n"
        "end 5 5\n"
        "start-room1\n"
        "start-room2\n"
        "room1-room3\n"
        "room2-room4\n"
        "room3-junction\n"
        "room4-junction\n"
        "junction-end\n";
    
    print_test_result("Complex map file created", 
        create_test_map_file("test_map_complex.txt", complex_map));
    
    printf("    " TEST_INFO " Complex map content:\n");
    printf("        10 ants\n");
    printf("        7 rooms with multiple paths\n");
    printf("        7 links forming a network\n");
    
    // TODO: When you have a complete lem-in implementation
    // Test that the algorithm finds optimal paths and moves ants efficiently
    
    printf("    " TEST_INFO " Complex integration test will run when lem-in is implemented\n");
    
    ft_arna_free();
}

void test_error_handling(void)
{
    print_test_header("Integration - Error handling");
    
    // Test various error conditions
    const char *error_cases[] = {
        "",  // Empty input
        "0\n##start\nstart 0 0\n##end\nend 1 1\nstart-end\n",  // Zero ants
        "5\n##start\nstart 0 0\n",  // No end room
        "5\n##end\nend 0 0\n",  // No start room
        "5\n##start\nstart 0 0\n##end\nend 1 1\n",  // No links
        "abc\n##start\nstart 0 0\n##end\nend 1 1\nstart-end\n"  // Invalid ant count
    };
    
    const char *error_descriptions[] = {
        "Empty input",
        "Zero ants",
        "No end room",
        "No start room", 
        "No links between rooms",
        "Invalid ant count"
    };
    
    for (int i = 0; i < 6; i++) {
        char filename[50];
        sprintf(filename, "test_error_%d.txt", i);
        
        print_test_result("Error case file created", 
            create_test_map_file(filename, error_cases[i]));
        
        printf("        Testing: %s\n", error_descriptions[i]);
        
        // TODO: When you have error handling implemented
        // int result = system("./lem-in < test_error.txt 2>/dev/null");
        // print_test_result("Error handled correctly", result != 0);
        
        unlink(filename);
    }
    
    printf("    " TEST_INFO " Error handling tests will run when lem-in is implemented\n");
    
    ft_arna_free();
}

void run_integration_tests(void)
{
    printf("\n🔗 INTEGRATION TESTS\n");
    printf("==================================\n");
    printf("Testing complete lem-in functionality...\n\n");
    
    test_complete_simple_map();
    test_complete_complex_map();
    test_error_handling();
    
    printf("\n✅ Integration Tests Complete\n\n");
}
