#ifndef CUB_H
# define CUB_H
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <float.h>
# include <math.h>
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

# define ACCEPTED 424242

struct s_rule;

typedef struct s_item
{
	int				id;
	struct s_rule	*rule_of_item;
	int				dot_pos;
}				t_item;

typedef struct s_symbol
{
	char			*name;
	int				nbr;
	t_list			*firsts;
	t_list			*follows;
}				t_symbol;

typedef struct s_rule
{
	int				id;
	char			*left_symbol;
	t_list			*right_symbols;
	int				nb_items;
}				t_rule;

typedef struct s_state
{
	int				id;
	t_list			*items;
	t_list			*transitions;
}				t_state;

typedef struct s_transition
{
	char			*symbol;
	t_state			*dest_state;
}				t_transition;

struct s_engine;
struct s_monster_rt;
typedef void (*t_behavior_fn)(struct s_monster_rt *, struct s_engine *);

#define AUTO_ACTS_COUNT 10

typedef struct s_slr1
{
	t_list			*rules;
	t_list			*symbols;
	t_list			*states;
	int				**table;
	t_behavior_fn	*state_behaviors;
	int				auto_act_nbrs[AUTO_ACTS_COUNT];
	int				stim_alarm_heard_nbr;
	int				stim_alarm_off_nbr;
}				t_slr1;

typedef struct s_reduce_rule
{
	char			*left_symbol;
	int				nb_items;
}				t_reduce_rule;

//get_map.c
char **get_file(char *filename);
void	free_str_tab(char **tab);
void	print_str_tab(char **tab);

bool parse_file(char *filename, t_data *d);

// parsing_utils.c
bool is_char_fmt(char c);
bool is_space_fmt(char c);
bool skip_space_tab(char *s, t_index *i, bool expect_eol);
bool skip_char_fmt(char *s, t_index *i);
bool skip_separator(char *s, t_index *i, char sep);
bool skip_keyword(char *s, t_index *i, char *keyword);
bool is_hex_digit(char c);
bool isalpha_up(int c);
bool skip_hex_color(char *s, t_index *i, int32_t *dest);
bool skip_file_path(char *s, t_index *i, char **dest);
bool skip_tex_name(char *s, t_index *i, char **dest);
bool skip_and_get_symbol(char *s, t_index *i, char *dest);
ssize_t find_tex_from_str(char *s, size_t n, t_parsing *p);
bool	skip_uint16_value(char *s, t_index *i, uint16_t *out);
bool	skip_uint8_value(char *s, t_index *i, uint8_t *out);
bool	skip_percent_value(char *s, t_index *i, uint8_t *out);

// CHECK SECTION
bool check_texture_section(t_parsing *p, size_t len);
bool check_wall_section(t_parsing *p, size_t len);
bool check_air_section(t_parsing *p, size_t len);
bool check_door_section(t_parsing *p, size_t len);
bool check_monster_section(t_parsing *p, size_t len);
bool check_object_section(t_parsing *p, size_t len);
bool check_player_section(t_parsing *p, size_t len);
bool check_mandatory_map_section(t_parsing *p, size_t len);
bool check_bonus_map_section(t_parsing *p, size_t len);

bool	check_s(char *n, bool (*f)(t_parsing *, size_t), t_parsing *p, size_t l);

bool validate_map(t_data *d, t_index *i);
bool is_map_solvable(t_data *d);

// check IDS
bool get_ids(t_parsing *p);

// parsing_free_def.c
void free_ids(t_data *d);
void free_textures_def(t_texture_def **tex, size_t len);
void free_all_def(t_data *d);
void free_preprocessing_data(t_data *d);

