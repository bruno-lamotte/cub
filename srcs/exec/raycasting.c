#include "cub.h"
#include <math.h>

typedef struct s_dda_state
{
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	int		hit;
}				t_dda_state;

static void	init_ray_steps(t_player_rt *p, t_ray_data *out, t_dda_state *s)
{
	if (s->ray_dir_x < 0.0)
	{
		s->step_x = -1;
		s->side_dist_x = (p->pos.d.x - (double)out->map_x) * s->delta_dist_x;
	}
	else
	{
		s->step_x = 1;
		s->side_dist_x = ((double)out->map_x + 1.0 - p->pos.d.x) * s->delta_dist_x;
	}
	if (s->ray_dir_y < 0.0)
	{
		s->step_y = -1;
		s->side_dist_y = (p->pos.d.y - (double)out->map_y) * s->delta_dist_y;
	}
	else
	{
		s->step_y = 1;
		s->side_dist_y = ((double)out->map_y + 1.0 - p->pos.d.y) * s->delta_dist_y;
	}
}

static void	init_ray(double cam_x, t_player_rt *p, t_ray_data *out, t_dda_state *s)
{
	s->ray_dir_x = p->dir.d.x + p->plane.d.x * cam_x;
	s->ray_dir_y = p->dir.d.y + p->plane.d.y * cam_x;
	out->map_x = (int)p->pos.d.x;
	out->map_y = (int)p->pos.d.y;
	if (s->ray_dir_x == 0.0)
		s->delta_dist_x = 1e30;
	else
		s->delta_dist_x = fabs(1.0 / s->ray_dir_x);
	if (s->ray_dir_y == 0.0)
		s->delta_dist_y = 1e30;
	else
		s->delta_dist_y = fabs(1.0 / s->ray_dir_y);
	init_ray_steps(p, out, s);
	s->hit = 0;
	s->side = 0;
}

static void	dda_single_step(t_ray_data *out, t_dda_state *s)
{
	if (s->side_dist_x < s->side_dist_y)
	{
		s->side = 0;
		s->side_dist_x += s->delta_dist_x;
		out->map_x += s->step_x;
	}
	else
	{
		s->side = 1;
		s->side_dist_y += s->delta_dist_y;
		out->map_y += s->step_y;
	}
}

static void	perform_dda(t_ray_data *out, t_dda_state *s, t_player_rt *p,
	void *blob)
{
	int		w;
	int		idx;
	uint8_t	*flg;

	w = get_map_width(get_blob_hdr(blob));
	flg = get_map_flags(blob);
	while (s->hit == 0)
	{
		dda_single_step(out, s);
		if (out->map_x < 0 || out->map_x >= w || out->map_y < 0
			|| out->map_y >= get_map_height(get_blob_hdr(blob)))
			s->hit = (out->hit_type = 0, 1);
		else if (flg[idx = out->map_y * w + out->map_x] & CELL_HAS_WALL)
			s->hit = (out->hit_type = get_map_block_ids(blob)[idx], 1);
		else if ((flg[idx] & CELL_HAS_DOOR) && (out->ray_dir.fp.x
				= DOUBLE_TO_FP(s->ray_dir_x), 1) && (out->ray_dir.fp.y
				= DOUBLE_TO_FP(s->ray_dir_y), 1))
			if (check_door_hit(out, p, blob))
				s->hit = (out->hit_type = get_map_occ_ids(blob)[idx], 1);
	}
}

static double	get_dda_dist(t_dda_state *s)
{
	double	dist;

	if (s->side == 0)
		dist = s->side_dist_x - s->delta_dist_x;
	else
		dist = s->side_dist_y - s->delta_dist_y;
	if (dist <= 0.0)
		return (0.1);
	if (dist > 30000.0)
		return (30000.0);
	return (dist);
}

static void	fill_ray_out(t_ray_data *out, t_dda_state *s, t_player_rt *p,
	void *blob)
{
	double	dist;
	double	wx;

	if (get_map_flags(blob)[out->map_y * get_map_width(get_blob_hdr(blob))
			+ out->map_x] & CELL_HAS_WALL)
	{
		dist = get_dda_dist(s);
		if (s->side == 0)
			wx = p->pos.d.y + dist * s->ray_dir_y;
		else
			wx = p->pos.d.x + dist * s->ray_dir_x;
		wx -= floor(wx);
		out->perp_wall_dist = DOUBLE_TO_FP(dist);
		out->side = s->side;
		out->wall_x = DOUBLE_TO_FP(wx);
	}
	out->ray_dir.fp.x = DOUBLE_TO_FP(s->ray_dir_x);
	out->ray_dir.fp.y = DOUBLE_TO_FP(s->ray_dir_y);
}

void	cast_ray(t_worker *w, int x, t_ray_data *out)
{
	t_dda_state	s;
	double		cam_x;

	cam_x = 2.0 * x / (double)w->screen->win_width - 1.0;
	init_ray(cam_x, w->player, out, &s);
	perform_dda(out, &s, w->player, w->blob);
	fill_ray_out(out, &s, w->player, w->blob);
}