/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	size_to_u32(size_t val, uint32_t *out)
{
	return (!__builtin_add_overflow(val, 0, out));
}

int16_t	get_block_id(char sym, t_data *d, t_block_type type)
{
	int16_t	i;
	char	*arr;

	if (type == T_WALL)
		arr = d->wall_ids;
	else if (type == T_AIR)
		arr = d->air_ids;
	else if (type == T_DOOR)
		arr = d->door_ids;
	else if (type == T_OBJ)
		arr = d->obj_ids;
	else if (type == T_MSTR)
		arr = d->monster_ids;
	else
		return (-1);
	i = 0;
	while (arr[i])
	{
		if (arr[i] == sym)
			return (i);
		i++;
	}
	return (-1);
}

int16_t	get_def_id(char *ids, char sym)
{
	int16_t	i;

	i = 0;
	while (ids[i])
	{
		if (ids[i] == sym)
			return (i);
		i++;
	}
	return (-1);
}
