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
    Graph* graph = createGraph(3);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "middle", 5, 5, false, false);
        addNode(graph, "end", 10, 10, false, true);

        addEdge(graph, 0, 1);
        addEdge(graph, 1, 2);

        Path* path = find_path(graph);
        print_test_result("Path found", path != NULL);

        if (path) {
            print_test_result("Path length correct", path->len == 3);
            print_test_result("Path starts with start node", path->nodes[0] == 0);
            print_test_result("Path ends with end node", path->nodes[path->len - 1] == 2);

            printf("Found path: ");
            for (int i = 0; i < path->len; i++) {
                Node* node = getNodeByIndex(graph, path->nodes[i]);
                printf("%s", node->Nan);
                if (i < path->len - 1) printf(" -> ");
            }
            printf("\n");
        }
    }

    ft_arna_free();
}

void test_find_multiple_paths(void)
{
    print_test_header("Algorithms - Multiple path scenarios");

    resetNodePosition();
    Graph* graph = createGraph(6);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "path1_mid", 5, 5, false, false);
        addNode(graph, "path2_mid", 5, -5, false, false);
        addNode(graph, "junction", 10, 0, false, false);
        addNode(graph, "end", 15, 0, false, true);

        addEdge(graph, 0, 1);
        addEdge(graph, 0, 2);
        addEdge(graph, 1, 3);
        addEdge(graph, 2, 3);
        addEdge(graph, 3, 4);

        Path* path = find_path(graph);
        print_test_result("Path found in complex graph", path != NULL);

        if (path) {
            print_test_result("Path starts with start", path->nodes[0] == 0);
            print_test_result("Path ends with end", path->nodes[path->len - 1] == 4);
            print_test_result("Path length reasonable", path->len >= 3 && path->len <= 5);

            printf("Found path: ");
            for (int i = 0; i < path->len; i++) {
                Node* node = getNodeByIndex(graph, path->nodes[i]);
                printf("%s", node->Nan);
                if (i < path->len - 1) printf(" -> ");
            }
            printf("\n");
        }
    }

    ft_arna_free();
}

void test_no_path_exists(void)
{
    print_test_header("Algorithms - No path scenarios");

    resetNodePosition();
    Graph* graph = createGraph(4);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "isolated1", 5, 5, false, false);
        addNode(graph, "isolated2", 10, 10, false, false);
        addNode(graph, "end", 15, 15, false, true);

        addEdge(graph, 1, 2);

        Path* path = find_path(graph);
        print_test_result("No path found correctly", path == NULL);

        printf("Correctly detected no path in disconnected graph\n");
    }

    ft_arna_free();

    resetNodePosition();
    Graph* single_graph = createGraph(1);
    if (single_graph) {
        addNode(single_graph, "start_end", 0, 0, true, true);

        Path* path = find_path(single_graph);
        print_test_result("Single node path found", path != NULL);
        if (path) {
            print_test_result("Single node path length is 1", path->len == 1);
        }
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
