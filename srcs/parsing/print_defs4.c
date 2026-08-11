/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_defs4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_perr(t_index *i)
{
	if (i->err == PERR_NONE)
		return ;
	printf("Error on line %zd col %zd : ", i->line + 1, i->col);
	if (print_perr1(i->err))
		return ;
	if (print_perr2(i->err))
		return ;
	if (i->err == PERR_UNEVEN_MAPS)
		printf("maps are not aligned\n");
	else if (i->err == PERR_OVERFLOW)
		printf("overflow detected\n");
	else if (i->err == PERR_BAD_HEX_PREFIX)
		printf("invalid hex prefix\n");
	else if (i->err == PERR_BAD_HEX_COLOR_LEN)
		printf("invalid hex color length\n");
	else if (i->err == PERR_BAD_FILEPATH)
		printf("invalid file path\n");
	else if (i->err == PERR_MALLOC_ERR)
		printf("malloc failed\n");
	else
		printf("unknown error\n");
}
