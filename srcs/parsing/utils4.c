/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	skip_uint8_value(char *s, t_index *i, uint8_t *out)
{
	uint16_t	val;
	uint16_t	digit;

	if (!s[i->col] || !ft_isdigit((unsigned char)s[i->col]))
		return (i->err = PERR_NO_NUMBER, false);
	val = 0;
	while (s[i->col] && ft_isdigit((unsigned char)s[i->col]))
	{
		digit = (uint16_t)(s[i->col] - '0');
		if (val > (UINT8_MAX - digit) / 10)
			return (i->err = PERR_TOO_LARGE_NUMBER, false);
		val = val * 10 + digit;
		i->col++;
	}
	*out = (uint8_t)val;
	return (true);
}

bool	skip_percent_value(char *s, t_index *i, uint8_t *out)
{
	if (!skip_uint8_value(s, i, out))
		return (false);
	if (*out > 100)
		return (i->err = PERR_TOO_LARGE_NUMBER, false);
	if (s[i->col] != '%')
		return (i->err = PERR_INVALID_PERCENT, false);
	i->col++;
	return (true);
}
