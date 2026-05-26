#include "cub.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int	get_num_threads(void)
{
	static int	num_threads;
	long		nprocs;

	if (num_threads == 0)
	{
		nprocs = sysconf(_SC_NPROCESSORS_ONLN);
		if (nprocs > 0)
			num_threads = (int)nprocs;
		else
			num_threads = 8;
		if (num_threads > 64)
			num_threads = 64;
	}
	return (num_threads);
}

static void	worker_render_slice(t_worker *w)
{
	int	x;

	x = w->start_x;
	while (x < w->end_x)
	{
		cast_ray(x, w->player, w->blob, &w->z_buffer[x]);
		draw_column(x, w, &w->z_buffer[x]);
		x++;
	}
}

void	*pool_worker_thread(void *arg)
{
	t_worker		*w;
	t_thread_pool	*pool;

	w = (t_worker *)arg;
	pool = &((t_engine *)w->engine)->pool;
	pthread_mutex_lock(&pool->mutex);
	while (1)
	{
		while (w->last_rendered_frame == pool->current_frame && !pool->shutdown)
			pthread_cond_wait(&pool->render_cond, &pool->mutex);
		if (pool->shutdown)
			break ;
		pthread_mutex_unlock(&pool->mutex);
		worker_render_slice(w);
		pthread_mutex_lock(&pool->mutex);
		w->last_rendered_frame = pool->current_frame;
		pool->active_workers--;
		if (pool->active_workers == 0)
			pthread_cond_signal(&pool->main_cond);
	}
	pthread_mutex_unlock(&pool->mutex);
	return (NULL);
}

static void	init_worker(t_engine *eng, t_thread_pool *pool, int i,
				int slice_width)
{
	pool->workers[i].id = i;
	pool->workers[i].start_x = i * slice_width;
	pool->workers[i].end_x = (i + 1) * slice_width;
	if (i == pool->workers_count - 1)
		pool->workers[i].end_x = WINDOW_WIDTH;
	pool->workers[i].screen = eng->screen;
	pool->workers[i].blob = eng->blob;
	pool->workers[i].player = eng->player;
	pool->workers[i].z_buffer = eng->z_buffer;
	pool->workers[i].engine = eng;
	pool->workers[i].last_rendered_frame = 0;
	pthread_create(&pool->threads[i], NULL, pool_worker_thread,
		&pool->workers[i]);
}

void	init_thread_pool(t_engine *engine)
{
	t_thread_pool	*pool;
	int				slice_width;
	int				i;

	pool = &engine->pool;
	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->render_cond, NULL);
	pthread_cond_init(&pool->main_cond, NULL);
	pool->shutdown = false;
	pool->current_frame = 0;
	pool->active_workers = 0;
	pool->workers_count = get_num_threads();
	slice_width = WINDOW_WIDTH / pool->workers_count;
	i = -1;
	while (++i < pool->workers_count)
		init_worker(engine, pool, i, slice_width);
}

void	shutdown_thread_pool(t_engine *engine)
{
	t_thread_pool	*pool;
	int				i;

	if (!engine)
		return ;
	pool = &engine->pool;
	pthread_mutex_lock(&pool->mutex);
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->render_cond);
	pthread_mutex_unlock(&pool->mutex);
	i = 0;
	while (i < pool->workers_count)
	{
		pthread_join(pool->threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->render_cond);
	pthread_cond_destroy(&pool->main_cond);
}

static void	fill_fps_str(char *str, int fps)
{
	str[0] = 'F';
	str[1] = 'P';
	str[2] = 'S';
	str[3] = ':';
	str[4] = ' ';
	if (fps >= 100)
	{
		str[5] = '0' + (fps / 100);
		str[6] = '0' + ((fps / 10) % 10);
		str[7] = '0' + (fps % 10);
		str[8] = '\0';
	}
	else
	{
		str[5] = '0' + (fps / 10);
		str[6] = '0' + (fps % 10);
		str[7] = '\0';
	}
}

static struct timeval	g_last;
static int				g_fps;
static int				g_frames;
static long				g_time_acc;

static void	display_fps(t_engine *eng)
{
	struct timeval			now;
	char					str[24];

	gettimeofday(&now, NULL);
	if (g_last.tv_sec != 0)
	{
		g_time_acc += (now.tv_sec - g_last.tv_sec) * 1000000L
			+ (now.tv_usec - g_last.tv_usec);
		if (++g_frames && g_time_acc >= 500000L)
		{
			g_fps = (int)(g_frames * 1000000.0 / g_time_acc);
			g_frames = 0;
			g_time_acc = 0;
		}
	}
	g_last = now;
	fill_fps_str(str, g_fps);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		10, 20, 0xFFFFFF, str);
}

