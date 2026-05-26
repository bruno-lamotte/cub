#include "../../includes/cub.h"

int16_t get_block_id(char sym, t_data *d, t_block_type type)
{
	int16_t i;
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
	while(arr[i])
	{
		if (arr[i] == sym)
			return(i);
		i++;
	}
	return (-1);
}

int16_t get_def_id(char *ids, char sym)
{
	int16_t i;

	i = 0;
	while (ids[i])
	{
		if (ids[i] == sym)
			return (i);
		i++;
	}
	return (-1);
}