#include "cub.h"

typedef union u_conv
{
	float		f;
	int32_t		i;
}	t_conv;

void	rotate_vec_fp(t_vec2_fp *v, t_fp fp_cos, t_fp fp_sin)
{
	t_fp	old_x;

	old_x = v->x;
	v->x = FP_MUL(old_x, fp_cos) - FP_MUL(v->y, fp_sin);
	v->y = FP_MUL(old_x, fp_sin) + FP_MUL(v->y, fp_cos);
}

float	fast_inv_sqrt(float number)
{
	t_conv		conv;
	float		x2;
	float		y;

	x2 = number * 0.5f;
	conv.f = number;
	conv.i = 0x5f3759df - (conv.i >> 1);
	y = conv.f;
	y = y * (1.5f - (x2 * y * y));
	return (y);
}

double	get_door_ratio(int mx, int my, int width, void *blob)
{
	t_door_rt	*doors;
	int			door_count;
	int			i;

	doors = get_door_rt(blob);
	door_count = get_blob_hdr(blob)->door_rt.count;
	i = 0;
	while (i < door_count)
	{
		if (doors[i].map_id == (uint32_t)(my * width + mx))
			return ((double)doors[i].open_ratio_255 / 255.0);
		i++;
	}
	return (0.0);
}

int	is_door_horiz(int mx, int my, int w, void *blob)
{
	uint8_t	*flags;
	int		h;

	flags = get_map_flags(blob);
	h = get_map_height(get_blob_hdr(blob));
	if (my > 0 && my < h - 1)
	{
		if ((flags[(my - 1) * w + mx] & CELL_HAS_WALL) || 
			(flags[(my + 1) * w + mx] & CELL_HAS_WALL))
			return (0);
	}
	return (1);
}

int	check_door_hit(t_ray_data *ray, t_player_rt *p, void *blob)
{
	int		hz;
	float	t;
	float	da;
	float	db;
	float	wx;

	hz = is_door_horiz(ray->map_x, ray->map_y,
			get_map_width(get_blob_hdr(blob)), blob);
	da = hz ? FP_TO_FLOAT(ray->ray_dir.y) : FP_TO_FLOAT(ray->ray_dir.x);
	db = hz ? FP_TO_FLOAT(ray->ray_dir.x) : FP_TO_FLOAT(ray->ray_dir.y);
	if (fabsf(da) < 1e-6f)
		return (0);
	t = ((hz ? ray->map_y : ray->map_x) + 0.5f
			- (hz ? p->pos.y : p->pos.x)) / da;
	if (t <= 0.0f)
		return (0);
	wx = (hz ? p->pos.x : p->pos.y) + t * db - (hz ? ray->map_x : ray->map_y);
	if (wx < 0.0f || wx > 1.0f || wx < (float)get_door_ratio(ray->map_x,
			ray->map_y, get_map_width(get_blob_hdr(blob)), blob))
		return (0);
	ray->perp_wall_dist = DOUBLE_TO_FP(t);
	ray->side = hz;
	ray->wall_x = DOUBLE_TO_FP(wx);
	return (1);
}