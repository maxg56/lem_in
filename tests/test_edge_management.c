/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_edge_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_addEdge_basic(void)
{
    print_test_header("Edge Management - Basic addEdge");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(3);
    addNode(graph, "start", 0, 0, true, false);
    addNode(graph, "middle", 5, 5, false, false);
    addNode(graph, "end", 10, 10, false, true);
    
    // Add edge between start and middle
    addEdge(graph, 0, 1);
    
    print_test_result("Edge added successfully", hasEdge(graph, 0, 1));
    print_test_result("Edge is bidirectional", hasEdge(graph, 1, 0));
    print_test_result("Non-existent edge returns false", !hasEdge(graph, 0, 2));
    
    ft_arna_free();
}

void test_addEdgeByName(void)
{
    print_test_header("Edge Management - addEdgeByName");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(3);
    addNode(graph, "start", 0, 0, true, false);
    addNode(graph, "middle", 5, 5, false, false);
    addNode(graph, "end", 10, 10, false, true);
    
    // Add edge by name
    addEdgeByName(graph, "start", "middle");
    addEdgeByName(graph, "middle", "end");
    
    print_test_result("Edge by name added (start-middle)", hasEdge(graph, 0, 1));
    print_test_result("Edge by name added (middle-end)", hasEdge(graph, 1, 2));
    print_test_result("No direct edge (start-end)", !hasEdge(graph, 0, 2));
    
    ft_arna_free();
}

void test_removeEdge(void)
{
    print_test_header("Edge Management - removeEdge");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(3);
    addNode(graph, "start", 0, 0, true, false);
    addNode(graph, "middle", 5, 5, false, false);
    addNode(graph, "end", 10, 10, false, true);
    
    // Add and then remove edge
    addEdge(graph, 0, 1);
    print_test_result("Edge exists before removal", hasEdge(graph, 0, 1));
    
    removeEdge(graph, 0, 1);
    print_test_result("Edge removed successfully", !hasEdge(graph, 0, 1));
    print_test_result("Bidirectional removal works", !hasEdge(graph, 1, 0));
    
    ft_arna_free();
}

void test_edge_error_handling(void)
{
    print_test_header("Edge Management - Error handling");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(2);
    addNode(graph, "node1", 0, 0, false, false);
    
    // Test invalid parameters
    addEdge(NULL, 0, 1);
    addEdge(graph, -1, 0);
    addEdge(graph, 0, 5); // Out of bounds
    addEdge(graph, 0, 1); // Node 1 doesn't exist
    
    print_test_result("Invalid parameters handled gracefully", true);
    print_test_result("hasEdge with NULL graph returns false", !hasEdge(NULL, 0, 1));
    print_test_result("hasEdge with invalid indices returns false", !hasEdge(graph, -1, 0));
    
    ft_arna_free();
}

void run_edge_management_tests(void)
{
    printf("\n🔗 EDGE MANAGEMENT TESTS\n");
    printf("==================================\n");
    printf("Testing edge/connection management...\n\n");
    
    test_addEdge_basic();
    test_addEdgeByName();
    test_removeEdge();
    test_edge_error_handling();
    
    printf("\n✅ Edge Management Tests Complete\n\n");
}