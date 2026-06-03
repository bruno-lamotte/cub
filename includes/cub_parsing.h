#ifndef CUB_PARSING_H
# define CUB_PARSING_H

# include "cub_types.h"

// get_map.c
char	**get_file(char *filename);
void	free_str_tab(char **tab);
void	print_str_tab(char **tab);

bool	parse_file(char *filename, t_data *d);

// parsing_utils.c (utils.c)
bool	is_char_fmt(char c);
bool	is_space_fmt(char c);
bool	skip_space_tab(char *s, t_index *i, bool expect_eol);
bool	skip_char_fmt(char *s, t_index *i);
bool	skip_separator(char *s, t_index *i, char sep);
bool	skip_keyword(char *s, t_index *i, char *keyword);
bool	is_hex_digit(char c);
bool	isalpha_up(int c);
bool	skip_hex_color(char *s, t_index *i, int32_t *dest);
bool	skip_file_path(char *s, t_index *i, char **dest);
bool	skip_tex_name(char *s, t_index *i, char **dest);
bool	skip_and_get_symbol(char *s, t_index *i, char *dest);
ssize_t	find_tex_from_str(char *s, size_t n, t_parsing *p);
bool	skip_uint16_value(char *s, t_index *i, uint16_t *out);
bool	skip_uint8_value(char *s, t_index *i, uint8_t *out);
bool	skip_percent_value(char *s, t_index *i, uint8_t *out);

// CHECK SECTION
bool	check_texture_section(t_parsing *p, size_t len);
bool	check_wall_section(t_parsing *p, size_t len);
bool	check_air_section(t_parsing *p, size_t len);
bool	check_door_section(t_parsing *p, size_t len);
bool	check_monster_section(t_parsing *p, size_t len);
bool	check_object_section(t_parsing *p, size_t len);
bool	check_player_section(t_parsing *p, size_t len);
bool	check_mandatory_map_section(t_parsing *p, size_t len);
bool	check_bonus_map_section(t_parsing *p, size_t len);

bool	check_s(char *n, bool (*f)(t_parsing *, size_t), t_parsing *p, size_t l);

bool	validate_map(t_data *d, t_index *i);
bool	is_map_solvable(t_data *d);

// check IDS
bool	get_ids(t_parsing *p);

// parsing_free_def.c (free_def.c)
void	free_ids(t_data *d);
void	free_textures_def(t_texture_def **tex, size_t len);
void	free_all_def(t_data *d);
void	free_preprocessing_data(t_data *d);

// parsing_print_def.c (print_defs.c)
void	print_parsing(t_data *d);
void	print_textures_def(t_texture_def *tex, size_t len);
void	print_walls_def(t_wall_def *w, size_t len, t_texture_def *tex);
void	print_airs_def(t_air_def *a, size_t len, t_texture_def *tex);
void	print_doors_def(t_door_def *d, size_t len, t_texture_def *tex);
void	print_monsters_def(t_monster_def *m, size_t len, t_texture_def *tex);
void	print_objects_def(t_obj_def *o, size_t len, t_texture_def *tex);
void	print_players_def(t_player_def *o, size_t len, t_texture_def *tex);
void	print_perr(t_index *i);

// system / general utils
void	safe_free(void **ptr);

// mlx helpers
void	free_screen(t_screen *s);
bool	init_mlx(t_screen *screen, char *title);
void	free_img_tab(void *mlx, t_img *img, size_t lim);
bool	get_all_tex(t_data *data, t_screen *s);
void	draw_tex_lib(t_screen *s, t_data *d, int i);

typedef struct	s_tmp_map_data
{
	char		**map;
	char		**bonus;
	char		*wall_sym;
	char		*air_sym;
	char		*others_sym;
	bool		**mask;
	size_t		c;
	size_t		r;
	t_perr		err;
}				t_tmp_map_data;

#endif
