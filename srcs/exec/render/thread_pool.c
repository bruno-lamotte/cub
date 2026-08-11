/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <pthread.h>
#include <stdlib.h>

void	worker_render_slice(void *arg, t_worker *w)
{
	t_worker	*worker;
	int			x;

	(void)w;
	worker = (t_worker *)arg;
	x = worker->start_x;
	while (x < worker->end_x)
	{
		cast_ray(worker, x, &worker->z_buffer[x]);
		draw_column(x, worker, &worker->z_buffer[x]);
		x++;
	}
	render_floor_slice(worker);
}

void	execute_worker_job(t_thread_pool *pool, t_job job, t_worker *w)
{
	job.func(job.arg, w);
	pthread_mutex_lock(&pool->mutex);
	pool->active_jobs--;
	if (pool->active_jobs == 0)
		pthread_cond_signal(&pool->working_cond);
}

void	*pool_worker_thread(void *arg)
{
	t_worker		*w;
	t_thread_pool	*pool;
	t_job			job;

	w = (t_worker *)arg;
	pool = &((t_engine *)w->engine)->pool;
	pthread_mutex_lock(&pool->mutex);
	while (1)
	{
		while (pool->jobs_count == 0 && !pool->shutdown)
			pthread_cond_wait(&pool->job_cond, &pool->mutex);
		if (pool->shutdown)
			break ;
		job = pool->jobs[pool->job_tail];
		pool->job_tail = (pool->job_tail + 1) % JOB_QUEUE_SIZE;
		pool->jobs_count--;
		pthread_mutex_unlock(&pool->mutex);
		execute_worker_job(pool, job, w);
	}
	pthread_mutex_unlock(&pool->mutex);
	return (NULL);
}

static void	init_worker(t_engine *eng, t_thread_pool *pool, int i,
				int slice_width)
{
	int	map_size;

	map_size = get_map_width(get_blob_hdr(eng->blob))
		* get_map_height(get_blob_hdr(eng->blob));
	pool->workers[i].id = i;
	pool->workers[i].start_x = i * slice_width;
	pool->workers[i].end_x = (i + 1) * slice_width;
	if (i == pool->workers_count - 1)
		pool->workers[i].end_x = eng->screen->win_width;
	pool->workers[i].screen = eng->screen;
	pool->workers[i].blob = eng->blob;
	pool->workers[i].player = eng->player;
	pool->workers[i].z_buffer = eng->z_buffer;
	pool->workers[i].engine = eng;
	pool->workers[i].last_rendered_frame = 0;
	pool->workers[i].bfs_visited = malloc(sizeof(int) * map_size);
	pool->workers[i].bfs_queue = malloc(sizeof(int) * map_size);
	pool->workers[i].bfs_parent = malloc(sizeof(int) * map_size);
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
	pthread_cond_init(&pool->job_cond, NULL);
	pthread_cond_init(&pool->working_cond, NULL);
	pool->shutdown = false;
	pool->job_head = 0;
	pool->job_tail = 0;
	pool->jobs_count = 0;
	pool->active_jobs = 0;
	pool->workers_count = get_num_threads();
	slice_width = engine->screen->win_width / pool->workers_count;
	i = -1;
	while (++i < pool->workers_count)
		init_worker(engine, pool, i, slice_width);
}
