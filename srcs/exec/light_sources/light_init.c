#include "cub.h"

static void	init_light_properties(t_engine *eng, t_light *l, int idx,
				char sym)
{
	if (sym == 'T')
	{
		l->intensity = 0.4f;
		l->radius = 2.0f;
		l->is_alarm = 0;
		l->is_active = 1;
	}
	else
	{
		l->intensity = 0.8f;
		l->radius = 4.0f;
		l->is_alarm = 1;
		l->is_active = eng->data->obj_defs[idx].pad;
	}
	l->radius_sq = l->radius * l->radius;
	l->is_triggered = 0;
}

static void	add_light_at_pos(t_engine *eng, int x, int y, int w)
{
	t_light	*l;
	uint8_t	*occ;
	int		idx;

	occ = get_map_occ_ids(eng->blob);
	l = &eng->static_lights[eng->static_light_count++];
	l->x = x + 0.5;
	l->y = y + 0.5;
	l->angle = 0.0f;
	idx = occ[y * w + x];
	init_light_properties(eng, l, idx, eng->data->obj_defs[idx].symbol);
}

void	init_static_lights(t_engine *eng)
{
	uint8_t	*flags;
	int		w;
	int		pos[2];

	flags = get_map_flags(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	eng->static_light_count = 0;
	eng->alarm_triggered = 0;
	eng->hacking_timer = 0;
	pos[1] = -1;
	while (++pos[1] < get_map_height(get_blob_hdr(eng->blob)))
	{
		pos[0] = -1;
		while (++pos[0] < w)
		{
			if (flags[pos[1] * w + pos[0]] & CELL_HAS_OBJ
				&& eng->static_light_count < 64)
				add_light_at_pos(eng, pos[0], pos[1], w);
		}
	}
	update_global_alarm_state(eng);
}
