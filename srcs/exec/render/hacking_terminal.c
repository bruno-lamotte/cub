#include "cub.h"
#include <math.h>

static double	get_terminal_map_px_per_tile(t_engine *eng, int radius)
{
	int				i;
	t_interact_obj	*obj;
	t_vec2			d;
	double			dist;
	double			max_dist;

	max_dist = 0.0;
	i = -1;
	while (++i < eng->interact_obj_count)
	{
		obj = &eng->interact_objs[i];
		d.d.x = obj->pos.d.x - eng->player->pos.d.x;
		d.d.y = obj->pos.d.y - eng->player->pos.d.y;
		dist = sqrt(d.d.x * d.d.x + d.d.y * d.d.y);
		if (dist > max_dist)
			max_dist = dist;
	}
	if (max_dist > 0.001)
	{
		if ((double)(radius - 20) / max_dist > 25.0)
			return (25.0);
		return ((double)(radius - 20) / max_dist);
	}
	return (25.0);
}

static void	draw_terminal_map_objects(t_engine *eng, int cx, int cy)
{
	int				i;
	t_vec2			p;
	t_interact_obj	*obj;
	int				radius;
	double			px_per_tile;

	radius = 196;
	px_per_tile = get_terminal_map_px_per_tile(eng, radius);
	i = -1;
	while (++i < eng->interact_obj_count)
	{
		obj = &eng->interact_objs[i];
		p.i.x = cx + (int)((obj->pos.d.x - eng->player->pos.d.x) * px_per_tile);
		p.i.y = cy + (int)((obj->pos.d.y - eng->player->pos.d.y) * px_per_tile);
		if ((p.i.x - cx) * (p.i.x - cx)
			+ (p.i.y - cy) * (p.i.y - cy) <= radius * radius)
			draw_single_map_obj(eng, obj, p, eng->selected_obj_idx == i);
	}
}

void	draw_terminal_hacking_screen(t_engine *eng)
{
	int	cx;
	int	cy;

	cx = eng->screen->win_width / 2;
	cy = eng->screen->win_height / 2;
	draw_filled_circle(&eng->screen->img2, (t_vec2){.i = {cx, cy}}, 200, 0x000F05);
	draw_circle_outline(&eng->screen->img2, (t_vec2){.i = {cx, cy}}, 200, 0x00FF33);
	draw_filled_circle(&eng->screen->img2, (t_vec2){.i = {cx, cy}}, 5, 0x00FFFF);
	draw_terminal_map_objects(eng, cx, cy);
}

static void	draw_door_info(t_engine *eng, t_interact_obj *obj, int x, int cy)
{
	t_door_rt		*doors;

	doors = get_door_rt(eng->blob);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 60,
		0x00FF33, "TYPE: SECURE AREA DOOR");
	if (doors[obj->idx].flags & DOOR_BLOCKED)
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 30,
			0xFF3300, "STATUS: BLOCKED (SECURE)");
	else
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 30,
			0x33FF33, "STATUS: UNLOCKED (CLEARED)");
}

static void	draw_alarm_info(t_engine *eng, t_interact_obj *obj, int x, int cy)
{
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 60,
		0x00FF33, "TYPE: ZONE ALARM SOURCE");
	if (eng->static_lights[obj->idx].is_triggered)
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 30,
			0xFF0055, "STATUS: DURESS ALARM ON!");
	else
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 30,
			0x33FF33, "STATUS: ALARM INACTIVE");
}

static void	draw_selected_info_text(t_engine *eng, int cx, int cy)
{
	t_interact_obj	*obj;
	int				x;

	x = cx + 250;
	if (eng->interact_obj_count <= 0)
	{
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 60,
			0xFF3300, "NO TARGETS DETECTED");
		return ;
	}
	obj = &eng->interact_objs[eng->selected_obj_idx];
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr, x, cy - 100,
		0x00FF33, "--- TARGET METADATA ---");
	if (obj->is_door)
		draw_door_info(eng, obj, x, cy);
	else
		draw_alarm_info(eng, obj, x, cy);
}

void	draw_terminal_hacking_text(t_engine *eng)
{
	int	cx;
	int	cy;

	cx = eng->screen->win_width / 2;
	cy = eng->screen->win_height / 2;
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 100, 0x00FF33, "=== TERMINAL DECRYPTION LINK ===");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 60, 0x00FF33, "Controls:");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy - 30, 0x00FF33, "  [A / D] or [Arrows] : Select target");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy, 0x00FF33, "  [E] or [Space] : Toggle target status");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		50, cy + 30, 0x00FF33, "  [Escape] : Close link");
	draw_selected_info_text(eng, cx, cy);
}
