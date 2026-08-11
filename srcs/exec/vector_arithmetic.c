/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_arithmetic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <math.h>

float	fast_inv_sqrt(float number)
{
	return (1.0f / sqrtf(number));
}

double	get_door_ratio(int mx, int my, int width, void *blob)
{
	t_door_rt	*doors;
	int			door_count;
	int			i;

	doors = get_door_rt(blob);
	door_count = get_blob_hdr(blob)->door_rt.u_data.count;
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
		if ((flags[(my - 1) * w + mx] & CELL_HAS_WALL)
			|| (flags[(my + 1) * w + mx] & CELL_HAS_WALL))
			return (0);
	}
	return (1);
}

static void	calc_door_params(t_ray_data *ray, t_player_rt *p, int hz,
				float *params)
{
	if (hz)
	{
		params[0] = fp_to_float(ray->ray_dir.fp.y);
		params[1] = fp_to_float(ray->ray_dir.fp.x);
		params[2] = (ray->map_y + 0.5f - p->pos.d.y) / params[0];
		params[3] = p->pos.d.x + params[2] * params[1] - ray->map_x;
	}
	else
	{
		params[0] = fp_to_float(ray->ray_dir.fp.x);
		params[1] = fp_to_float(ray->ray_dir.fp.y);
		params[2] = (ray->map_x + 0.5f - p->pos.d.x) / params[0];
		params[3] = p->pos.d.y + params[2] * params[1] - ray->map_y;
	}
}

int	check_door_hit(t_ray_data *ray, t_player_rt *p, void *blob)
{
	int		hz;
	float	params[4];
	double	ratio;
	int		w;

	w = get_map_width(get_blob_hdr(blob));
	hz = is_door_horiz(ray->map_x, ray->map_y, w, blob);
	calc_door_params(ray, p, hz, params);
	if (fabsf(params[0]) < 1e-6f || params[2] <= 0.0f)
		return (0);
	ratio = get_door_ratio(ray->map_x, ray->map_y, w, blob);
	if (params[3] < 0.0f || params[3] > 1.0f || params[3] < (float)ratio)
		return (0);
	ray->perp_wall_dist = double_to_fp(params[2]);
	ray->side = hz;
	ray->wall_x = double_to_fp(params[3]);
	return (1);
}
