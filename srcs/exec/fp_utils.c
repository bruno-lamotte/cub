/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

t_fp	int_to_fp(int n)
{
	return (n << FP_SHIFT);
}

int	fp_to_int(t_fp n)
{
	return (n >> FP_SHIFT);
}

t_fp	float_to_fp(float f)
{
	return ((t_fp)(f * FP_SCALE));
}

float	fp_to_float(t_fp x)
{
	return ((float)x / FP_SCALE);
}

t_fp	double_to_fp(double d)
{
	return ((t_fp)(d * FP_SCALE));
}
