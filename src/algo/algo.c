/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

/*
 * Ce fichier principal algo.c a été réorganisé en plusieurs modules :
 * 
 * - bfs_algorithms.c     : Algorithmes BFS (find_path, findShortestPathBFS, findPathsBFSSimple)
 * - path_optimization.c  : Optimisation des chemins (calculateTurns, selectOptimalPaths, findAllPaths)
 * - ant_movement.c       : Gestion des fourmis (resetNodePositions, moveAnts)
 * - path_validation.c    : Validation DFS pour compatibilité (findAllPathsRecursive)
 * 
 * Cette réorganisation améliore la lisibilité et la maintenabilité du code.
 */