#include "cub.h"
#include <stdio.h>
#include <stdlib.h>

int	main(int ac, char **av)
{
	t_data	*data;
	int		res;

	if (ac != 2)
	{
		printf("Usage: ./solver <map_path.cub>\n");
		return (2);
	}
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (2);
	if (!parse_file(av[1], data))
	{
		free_preprocessing_data(data);
		free(data);
		return (2);
	}
	res = is_map_solvable(data);
	free_preprocessing_data(data);
	free(data);
	if (res == 1)
		return (0);
	if (res == -1)
		return (2);
	return (1);
}
