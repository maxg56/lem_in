/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ant_movement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

// Fonction pour réinitialiser les positions des nœuds
void resetNodePositions(Graph *graph)
{
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++)
    {
        Node *node = getNodeByIndex(graph, i);
        if (node)
        {
            node->visited = false;
            node->parentNode = -1;
            node->antCount = 0; // Ajout pour suivre le nombre de fourmis dans chaque nœud
        }
    }
}

// Fonction pour déplacer les fourmis le long des chemins
void moveAnts(Graph *graph, Path **paths, int pathCount)
{
    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);

    if (!startNode || !endNode)
        return;

    // Déplacer les fourmis le long des chemins
    for (int i = 0; i < pathCount; i++)
    {
        Path *path = paths[i];
        for (int j = path->len - 1; j > 0; j--) // Parcourir le chemin à l'envers
        {
            Node *currentNode = getNodeByIndex(graph, path->nodes[j]);
            Node *previousNode = getNodeByIndex(graph, path->nodes[j - 1]);

            if (previousNode->antCount > 0) // Si une fourmi est présente dans le nœud précédent
            {
                currentNode->antCount++; // Ajouter une fourmi au nœud actuel
                previousNode->antCount--; // Retirer une fourmi du nœud précédent

                // Si la fourmi atteint le nœud final, elle est retirée du graphe
                if (currentNode == endNode)
                    currentNode->antCount--;
            }
        }
    }

    // Ajouter une nouvelle fourmi au départ si possible
    if (startNode->antCount > 0)
    {
        for (int i = 0; i < pathCount; i++)
        {
            Path *path = paths[i];
            Node *nextNode = getNodeByIndex(graph, path->nodes[1]);
            if (nextNode->antCount == 0) // Vérifier si le prochain nœud est libre
            {
                nextNode->antCount++;
                startNode->antCount--;
                break; // Une seule fourmi quitte le départ par tour
            }
        }
    }
}
