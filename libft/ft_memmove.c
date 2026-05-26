/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:47:25 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/10 20:41:25 by rstarcev         ###   ########.fr       */
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

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;

	s = (unsigned char *)src;
	if (!src && !dest)
		return (NULL);
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			ft_memset((dest + i), s[i], 1);
			i++;
		}
	}
	else if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			ft_memset((dest + i - 1), s[i - 1], 1);
			i--;
		}
	}
	return (dest);
}
 */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	*d;

	i = 0;
	s = (unsigned char *)src;
	d = (unsigned char *)dest;
	if ((!dest && !src) && n)
		return (NULL);
	if (src < dest)
	{
		while (n > 0)
		{
			ft_memset((d + n - 1), s[n - 1], 1);
			n--;
		}
		return (d);
	}
	while (i < n)
	{
		ft_memset((d + i), s[i], 1);
		i++;
	}
	return (d);
}
/* 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	main(void)
{
	char *str = NULL;
	// char	str2[20] = "0123456789";
	// char	str2o[20] = "0123456789";

	// char str1[20] = "ABCDEFGHIJ";
	// char str1o[20] = "ABCDEFGHIJ";
	// memmove(str2o + 2, str2o, 5);
	// ft_memmove(str2 + 2, str2, 5);
	printf("%p\n", memmove((void *) 0, (void *) 0, 0));
	printf("%p\n", ft_memmove((void *) 0, (void *) 0, 0));
	// printf("O %s\n", str2o);
	// printf("M %s\n", str2);
	return (0);
}
 */