static void	draw_rect_blend(t_img *img, int x, int y, int w,
				int h, int color)
{
	int				i;
	int				j;
	char			*dst_ptr;
	unsigned int	orig;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (x + j >= 0 && x + j < WINDOW_WIDTH
				&& y + i >= 0 && y + i < WINDOW_HEIGHT)
			{
				dst_ptr = img->addr + ((y + i) * img->line_len
						+ (x + j) * (img->bpp / 8));
				orig = *(unsigned int *)dst_ptr;
				*(unsigned int *)dst_ptr = blend_colors(color, orig, 180);
			}
			j++;
		}
		i++;
	}
}

static void	fill_pct_str(char *str, int pct)
{
	int	i;
	int	bars;

	ft_memcpy(str, "HACK: [", 7);
	bars = (pct * 15) / 100;
	i = -1;
	while (++i < 15)
	{
		if (i < bars)
			str[7 + i] = '=';
		else
			str[7 + i] = '.';
	}
	str[22] = ']';
	str[23] = ' ';
	str[24] = '0' + (pct / 100);
	str[25] = '0' + ((pct / 10) % 10);
	str[26] = '0' + (pct % 10);
	str[27] = '%';
	str[28] = '\0';
}

static void	draw_hacking_overlay(t_engine *eng)
{
	int	x;
	int	y;

	if (eng->hacking_timer <= 0)
		return ;
	x = (WINDOW_WIDTH - 400) / 2;
	y = (WINDOW_HEIGHT - 120) / 2;
	draw_rect_blend(&eng->screen->img2, x, y, 400, 120, 0x000000);
}

static int	check_terminal_dist(t_engine *eng, int x, int y)
{
	double	dx;
	double	dy;

	dx = eng->player->pos.x - (x + 0.5);
	dy = eng->player->pos.y - (y + 0.5);
	return (dx * dx + dy * dy < 2.25);
}

static int	is_near_terminal(t_engine *eng)
{
	uint8_t	*flags;
	uint8_t	*occ;
	int		x;
	int		y;
	int		w;

	flags = get_map_flags(eng->blob);
	occ = get_map_occ_ids(eng->blob);
	w = get_map_width(get_blob_hdr(eng->blob));
	y = -1;
	while (++y < get_map_height(get_blob_hdr(eng->blob)))
	{
		x = -1;
		while (++x < w)
		{
			if (flags[y * w + x] & CELL_HAS_OBJ
				&& eng->data->obj_defs[occ[y * w + x]].symbol == 'T')
			{
				if (check_terminal_dist(eng, x, y))
					return (1);
			}
		}
	}
	return (0);
}

static void	draw_hacking_prompt(t_engine *eng)
{
	if (!is_near_terminal(eng))
		return ;
	if (eng->alarm_triggered)
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
			WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 100,
			0xFF0000, "[E] PRESS E TO DEACTIVATE ALARM");
	else
		mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
			WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 100,
			0x00FF00, "[E] PRESS E TO DECRYPT TERMINAL");
}

static void	draw_hacking_text(t_engine *eng)
{
	int		x;
	int		y;
	int		pct;
	char	str[32];

	if (eng->hacking_timer <= 0)
		return (draw_hacking_prompt(eng));
	x = (WINDOW_WIDTH - 400) / 2;
	y = (WINDOW_HEIGHT - 120) / 2;
	pct = (90 - eng->hacking_timer) * 100 / 90;
	fill_pct_str(str, pct);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 35, 0x00FF00, ">>> DECRYPTING CYBER SYSTEM LINK <<<");
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 65, 0x00FF00, str);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		x + 30, y + 95, 0xFF0000, "SYS WARNING: MONSTERS REDIRECTING...");
}

static void	draw_all_entities(t_engine *engine)
{
	draw_monsters_3d(engine);
	draw_objects_3d(engine);
	draw_minimap(engine);
	draw_hacking_overlay(engine);
}

void	render_frame(t_engine *engine)
{
	t_thread_pool	*pool;
	t_screen		*screen;
	t_img			temp;

	pool = &engine->pool;
	screen = engine->screen;
	pthread_mutex_lock(&pool->mutex);
	pool->current_frame++;
	pool->active_workers = pool->workers_count;
	pthread_cond_broadcast(&pool->render_cond);
	while (pool->active_workers > 0)
		pthread_cond_wait(&pool->main_cond, &pool->mutex);
	pthread_mutex_unlock(&pool->mutex);
	temp = screen->img;
	screen->img = screen->img2;
	screen->img2 = temp;
	draw_all_entities(engine);
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr,
		screen->img2.img_ptr, 0, 0);
	draw_hacking_text(engine);
	display_fps(engine);
	mlx_do_sync(screen->mlx_ptr);
}

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

unsigned int	blend_colors(unsigned int src, unsigned int dst,
					unsigned char alpha)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	inv_alpha;

	inv_alpha = 255 - alpha;
	r = ((src >> 16 & 0xFF) * alpha + (dst >> 16 & 0xFF) * inv_alpha) >> 8;
	g = ((src >> 8 & 0xFF) * alpha + (dst >> 8 & 0xFF) * inv_alpha) >> 8;
	b = ((src & 0xFF) * alpha + (dst & 0xFF) * inv_alpha) >> 8;
	return (r << 16 | g << 8 | b);
}