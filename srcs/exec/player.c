#include "cub.h"
#include <stdlib.h>

static void	set_player_dir(t_player_rt *p, t_dir dir)
{
	if (dir == DIR_NORTH)
	{
		p->dir.d.y = -1.0;
		p->plane.d.x = 0.66;
	}
	else if (dir == DIR_SOUTH)
	{
		p->dir.d.y = 1.0;
		p->plane.d.x = -0.66;
	}
	else if (dir == DIR_EAST)
	{
		p->dir.d.x = 1.0;
		p->plane.d.y = 0.66;
	}
	else if (dir == DIR_WEST)
	{
		p->dir.d.x = -1.0;
		p->plane.d.y = -0.66;
	}
}

void	init_player_rt(t_data *data, t_player_rt *p)
{
	p->pos.d.x = data->player_col + 0.5;
	p->pos.d.y = data->player_row + 0.5;
	set_player_dir(p, data->player_dir);
}

// t_player_rt	*init_player_rt(t_data *data)
// {
// 	t_player_rt	*p;

// 	p = ft_calloc(1, sizeof(t_player_rt));
// 	if (!p)
// 		return (NULL);
// 	p->pos.d.x = data->player_col + 0.5;
// 	p->pos.d.y = data->player_row + 0.5;
// 	set_player_dir(p, data->player_dir);
// 	return (p);
// }
