#include "cub.h"

int	branch_terminal_wait(t_solver_ctx *ctx)
{
	t_solver_checkpoint	*cp;
	t_map_state			state;
	int					f;
	t_solver_checkpoint	*new_cp;

	cp = clone_checkpoint(ctx->eng);
	f = -1;
	while (++f < 40)
		simulate_engine_tick(ctx->eng, ctx->worker);
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
	return (restore_checkpoint(ctx->eng, cp), free_checkpoint(cp), 0);
}

int	branch_terminal_doors(t_solver_ctx *ctx)
{
	t_solver_checkpoint	*cp;
	t_map_state			state;
	uint32_t			d;
	int					f;
	t_solver_checkpoint	*new_cp;

	d = 0;
	while (d < get_blob_hdr(ctx->eng->blob)->door_rt.count)
	{
		cp = clone_checkpoint(ctx->eng);
		toggle_door_from_terminal(ctx->eng, d);
		f = -1;
		while (++f < 20)
			simulate_engine_tick(ctx->eng, ctx->worker);
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
		restore_checkpoint(ctx->eng, cp);
		free_checkpoint(cp);
		d++;
	}
	return (0);
}

static void	branch_alarm_idx(t_solver_ctx *ctx, int idx)
{
	t_solver_checkpoint	*cp;
	t_map_state			state;
	int					f;
	t_solver_checkpoint	*new_cp;

	cp = clone_checkpoint(ctx->eng);
	toggle_alarm_from_terminal(ctx->eng, idx);
	f = -1;
	while (++f < 20)
		simulate_engine_tick(ctx->eng, ctx->worker);
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
	restore_checkpoint(ctx->eng, cp);
	free_checkpoint(cp);
}

int	branch_terminal_alarms(t_solver_ctx *ctx)
{
	int	i;

	i = -1;
	while (++i < ctx->eng->static_light_count)
	{
		if (ctx->eng->static_lights[i].is_alarm)
			branch_alarm_idx(ctx, i);
	}
	return (0);
}

int	bfs_loop_step(t_solver_ctx *ctx)
{
	t_solver_checkpoint	*cp;
	t_vec2				term;

	cp = dequeue(ctx->q, &ctx->depth);
	restore_checkpoint(ctx->eng, cp);
	free_checkpoint(cp);
	if (is_player_on_exit(ctx->eng))
		return (1);
	if (ctx->depth >= 10)
		return (0);
	if (is_adjacent_to_terminal(ctx->eng, &term))
	{
		if (try_all_targets_bfs(ctx, term))
			return (1);
		branch_terminal_wait(ctx);
		branch_terminal_doors(ctx);
		branch_terminal_alarms(ctx);
	}
	else
	{
		term.i.x = -1;
		term.i.y = -1;
		if (try_all_targets_bfs(ctx, term))
			return (1);
	}
	return (0);
}
