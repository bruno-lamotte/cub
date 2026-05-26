/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:25:37 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 15:38:36 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
 */
int	ft_atoi(const char *s)
{
	int	sign;
	int	res;
	int	i;

	sign = 1;
	res = 0;
	i = 0;
	while ((s[i] && s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] && (s[i] == '+' || s[i] == '-'))
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	while (s[i] && ft_isdigit(s[i]))
	{
		res = (res * 10) + (s[i] - '0');
		i++;
	}
	return (res * sign);
}
/*
#include <stdio.h>

int	main(void)
{
	char *val = "2147483647";
	printf("%d\n", ft_atoi(val));
	printf("%d\n", atoi(val));
	return (0);
}
 */