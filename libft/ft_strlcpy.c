/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:56:14 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/08 22:42:39 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
 */
size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	lensrc;

	i = 0;
	lensrc = ft_strlen(src);
	if (size == 0)
		return (lensrc);
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (lensrc);
}
/* 

// COMPILE WITH gcc ft_strlcpy.c -lbsd
// FOR TESTING OFFICIAL strlcpy

#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int		size = 25;
	char	ft_src[] = "hellohello";
	char	ft_dest[size];
	char	src[] = "hellohello";
	char	dest[size];

	printf("%zu\n", ft_strlcpy(ft_dest, ft_src, size));
	printf("%zu\n", strlcpy(dest, src, size));
	printf("%s\n", ft_dest);
	printf("%s\n", dest);
	return (0);
}
 */