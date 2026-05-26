/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:22:56 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/08 22:41:57 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t count)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)ptr;
	while (i < count)
	{
		str[i] = value;
		i++;
	}
	return (ptr);
}
/*
let's see if we need to put an \0 char because
real memset func is getting a context error with valgrind
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

int	main(void)
{
	char	c;
	int		size;
	char	*str;

	c = 'A';
	size = 10;
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str = ft_memset(str, 'A', (size_t)size);
	ft_putstr(str);
	free(str);
	return (0);
}
*/