// parsing_print_def.c
void print_parsing(t_data *d);
void print_textures_def(t_texture_def *tex, size_t len);
void print_walls_def(t_wall_def *w, size_t len, t_texture_def *tex);
void print_airs_def(t_air_def *a, size_t len, t_texture_def *tex);
void print_doors_def(t_door_def *d, size_t len, t_texture_def *tex);
void print_monsters_def(t_monster_def *m, size_t len, t_texture_def *tex);
void print_objects_def(t_obj_def *o, size_t len, t_texture_def *tex);
void print_players_def(t_player_def *o, size_t len, t_texture_def *tex);
void print_perr(t_index *i);

/*SYSTEM*/

void safe_free(void **ptr);

/*MLX*/

extern int g_win_width;
extern int g_win_height;
# define WINDOW_WIDTH g_win_width
# define WINDOW_HEIGHT g_win_height

typedef struct s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	t_img		img2;
}	t_screen;

void free_screen(t_screen *s);
bool	init_mlx(t_screen *screen, char *title);
void free_img_tab(void *mlx, t_img *img, size_t lim);
bool get_all_tex(t_data *data, t_screen *s);
void draw_tex_lib(t_screen *s, t_data *d);
/*BLOB*/

#define DIST_MAX 1024
#define GAMMA 1.0f

typedef struct s_lut
{
    float shade_table[DIST_MAX];
}   t_lut;

// helper struct

typedef struct	s_xy_double
{
	double		x;
	double		y;
}				t_xy_double;

typedef struct	s_xy_uint32
{
	uint32_t	x;
	uint32_t	y;
}				t_xy_uint32;

/*RUNTIME STRUCT*/

typedef struct	s_world
{
	void *blob_raw;

}				t_world;


typedef struct	s_player_rt
{
	t_xy_double	pos;
	t_xy_double	dir;
	t_xy_double	plane;
	uint8_t		tex_id;
}				t_player_rt;

/*BLOB*/

typedef struct	s_blob_seg // size = 16 // 4 unit on 1 cache line
{
	uint32_t	start;
	uint32_t	end;
	uint32_t	used;
	union {
		struct {
			uint16_t width;
			uint16_t height;
		} map_data;
		uint32_t	count;
	};
}				t_blob_seg;

typedef struct  __attribute__((aligned(64))) s_blob_hdr // size = 192 // 1 unit on 3 cache line
{
	t_blob_seg	map;
	t_blob_seg	door_rt;
	t_blob_seg	mstr_rt;
	t_blob_seg	wall_def;

	t_blob_seg	air_def;
	t_blob_seg	door_def;
	t_blob_seg	mstr_def;
	t_blob_seg	obj_def;

	t_blob_seg	tex_def;
	t_blob_seg	pixels_data;
	t_blob_seg  lut_def;
	uint32_t	end_offset;
	uint32_t	pad[3]; //si mes calculs sont bons
}				t_blob_hdr;



/*
put flag 	flags |= CELL_HAS_DOOR;
rm flag  	flags &= ~CELL_HAS_DOOR;
test flag	if (flags & CELL_HAS_DOOR)
*/

# define CELL_HAS_WALL			(1u << 0)
# define CELL_HAS_AIR			(1u << 1)
# define CELL_HAS_DOOR			(1u << 2)
# define CELL_HAS_OBJ			(1u << 3)
# define CELL_HAS_MSTR			(1u << 4)
# define MAP_SOA_LAYERS			3

typedef struct	s_map_soa
{
	uint8_t		*flags;
	uint8_t		*block_id; // wall and air
	uint8_t		*occ_id; // door and obj
}				t_map_soa;

# define MONSTER_MOVE			(1u << 0)
# define MONSTER_ATTACK			(1u << 1)
# define MONSTER_DEAD			(1u << 2)
# define MONSTER_HAS_PLAYER_POS	(1u << 3)
# define MONSTER_IS_ACTIVE		(1u << 4)

# define MSTR_STATE_IDLE		0
# define MSTR_STATE_PATROL		1
# define MSTR_STATE_CHASE		2
# define MSTR_STATE_ATTACK		3
# define MSTR_STATE_SCAN		4
# define MSTR_STATE_RELOC		5
# define MSTR_STATE_ALARM		6

