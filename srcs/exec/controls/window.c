/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

static void	free_anims(t_engine *engine)
{
	int	a;
	int	f;

	a = -1;
	while (++a < MSTR_ANIM_COUNT)
	{
		f = -1;
		while (++f < engine->anims[a].frame_count)
		{
			if (engine->screen && engine->anims[a].frames[f].img_ptr)
				mlx_destroy_image(engine->screen->mlx_ptr,
					engine->anims[a].frames[f].img_ptr);
		}
	}
}

void	free_engine_graphics(t_engine *engine)
{
	if (engine->screen && engine->lamp_tex.img_ptr)
		mlx_destroy_image(engine->screen->mlx_ptr, engine->lamp_tex.img_ptr);
	if (engine->screen && engine->terminal_tex.img_ptr)
		mlx_destroy_image(engine->screen->mlx_ptr,
			engine->terminal_tex.img_ptr);
	if (engine->screen && engine->data)
		free_img_tab(engine->screen->mlx_ptr, engine->data->img_tab,
			engine->data->textures_len);
	free_anims(engine);
	free_screen(engine->screen);
	free_preprocessing_data(engine->data);
}

int	close_window(t_engine *engine, int ret_val)
{
	engine->ret_val = ret_val;
	mlx_loop_end(engine->screen->mlx_ptr);
	return (0);
}

static void	check_game_state(t_engine *engine)
{
	if (is_player_on_exit(engine))
	{
		printf("Victory! You reached the exit!\n");
		close_window(engine, CUB_EXIT_MENU);
	}
	if (engine->player->hp <= 0)
	{
		printf("Game Over! You were defeated by a monster.\n");
		close_window(engine, CUB_EXIT_MENU);
	}
	if (engine->hacking_timer <= 0 && !engine->terminal_mode)
	{
		update_position(engine, &engine->keys);
		update_rotation(engine->player, &engine->keys);
	}
}

int	game_loop(t_engine *engine)
{
	struct timeval	start;
	struct timeval	end;
	long			elapsed;

	gettimeofday(&start, NULL);
	update_doors(engine);
	update_interaction(engine);
	update_monsters(engine);
	check_game_state(engine);
	render_frame(engine);
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec);
	if (elapsed < 16666L)
		usleep(16666L - elapsed);
	return (0);
}
