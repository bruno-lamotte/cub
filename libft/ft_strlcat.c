/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:03:21 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/08 22:42:35 by rstarcev         ###   ########.fr       */
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
size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	i = 0;
	if (size == 0)
		return (src_len);
	while (src[i] && dest_len + i < size - 1)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	if (size > dest_len)
	{
		dest[dest_len + i] = '\0';
		return (dest_len + src_len);
	}
	return (src_len + size);
}
/* 
#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>

int	main(void)
{
	char	src[11] = "hellohello";
	char	dest[2000] = "aia";
	char	osrc[11] = "hellohello";
	char	odest[2000] = "aia";

	printf("%zu\n", ft_strlcat(dest, src, 100));
	printf("%s\n", dest);
	printf("%zu\n", strlcat(odest, osrc, 100));
	printf("%s\n", odest);
	return (0);
}
 */