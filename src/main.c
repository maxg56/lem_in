#include "lem-in.h"

int	main(void)
{

	// Exécution de l'algo BFS
	Path *path = find_path(graph);

	// Affichage du chemin
	if (path)
	{
		ft_putstr_fd("Chemin trouvé : ", 1);
		for (int i = 0; i < path->len; i++)
		{
			Node *node = getNodeByIndex(graph, path->nodes[i]);
			ft_putstr_fd(node->Nan, 1);
			if (i < path->len - 1)
				ft_putstr_fd(" -> ", 1);
		}
		ft_putchar_fd('\n', 1);
	}
	else
		ft_putstr_fd("Aucun chemin trouvé.\n", 2);

	// Nettoyage mémoire
	if (path)
	{
		free(path->nodes);
		free(path);
	}

	return 0;
}
