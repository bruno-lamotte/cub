#include "cub.h"
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int	close_window(t_engine *engine)
{
	if (engine)
	{
		shutdown_thread_pool(engine);
		if (engine->slr)
			free_all(engine->slr, NULL);
		free(engine->player);
		free(engine->z_buffer);
		free(engine->blob);
		if (engine->screen && engine->data)
			free_img_tab(engine->screen->mlx_ptr, engine->data->img_tab,
				engine->data->textures_len);
		if (engine->screen)
		{
			free_screen(engine->screen);
			free(engine->screen);
		}
		if (engine->data)
		{
			free_preprocessing_data(engine->data);
			free(engine->data);
		}
	}
	exit(0);
	return (0);
}

int	key_press(int keycode, t_engine *engine)
{
	if (keycode == XK_w || keycode == 'w' || keycode == 'W'
		|| keycode == XK_z || keycode == 'z' || keycode == 'Z')
		engine->keys.w = true;
	else if (keycode == XK_s || keycode == 's' || keycode == 'S')
		engine->keys.s = true;
	else if (keycode == XK_a || keycode == 'a' || keycode == 'A'
		|| keycode == XK_q || keycode == 'q' || keycode == 'Q')
		engine->keys.a = true;
	else if (keycode == XK_d || keycode == 'd' || keycode == 'D')
		engine->keys.d = true;
	else if (keycode == XK_Left)
		engine->keys.left = true;
	else if (keycode == XK_Right)
		engine->keys.right = true;
	else if (keycode == 'e' || keycode == 'E')
		engine->keys.e = true;
	else if (keycode == XK_Escape)
		close_window(engine);
	return (0);
}

int	key_release(int keycode, t_engine *engine)
{
	if (keycode == XK_w || keycode == 'w' || keycode == 'W'
		|| keycode == XK_z || keycode == 'z' || keycode == 'Z')
		engine->keys.w = false;
	else if (keycode == XK_s || keycode == 's' || keycode == 'S')
		engine->keys.s = false;
	else if (keycode == XK_a || keycode == 'a' || keycode == 'A'
		|| keycode == XK_q || keycode == 'q' || keycode == 'Q')
		engine->keys.a = false;
	else if (keycode == XK_d || keycode == 'd' || keycode == 'D')
		engine->keys.d = false;
	else if (keycode == XK_Left)
		engine->keys.left = false;
	else if (keycode == XK_Right)
		engine->keys.right = false;
	else if (keycode == 'e' || keycode == 'E')
		engine->keys.e = false;
	return (0);
}

static t_light	*find_closest_alarm(t_engine *eng, double tx, double ty,
				double *min_dist)
{
	int			i;
	t_light		*closest;
	double		d;

	closest = NULL;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (!eng->static_lights[i].is_alarm)
			continue ;
		d = (eng->static_lights[i].x - tx) * (eng->static_lights[i].x - tx)
			+ (eng->static_lights[i].y - ty) * (eng->static_lights[i].y - ty);
		if (d < *min_dist)
		{
			*min_dist = d;
			closest = &eng->static_lights[i];
		}
	}
	return (closest);
}

static t_door_rt	*find_closest_door(t_engine *eng, double tx, double ty,
				double *min_dist)
{
	int			i;
	t_door_rt	*doors;
	t_door_rt	*closest;
	int			w;
	double		d;

	doors = get_door_rt(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	closest = NULL;
	i = -1;
	while (++i < (int)get_blob_hdr(eng->blob)->door_rt.count)
	{
		d = ((doors[i].map_id % w) + 0.5 - tx)
			* ((doors[i].map_id % w) + 0.5 - tx)
			+ ((doors[i].map_id / w) + 0.5 - ty)
			* ((doors[i].map_id / w) + 0.5 - ty);
		if (d < *min_dist)
		{
			*min_dist = d;
			closest = &doors[i];
		}
	}
	return (closest);
}

void	update_global_alarm_state(t_engine *eng)
{
	int	i;

	eng->alarm_triggered = 0;
	i = -1;
	while (++i < eng->static_light_count)
	{
		if (eng->static_lights[i].is_alarm
			&& eng->static_lights[i].is_triggered)
		{
			eng->alarm_triggered = 1;
			break ;
		}
	}
}

static void	toggle_terminal_target(t_engine *eng, double tx, double ty)
{
	double		d_alarm;
	double		d_door;
	t_light		*closest_alarm;
	t_door_rt	*closest_door;

	d_alarm = 1e30;
	d_door = 1e30;
	closest_alarm = find_closest_alarm(eng, tx, ty, &d_alarm);
	closest_door = find_closest_door(eng, tx, ty, &d_door);
	if (closest_door && d_door < d_alarm)
		closest_door->flags ^= DOOR_BLOCKED;
	else if (closest_alarm)
		closest_alarm->is_triggered ^= 1;
	update_global_alarm_state(eng);
}

static int	is_near_t(t_engine *eng, int x, int y,
				uint8_t *flags, uint8_t *occ)
{
	double	dx;
	double	dy;
	int		w;

	w = get_map_width(eng->blob);
	if (!(flags[y * w + x] & CELL_HAS_OBJ))
		return (0);
	if (eng->data->obj_defs[occ[y * w + x]].symbol != 'T')
		return (0);
	dx = eng->player->pos.x - (x + 0.5);
	dy = eng->player->pos.y - (y + 0.5);
	return (dx * dx + dy * dy < 2.25);
}

static void	check_proximity(t_engine *eng, int w, int h, uint8_t *flags)
{
	uint8_t	*occ;
	int		x;
	int		y;

	occ = get_map_occ_ids(eng->blob);
	y = -1;
	while (++y < h)
	{
		x = -1;
		while (++x < w)
		{
			if (is_near_t(eng, x, y, flags, occ))
			{
				eng->hacking_timer = 90;
				eng->hacking_x = x + 0.5;
				eng->hacking_y = y + 0.5;
				eng->keys.e = false;
				return ;
			}
		}
	}
}

static void	update_interaction(t_engine *eng)
{
	if (eng->hacking_timer > 0)
	{
		if (--eng->hacking_timer == 0)
			toggle_terminal_target(eng, eng->hacking_x, eng->hacking_y);
		eng->keys.e = false;
		return ;
	}
	if (eng->keys.e)
	{
		eng->keys.e = false;
		check_proximity(eng, get_map_width(get_blob_hdr(eng->blob)),
			get_map_height(get_blob_hdr(eng->blob)),
			get_map_flags(eng->blob));
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
	if (engine->hacking_timer <= 0)
	{
		update_position(engine, &engine->keys);
		update_rotation(engine->player, &engine->keys);
	}
	render_frame(engine);
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec);
	if (elapsed < 16666L)
		usleep(16666L - elapsed);
	return (0);
}