typedef struct	s_monster_rt
{
	uint8_t		flags;
	uint8_t		def_id;
	uint8_t		current_hp;
	uint8_t		state;
	t_xy_double	pos;
	t_xy_double	dir;
	t_xy_double	last_player_pos;
	uint32_t	map_id;
	uint32_t	alert_timer;
	double		guard_x;
	double		guard_y;
	int			has_guard_pos;
	int			state_stack[32];
	int			state_stack_top;
}				t_monster_rt;

# define DOOR_CLOSING			(1u << 0)
# define DOOR_OPENING			(1u << 1)
# define DOOR_IS_OPEN			(1u << 2)
# define DOOR_HAD_PLAYER_NEAR	(1u << 3)
# define DOOR_BLOCKED			(1u << 4)


typedef struct	s_door_rt
{
	uint8_t		flags;
	uint8_t		def_id;
	uint8_t		open_ratio_255;
	uint8_t		pad;
	uint32_t	map_id;
}				t_door_rt;

/*DEFS*/

/*if off_px_from_blob == 0 then color hex_code*/
// typedef struct	s_bdef_tex
// {
// 	uint16_t	width;
// 	uint16_t	height;
// 	uint32_t	size;
// 	uint32_t	off_px_from_blob;
// 	uint32_t	color;
// }				t_bdef_tex;

typedef struct	s_bdef_wall
{
	uint8_t tex_north;
	uint8_t tex_south;
	uint8_t tex_west;
	uint8_t tex_east;
}				t_bdef_wall;

typedef struct	s_bdef_air
{
	uint8_t tex_floor;
	uint8_t tex_ceiling;
}				t_bdef_air;

typedef struct	s_bdef_door
{
	uint8_t tex_front;
	uint8_t tex_back;
	uint8_t tex_side;
	uint8_t	padding;
}				t_bdef_door;

typedef struct	s_bdef_monster
{
	uint8_t	tex;
	uint8_t	initial_hp;
	uint8_t size_pc;
	uint8_t	padding;
}				t_bdef_monster;

typedef struct	s_bdef_obj
{
	uint8_t	tex;
	uint8_t size_pc;
}				t_bdef_obj;

// if width == 0 and height == 0 then get color
typedef struct	s_bdef_tex
{
	uint16_t	width;
	uint16_t	height;
	union
	{
		uint32_t	offset;
		uint32_t	color;
	};
}				t_bdef_tex;

/* typedef struct	s_blob
{
	t_blob_hdr	*header;

	uint8_t		wall_count;
	t_bdef_wall	*wall_def;

	uint8_t		air_count;
	t_bdef_air	*air_def;

	uint8_t		door_count;
	t_bdef_air	*air_def;
}				t_blob; */

/*BUILD BLOB*/

typedef struct s_tmp_header
{
	size_t	header_size;
	
	size_t	door_rt_size;
	size_t	monster_rt_size;
	size_t	map_soa_size;

	size_t	wall_size;
	size_t	air_size;
	size_t	door_size;
	size_t	monster_size;
	size_t	obj_size;
	size_t	tex_size;
	size_t	px_size;

	size_t	lut_size;
}	t_tmp_header;

typedef enum	e_block_type
{
	T_WALL,
	T_AIR,
	T_DOOR,
	T_OBJ,
	T_MSTR,
	T_PLAYER
}				t_block_type;

typedef struct	s_tmp_cell
{
	size_t l;
	size_t c;
	size_t soa_idx;
	size_t mstr_idx;
	size_t door_idx;
	char m;
	char b;
}				t_tmp_cell;

/* Définition stricte du type virgule fixe 16.16 */
typedef int32_t t_fp;

