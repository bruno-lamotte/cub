/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_anim_loader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int	load_frame_path(t_engine *eng, t_mstr_anim *anim,
		char *fld_ln[2], int count)
{
	char	path[512];
	int		len;

	len = ft_strlen(fld_ln[1]);
	while (len > 0 && (fld_ln[1][len - 1] == '\n'
		|| fld_ln[1][len - 1] == '\r'))
	{
		fld_ln[1][len - 1] = '\0';
		len--;
	}
	ft_bzero(path, sizeof(path));
	ft_strlcpy(path, "assets/monstre/", sizeof(path));
	ft_strlcat(path, fld_ln[0], sizeof(path));
	ft_strlcat(path, "/", sizeof(path));
	ft_strlcat(path, fld_ln[1], sizeof(path));
	if (ft_strnstr(path, ".dxpm", ft_strlen(path)))
		load_single_dxpm(eng->screen->mlx_ptr, path,
			&anim->frames[count - 1], &anim->frames[count]);
	else if (ft_strnstr(path, ".xpm", ft_strlen(path)))
		get_one_xpm(eng->screen->mlx_ptr, path, &anim->frames[count]);
	else
		return (count);
	return (count + 1);
}

static int	process_anim_line(t_engine *eng, t_mstr_anim *anim,
				char *fld_ln[2], int count)
{
	char	*space;

	space = ft_strchr(fld_ln[1], ' ');
	if (space)
		*space = '\0';
	return (load_frame_path(eng, anim, fld_ln, count));
}

int	process_anim_lines(t_engine *eng, t_mstr_anim *anim,
		const char *folder, char **lines)
{
	int		i;
	int		count;
	char	*fld_ln[2];

	count = 0;
	i = -1;
	fld_ln[0] = (char *)folder;
	while (lines[++i])
	{
		if (ft_strncmp(lines[i], "offset_x", 8) == 0)
			anim->offset.i.x = ft_atoi(lines[i] + 8);
		else if (ft_strncmp(lines[i], "offset_y", 8) == 0)
			anim->offset.i.y = ft_atoi(lines[i] + 8);
		else if (ft_strncmp(lines[i], "original_", 9) != 0
			&& lines[i][0] != '\n' && lines[i][0] != '\0'
			&& lines[i][0] != '\r' && count < 64)
		{
			fld_ln[1] = lines[i];
			count = process_anim_line(eng, anim, fld_ln, count);
		}
		free(lines[i]);
	}
	return (count);
}
