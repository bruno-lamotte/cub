/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:46:22 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/09 02:14:38 by rstarcev         ###   ########.fr       */
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
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;

	s = (unsigned char *) src;
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
/* 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

int	main(void)
{
	char	src[20] = "Hello World!";
	char	dest[20] = "jjjjjjjjjjjjjjjjjjj";
	char	dest2[20] = "jjjjjjjjjjjjjjjjjjj";

	ft_memcpy(dest, src, 10);
	printf("%s\n", dest);
	memcpy(dest2, src, 10);
	printf("%s\n", dest2);
	return (0);
}
 */