#define FP_SHIFT 16
#define FP_SCALE 65536
#define INT_TO_FP(n) ((n) << FP_SHIFT)
#define FP_TO_INT(n) ((n) >> FP_SHIFT)
#define FLOAT_TO_FP(f) ((t_fp)((f) * FP_SCALE))
#define FP_TO_FLOAT(x) ((float)(x) / FP_SCALE)

#define DOUBLE_TO_FP(d) ((t_fp)((d) * FP_SCALE))

#define FP_MUL(a, b) ((t_fp)(((int64_t)(a) * (int64_t)(b)) >> FP_SHIFT))
#define FP_DIV(a, b) ((t_fp)(((int64_t)(a) << FP_SHIFT) / (b)))

typedef struct s_vec2_fp
{
	t_fp	x;
	t_fp	y;
}	t_vec2_fp;

typedef struct s_vec2_i
{
	int	x;
	int	y;
}	t_vec2_i; // __attribute__((packed));

#include <pthread.h>

int get_num_threads(void);
#define NUM_THREADS get_num_threads()

typedef struct s_ray_data
{
    t_fp    perp_wall_dist; // Distance perpendiculaire pour éviter le fisheye (16.16)
    int     side;
    int     hit_type;
    int     map_x;
    int     map_y;
    t_fp    wall_x;
    t_vec2_fp ray_dir;
}   		t_ray_data;

typedef struct s_worker
{
    int             id;
    int             start_x;
    int             end_x;
    t_screen        *screen;
    void            *blob;
    t_player_rt     *player;
    t_ray_data      *z_buffer;
    void            *engine;
    int             last_rendered_frame;
}                   t_worker;

typedef struct s_thread_pool
{
    pthread_t       threads[64];
    t_worker        workers[64];
    pthread_mutex_t mutex;
    pthread_cond_t  render_cond;
    pthread_cond_t  main_cond;
    int             active_workers;
    int             workers_count;
    int             current_frame;
    bool            shutdown;
}                   t_thread_pool;

typedef struct s_draw
{
    int         x;
    int         line_height;
    int         draw_start;
    int         draw_end;
    t_bdef_tex  tex;
    uint32_t    *pixels;
    int         tex_x;
    t_fp        step;
    t_fp        tex_pos;
}   t_draw;

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

typedef struct s_light
{
	double	x;
	double	y;
	float	intensity;
	float	radius;
	int		is_alarm;
	int		is_active;
	int		is_triggered;
	float	angle;
	int		check_timer;
}	t_light;

typedef struct s_interact_obj
{
	int		is_door;
	int		idx;
	double	x;
	double	y;
}				t_interact_obj;

typedef struct s_engine
{
	t_screen		*screen;
	void			*blob;
	t_player_rt		*player;
	t_keys			keys;
	t_ray_data		*z_buffer;
	t_data			*data;
	t_thread_pool	pool;
	t_light			static_lights[64];
	int				static_light_count;
	int				alarm_triggered;
	int				hacking_timer;
	double			hacking_x;
	double			hacking_y;
	t_slr1			*slr;
	int				terminal_mode;
	int				selected_obj_idx;
	t_interact_obj	interact_objs[64];
	int				interact_obj_count;
}   t_engine;

bool get_alloc_size(t_tmp_header *hdr, size_t *out);
bool build_blob(t_data *d, void **blob);

bool fill_tmp_hdr(t_data *d, t_tmp_header *hdr);
bool build_hdr(t_data *d, void *blob, t_tmp_header *tmp);

void build_rt(t_data *d, void *blob);
void build_bdef(t_data *d, void *blob);
void build_px(t_data *d, void *blob);

void print_blob(void *blob);

// build_helpers
int16_t get_block_id(char sym, t_data *d, t_block_type type);
int16_t get_def_id(char *ids, char sym);


// blob_helpers
t_blob_hdr     *get_blob_hdr(void *blob);

uint16_t        get_map_height(t_blob_hdr *hdr);
uint16_t        get_map_width(t_blob_hdr *hdr);
uint16_t        get_map_size(t_blob_hdr *hdr);

