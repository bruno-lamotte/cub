#include "cub.h"
#include <stdlib.h>

void	toggle_door_from_terminal(t_engine *eng, int door_idx)
{
	t_door_rt	*doors;

	doors = get_door_rt(eng->blob);
	doors[door_idx].flags ^= DOOR_BLOCKED;
}

void	toggle_alarm_from_terminal(t_engine *eng, int alarm_idx)
{
	eng->static_lights[alarm_idx].is_triggered ^= 1;
	update_global_alarm_state(eng);
}

void	find_exit(t_data *d, t_vec2 *exit_pos)
{
	int	r;
	int	c;

	exit_pos->i.x = -1;
	exit_pos->i.y = -1;
	r = -1;
	while (++r < d->m_height)
	{
		c = -1;
		while (++c < d->m_max_width && d->m_map[r][c])
		{
			if ((d->have_bonus && d->b_map[r][c] == 'X')
				|| d->m_map[r][c] == 'X')
			{
				exit_pos->i.x = c;
				exit_pos->i.y = r;
				return ;
			}
		}
	}
}

int	try_branch_bfs(t_solver_ctx *ctx, t_vec2 target)
{
	t_solver_checkpoint	*cp;
	t_map_state			state;
	t_solver_checkpoint	*new_cp;

	cp = clone_checkpoint(ctx->eng);
	if (simulate_move_to_target(ctx->eng, ctx->worker, target))
	{
		if (is_player_on_exit(ctx->eng))
			return (free_checkpoint(cp), 1);
		state.player_pos.i.x = (uint16_t)ctx->eng->player->pos.d.x;
		state.player_pos.i.y = (uint16_t)ctx->eng->player->pos.d.y;
		state.door_mask = get_door_mask(ctx->eng);
		state.alarm_mask = get_alarm_mask(ctx->eng);
		state.monster_mask = get_monster_mask(ctx->eng);
		if (!is_visited(ctx->visited, &state))
		{
			insert_visited(ctx->visited, &state);
			new_cp = clone_checkpoint(ctx->eng);
			if (new_cp && !enqueue(ctx->q, new_cp, ctx->depth + 1))
				free_checkpoint(new_cp);
		}
	}
	return (restore_checkpoint(ctx->eng, cp), free_checkpoint(cp), 0);
}

int	try_all_targets_bfs(t_solver_ctx *ctx, t_vec2 cur_t)
{
	t_vec2	exit_pos;
	int		w;
	int		h;
	t_vec2	grid;
	t_vec2	adj;

	find_exit(ctx->eng->data, &exit_pos);
	if (exit_pos.i.x != -1 && try_branch_bfs(ctx, exit_pos))
		return (1);
	w = get_map_width(get_blob_hdr(ctx->eng->blob));
	h = get_map_height(get_blob_hdr(ctx->eng->blob));
	grid.i.y = -1;
	while (++grid.i.y < h)
	{
		grid.i.x = -1;
		while (++grid.i.x < w)
		{
			if ((get_map_flags(ctx->eng->blob)[grid.i.y * w + grid.i.x]
				& CELL_HAS_OBJ) && ctx->eng->data->obj_defs[get_map_occ_ids(
				ctx->eng->blob)[grid.i.y * w + grid.i.x]].symbol == 'T')
			{
				if (grid.i.x != cur_t.i.x || grid.i.y != cur_t.i.y)
				{
					adj = find_walkable_neighbor(ctx->eng, grid);
					if (adj.i.x != -1 && try_branch_bfs(ctx, adj))
						return (1);
				}
			}
		}
	}
	return (0);
}
