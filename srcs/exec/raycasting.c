#include "cub.h"
#include <math.h>

typedef struct s_dda_state
{
	float	ray_dir_x;
	float	ray_dir_y;
	float	delta_dist_x;
	float	delta_dist_y;
	float	side_dist_x;
	float	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	int		hit;
}				t_dda_state;

static void	init_ray(int x, t_player_rt *p, t_ray_data *out, t_dda_state *s)
{
	float	cam_x;

	cam_x = 2.0f * x / (float)WINDOW_WIDTH - 1.0f;
	s->ray_dir_x = (float)p->dir.x + (float)p->plane.x * cam_x;
	s->ray_dir_y = (float)p->dir.y + (float)p->plane.y * cam_x;
	out->map_x = (int)p->pos.x;
	out->map_y = (int)p->pos.y;
	s->delta_dist_x = (s->ray_dir_x == 0.0f) ? 1e30f
		: fabsf(1.0f / s->ray_dir_x);
	s->delta_dist_y = (s->ray_dir_y == 0.0f) ? 1e30f
		: fabsf(1.0f / s->ray_dir_y);
	s->step_x = (s->ray_dir_x < 0.0f) ? -1 : 1;
	s->side_dist_x = (s->ray_dir_x < 0.0f) ? ((float)p->pos.x - out->map_x)
		* s->delta_dist_x : (out->map_x + 1.0f - (float)p->pos.x)
		* s->delta_dist_x;
	s->step_y = (s->ray_dir_y < 0.0f) ? -1 : 1;
	s->side_dist_y = (s->ray_dir_y < 0.0f) ? ((float)p->pos.y - out->map_y)
		* s->delta_dist_y : (out->map_y + 1.0f - (float)p->pos.y)
		* s->delta_dist_y;
	s->hit = 0;
	s->side = 0;
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
		s->side = (s->side_dist_x < s->side_dist_y) ? 0 : 1;
		if (s->side == 0)
			out->map_x += (s->side_dist_x += s->delta_dist_x, s->step_x);
		else
			out->map_y += (s->side_dist_y += s->delta_dist_y, s->step_y);
		if (out->map_x < 0 || out->map_x >= w || out->map_y < 0
			|| out->map_y >= get_map_height(get_blob_hdr(blob)))
			s->hit = (out->hit_type = 0, 1);
		else if (flg[idx = out->map_y * w + out->map_x] & CELL_HAS_WALL)
			s->hit = (out->hit_type = get_map_block_ids(blob)[idx], 1);
		else if ((flg[idx] & CELL_HAS_DOOR) && (out->ray_dir.x
				= DOUBLE_TO_FP(s->ray_dir_x), 1) && (out->ray_dir.y
				= DOUBLE_TO_FP(s->ray_dir_y), 1))
			if (check_door_hit(out, p, blob))
				s->hit = (out->hit_type = get_map_occ_ids(blob)[idx], 1);
	}
}

static void	fill_ray_out(t_ray_data *out, t_dda_state *s, t_player_rt *p,
	void *blob)
{
	float	dist;
	float	wx;

	if (get_map_flags(blob)[out->map_y * get_map_width(get_blob_hdr(blob))
			+ out->map_x] & CELL_HAS_WALL)
	{
		if (s->side == 0)
			dist = ((float)out->map_x - (float)p->pos.x
					+ (1.0f - (float)s->step_x) * 0.5f) / s->ray_dir_x;
		else
			dist = ((float)out->map_y - (float)p->pos.y
					+ (1.0f - (float)s->step_y) * 0.5f) / s->ray_dir_y;
		if (dist <= 0.0f)
			dist = 0.1f;
		if (dist > 30000.0f)
			dist = 30000.0f;
		wx = (s->side == 0) ? (float)p->pos.y + dist * s->ray_dir_y
			: (float)p->pos.x + dist * s->ray_dir_x;
		wx -= floorf(wx);
		out->perp_wall_dist = DOUBLE_TO_FP(dist);
		out->side = s->side;
		out->wall_x = DOUBLE_TO_FP(wx);
	}
	out->ray_dir.x = DOUBLE_TO_FP(s->ray_dir_x);
	out->ray_dir.y = DOUBLE_TO_FP(s->ray_dir_y);
}

void	cast_ray(int x, t_player_rt *p, void *blob, t_ray_data *out)
{
	t_dda_state	s;

	init_ray(x, p, out, &s);
	perform_dda(out, &s, p, blob);
	fill_ray_out(out, &s, p, blob);
}