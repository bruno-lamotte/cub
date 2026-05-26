/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:47:14 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/09 18:07:21 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	cc;
	size_t			i;

	str = (unsigned char *)s;
	cc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == cc)
			return ((void *)(str + i));
		i++;
	}
	return (NULL);
}
/* 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	main(void)
{
	

	char str[] = {0, 1, 2 ,3 ,4 ,5};
	char c = 2;
	size_t n = 3;
	printf("%s\n", ft_memchr(str, c, n));
	printf("%p\n", ft_memchr(str, c, n));
	printf("%s\n", memchr(str, c, n));
	printf("%p\n", memchr(str, c, n));
	return (0);
}
 */