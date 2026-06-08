#include "cub.h"
#include <math.h>

int	sh_idx(double t)
{
	int	idx;

	idx = (int)(t * 10.0f);
	if (idx < 0)
		return (0);
	if (idx >= DIST_MAX)
		return (DIST_MAX - 1);
	return (idx);
}

static int	calc_monster_octant(t_engine *eng, t_monster_rt *m)
{
	double	view_angle;
	double	monster_angle;
	double	rel_angle;

	view_angle = atan2(m->pos.d.y - eng->player->pos.d.y,
			m->pos.d.x - eng->player->pos.d.x);
	monster_angle = atan2(m->dir.d.y, m->dir.d.x);
	rel_angle = view_angle - monster_angle - M_PI;
	while (rel_angle < -M_PI)
		rel_angle += 2.0 * M_PI;
	while (rel_angle > M_PI)
		rel_angle -= 2.0 * M_PI;
	if (rel_angle < 0)
		rel_angle += 2.0 * M_PI;
	rel_angle += M_PI / 8.0;
	if (rel_angle >= 2.0 * M_PI)
		rel_angle -= 2.0 * M_PI;
	return ((int)(rel_angle / (M_PI / 4.0)));
}

t_mstr_anim_type	get_monster_active_anim(t_engine *eng, t_sprite *s,
						int *mirror)
{
	t_monster_rt		*m;
	int					octant;

	m = &get_monster_rt(eng->blob)[s->m_idx];
	*mirror = 0;
	if (m->state == MSTR_STATE_ATTACK)
		return (MSTR_ANIM_PUNCH);
	if (m->state == MSTR_STATE_CHASE)
		return (MSTR_ANIM_COURSE);
	octant = calc_monster_octant(eng, m);
	if (octant == 0)
		return (MSTR_ANIM_FACE);
	if (octant == 1 || octant == 7)
	{
		*mirror = (octant == 7);
		return (MSTR_ANIM_18FACE);
	}
	if (octant == 2 || octant == 6)
	{
		*mirror = (octant == 6);
		return (MSTR_ANIM_PROFIL);
	}
	if (octant == 4)
		return (MSTR_ANIM_DOS);
	*mirror = (octant == 5);
	return (MSTR_ANIM_18DOS);
}

void	calc_monster_bounds(t_engine *eng, t_sprite *s,
			t_vec2 params, int bounds[2])
{
	int					mirror;
	t_mstr_anim_type	anim_type;
	t_mstr_anim			*anim;
	double				scale;

	anim_type = get_monster_active_anim(eng, s, &mirror);
	anim = &eng->anims[anim_type];
	if (anim->frame_count <= 0)
	{
		bounds[0] = 0;
		bounds[1] = -1;
		return ;
	}
	scale = (params.i.y * MONSTER_SCALE) / MSTR_ORIG_WIDTH;
	if (anim_type == MSTR_ANIM_PUNCH)
		scale *= 0.5;
	bounds[0] = params.i.x + anim->offset.i.x * scale
		- (anim->frames[0].width * scale) / 2.0;
	bounds[1] = bounds[0] + anim->frames[0].width * scale;
	if (bounds[0] < 0)
		bounds[0] = 0;
	if (bounds[1] >= eng->screen->win_width)
		bounds[1] = eng->screen->win_width - 1;
}
