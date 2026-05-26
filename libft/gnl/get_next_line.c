/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 22:39:06 by rstarcev          #+#    #+#             */
/*   Updated: 2026/02/18 17:25:07 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_sub_reader(char *stach, size_t *len, size_t *cap, int fd)
{
	int			nread;
	char		*buf;

	nread = 1;
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (gnl_free_stach_ret_null(stach));
	while (!gnl_have_nl(stach) && nread > 0)
	{
		nread = read(fd, buf, BUFFER_SIZE);
		if (nread < 0)
			return (free(buf), gnl_free_stach_ret_null(stach), NULL);
		if (nread == 0)
			break ;
		buf[nread] = '\0';
		if (nread + *len + 1 > *cap)
		{
			stach = gnl_extend_stach(&nread, len, cap, stach);
			if (!stach)
				return (free(buf), NULL);
		}
		gnl_copy_buf(&nread, len, stach, buf);
	}
	return (free(buf), stach);
}

char	*gnl_reader(char *stach, int fd)
{
	size_t	len;
	size_t	cap;

	len = gnl_strlen(stach);
	cap = len + 1;
	if (!stach)
	{
		cap = BUFFER_SIZE * 2;
		stach = malloc(sizeof(char) * (cap + 1));
		if (!stach)
			return (NULL);
		stach[0] = '\0';
	}
	stach = gnl_sub_reader(stach, &len, &cap, fd);
	if (!stach)
		return (NULL);
	if (stach && stach[0] == '\0')
		return (gnl_free_stach_ret_null(stach));
	return (stach);
}

char	*gnl_extract_line(char *s)
{
	int		i;
	char	*line;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
	{
		line[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
		line[i] = s[i];
	if (s[i] == '\n')
		i++;
	line[i] = '\0';
	return (line);
}

char	*gnl_trim_stach(char *s)
{
	int		i;
	int		j;
	char	*out;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\0')
		return (gnl_free_stach_ret_null(s));
	if (s[i] == '\n')
		i++;
	out = malloc(sizeof(char) * ((gnl_strlen(s) + 1) - i));
	if (!out)
		return (gnl_free_stach_ret_null(s));
	while (s[i])
		out[j++] = s[i++];
	out[j] = '\0';
	free(s);
	return (out);
}

char	*get_next_line(int fd)
{
	static char	*stach;
	char		*line;

	if (stach && fd < 0)
	{
		free(stach);
		stach = NULL;
	}
	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	stach = gnl_reader(stach, fd);
	if (!stach || (stach && stach[0] == '\0'))
	{
		free(stach);
		stach = NULL;
		return (NULL);
	}
	line = gnl_extract_line(stach);
	stach = gnl_trim_stach(stach);
	if ((stach && stach[0] == '\0') || !line)
	{
		free(stach);
		stach = NULL;
	}
	return (line);
}

/* 
int	main(int argc, char **argv)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line1;
	char	*line2;
	char	*line3;
	int		i;

	i = 0;
	if (argc != 2)
	{
		printf("Usage: %s file1\n", argv[0]);
		return (1);
	}
	fd1 = open(argv[1], O_RDONLY);
	// fd2 = open(argv[2], O_RDONLY);
	// fd3 = open(argv[3], O_RDONLY);
	while (i < 10)
	{
		line1 = get_next_line(fd1);
		// line2 = get_next_line(fd2);
		// line3 = get_next_line(fd3);
		if (!line1) //  && !line2 && !line3
			break;
		if (!line1)
			printf("F1. l%3d : %s\n", i, line1);
		if (line1)
		{
			printf("F1. l%3d : %s", i, line1);
			free(line1);
		}
		// if (!line2)
		// 	printf("F2. l%3d : %s\n", i, line2);
		// if (line2)
		// {
		// 	printf("F2. l%3d : %s", i, line2);
		// 	free(line2);
		// }
		// if (!line3)
		// 	printf("F3. l%3d : %s\n", i, line3);
		// if (line3)
		// {
		// 	printf("F3. l%3d : %s", i, line3);
		// 	free(line3);
		// }
		printf("----\n");
		i++;
	}
	close(fd1);
	get_next_line(-1);
	// close(fd2);
	// close(fd3);
	return (0);
}
 */