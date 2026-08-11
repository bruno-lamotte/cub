/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <pthread.h>
#include <stdlib.h>

void	shutdown_thread_pool(t_engine *engine)
{
	t_thread_pool	*pool;
	int				i;

	if (!engine)
		return ;
	pool = &engine->pool;
	pthread_mutex_lock(&pool->mutex);
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->job_cond);
	pthread_mutex_unlock(&pool->mutex);
	i = 0;
	while (i < pool->workers_count)
	{
		pthread_join(pool->threads[i], NULL);
		free(pool->workers[i].bfs_visited);
		free(pool->workers[i].bfs_queue);
		free(pool->workers[i].bfs_parent);
		i++;
	}
	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->job_cond);
	pthread_cond_destroy(&pool->working_cond);
}

void	submit_job(t_thread_pool *pool,
			void (*func)(void *, t_worker *), void *arg)
{
	pthread_mutex_lock(&pool->mutex);
	pool->jobs[pool->job_head].func = func;
	pool->jobs[pool->job_head].arg = arg;
	pool->job_head = (pool->job_head + 1) % JOB_QUEUE_SIZE;
	pool->jobs_count++;
	pool->active_jobs++;
	pthread_cond_signal(&pool->job_cond);
	pthread_mutex_unlock(&pool->mutex);
}

void	wait_for_jobs(t_thread_pool *pool)
{
	pthread_mutex_lock(&pool->mutex);
	while (pool->active_jobs > 0)
		pthread_cond_wait(&pool->working_cond, &pool->mutex);
	pthread_mutex_unlock(&pool->mutex);
}
