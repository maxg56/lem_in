/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_hasStartEnd(void)
{
    print_test_header("Validation - hasStartEnd");
    
    // Test valid graph with start and end
    resetNodePosition(); // Reset before each test
    Graph* valid_graph = createGraph(3);
    addNode(valid_graph, "start", 0, 0, true, false);
    addNode(valid_graph, "middle", 5, 5, false, false);
    addNode(valid_graph, "end", 10, 10, false, true);
    
    print_test_result("Valid graph with start and end", hasStartEnd(valid_graph));
    
    ft_arna_free();
    
    // Test graph without start
    resetNodePosition(); // Reset before each test
    Graph* no_start = createGraph(2);
    addNode(no_start, "room1", 0, 0, false, false);
    addNode(no_start, "end", 10, 10, false, true);
    
    print_test_result("Graph without start returns false", !hasStartEnd(no_start));
    
    ft_arna_free();
    
    // Test graph without end
    resetNodePosition(); // Reset before each test
    Graph* no_end = createGraph(2);
    addNode(no_end, "start", 0, 0, true, false);
    addNode(no_end, "room1", 10, 10, false, false);
    
    print_test_result("Graph without end returns false", !hasStartEnd(no_end));
    
    ft_arna_free();
    
    // Test multiple starts (should fail)
    resetNodePosition(); // Reset before each test
    Graph* multi_start = createGraph(3);
    addNode(multi_start, "start1", 0, 0, true, false);
    addNode(multi_start, "start2", 5, 5, true, false);
    addNode(multi_start, "end", 10, 10, false, true);
    
    print_test_result("Multiple starts returns false", !hasStartEnd(multi_start));
    
    ft_arna_free();
    
    print_test_result("NULL graph returns false", !hasStartEnd(NULL));
}

void test_isGraphConnected(void)
{
    print_test_header("Validation - isGraphConnected");
    
    // Test connected graph
    resetNodePosition(); // Reset before each test
    Graph* connected = createGraph(4);
    addNode(connected, "start", 0, 0, true, false);
    addNode(connected, "middle1", 5, 5, false, false);
    addNode(connected, "middle2", 10, 5, false, false);
    addNode(connected, "end", 15, 10, false, true);
    
    // Connect all nodes in a line
    addEdge(connected, 0, 1);
    addEdge(connected, 1, 2);
    addEdge(connected, 2, 3);
    
    print_test_result("Connected graph returns true", isGraphConnected(connected));
    
    ft_arna_free();
    
    // Test disconnected graph
    resetNodePosition(); // Reset before each test
    Graph* disconnected = createGraph(4);
    addNode(disconnected, "start", 0, 0, true, false);
    addNode(disconnected, "middle1", 5, 5, false, false);
    addNode(disconnected, "middle2", 10, 5, false, false);
    addNode(disconnected, "end", 15, 10, false, true);
    
    // Only connect first two nodes
    addEdge(disconnected, 0, 1);
    // Nodes 2 and 3 are isolated
    
    print_test_result("Disconnected graph returns false", !isGraphConnected(disconnected));
    
    ft_arna_free();
    
    // Test single node
    resetNodePosition(); // Reset before each test
    Graph* single = createGraph(1);
    addNode(single, "only", 0, 0, true, true);
    
    print_test_result("Single node graph is connected", isGraphConnected(single));
    
    ft_arna_free();
    
    print_test_result("NULL graph returns false", !isGraphConnected(NULL));
}

void test_isConnected(void)
{
    print_test_header("Validation - isConnected (specific nodes)");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(5);
    addNode(graph, "node0", 0, 0, true, false);
    addNode(graph, "node1", 5, 5, false, false);
    addNode(graph, "node2", 10, 5, false, false);
    addNode(graph, "node3", 15, 10, false, false);
    addNode(graph, "node4", 20, 0, false, true);
    
    // Create path: 0-1-2-3-4 and shortcut 1-4
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 1, 4); // Shortcut
    
    print_test_result("Direct path exists (0 to 1)", isConnected(graph, 0, 1));
    print_test_result("Indirect path exists (0 to 4)", isConnected(graph, 0, 4));
    print_test_result("Self connection", isConnected(graph, 0, 0));
    print_test_result("Bidirectional connection", isConnected(graph, 4, 0));
    
    // Remove the shortcut to test longer path
    removeEdge(graph, 1, 4);
    print_test_result("Long path still works (0 to 4)", isConnected(graph, 0, 4));
    
    print_test_result("Invalid parameters handled", !isConnected(NULL, 0, 1));
    print_test_result("Out of bounds handled", !isConnected(graph, 0, 10));
    
    ft_arna_free();
}

void test_validateGraph(void)
{
    print_test_header("Validation - validateGraph (complete)");
    
    // Test completely valid graph
    resetNodePosition(); // Reset before each test
    Graph* valid = createGraph(4);
    addNode(valid, "start", 0, 0, true, false);
    addNode(valid, "middle1", 5, 5, false, false);
    addNode(valid, "middle2", 10, 5, false, false);
    addNode(valid, "end", 15, 10, false, true);
    
    addEdge(valid, 0, 1);
    addEdge(valid, 1, 2);
    addEdge(valid, 2, 3);
    
    print_test_result("Valid connected graph passes", validateGraph(valid));
    
    ft_arna_free();
    
    // Test graph without connections between start and end
    resetNodePosition(); // Reset before each test
    Graph* unconnected = createGraph(4);
    addNode(unconnected, "start", 0, 0, true, false);
    addNode(unconnected, "middle1", 5, 5, false, false);
    addNode(unconnected, "middle2", 10, 5, false, false);
    addNode(unconnected, "end", 15, 10, false, true);
    
    addEdge(unconnected, 0, 1); // start-middle1
    addEdge(unconnected, 2, 3); // middle2-end (no connection between the two parts)
    
    print_test_result("Unconnected start-end fails", !validateGraph(unconnected));
    
    ft_arna_free();
    
    // Test graph with only one node
    resetNodePosition(); // Reset before each test
    Graph* single = createGraph(1);
    addNode(single, "only", 0, 0, true, true);
    
    print_test_result("Single node (start+end) is valid", validateGraph(single));
    
    ft_arna_free();
    
    print_test_result("NULL graph fails validation", !validateGraph(NULL));
}

void run_validation_tests(void)
{
    printf("\n✅ VALIDATION TESTS\n");
    printf("==================================\n");
    printf("Testing graph validation functions...\n\n");
    
    test_hasStartEnd();
    test_isGraphConnected();
    test_isConnected();
    test_validateGraph();
    
    printf("\n✅ Validation Tests Complete\n\n");
}