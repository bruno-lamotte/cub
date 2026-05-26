/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:25:53 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/08 22:40:29 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
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
 */
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
/* 
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int		size;
	char	*str;

	size = 10;
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	ft_bzero(str, (size_t)size + 1);
	str = ft_memset(str, 'A', (size_t)size);
	printf("%s\n", str);
	free(str);
	return (0);
}
 */