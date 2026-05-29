#include "cub.h"
#include <unistd.h>
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

void	fill_fps_str(char *str, int fps)
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

void	display_fps(t_engine *eng)
{
	struct timeval			now;
	char					str[24];

	gettimeofday(&now, NULL);
	if (eng->fps_last.tv_sec != 0)
	{
		eng->fps_time_acc += (now.tv_sec - eng->fps_last.tv_sec) * 1000000L
			+ (now.tv_usec - eng->fps_last.tv_usec);
		if (++eng->fps_frames && eng->fps_time_acc >= 500000L)
		{
			eng->fps_val = (int)(eng->fps_frames * 1000000.0 / eng->fps_time_acc);
			eng->fps_frames = 0;
			eng->fps_time_acc = 0;
		}
	}
	eng->fps_last = now;
	fill_fps_str(str, eng->fps_val);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		10, 20, 0xFFFFFF, str);
}
