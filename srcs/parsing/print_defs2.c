/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_defs2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_monsters_def(t_monster_def *m, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!m)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %u hp, %s\n", m[i].symbol, m[i].size_pc, m[i].hp,
			tex[m[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void	print_objects_def(t_obj_def *o, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : %u%%, %s\n", o[i].symbol, o[i].size_pc,
			tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

char	get_dir(t_dir dir)
{
	if (dir == DIR_NORTH)
		return ('N');
	else if (dir == DIR_WEST)
		return ('W');
	else if (dir == DIR_SOUTH)
		return ('S');
	else if (dir == DIR_EAST)
		return ('E');
	else
		return ('X');
}

void	print_players_def(t_player_def *o, size_t len, t_texture_def *tex)
{
	size_t	i;

	i = 0;
	if (!o)
		return ;
	while (i < len)
	{
		printf("%c : dir = %c, tex = %s\n", o[i].symbol, get_dir(o[i].dir),
			tex[o[i].tex].name);
		i++;
	}
	printf("------------------\n");
}

void	print_line_exclude_sym(char *s, char *sym)
{
	char		buf[MAX_DEBUG_LINE + 1];
	uint16_t	i;

	i = 0;
	while (s[i] && i < MAX_DEBUG_LINE)
	{
		buf[i] = s[i];
		if (ft_isset(buf[i], sym))
			buf[i] = ' ';
		i++;
	}
	buf[i] = '\0';
	printf("%s\n", buf);
}
