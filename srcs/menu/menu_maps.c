/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_maps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:29:59 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/05 18:40:42 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

static int32_t	count_maps(char *dir_name)
{
	DIR				*dir_stream;
	struct dirent	*dir_struct;	
	int32_t			count;

	count = 0;
	dir_stream = opendir(dir_name);
	if (!dir_stream)
		return (menu_error("opendir failure"), -1);
	dir_struct = readdir(dir_stream);
	while (dir_struct)
	{
		if (check_filename(dir_struct->d_name))
			count++;
		dir_struct = readdir(dir_stream);
	}
	closedir(dir_stream);
	return (count);
}

bool	sub_load_maps(struct dirent *file, char *name, char **maps, int *i)
{
	if (check_filename(file->d_name))
	{
		maps[*i] = ft_strjoin(name, file->d_name);
		if (!maps[*i])
			return (menu_error("malloc failure"), free_str_tab(maps), false);
		(*i)++;
	}
	return (true);
}

char	**load_maps(char *dir_name, int32_t *count)
{
	DIR				*dir_stream;
	struct dirent	*dir_struct;
	char			**maps;
	int				i;

	*count = count_maps(dir_name);
	if (*count <= 0)
		return (NULL);
	maps = malloc(sizeof(char *) * (*count + 1));
	if (!maps)
		return (menu_error("malloc failure"), NULL);
	dir_stream = opendir(dir_name);
	if (!dir_stream)
		return (free(maps), NULL);
	i = 0;
	dir_struct = readdir(dir_stream);
	while (dir_struct)
	{
		if (!sub_load_maps(dir_struct, dir_name, maps, &i))
			return (closedir(dir_stream), NULL);
		dir_struct = readdir(dir_stream);
	}
	return (maps[i] = NULL, closedir(dir_stream), maps);
}
