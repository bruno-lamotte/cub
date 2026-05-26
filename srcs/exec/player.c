#include "cub.h"
#include <stdlib.h>

static void	set_player_dir(t_player_rt *p, t_dir dir)
{
	if (dir == DIR_NORTH)
	{
		p->dir.y = -1.0;
		p->plane.x = 0.66;
	}
	else if (dir == DIR_SOUTH)
	{
		p->dir.y = 1.0;
		p->plane.x = -0.66;
	}
	else if (dir == DIR_EAST)
	{
		p->dir.x = 1.0;
		p->plane.y = 0.66;
	}
	else if (dir == DIR_WEST)
	{
		p->dir.x = -1.0;
		p->plane.y = -0.66;
	}
}

t_player_rt	*init_player_rt(t_data *data)
{
	t_player_rt	*p;

	p = ft_calloc(1, sizeof(t_player_rt));
	if (!p)
		return (NULL);
	p->pos.x = data->player_col + 0.5;
	p->pos.y = data->player_row + 0.5;
	set_player_dir(p, data->player_dir);
	return (p);
}
