#include "../../includes/cub.h"

/*GET IDS ARRAYS*/

static size_t all_ids_len(t_data *d)
{
	return ((size_t)\
		(d->walls_len + d->airs_len + \
		d->doors_len + d->monsters_len + \
		d->obj_len + d->player_len));
}

static bool add_char_if_single(char c, char *dest, size_t max_len)
{
	size_t len;
	size_t i;

	i = 0;
	len = ft_strlen(dest);
	if (len >= max_len)
		return (false);
	while (dest[i])
	{
		if (dest[i] == c)
			return (false);
		i++;
	}
	dest[len] = c;
	return (true);
}

bool get_wall_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_wall_def	*w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->walls_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->walls_section_id;
	out = malloc(sizeof(char) * (p->data->walls_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->walls_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->wall_ids = out;
	return (true);
}

bool get_air_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_air_def	*w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->airs_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->airs_section_id;
	out = malloc(sizeof(char) * (p->data->airs_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->airs_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->air_ids = out;
	return (true);
}

bool get_door_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_door_def	*w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->doors_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->doors_section_id;
	out = malloc(sizeof(char) * (p->data->doors_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->doors_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->door_ids = out;
	return (true);
}

bool get_monster_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_monster_def *w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->monsters_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->monsters_section_id;
	out = malloc(sizeof(char) * (p->data->monsters_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->monsters_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->monster_ids = out;
	return (true);
}

bool get_object_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_obj_def	*w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->obj_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->objs_section_id;
	out = malloc(sizeof(char) * (p->data->obj_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->obj_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->obj_ids = out;
	return (true);
}

bool get_player_ids(t_parsing *p, size_t max_len)
{
	t_index		*i;
	t_player_def *w;
	size_t		y;
	char		sym;
	char		*out;

	w = p->data->player_defs;
	i = &p->idx;
	i->col = 0;
	i->line = p->data->players_section_id;
	out = malloc(sizeof(char) * (p->data->player_len + 1));
	if (!out)
		return (i->err = PERR_MALLOC_ERR, false);
	y = 0;
	while (y < p->data->player_len)
	{
		sym = w[y].symbol;
		if (!add_char_if_single(sym, p->data->all_ids, max_len))
			return (i->err = PERR_REPETED_SYM, free(out), false);
		out[y] = sym;
		y++;
		i->line++;
	}
	out[y] = 0;
	p->data->player_ids = out;
	return (true);
}

bool get_ids(t_parsing *p)
{
	t_data	*d;
	t_index	*i;
	size_t	alloc_len;
	
	d = p->data;
	i = &p->idx;
	alloc_len = all_ids_len(d);
	d->all_ids = ft_calloc(alloc_len + 1, sizeof(char));
	if (!d->all_ids)
		return (i->err = PERR_MALLOC_ERR, false);
	if (!get_wall_ids(p, alloc_len))
		return (false);
	if (!get_air_ids(p, alloc_len))
		return (false);
	if (!get_door_ids(p, alloc_len))
		return (false);
	if (!get_monster_ids(p, alloc_len))
		return (false);
	if (!get_object_ids(p, alloc_len))
		return (false);
	if (!get_player_ids(p, alloc_len))
		return (false);
	return (true);
}