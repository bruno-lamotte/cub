#include "cub.h"
#include <stdlib.h>

int	is_visited(t_bst **root, t_map_state *state)
{
	int	cmp;

	if (!*root)
		return (0);
	cmp = compare_states(state, (*root)->state);
	if (cmp == 0)
		return (1);
	if (cmp < 0)
		return (is_visited(&((*root)->left), state));
	else
		return (is_visited(&((*root)->right), state));
}

void	insert_visited(t_bst **root, t_map_state *state)
{
	int	cmp;

	if (!*root)
	{
		*root = malloc(sizeof(t_bst));
		if (!*root)
			return ;
		(*root)->state = malloc(sizeof(t_map_state));
		if (!(*root)->state)
		{
			free(*root);
			*root = NULL;
			return ;
		}
		*((*root)->state) = *state;
		(*root)->left = NULL;
		(*root)->right = NULL;
		return ;
	}
	cmp = compare_states(state, (*root)->state);
	if (cmp < 0)
		insert_visited(&((*root)->left), state);
	else if (cmp > 0)
		insert_visited(&((*root)->right), state);
}

void	free_bst(t_bst *node)
{
	if (!node)
		return ;
	free_bst(node->left);
	free_bst(node->right);
	free(node->state);
	free(node);
}
