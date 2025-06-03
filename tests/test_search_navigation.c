/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_search_navigation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

void test_findNodeByName(void)
{
    print_test_header("Search Navigation - findNodeByName");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(3);
    addNode(graph, "start_room", 0, 0, true, false);
    addNode(graph, "middle_room", 5, 5, false, false);
    addNode(graph, "end_room", 10, 10, false, true);
    
    print_test_result("Find existing node (start_room)", findNodeByName(graph, "start_room") == 0);
    print_test_result("Find existing node (middle_room)", findNodeByName(graph, "middle_room") == 1);
    print_test_result("Find existing node (end_room)", findNodeByName(graph, "end_room") == 2);
    print_test_result("Non-existent node returns -1", findNodeByName(graph, "nonexistent") == -1);
    print_test_result("NULL parameters handled", findNodeByName(NULL, "start_room") == -1);
    print_test_result("NULL name handled", findNodeByName(graph, NULL) == -1);
    
    ft_arna_free();
}

void test_getNodeByIndex(void)
{
    print_test_header("Search Navigation - getNodeByIndex");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(3);
    addNode(graph, "node0", 0, 0, true, false);
    addNode(graph, "node1", 5, 5, false, false);
    addNode(graph, "node2", 10, 10, false, true);
    
    Node* node0 = getNodeByIndex(graph, 0);
    Node* node1 = getNodeByIndex(graph, 1);
    Node* node2 = getNodeByIndex(graph, 2);
    
    print_test_result("Get valid node at index 0", node0 != NULL && ft_strcmp(node0->Nan, "node0") == 0);
    print_test_result("Get valid node at index 1", node1 != NULL && ft_strcmp(node1->Nan, "node1") == 0);
    print_test_result("Get valid node at index 2", node2 != NULL && ft_strcmp(node2->Nan, "node2") == 0);
    print_test_result("Invalid index returns NULL", getNodeByIndex(graph, 5) == NULL);
    print_test_result("Negative index returns NULL", getNodeByIndex(graph, -1) == NULL);
    print_test_result("NULL graph returns NULL", getNodeByIndex(NULL, 0) == NULL);
    
    ft_arna_free();
}

void test_getStartEndNodes(void)
{
    print_test_header("Search Navigation - getStartNode/getEndNode");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(4);
    addNode(graph, "room1", 0, 0, false, false);
    addNode(graph, "start_room", 5, 5, true, false);
    addNode(graph, "room3", 10, 5, false, false);
    addNode(graph, "end_room", 15, 10, false, true);
    
    Node* startNode = getStartNode(graph);
    Node* endNode = getEndNode(graph);
    
    print_test_result("Start node found", startNode != NULL);
    print_test_result("Start node has correct name", startNode && ft_strcmp(startNode->Nan, "start_room") == 0);
    print_test_result("Start node isStart flag correct", startNode && startNode->isStart);
    
    print_test_result("End node found", endNode != NULL);
    print_test_result("End node has correct name", endNode && ft_strcmp(endNode->Nan, "end_room") == 0);
    print_test_result("End node isEnd flag correct", endNode && endNode->isEnd);
    
    print_test_result("NULL graph handled for start", getStartNode(NULL) == NULL);
    print_test_result("NULL graph handled for end", getEndNode(NULL) == NULL);
    
    ft_arna_free();
}

void test_getNodeCount(void)
{
    print_test_header("Search Navigation - getNodeCount");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(5);
    
    print_test_result("Empty graph has 0 nodes", getNodeCount(graph) == 0);
    
    addNode(graph, "node1", 0, 0, false, false);
    print_test_result("Graph with 1 node", getNodeCount(graph) == 1);
    
    addNode(graph, "node2", 5, 5, false, false);
    addNode(graph, "node3", 10, 10, false, false);
    print_test_result("Graph with 3 nodes", getNodeCount(graph) == 3);
    
    print_test_result("NULL graph has 0 nodes", getNodeCount(NULL) == 0);
    
    ft_arna_free();
}

void test_neighbors_functions(void)
{
    print_test_header("Search Navigation - Neighbors functions");
    
    resetNodePosition(); // Reset before each test
    Graph* graph = createGraph(4);
    addNode(graph, "center", 5, 5, false, false);
    addNode(graph, "north", 5, 0, false, false);
    addNode(graph, "south", 5, 10, false, false);
    addNode(graph, "east", 10, 5, false, false);
    
    // Connect center to all others
    addEdge(graph, 0, 1); // center-north
    addEdge(graph, 0, 2); // center-south
    addEdge(graph, 0, 3); // center-east
    
    int count = getNeighborCount(graph, 0);
    print_test_result("Center node has 3 neighbors", count == 3);
    print_test_result("North node has 1 neighbor", getNeighborCount(graph, 1) == 1);
    
    print_test_result("Center and North are neighbors", areNeighbors(graph, 0, 1));
    print_test_result("North and South are not neighbors", !areNeighbors(graph, 1, 2));
    
    print_test_result("Invalid neighbor checks handled", getNeighborCount(NULL, 0) == 0);
    print_test_result("areNeighbors with NULL returns false", !areNeighbors(NULL, 0, 1));
    
    ft_arna_free();
}

void run_search_navigation_tests(void)
{
    printf("\n🔍 SEARCH & NAVIGATION TESTS\n");
    printf("==================================\n");
    printf("Testing search and navigation functions...\n\n");
    
    test_findNodeByName();
    test_getNodeByIndex();
    test_getStartEndNodes();
    test_getNodeCount();
    test_neighbors_functions();
    
    printf("\n✅ Search & Navigation Tests Complete\n\n");
}