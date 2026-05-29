#include "cub.h"

static void	update_monster_job(void *arg, t_worker *w)
{
	t_mstr_job	*job;

	job = (t_mstr_job *)arg;
	update_single_monster(job->m, job->eng, w);
}

static void	queue_monster_jobs(t_engine *eng, t_monster_rt *mstr,
				t_mstr_job *jobs)
{
	int	i;

	i = -1;
	while (++i < eng->data->monster_rt_count)
	{
		if (!(mstr[i].flags & MONSTER_DEAD))
		{
			jobs[i].m = &mstr[i];
			jobs[i].self_idx = i;
			jobs[i].eng = eng;
			submit_job(&eng->pool, update_monster_job, &jobs[i]);
		}
	}
	wait_for_jobs(&eng->pool);
}

void	update_monsters(t_engine *eng)
{
	t_monster_rt	*mstr;
	t_mstr_job		jobs[64];
	static int		frame = 0;

	if (eng->data->monster_rt_count <= 0)
		return ;
	mstr = get_monster_rt(eng->blob);
	if (!mstr[0].has_guard_pos)
		init_monster_targets(eng);
	queue_monster_jobs(eng, mstr, jobs);
	update_alarm_disarm(eng);
	frame++;
	if (frame % MSTR_DEBUG_PRINT_FREQ == 0)
		print_monsters_debug(mstr, eng->data->monster_rt_count);
}
