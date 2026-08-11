/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_anim_loader2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <fcntl.h>
#include <unistd.h>

int	process_anim_lines(t_engine *eng, t_mstr_anim *anim,
		const char *folder, char **lines);

static int	read_anim_fd(int fd, char **lines)
{
	int	line_count;

	line_count = 0;
	while (line_count < 256)
	{
		lines[line_count] = get_next_line(fd);
		if (!lines[line_count])
			break ;
		line_count++;
	}
	lines[line_count] = NULL;
	close(fd);
	return (line_count);
}

static int	load_anim_sequence(t_engine *eng, t_mstr_anim *anim,
				const char *folder, const char *anim_file)
{
	int		fd;
	char	*lines[257];
	char	path[512];

	anim->offset.i.x = 0;
	anim->offset.i.y = 0;
	ft_bzero(path, sizeof(path));
	ft_strlcpy(path, "assets/monstre/", sizeof(path));
	ft_strlcat(path, folder, sizeof(path));
	ft_strlcat(path, "/", sizeof(path));
	ft_strlcat(path, anim_file, sizeof(path));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	read_anim_fd(fd, lines);
	anim->frame_count = process_anim_lines(eng, anim, folder, lines);
	return (anim->frame_count);
}

void	load_monster_anim(t_engine *eng)
{
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_COURSE],
		"course", "course.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_PUNCH],
		"punch", "punch.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_FACE],
		"face", "face.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_18FACE],
		"18face", "18face.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_PROFIL],
		"profil", "profil.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_18DOS],
		"18dos", "18dos.anim");
	load_anim_sequence(eng, &eng->anims[MSTR_ANIM_DOS],
		"dos", "dos.anim");
}
