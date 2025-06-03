/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_algorithms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_find_path_basic(void)
{
    print_test_header("Algorithms - Basic pathfinding");
    
    resetNodePosition(); // Reset position counter
    // Create a simple linear graph: start -> middle -> end
    Graph* graph = createGraph(3);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "middle", 5, 5, false, false);
        addNode(graph, "end", 10, 10, false, true);
        
        printf("    " TEST_INFO " Graph created for pathfinding test:\n");
        printf("    ");
        displayGraph(graph);
        
        // TODO: When you implement pathfinding algorithms
        // Path* path = find_shortest_path(graph);
        // print_test_result("Path found", path != NULL);
        // print_test_result("Path length correct", path && path->length == 3);
        
        printf("    " TEST_INFO " Pathfinding tests will be implemented when algorithms are ready\n");
    }
    
    ft_arna_free();
}

void test_find_multiple_paths(void)
{
    print_test_header("Algorithms - Multiple path finding");
    
    resetNodePosition(); // Reset position counter
    // Create a more complex graph with multiple possible paths
    Graph* graph = createGraph(5);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "path1_mid", 5, 5, false, false);
        addNode(graph, "path2_mid", 5, -5, false, false);
        addNode(graph, "junction", 10, 0, false, false);
        addNode(graph, "end", 15, 0, false, true);
        
        printf("    " TEST_INFO " Complex graph created for multiple path test:\n");
        printf("    ");
        displayGraph(graph);
        
        // TODO: When you implement multiple path algorithms
        // PathList* paths = find_multiple_paths(graph);
        // print_test_result("Multiple paths found", paths != NULL);
        // print_test_result("Optimal number of paths", paths && paths->count >= 2);
        
        printf("    " TEST_INFO " Multiple path tests will be implemented when algorithms are ready\n");
    }
    
    ft_arna_free();
}

void test_no_path_exists(void)
{
    print_test_header("Algorithms - No path scenarios");
    
    // Create disconnected graph
    Graph* graph = createGraph(4);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "isolated1", 5, 5, false, false);
        addNode(graph, "isolated2", 10, 10, false, false);
        addNode(graph, "end", 15, 15, false, true);
        
        printf("    " TEST_INFO " Disconnected graph created (no path should exist):\n");
        printf("    ");
        displayGraph(graph);
        
        // TODO: When you implement pathfinding
        // Path* path = find_shortest_path(graph);
        // print_test_result("No path found correctly", path == NULL);
        
        printf("    " TEST_INFO " No path tests will be implemented when algorithms are ready\n");
    }
    
    ft_arna_free();
}

void test_path_optimization(void)
{
    print_test_header("Algorithms - Path optimization");
    
    // Test scenarios for ant movement optimization
    printf("    " TEST_INFO " Testing optimization scenarios:\n");
    printf("        - Single ant, single path\n");
    printf("        - Multiple ants, single path\n");
    printf("        - Multiple ants, multiple paths\n");
    printf("        - Bottleneck situations\n");
    
    // TODO: When you implement optimization algorithms
    // test different ant counts and path configurations
    
    printf("    " TEST_INFO " Optimization tests will be implemented when algorithms are ready\n");
    
    ft_arna_free();
}

void run_algorithm_tests(void)
{
    printf("\n🧮 ALGORITHM TESTS\n");
    printf("==================================\n");
    printf("Testing pathfinding algorithms for lem-in...\n\n");
    
    test_find_path_basic();
    test_find_multiple_paths();
    test_no_path_exists();
    test_path_optimization();
    
    printf("\n✅ Algorithm Tests Complete\n\n");
}
