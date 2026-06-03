#include "cub.h"

void	draw_hacking_overlay(t_engine *eng);
void	draw_hacking_text(t_engine *eng);
void	draw_terminal_hacking_screen(t_engine *eng);
void	draw_terminal_hacking_text(t_engine *eng);
void	display_fps(t_engine *eng);
int		is_near_terminal(t_engine *eng);

void	draw_hacking_prompt(t_engine *eng)
{
	if (!is_near_terminal(eng))
		return ;
	if (eng->alarm_triggered)
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
			eng->screen->win_width / 2 - 120, eng->screen->win_height / 2 + 100,
			0xFF0000, "[E] PRESS E TO DEACTIVATE ALARM");
	else
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
			eng->screen->win_width / 2 - 120, eng->screen->win_height / 2 + 100,
			0x00FF00, "[E] PRESS E TO DECRYPT TERMINAL");
}

static void	draw_all_entities(t_engine *engine)
{
	draw_monsters_3d(engine);
	draw_objects_3d(engine);
	draw_minimap(engine);
	draw_hacking_overlay(engine);
}

static void	sync_workers(t_thread_pool *pool)
{
	int	i;

	i = 0;
	while (i < pool->workers_count)
	{
		submit_job(pool, worker_render_slice, &pool->workers[i]);
		i++;
	}
	wait_for_jobs(pool);
}

void	render_frame(t_engine *engine)
{
	t_screen		*screen;
	t_img			temp;

	engine->pool.current_frame++;
	screen = engine->screen;
	sync_workers(&engine->pool);
	temp = screen->img;
	screen->img = screen->img2;
	screen->img2 = temp;
	if (engine->terminal_mode)
	{
		ft_bzero(screen->img2.addr, screen->win_width * screen->win_height
			* (screen->img2.bpp / 8));
		draw_terminal_hacking_screen(engine);
	}
	else
		draw_all_entities(engine);
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr,
		screen->img2.img_ptr, 0, 0);
	if (engine->terminal_mode)
		draw_terminal_hacking_text(engine);
	else
		draw_hacking_text(engine);
	display_fps(engine);
	mlx_do_sync(screen->mlx_ptr);
}
