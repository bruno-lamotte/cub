/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

t_fp	fp_mul(t_fp a, t_fp b)
{
	return ((t_fp)(((int64_t)a * (int64_t)b) >> FP_SHIFT));
}

t_fp	fp_div(t_fp a, t_fp b)
{
	return ((t_fp)(((int64_t)a << FP_SHIFT) / b));
}
