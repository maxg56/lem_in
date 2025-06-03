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
#include <stdio.h>  // Pour snprintf
#include <stdlib.h> // Pour atoi

void test_parse_rooms(void)
{
    print_test_header("Parsing - Room parsing");
    
    // Create test map with rooms
    const char *test_map = "3\n##start\nstart 0 0\nroom1 5 5\nroom2 10 10\n##end\nend 15 15\n";
    
    print_test_result("Test map created", create_test_map_file("test_rooms.txt", test_map));
    
    // Test actual parsing
    Graph* graph = parse_from_file("test_rooms.txt");
    print_test_result("Map parsed successfully", graph != NULL);
    
    if (graph) {
        print_test_result("Correct number of rooms", graph->node_count == 4);
        print_test_result("Correct ant count", graph->nb_fourmis == 3);
        
        // Test start and end nodes
        Node* start = getStartNode(graph);
        Node* end = getEndNode(graph);
        print_test_result("Start node found", start != NULL);
        print_test_result("End node found", end != NULL);
        
        if (start) {
            print_test_result("Start node name correct", ft_strcmp(start->Nan, "start") == 0);
            print_test_result("Start node coordinates", start->x == 0 && start->y == 0);
        }
        
        if (end) {
            print_test_result("End node name correct", ft_strcmp(end->Nan, "end") == 0);
            print_test_result("End node coordinates", end->x == 15 && end->y == 15);
        }
    }
    
    unlink("test_rooms.txt");
    ft_arna_free();
}

void test_parse_links(void)
{
    print_test_header("Parsing - Link parsing");
    
    const char *test_map = "3\n##start\nstart 0 0\nmiddle 5 5\n##end\nend 10 10\nstart-middle\nmiddle-end\n";
    
    print_test_result("Test map with links created", create_test_map_file("test_links.txt", test_map));
    
    // Test actual parsing with connections
    Graph* graph = parse_from_file("test_links.txt");
    print_test_result("Map with links parsed successfully", graph != NULL);
    
    if (graph) {
        print_test_result("Correct number of rooms", graph->node_count == 3);
        print_test_result("Correct ant count", graph->nb_fourmis == 3);
        
        // Test connections
        int startIdx = findNodeByName(graph, "start");
        int middleIdx = findNodeByName(graph, "middle");
        int endIdx = findNodeByName(graph, "end");
        
        print_test_result("Start node found by name", startIdx != -1);
        print_test_result("Middle node found by name", middleIdx != -1);
        print_test_result("End node found by name", endIdx != -1);
        
        if (startIdx != -1 && middleIdx != -1 && endIdx != -1) {
            print_test_result("start-middle connection exists", hasEdge(graph, startIdx, middleIdx));
            print_test_result("middle-end connection exists", hasEdge(graph, middleIdx, endIdx));
            print_test_result("No direct start-end connection", !hasEdge(graph, startIdx, endIdx));
        }
    }
    
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
        
        // Create test map with this ant count
        char test_map[100];
        snprintf(test_map, sizeof(test_map), "%s\n##start\nstart 0 0\n##end\nend 5 5\nstart-end\n", valid_ant_counts[i]);
        
        char filename[50];
        snprintf(filename, sizeof(filename), "test_ants_%d.txt", i);
        
        if (create_test_map_file(filename, test_map)) {
            Graph* graph = parse_from_file(filename);
            if (graph) {
                int expected_ants = atoi(valid_ant_counts[i]);
                print_test_result("Valid ant count parsed correctly", graph->nb_fourmis == expected_ants);
            }
            unlink(filename);
        }
    }
    
    printf("    " TEST_INFO " Testing invalid ant counts:\n");
    for (int i = 0; i < 5; i++) {
        printf("        Invalid: %s\n", invalid_ant_counts[i]);
        
        // Create test map with invalid ant count
        char test_map[100];
        snprintf(test_map, sizeof(test_map), "%s\n##start\nstart 0 0\n##end\nend 5 5\nstart-end\n", invalid_ant_counts[i]);
        
        char filename[50];
        snprintf(filename, sizeof(filename), "test_invalid_ants_%d.txt", i);
        
        if (create_test_map_file(filename, test_map)) {
            Graph* graph = parse_from_file(filename);
            // For invalid counts, either parsing should fail or ant count should be 0
            print_test_result("Invalid ant count handled", !graph || graph->nb_fourmis == 0);
            unlink(filename);
        }
    }
    
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
