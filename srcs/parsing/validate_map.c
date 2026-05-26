#include "../../includes/cub.h"

bool check_m_map_sym(char *all_sym_array, char **map, t_index *i, uint32_t map_pos)
{
	size_t l;
	size_t c;

	l = 0;
	while (map[l])
	{
		c = 0;
		while (map[l][c])
		{
			if (map[l][c] != ' ' && !ft_isset(map[l][c], all_sym_array))
			{
				i->line = map_pos + l;
				i->col = c;
				return (i->err = PERR_INVALIDE_SYMBOL, false);
			}
			c++;
		}
		l++;
	}
	return (true);
}

bool check_player(t_data *d, t_index *i)
{
	size_t l;
	size_t c;
	bool	player_found;
	
	player_found = false;
	l = 0;
	while (d->m_map[l])
	{
		c = 0;
		while (d->m_map[l][c])
		{
			if (player_found && ft_isset(d->m_map[l][c], d->player_ids))
			{
				i->line = d->m_map_section_id + l;
				i->col = c;
				return (i->err = PERR_TOO_MANY_PLAYER, false);
			}
			if (ft_isset(d->m_map[l][c], d->player_ids))
			{
				// assign player pos
				d->player_row = l;
				d->player_col = c;
				size_t p_idx = 0;
				while (p_idx < d->player_len)
				{
					if (d->player_defs[p_idx].symbol == d->m_map[l][c])
					{
						d->player_dir = d->player_defs[p_idx].dir;
						break;
					}
					p_idx++;
				}
				player_found = true;
			}
			c++;
		}
		l++;
	}
	if (!player_found)
	{
		i->line = d->m_map_section_id;
		i->col = 0;
		return (i->err = PERR_PLAYER_NOT_FOUND, false);
	}
	return (true);
}

typedef struct	s_tmp_map_data
{
	char		**map;
	char		**bonus;
	char		*wall_sym;
	char		*air_sym;
	char		*others_sym;
	bool		**mask;
	size_t		c;
	size_t		r;
	t_perr		err;
}				t_tmp_map_data;

void free_bool_tab(bool **tab)
{
	size_t i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

bool **dup_str_tab_to_bool_tab(char **map, size_t row_len)
{
	bool **out;
	size_t i;
	size_t tmp_len;

	i = 0;
	out = malloc(sizeof(bool *) * (row_len + 1));
	if (!out)
		return (NULL);
	while (i < row_len)
	{
		tmp_len = ft_strlen(map[i]);
		out[i] = malloc(sizeof(bool) * (tmp_len));
		if (!out[i])
			return (free_bool_tab(out), NULL);
		ft_bzero(out[i], sizeof(bool) * (tmp_len));
		i++;
	}
	out[i] = NULL;
	return (out);
}

bool set_error_tmp_map(t_tmp_map_data *f, ssize_t r, ssize_t c, t_perr err)
{
	printf("FLOOD ERROR: row %zd col %zd\n", r, c);
	if (r < 0)
		r = 0;
	if (c < 0)
		c = 0;
	f->r = (size_t)r;
	f->c = (size_t)c;
	f->err = err;
	return (false);
}

bool flood_fill(t_tmp_map_data *f, ssize_t r, ssize_t c)
{
	char **map;
	bool **mask;

	map = f->map;
	mask = f->mask;
	if (r < 0 || !map[r] || c < 0 || !map[r][c])
		return (set_error_tmp_map(f, r, c, PERR_UNCLOSED_MAP));
	if (map[r][c] == ' ')
		return (set_error_tmp_map(f, r, c, PERR_UNEXPECTED_SPACE));
	if (ft_isset(map[r][c], f->wall_sym))
		return (true);
	if (mask[r][c])
		return (true);
	mask[r][c] = true;
	return (flood_fill(f, r + 1, c) && \
			flood_fill(f, r - 1, c) && \
			flood_fill(f, r, c + 1) && \
			flood_fill(f, r, c - 1));
}

bool check_bonus_map(t_tmp_map_data *f)
{
	char **map;
	char **b;
	size_t r;
	size_t c;

	map = f->map;
	b = f->bonus;
	r = 0;
	while (map[r])
	{
		c = 0;
		while (map[r][c])
		{
			if (!b[r] || !b[r][c])
				return (set_error_tmp_map(f, (ssize_t)r, (ssize_t)c, PERR_MAP_MISMATCH));
			if (b[r][c] != map[r][c])
			{
				if (!ft_isset(map[r][c], f->others_sym))
					return (set_error_tmp_map(f, (ssize_t)r, (ssize_t)c, PERR_MAP_MISMATCH));
			}
			c++;
		}
		if (b[r][c] != '\0')
			return (set_error_tmp_map(f, (ssize_t)r, (ssize_t)c, PERR_MAP_MISMATCH));
		r++;
	}
	if (b[r] != NULL)
		return (set_error_tmp_map(f, (ssize_t)r, 0, PERR_MAP_MISMATCH));
	return (true);
}

bool validate_map(t_data *d, t_index *i)
{
	t_tmp_map_data	f;
	bool			flood_success;
	
	if (!check_m_map_sym(d->all_ids, d->m_map, i, d->m_map_section_id))
		return (false);
	if (!check_player(d, i))
		return (false);

	f.map = d->m_map;
	f.bonus = d->b_map;
	f.wall_sym = d->wall_ids;
	f.air_sym = d->air_ids;
	f.others_sym = &d->all_ids[d->walls_len + d->airs_len];

	f.mask = dup_str_tab_to_bool_tab(f.map, d->m_height);
	if (!f.mask)
		return (i->err = PERR_MALLOC_ERR, printf("1\n"), false);
	flood_success = flood_fill(&f, (ssize_t)d->player_row, (ssize_t)d->player_col);
	free_bool_tab(f.mask);

	if (!flood_success)
	{
		i->line = d->m_map_section_id + f.r;
		i->col = f.c;
		i->err = f.err;
		return (false);
	}
	if (d->have_bonus && !check_bonus_map(&f))
	{
		i->line = d->b_map_section_id + f.r;
		i->col = f.c;
		i->err = f.err;
		return (false);
	}
	return (true);
}