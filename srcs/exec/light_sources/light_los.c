#include "cub.h"
#include <math.h>

static int	check_los_shadow(double mx, double my, double px, double py)
{
	double	fx;
	double	fy;

	fx = px - (int)px;
	fy = py - (int)py;
	if (fabs(fx - 0.99) < 1e-9 && mx >= (int)px + 1)
		return (0);
	if (fabs(fx - 0.01) < 1e-9 && mx <= (int)px)
		return (0);
	if (fabs(fy - 0.99) < 1e-9 && my >= (int)py + 1)
		return (0);
	if (fabs(fy - 0.01) < 1e-9 && my <= (int)py)
		return (0);
	return (1);
}

static void	init_los_steps(double mx, double my, double d[4], double side[2],
				int step[4])
{
	step[2] = (int)mx;
	step[3] = (int)my;
	if (d[0] < 0)
	{
		step[0] = -1;
		side[0] = (mx - step[2]) * d[2];
	}
	else
	{
		step[0] = 1;
		side[0] = (step[2] + 1.0 - mx) * d[2];
	}
	if (d[1] < 0)
	{
		step[1] = -1;
		side[1] = (my - step[3]) * d[3];
	}
	else
	{
		step[1] = 1;
		side[1] = (step[3] + 1.0 - my) * d[3];
	}
}

static void	init_los_dda(double mx, double my, double px, double py,
				double d[4], double side[2], int step[4], int target[2])
{
	d[0] = px - mx;
	d[1] = py - my;
	if (d[0] == 0)
		d[2] = 1e30;
	else
		d[2] = fabs(1.0 / d[0]);
	if (d[1] == 0)
		d[3] = 1e30;
	else
		d[3] = fabs(1.0 / d[1]);
	target[0] = (int)px;
	target[1] = (int)py;
	init_los_steps(mx, my, d, side, step);
}

static void	dda_step(double d[4], double side[2], int step[4])
{
	if (side[0] < side[1])
	{
		side[0] += d[2];
		step[2] += step[0];
	}
	else
	{
		side[1] += d[3];
		step[3] += step[1];
	}
}

int	check_los(t_vec2 p1, t_vec2 p2, void *blob)
{
	double	d[4];
	double	side[2];
	int		step[4];
	int		target[2];

	if (!check_los_shadow(p1.d.x, p1.d.y, p2.d.x, p2.d.y))
		return (0);
	init_los_dda(p1.d.x, p1.d.y, p2.d.x, p2.d.y, d, side, step, target);
	while (step[2] != target[0] || step[3] != target[1])
	{
		dda_step(d, side, step);
		if (step[2] == target[0] && step[3] == target[1])
			break ;
		if (!is_walkable(step[2], step[3], blob))
			return (0);
	}
	return (1);
}
