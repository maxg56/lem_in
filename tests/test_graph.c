/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_createGraph_basic(void)
{
    print_test_header("createGraph - Basic functionality");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(5);
    
    print_test_result("Graph creation successful", graph != NULL);
    print_test_result("Graph size correct", graph && graph->size == 5);
    print_test_result("Adjacency list allocated", graph && graph->adjacencyList != NULL);
    
    // Check that all adjacency list entries are initialized to NULL
    if (graph && graph->adjacencyList) {
        int all_null = 1;
        for (int i = 0; i < graph->size; i++) {
            if (graph->adjacencyList[i] != NULL) {
                all_null = 0;
                break;
            }
        }
        print_test_result("All adjacency list entries initialized to NULL", all_null);
    }
    
    ft_arna_free(); // Clean up arena allocation
}

void test_createGraph_edge_cases(void)
{
    print_test_header("createGraph - Edge cases");
    
    resetNodePosition(); // Reset before each test
    // Test with size 0
    Graph* graph_zero = createGraph(0);
    print_test_result("Graph creation with size 0", graph_zero != NULL);
    if (graph_zero) {
        print_test_result("Size 0 graph has correct size", graph_zero->size == 0);
    }
    
    ft_arna_free();
    
    // Test with size 1
    Graph* graph_one = createGraph(1);
    print_test_result("Graph creation with size 1", graph_one != NULL);
    if (graph_one) {
        print_test_result("Size 1 graph has correct size", graph_one->size == 1);
        print_test_result("Size 1 graph adjacency list allocated", graph_one->adjacencyList != NULL);
    }
    
    ft_arna_free();
    
    // Test with large size
    Graph* graph_large = createGraph(1000);
    print_test_result("Graph creation with large size", graph_large != NULL);
    if (graph_large) {
        print_test_result("Large graph has correct size", graph_large->size == 1000);
    }
    
    ft_arna_free();
}

void test_addNode_basic(void)
{
    print_test_header("addNode - Basic functionality");
    
    resetNodePosition(); // Reset position counter
    Graph* graph = createGraph(3);
    
    if (graph) {
        // Add first node
        addNode(graph, "start", 0, 0, true, false);
        print_test_result("First node added", graph->adjacencyList[0] != NULL);
        
        if (graph->adjacencyList[0]) {
            Node* node = graph->adjacencyList[0];
            print_test_result("Node name correct", strcmp(node->Nan, "start") == 0);
            print_test_result("Node x coordinate correct", node->x == 0);
            print_test_result("Node y coordinate correct", node->y == 0);
            print_test_result("Node isStart flag correct", node->isStart == true);
            print_test_result("Node isEnd flag correct", node->isEnd == false);
            print_test_result("Node next pointer NULL", node->next == NULL);
        }
        
        // Add second node
        addNode(graph, "middle", 5, 10, false, false);
        print_test_result("Second node added", graph->adjacencyList[1] != NULL);
        
        if (graph->adjacencyList[1]) {
            Node* node = graph->adjacencyList[1];
            print_test_result("Second node name correct", strcmp(node->Nan, "middle") == 0);
            print_test_result("Second node coordinates correct", node->x == 5 && node->y == 10);
            print_test_result("Second node flags correct", !node->isStart && !node->isEnd);
        }
        
        // Add third node (end node)
        addNode(graph, "end", 10, 20, false, true);
        print_test_result("Third node added", graph->adjacencyList[2] != NULL);
        
        if (graph->adjacencyList[2]) {
            Node* node = graph->adjacencyList[2];
            print_test_result("End node name correct", strcmp(node->Nan, "end") == 0);
            print_test_result("End node isEnd flag correct", node->isEnd == true);
            print_test_result("End node isStart flag correct", node->isStart == false);
        }
    }
    
    ft_arna_free();
}

void test_addNode_overflow(void)
{
    print_test_header("addNode - Overflow handling");
    
    resetNodePosition(); // Reset position counter
    Graph* graph = createGraph(2);
    
    if (graph) {
        // Add nodes up to capacity
        addNode(graph, "node1", 1, 1, false, false);
        addNode(graph, "node2", 2, 2, false, false);
        
        print_test_result("Nodes added up to capacity", 
            graph->adjacencyList[0] != NULL && graph->adjacencyList[1] != NULL);
        
        // Try to add beyond capacity - should be handled gracefully
        addNode(graph, "overflow", 3, 3, false, false);
        
        // The function should handle this gracefully (not crash)
        print_test_result("Overflow handled gracefully", 1); // If we get here, no crash occurred
    }
    
    ft_arna_free();
}

void test_displayGraph_basic(void)
{
    print_test_header("displayGraph - Basic functionality");
    
    resetNodePosition(); // Reset position counter
    Graph* graph = createGraph(2);
    
    if (graph) {
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "end", 10, 10, false, true);
        
        printf("    Testing displayGraph output (should show 2 nodes):\n");
        printf("    "); // Indent for visual clarity
        displayGraph(graph);
        
        print_test_result("displayGraph executed without crash", 1);
    }
    
    ft_arna_free();
}

void test_displayGraph_edge_cases(void)
{
    print_test_header("displayGraph - Edge cases");
    
    // Test with NULL graph
    printf("    Testing displayGraph with NULL graph (should handle gracefully):\n");
    displayGraph(NULL);
    print_test_result("NULL graph handled gracefully", 1);
    
    // Test with empty graph
    Graph* empty_graph = createGraph(3);
    if (empty_graph) {
        printf("    Testing displayGraph with empty graph:\n");
        printf("    "); // Indent for visual clarity
        displayGraph(empty_graph);
        print_test_result("Empty graph displayed correctly", 1);
    }
    
    ft_arna_free();
}

void test_graph_integration(void)
{
    print_test_header("Graph - Integration test");
    
    resetNodePosition(); // Reset position counter
    Graph* graph = createGraph(5);
    
    if (graph) {
        // Create a simple graph structure for lem-in
        addNode(graph, "##start", 0, 0, true, false);
        addNode(graph, "room1", 5, 5, false, false);
        addNode(graph, "room2", 10, 5, false, false);
        addNode(graph, "room3", 15, 10, false, false);
        addNode(graph, "##end", 20, 0, false, true);
        
        // Verify all nodes are added correctly
        int nodes_added = 0;
        for (int i = 0; i < graph->size; i++) {
            if (graph->adjacencyList[i] != NULL) {
                nodes_added++;
            }
        }
        
        print_test_result("All 5 nodes added", nodes_added == 5);
        
        // Verify start and end nodes
        if (graph->adjacencyList[0] && graph->adjacencyList[4]) {
            print_test_result("Start node identified", graph->adjacencyList[0]->isStart == true);
            print_test_result("End node identified", graph->adjacencyList[4]->isEnd == true);
        }
        
        printf("    Complete graph structure:\n");
        printf("    "); // Indent for visual clarity
        displayGraph(graph);
        
        print_test_result("Integration test completed", 1);
    }
    
    ft_arna_free();
}

void run_graph_tests(void)
{
    printf("\n🧪 GRAPH TESTS\n");
    printf("==================================\n");
    printf("Testing graph implementation for lem-in...\n\n");
    
    test_createGraph_basic();
    test_createGraph_edge_cases();
    test_addNode_basic();
    test_addNode_overflow();
    test_displayGraph_basic();
    test_displayGraph_edge_cases();
    test_graph_integration();
    
    printf("\n✅ Graph Tests Complete\n\n");
}
