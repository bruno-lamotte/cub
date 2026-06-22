#include "cub.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int	init_search(t_engine *eng, t_solver_ctx *ctx)
{
	t_map_state			state;
	t_solver_checkpoint	*cp;

	init_queue(ctx->q);
	if (is_player_on_exit(eng))
		return (1);
	state.player_pos.i.x = (uint16_t)eng->player->pos.d.x;
	state.player_pos.i.y = (uint16_t)eng->player->pos.d.y;
	state.door_mask = get_door_mask(eng);
	state.alarm_mask = get_alarm_mask(eng);
	state.monster_mask = get_monster_mask(eng);
	insert_visited(ctx->visited, &state);
	cp = clone_checkpoint(eng);
	if (!cp)
		return (0);
	if (!enqueue(ctx->q, cp, 0))
	{
		free_checkpoint(cp);
		return (0);
	}
	return (2);
}

static int	bfs_search(t_engine *eng, t_worker *worker, t_bst **visited_tree)
{
	t_solver_ctx	ctx;
	t_queue			q;
	int				init_res;
	time_t			start;

	ctx.eng = eng;
	ctx.worker = worker;
	ctx.visited = visited_tree;
	ctx.q = &q;
	init_res = init_search(eng, &ctx);
	if (init_res != 2)
		return (init_res);
	start = time(NULL);
	while (q.front)
	{
		if (time(NULL) - start >= 60)
			return (clear_queue(&q), -1);
		if (bfs_loop_step(&ctx))
			return (clear_queue(&q), 1);
	}
	clear_queue(&q);
	return (0);
}

static int	init_solver_engine(t_data *d, t_engine *eng)
{
	ft_bzero(eng, sizeof(t_engine));
	eng->is_solver = 1;
	eng->data = d;
	init_dummy_textures(d);
	// eng->player = init_player_rt(d);
	init_player_rt(d, eng->player);
	if (!eng->player)
		return (0);
	if (!build_blob(d, &eng->blob))
		return (free(eng->player), 0);
	init_static_lights(eng);
	eng->slr = malloc(sizeof(t_slr1));
	if (eng->slr)
	{
		initialize_data(eng->slr);
		eng->slr->table = create_parsing_table(eng->slr);
		if (eng->slr->table)
			init_all_ai_behaviors(eng->slr, eng);
	}
	return (1);
}

static void	free_solver_engine(t_engine *eng)
{
	if (eng->slr)
		free_all(eng->slr, NULL);
	free(eng->player);
	free(eng->blob);
}

int	is_map_solvable(t_data *d)
{
	t_engine	eng;
	t_worker	worker;
	t_bst		*visited_tree;
	int			res;
	int			width;

	visited_tree = NULL;
	if (!init_solver_engine(d, &eng))
		return (-1);
	width = get_map_width(get_blob_hdr(eng.blob));
	ft_bzero(&worker, sizeof(t_worker));
	worker.bfs_visited = malloc(sizeof(int) * width
			* get_map_height(get_blob_hdr(eng.blob)));
	worker.bfs_queue = malloc(sizeof(int) * width
			* get_map_height(get_blob_hdr(eng.blob)));
	worker.bfs_parent = malloc(sizeof(int) * width
			* get_map_height(get_blob_hdr(eng.blob)));
	printf("\033[1;33m[SOLVER] Running BFS solvability search...\033[0m\n");
	res = bfs_search(&eng, &worker, &visited_tree);
	printf("[SOLVER] BFS search complete. Solvability result: %d\n", res);
	free(worker.bfs_visited); free(worker.bfs_queue); free(worker.bfs_parent);
	free_bst(visited_tree);
	free_solver_engine(&eng);
	free(d->img_tab);
	d->img_tab = NULL;
	return (res);
}
