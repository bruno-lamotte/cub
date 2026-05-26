#include "../../includes/cub.h"

// with t_parsing build t_data and stop at error with parsing error

bool check_filename(char *filename)
{
	int len;

	len = 0;
	if (!filename)
		return (false);
	len = ft_strlen(filename);
	if (len <= 4)
		return (false);
	if (ft_strcmp(filename + len - 4, ".cub"))
		return (false);
	return (true);
}

size_t count_doors_rt(t_data *d)
{
	size_t count;
	char **map;
	size_t i;
	size_t y;

	map = d->m_map;
	count = 0;
	i = 0;
	while (i < d->m_height)
	{
		y = 0;
		while(map[i][y]) // y < d->m_max_width
		{
			if (ft_isset(map[i][y], d->door_ids))
				count++;
			y++;
		}
		i++;
	}
	return (count);
}

size_t count_monster_rt(t_data *d)
{
	size_t count;
	char **map;
	size_t i;
	size_t y;

	map = d->m_map;
	count = 0;
	i = 0;
	while (i < d->m_height)
	{
		y = 0;
		while(map[i][y]) // y < d->m_max_width
		{
			if (ft_isset(map[i][y], d->monster_ids))
				count++;
			y++;
		}
		i++;
	}
	return (count);
}

bool parse_file(char *filename, t_data *d)
{
	t_parsing p;

	// check name
	if (!check_filename(filename))
		return (false); // put_error
	// get file
	p.file_content = get_file(filename);
	if (!p.file_content)
		return (false);  // put_error
	// init data
	p.data = d;
	ft_bzero(&p.idx, sizeof(t_index));
	// check sections
	if (!check_section("TEXTURES\n", check_texture_section, &p))
		return (free_str_tab(p.file_content), free(p.data), print_perr(&p.idx), false);
	if (!check_section("WALLS\n", check_wall_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("AIRS\n", check_air_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("DOORS\n", check_door_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("MONSTERS\n", check_monster_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("OBJECTS\n", check_object_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("PLAYERS\n", check_player_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	if (!check_section("MANDATORY_MAP\n", check_mandatory_map_section, &p))
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	// check for bonus map
	if (p.file_content[p.idx.line] && !ft_strcmp("BONUS_MAP\n", p.file_content[p.idx.line]))
		p.data->have_bonus = true;
	if (p.data->have_bonus && !check_section("BONUS_MAP\n", check_bonus_map_section, &p) && p.idx.err != PERR_NONE)
		return (free_str_tab(p.file_content), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	// fill id arrays
	if (!get_ids(&p))
		return (free_str_tab(p.file_content), free_ids(p.data), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	// check map
	if (!validate_map(d, &p.idx))
		return (free_str_tab(p.file_content), free_ids(p.data), free_all_def(p.data), free(p.data), print_perr(&p.idx), false);
	p.data->door_rt_count = (uint16_t)count_doors_rt(p.data);
	p.data->monster_rt_count = (uint16_t)count_monster_rt(p.data);
	// free zone
	free_str_tab(p.file_content);
	return (true);
}