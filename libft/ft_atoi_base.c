/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:47:06 by rstarcev          #+#    #+#             */
/*   Updated: 2025/12/08 12:51:09 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_index_base(char c, char *base)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (c == base[i])
			return (i);
		i++;
	}
	return (-1);
}

static int	ft_strlen(char *base)
{
	int	len;

	len = 0;
	while (base[len])
		len++;
	return (len);
}

static int	check_base(char *base)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	if (ft_strlen(base) < 2)
		return (0);
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		if ((base[i] >= 9 && base[i] <= 13) || base[i] == 32)
			return (0);
		y = i + 1;
		while (base[y])
		{
			if (base[i] == base[y])
				return (0);
			y++;
		}
		i++;
	}
	return (1);
}

int	ft_atoi_base(char *str, char *base)
{
	int	i;
	int	sign;
	int	res;

	sign = 1;
	i = 0;
	res = 0;
	if (check_base(base) == 0)
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] && ft_index_base(str[i], base) != -1)
	{
		res = res * ft_strlen(base);
		res = res + ft_index_base(str[i], base);
		i++;
	}
	return (sign * res);
}
/* 
#include <stdio.h>

int	main(void)
{
	printf("%d\n", ft_atoi_base("f810202", "0123456789abcdef"));
	printf("%d\n", ft_atoi_base("F810202", "0123456789ABCDEF"));
	printf("%d\n", ft_atoi_base("lc", "blc"));
	return (0);
}
 */
/*
int	check_base(char *base)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		i++;
	}
	if (i < 2)
		return (0);
	i = 0;
	while (base[i])
	{
		y = i + 1;
		while (base[y])
		{
			if (base[i] == base[y])
				return (0);
			y++;
		}
		i++;
	}
	return (1);
}
*/