uint8_t        *get_map_flags(void *blob);
uint8_t        *get_map_block_ids(void *blob);
uint8_t        *get_map_occ_ids(void *blob);

t_map_soa       get_map_soa(void *blob);

t_monster_rt   *get_monster_rt(void *blob);
t_door_rt	   *get_door_rt(void *blob);

t_bdef_wall    *get_wall_bdef(void *blob);
t_bdef_air     *get_air_bdef(void *blob);
t_bdef_door    *get_door_bdef(void *blob);
t_bdef_monster *get_mstr_bdef(void *blob);
t_bdef_obj     *get_obj_bdef(void *blob);
t_bdef_tex		*get_tex_bdef(void *blob);
t_lut			*get_lut_bdef(void *blob);

// srcs/exec/raycasting.c
void    cast_ray(int x, t_player_rt *p, void *blob, t_ray_data *out);

// srcs/exec/render.c
void    render_frame(t_engine *engine);
void    init_thread_pool(t_engine *engine);
void    shutdown_thread_pool(t_engine *engine);
void    put_pixel(t_img *img, int x, int y, int color);
unsigned int blend_colors(unsigned int src, unsigned int dst, unsigned char alpha);
void    draw_minimap(t_engine *engine);
void		draw_minimap_monsters(t_engine *eng, t_vec2_i c, int radius,
			double px_per_tile);
void		draw_minimap_player(t_engine *eng, t_vec2_i c);


// srcs/exec/draw_column.c
uint8_t get_wall_tex_id(t_ray_data *ray, t_vec2_fp *ray_dir, void *blob);
void    init_projection(t_draw *d, t_ray_data *ray);
void    render_pixels(t_draw *d, t_worker *w, t_ray_data *ray, t_lut *lut);
void    draw_column(int x, t_worker *w, t_ray_data *ray);

// srcs/exec/vector_arithmetic.c
void	rotate_vec_fp(t_vec2_fp *v, t_fp fp_cos, t_fp fp_sin);
float	fast_inv_sqrt(float number);
double	get_door_ratio(int mx, int my, int width, void *blob);
int		is_door_horiz(int mx, int my, int w, void *blob);
int		check_door_hit(t_ray_data *ray, t_player_rt *p, void *blob);

// srcs/exec/player.c
t_player_rt	*init_player_rt(t_data *data);

// srcs/exec/movement.c
int			is_walkable(int x, int y, int width, int height, void *blob);
int			is_valid_position(double x, double y, int width, int height,
				void *blob);
void		update_position(t_engine *eng, t_keys *keys);
void		update_rotation(t_player_rt *p, t_keys *keys);

// srcs/exec/doors.c
void		update_doors(t_engine *engine);

// srcs/exec/controls.c
int			key_press(int keycode, t_engine *engine);
int			key_release(int keycode, t_engine *engine);
int			close_window(t_engine *engine);
int			game_loop(t_engine *engine);

// srcs/exec/light_sources.c
void		init_static_lights(t_engine *engine);
int			check_los(double mx, double my, double px, double py, void *blob);
float		compute_light_at_point(double wx, double wy, t_ray_data *ray, t_lut *lut, void *blob, t_engine *eng);
float		get_alarm_light_at_point(double wx, double wy, void *blob, t_engine *eng);
void		update_global_alarm_state(t_engine *eng);

// srcs/exec/monsters_ai.c
void		update_monsters(t_engine *engine);
void		init_all_ai_behaviors(t_slr1 *slr, t_engine *eng);

// srcs/exec/monsters_render.c
void		draw_monsters_3d(t_engine *eng);
void		draw_objects_3d(t_engine *eng);

/* ************************************************************************** */
/*                               slr_parser.c                                 */
/* ************************************************************************** */

t_item			*create_new_item(t_rule *rule, int dot_pos);
t_symbol		*get_symbol_from_list(t_slr1 *data, t_list *right_symbols,
					int dot_pos);
