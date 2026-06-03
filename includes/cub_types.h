#ifndef CUB_TYPES_H
# define CUB_TYPES_H

# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <float.h>
# include <math.h>
# include <pthread.h>
# include <sys/time.h>
# include <X11/keysym.h>
# include "../mlx/mlx.h"
# include "../libft/libft.h"

# define MAX_SECTION_ITEMS 254 // uint8_t max
# define MAX_SECTION_LINE_LEN 254 // uint8_t max
# define MAX_MAP_WIDTH 65534 // uint16_t max
# define MAX_MAP_HEIGHT 65534 // uint16_t max
# define N_DEF_SECTION 7 // all the section that are not maps
# define MAX_DEBUG_LINE 1024
# define HEX_COLOR_LEN 8
# define ACCEPTED 424242
# define AUTO_ACTS_COUNT 10
# define DIST_MAX 1024
# define GAMMA 1.0f

/* Définition stricte du type virgule fixe 16.16 */
typedef int32_t t_fp;

# define FP_SHIFT 16
# define FP_SCALE 65536
# define INT_TO_FP(n) ((n) << FP_SHIFT)
# define FP_TO_INT(n) ((n) >> FP_SHIFT)
# define FLOAT_TO_FP(f) ((t_fp)((f) * FP_SCALE))
# define FP_TO_FLOAT(x) ((float)(x) / FP_SCALE)
# define DOUBLE_TO_FP(d) ((t_fp)((d) * FP_SCALE))
# define FP_MUL(a, b) ((t_fp)(((int64_t)(a) * (int64_t)(b)) >> FP_SHIFT))
# define FP_DIV(a, b) ((t_fp)(((int64_t)(a) << FP_SHIFT) / (b)))

/* Structure de vecteur unifiée avec unions */
typedef union u_vec2
{
	struct {
		double		x;
		double		y;
	} d;
	struct {
		int			x;
		int			y;
	} i;
	struct {
		uint32_t	x;
		uint32_t	y;
	} u;
	struct {
		t_fp		x;
		t_fp		y;
	} fp;
} t_vec2;

typedef struct	s_texture_def
{
	char		*name;
	char		*path;
	int32_t		color;
	bool		is_color;
}				t_texture_def;

typedef struct	s_wall_def
{
	uint8_t		tex_north;
	uint8_t		tex_west;
	uint8_t		tex_south;
	uint8_t		tex_east;
	char 		symbol;
} t_wall_def;

typedef struct	s_air_def
{
	uint8_t		tex_floor;
	uint8_t		tex_ceiling;
	char 		symbol;
}				t_air_def;

typedef struct	s_door_def
{
	uint8_t		tex_front;
	uint8_t		tex_back;
	uint8_t		tex_side;
	char 		symbol;
	bool		is_closed;
}				t_door_def;

typedef struct	s_monster_def
{
	uint8_t		hp;
	uint8_t		size_pc;
	uint8_t		tex;
	char		symbol;
}				t_monster_def;

typedef struct	s_obj_def
{
	uint8_t		tex;
	uint8_t		size_pc;
	char		symbol;
	uint8_t		pad;
}				t_obj_def;

typedef enum	e_dir
{
	DIR_NONE,
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST
}				t_dir;

typedef struct	s_player_def
{
	t_dir		dir;
	uint8_t		tex;
	char		symbol;
}				t_player_def;

typedef struct s_img
{
	bool		is_color;
	uint32_t	hex_color;
	void		*img_ptr;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
}	t_img;

typedef struct	s_data
{
	// definitions
	uint32_t		textures_section_id;
	uint8_t			textures_len;
	t_texture_def 	*textures_defs;

	uint32_t		walls_section_id;
	uint8_t			walls_len;
	t_wall_def		*walls_defs;

	uint32_t		airs_section_id;
	uint8_t			airs_len;
	t_air_def		*airs_defs;

	uint32_t		doors_section_id;
	uint8_t			doors_len;
	t_door_def 		*doors_defs;

	uint32_t		monsters_section_id;
	uint8_t			monsters_len;
	t_monster_def	*monsters_defs;

	uint32_t		objs_section_id;
	uint8_t			obj_len;
	t_obj_def		*obj_defs;

	uint32_t		players_section_id;
	uint8_t			player_len;
	t_player_def 	*player_defs;

	// player data
	uint16_t		player_row;
	uint16_t		player_col;
	t_dir			player_dir;

	// ids arrays
	char *all_ids;
	char *wall_ids;
	char *air_ids;
	char *door_ids;
	char *monster_ids;
	char *obj_ids;
	char *player_ids;

	uint16_t		door_rt_count;
	uint16_t		monster_rt_count;

	// mandatory map
	uint32_t	m_map_section_id;
	char		**m_map;
	uint16_t	m_max_width; 
	uint16_t	m_height;

	bool have_bonus;
	// bonus map
	uint32_t	b_map_section_id;
	char		**b_map;
	uint16_t	b_max_width;
	uint16_t	b_height;

	// t_img s
	t_img		*img_tab;
}				t_data;

typedef enum	e_perr
{
	PERR_NONE = 0,
	PERR_OVERFLOW,
	PERR_SEGMENT_TOO_LONG,
	PERR_INVALID_CHAR,
	PERR_UNFINISHED_LINE,
	PERR_INVALID_ID,
	PERR_SECTION_TITLE,
	PERR_OPEN_BRACE,
	PERR_CLOSE_BRACE,
	PERR_NO_CONTENT,
	PERR_MAX_CONTENT,
	PERR_UNEXPECTED_EOF,
	PERR_UNEXPECTED_EOL,
	PERR_UNEXPECTED_REST_OF_LINE,
	PERR_INVALID_HEX_COLOR,
	PERR_BAD_SEPARATOR,
	PERR_BAD_KEYWORD,
	PERR_INVALID_LINE_FORMAT,
	PERR_NO_BOOL_FOUND,
	PERR_NO_DIR_FOUND,
	INVALIDE_TEXT_TYPE,
	PERR_INVALID_PATH,
	PERR_TEXT_NOT_RECOGNIZED,
	PERR_PLAYER_NOT_FOUND,
	PERR_UNEVEN_MAPS,
	PERR_BAD_HEX_PREFIX,
	PERR_BAD_HEX_COLOR_LEN,
	PERR_BAD_FILEPATH,
	PERR_NO_TEX_MATCH,
	PERR_INVALID_PERCENT,
	PERR_TOO_LARGE_NUMBER,
	PERR_NO_NUMBER,
	PERR_MAP_SIZE_TOO_LARGE,
	PERR_REPETED_SYM,
	PERR_INVALIDE_SYMBOL,
	PERR_TOO_MANY_PLAYER,
	PERR_UNCLOSED_MAP,
	PERR_UNEXPECTED_SPACE,
	PERR_MAP_MISMATCH,
	PERR_MLX_ERROR,
	PERR_MALLOC_ERR,
	PERR_MAP_UNSOLVABLE
}				t_perr;

typedef struct	s_index
{
	size_t			line;
	size_t			col;
	t_perr			err;
}				t_index;

typedef struct		s_parsing
{
	char			**file_content;
	t_data			*data;
	t_index			idx;
}					t_parsing;

typedef struct s_keys
{
    bool w;
    bool a;
    bool s;
    bool d;
    bool left;
    bool right;
    bool e;
}   t_keys;

// Include sub-types headers
# include "cub_types_slr.h"
# include "cub_types_exec.h"

#endif
