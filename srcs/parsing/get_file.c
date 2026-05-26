#include "../../includes/cub.h"

void	free_str_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while(tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return ;
}

void	print_str_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while(tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
	return ;
}

ssize_t get_file_size(char *filename)
{
	ssize_t	count;
	int		fd;
	char	*line;

	count = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	line = get_next_line(fd);
	while(line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

char **get_file(char *filename)
{
	ssize_t 	map_size;
	ssize_t		i;
	int			fd;
	char		**out;

	map_size = get_file_size(filename);
	if (map_size < 0)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	out = malloc((sizeof(char *) * (map_size + 1)));
	if (!out)
		return (close(fd), NULL);
	i = 0;
	while(i < map_size)
	{
		out[i] = get_next_line(fd);
		if (!out[i])
			return (close(fd), get_next_line(-1), free_str_tab(out), NULL);
		i++;
	}
	out[i] = NULL;
	close(fd);
	return (out);
}

/* int main(int ac, char **av)
{
	char **out;

	out = get_map(av[1]);
	if (!out)
		return (1);
	print_str_tab(out);
	free_str_tab(out);
	return (0);
} */