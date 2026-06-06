#ifndef CUB_TYPES_EXEC_H
# define CUB_TYPES_EXEC_H

typedef struct s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	t_img		img2;
	int			win_width;
	int			win_height;
}	t_screen;

typedef struct s_lut
{
    float shade_table[DIST_MAX];
}   t_lut;

typedef struct	s_world
{
	void *blob_raw;
}				t_world;

typedef struct	s_player_rt
{
	t_vec2		pos;
	t_vec2		dir;
	t_vec2		plane;
	uint8_t		tex_id;
}				t_player_rt;

typedef struct	s_blob_seg
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

typedef struct  __attribute__((aligned(64))) s_blob_hdr
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
	uint32_t	pad[3];
}				t_blob_hdr;

# define CELL_HAS_WALL			(1u << 0)
# define CELL_HAS_AIR			(1u << 1)
# define CELL_HAS_DOOR			(1u << 2)
# define CELL_HAS_OBJ			(1u << 3)
# define CELL_HAS_MSTR			(1u << 4)
# define MAP_SOA_LAYERS			3

typedef struct	s_map_soa
{
	uint8_t		*flags;
	uint8_t		*block_id;
	uint8_t		*occ_id;
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
	t_vec2		pos;
	t_vec2		dir;
	t_vec2		last_player_pos;
	uint32_t	map_id;
	uint32_t	alert_timer;
	t_vec2		guard;
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

typedef struct s_ray_data
{
    t_fp    perp_wall_dist;
    int     side;
    int     hit_type;
    int     map_x;
    int     map_y;
    t_fp    wall_x;
    t_vec2  ray_dir;
    int     draw_end;
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
    int             *bfs_visited;
    int             *bfs_queue;
    int             *bfs_parent;
}                   t_worker;

# define JOB_QUEUE_SIZE 1024

typedef struct s_job
{
    void            (*func)(void *, struct s_worker *);
    void            *arg;
}                   t_job;

struct s_engine;

typedef struct s_mstr_job
{
    t_monster_rt    *m;
    int             self_idx;
    struct s_engine *eng;
}                   t_mstr_job;

typedef struct s_thread_pool
{
    pthread_t       threads[64];
    t_worker        workers[64];
    pthread_mutex_t mutex;
    pthread_cond_t  job_cond;
    pthread_cond_t  working_cond;
    t_job           jobs[JOB_QUEUE_SIZE];
    int             job_head;
    int             job_tail;
    int             jobs_count;
    int             active_jobs;
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

typedef struct s_sprite
{
	t_vec2	pos;
	double	dist;
	int		is_monster;
	int		m_idx;
	char	sym;
	t_vec2	grid;
	double	shade;
}				t_sprite;

typedef struct s_light
{
	float	x;
	float	y;
	float	intensity;
	float	radius;
	float	radius_sq;
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
	t_vec2	pos;
}				t_interact_obj;

// ret values
#define CUB_EXIT_ERROR 1
#define CUB_EXIT_QUIT 0
#define CUB_EXIT_MENU 2

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



	t_img			mstr_frames[128];
	int				mstr_frame_count;
	// Variables FPS (pour supprimer les globales statiques)
	struct timeval	fps_last;
	int				fps_val;
	int				fps_frames;
	long			fps_time_acc;
	int				ret_val;
}   t_engine;

typedef struct s_Aengine
{
	t_screen		screen;
	void			*blob;
	t_player_rt	player;
	t_data		data;
	t_slr1		slr;
}			t_Aengine;

typedef struct s_bfs
{
	int		*parent;
	int		*queue;
	int		tail;
	int		width;
	int		height;
	void	*blob;
}				t_bfs;

typedef struct s_floor_row
{
	double	p0;
	double	fx;
	double	fy;
	double	step_x;
	double	step_y;
	float	ambient_shade;
	int		y;
}				t_floor_row;

#endif
