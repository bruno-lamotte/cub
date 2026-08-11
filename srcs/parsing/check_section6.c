/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

size_t	ft_strlen_no_right_space(char *s)
{
	bool	in_space;
	size_t	last_end;
	size_t	i;

	i = 0;
	last_end = 0;
	in_space = true;
	while (s && s[i])
	{
		if (in_space && !ft_isspace(s[i]))
			in_space = false;
		else if (!in_space && ft_isspace(s[i]))
		{
			last_end = i;
			in_space = true;
		}
		i++;
	}
	if (in_space)
		i = last_end;
	return (i);
}

char	**dup_map_and_trim_right_space(char **map, size_t len)
{
	char	**out;
	size_t	i;

	i = 0;
	out = malloc(sizeof(char *) * (len + 1));
	if (!out)
		return (NULL);
	while (i < len)
	{
		out[i] = ft_strndup(map[i], ft_strlen_no_right_space(map[i]));
		if (!out[i])
			return (free_str_tab(out), NULL);
		i++;
	}
	out[i] = NULL;
	return (out);
}

size_t	get_map_max_widht(char **map)
{
	size_t	i;
	size_t	max;
	size_t	res;

	max = 0;
	i = 0;
	while (map[i])
	{
		res = ft_strlen(map[i]);
		if (res > max)
			max = res;
		i++;
	}
	return (max);
}

bool	check_mandatory_map_section(t_parsing *p, size_t len)
{
	char	**map;
	t_index	*i;

	i = &p->idx;
	map = dup_map_and_trim_right_space(&p->file_content[i->line], len);
	if (!map)
		return (i->err = PERR_MALLOC_ERR, false);
	p->data->m_max_width = get_map_max_widht(map);
	p->data->m_map_section_id = i->line;
	p->data->m_height = len;
	if (p->data->m_max_width > MAX_MAP_WIDTH || len > MAX_MAP_HEIGHT)
		return (i->err = PERR_MAP_SIZE_TOO_LARGE, free_str_tab(map), false);
	p->data->m_map = map;
	i->line += len;
	return (true);
}

bool	check_bonus_map_section(t_parsing *p, size_t len)
{
	char	**map;
	t_index	*i;

	i = &p->idx;
	map = dup_map_and_trim_right_space(&p->file_content[i->line], len);
	if (!map)
		return (i->err = PERR_MALLOC_ERR, false);
	p->data->b_max_width = get_map_max_widht(map);
	p->data->b_map_section_id = i->line;
	p->data->b_height = len;
	if (p->data->b_max_width > MAX_MAP_WIDTH || len > MAX_MAP_HEIGHT)
		return (i->err = PERR_MAP_SIZE_TOO_LARGE, false);
	p->data->b_map = map;
	i->line += len;
	return (true);
}
