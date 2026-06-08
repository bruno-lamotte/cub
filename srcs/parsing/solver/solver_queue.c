#include "cub.h"
#include <stdlib.h>

void	init_queue(t_queue *q)
{
	q->front = NULL;
	q->rear = NULL;
}

int	enqueue(t_queue *q, t_solver_checkpoint *cp, int depth)
{
	t_qnode	*node;

	node = malloc(sizeof(t_qnode));
	if (!node)
		return (0);
	node->cp = cp;
	node->depth = depth;
	node->next = NULL;
	if (!q->front)
	{
		q->front = node;
		q->rear = node;
	}
	else
	{
		q->rear->next = node;
		q->rear = node;
	}
	return (1);
}

t_solver_checkpoint	*dequeue(t_queue *q, int *depth)
{
	t_qnode				*temp;
	t_solver_checkpoint	*cp;

	if (!q->front)
		return (NULL);
	temp = q->front;
	cp = temp->cp;
	if (depth)
		*depth = temp->depth;
	q->front = q->front->next;
	if (!q->front)
		q->rear = NULL;
	free(temp);
	return (cp);
}

void	clear_queue(t_queue *q)
{
	t_solver_checkpoint	*cp;
	int					d;

	while (q->front)
	{
		cp = dequeue(q, &d);
		free_checkpoint(cp);
	}
}
