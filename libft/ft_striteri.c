/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:46:52 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/08 22:42:27 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
void	ft_switch_maj(unsigned int n, char *c)
{
	if (n % 2 != 1)
		*c -= 32;
}
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>

int	main(void)
{
	char	str[27] = "abcdefghijklmnopqrstuvwxyz";
	ft_striteri(str, &ft_switch_maj);
	printf("%s\n", str);
	return (0);
}
 */