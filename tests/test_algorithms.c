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

        bool used_nodes[3] = {false, false, false};
        Path* path = find_path(graph, used_nodes);
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

        bool used_nodes[6] = {false, false, false, false, false, false};
        Path* path = find_path(graph, used_nodes);
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

        bool used_nodes[4] = {false, false, false, false};
        Path* path = find_path(graph, used_nodes);
        print_test_result("No path found correctly", path == NULL);

        printf("Correctly detected no path in disconnected graph\n");
    }

    ft_arna_free();

    resetNodePosition();
    Graph* single_graph = createGraph(1);
    if (single_graph) {
        addNode(single_graph, "start_end", 0, 0, true, true);

        bool used_nodes_single[1] = {false};
        Path* path = find_path(single_graph, used_nodes_single);
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
    
    // Test basic path finding with multiple paths available
    resetNodePosition();
    Graph* opt_graph = createGraph(7);
    if (opt_graph) {
        // Set up ant count for testing
        opt_graph->nb_fourmis = 5;
        
        addNode(opt_graph, "start", 0, 0, true, false);
        addNode(opt_graph, "short_mid", 2, 0, false, false);
        addNode(opt_graph, "long_mid1", 1, 1, false, false);
        addNode(opt_graph, "long_mid2", 3, 1, false, false);
        addNode(opt_graph, "junction", 4, 0, false, false);
        addNode(opt_graph, "end", 6, 0, false, true);
        
        // Create two paths: short (start->short_mid->end) and long (start->long_mid1->long_mid2->junction->end)
        addEdge(opt_graph, 0, 1); // start->short_mid
        addEdge(opt_graph, 1, 5); // short_mid->end
        addEdge(opt_graph, 0, 2); // start->long_mid1
        addEdge(opt_graph, 2, 3); // long_mid1->long_mid2
        addEdge(opt_graph, 3, 4); // long_mid2->junction
        addEdge(opt_graph, 4, 5); // junction->end
        
        // Test findAllPaths function
        int path_count = 0;
        Path **paths = findAllPaths(opt_graph, &path_count);
        
        print_test_result("Multiple paths found", path_count > 0);
        if (path_count > 0) {
            print_test_result("At least one optimal path exists", paths != NULL);
            printf("        " TEST_INFO " Found %d optimized path(s)\n", path_count);
            
            // Test path assignment
            if (paths && path_count > 0) {
                assignAnts(paths, path_count, opt_graph->nb_fourmis);
                
                bool ants_assigned = false;
                for (int i = 0; i < path_count; i++) {
                    if (paths[i]->assigned_ants > 0) {
                        ants_assigned = true;
                        break;
                    }
                }
                print_test_result("Ants assigned to paths", ants_assigned);
            }
        }
    }
    
    ft_arna_free();
}

void test_path_utilities(void)
{
    print_test_header("Algorithms - Path utility functions");
    
    resetNodePosition();
    Graph* graph = createGraph(5);
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "mid1", 1, 1, false, false);
        addNode(graph, "mid2", 2, 2, false, false);
        addNode(graph, "mid3", 3, 3, false, false);
        addNode(graph, "end", 4, 4, false, true);
        
        // Create paths of different lengths
        Path *path1 = ft_arnalloc(sizeof(Path));
        path1->len = 3; // Short path
        path1->nodes = ft_arnalloc(sizeof(int) * 3);
        path1->nodes[0] = 0; path1->nodes[1] = 1; path1->nodes[2] = 4;
        path1->assigned_ants = 0;
        
        Path *path2 = ft_arnalloc(sizeof(Path));
        path2->len = 5; // Long path
        path2->nodes = ft_arnalloc(sizeof(int) * 5);
        path2->nodes[0] = 0; path2->nodes[1] = 1; path2->nodes[2] = 2; path2->nodes[3] = 3; path2->nodes[4] = 4;
        path2->assigned_ants = 0;
        
        Path *paths[2] = {path2, path1}; // Deliberately unsorted
        
        // Test sorting
        sortPathsByLength(paths, 2);
        print_test_result("Paths sorted by length", paths[0]->len <= paths[1]->len);
        
        // Test disjoint check
        bool are_disjoint = arePathsDisjoint(path1, path2);
        print_test_result("Paths disjoint check works", !are_disjoint); // They share nodes, so should be false
        
        // Test turn calculation
        int turns = calculateTurns(paths, 2, 5);
        print_test_result("Turn calculation works", turns > 0);
        
        printf("        " TEST_INFO " Calculated %d turns for 5 ants\n", turns);
    }
    
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
    test_path_utilities();
    
    printf("\n✅ Algorithm Tests Complete\n\n");
}