t_symbol		*get_symbol_after_dot(t_slr1 *data, t_item *item);
t_symbol		*get_non_terminal_symbol_after_dot(t_slr1 *data, t_item *item);
int				does_state_contains_this_item(t_list *items, t_item *item);
void			closure(t_slr1 *data, t_state *state);

void			free_states(t_list *states);
void			free_symbols(t_list *symbols);
void			free_rules(t_list *rules);
void			free_table(int **table);
void			free_all(t_slr1 *data, int **table);

int				does_list_contains_this_symbol(t_list *list, char *name);
int				add_firsts_if_not_token(t_slr1 *data, t_rule *next_rule,
					t_symbol *left_symbol);
int				add_first_if_token(t_symbol *left_symbol,
					t_symbol *right_symbol);
int				should_look_for_next_right_symbol(t_symbol *left_symbol,
					t_rule *rule);
void			get_first_dfs(t_slr1 *data, t_rule *rule, t_symbol *left_symbol,
					t_symbol *right_symbol);
void			get_firsts(t_slr1 *data);

void			add_dollar_to_start_symbol(t_slr1 *data);
int				add_follows_from_follows(t_symbol **symbol,
					t_symbol *next_symbol);
int				add_follows_from_firsts(t_symbol **symbol,
					t_symbol *next_symbol);
int				contains_empty_in_firsts(t_symbol *symbol);
void			update_follows(t_slr1 *data, t_list *current_rule, int *added);
void			get_follows(t_slr1 *data);

void			get_leftsymbol_from_grammar(t_slr1 *data, t_rule **new_rule,
					char *line);
void			get_id_from_grammar(t_slr1 *data, t_rule **new_rule);
void			get_rightsymbols_from_grammar(t_rule **new_rule, char *line);
void			get_nbitems_from_grammar(t_rule **new_rule);
void			get_rule_from_grammar(t_slr1 *data, t_rule **new_rule,
					char *line);
int				parse_grammar(t_slr1 *data);

t_list			*get_rule_from_symbolname(t_slr1 *data, char *symbol);
t_symbol		*get_symbol_from_name(t_slr1 *data, char *left_symbol);
int				symbol_is_token(char *name);
void			complete_symbol_name(t_symbol **symbol, char *name);
int				symbol_already_parsed(t_list *symbols, char *left_symbol);
void			add_symbol_if_not_exists(t_slr1 *data, char *name, int *nbr);
void			get_symbols(t_slr1 *data);

int				get_transition_from_symbol(t_state *state, char *symbol_name);
void			fill_table_when_reduce(t_slr1 *data, int ***table,
					t_list *state, t_list *item);
void			fill_parsing_table(t_slr1 *data, int ***table);
t_state			*initialize_first_state(t_slr1 *data);
void			get_states(t_slr1 *data);
void			initialize_data(t_slr1 *data);
int				**create_parsing_table(t_slr1 *data);
int				slr_main(void);

void			free_tmp_state(t_state *state);
t_transition	*create_transition(char *symbol_name, t_state *dest_state);
t_state			*create_new_state(t_slr1 *data);
void			add_item_to_list(t_state **new_state, t_item *item);
int				are_items_equal(t_item *i1, t_item *i2);
t_state			*find_state(t_slr1 *data, t_state *new_state);
void			add_transition_to_state(t_slr1 *data, t_state *current_state,
					t_state *new_state, t_symbol *symbol);
void			get_new_state(t_slr1 *data, t_state *current_state,
					t_state *new_state, t_symbol *target_symbol);
void			go_to(t_slr1 *data, t_state *state);

void			print_table(int **table, t_slr1 *data);
void			print_table_in_c(int **table, int nb_states, int nb_tokens);
void			print_rules_in_c(t_list *rules_list);
void			print_symbols_in_c(t_list *symbols_list);
#endif