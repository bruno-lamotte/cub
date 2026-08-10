#include "cub.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int	load_frame_path(t_engine *eng, t_mstr_anim *anim,
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

static int	process_anim_lines(t_engine *eng, t_mstr_anim *anim,
				const char *folder, char **lines)
{
	int		i;
	int		count;
	char	*space;
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
			space = ft_strchr(lines[i], ' ');
			if (space)
				*space = '\0';
			fld_ln[1] = lines[i];
			count = load_frame_path(eng, anim, fld_ln, count);
		}
		free(lines[i]);
	}
	return (count);
}

static int	load_anim_sequence(t_engine *eng, t_mstr_anim *anim,
				const char *folder, const char *anim_file)
{
	int		fd;
	char	*lines[257];
	int		line_count;
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
	anim->frame_count = process_anim_lines(eng, anim, folder, lines);
	return (anim->frame_count);
}

void	load_monster_anim(t_engine *eng)
{
	printf("COURSE: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_COURSE], "course", "course.anim"));
	printf("PUNCH: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_PUNCH], "punch", "punch.anim"));
	printf("FACE: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_FACE], "face", "face.anim"));
	printf("18FACE: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_18FACE], "18face", "18face.anim"));
	printf("PROFIL: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_PROFIL], "profil", "profil.anim"));
	printf("18DOS: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_18DOS], "18dos", "18dos.anim"));
	printf("DOS: %d frames\n", load_anim_sequence(eng,
			&eng->anims[MSTR_ANIM_DOS], "dos", "dos.anim"));
}
