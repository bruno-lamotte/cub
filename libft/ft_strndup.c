/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:40:37 by rstarcev          #+#    #+#             */
/*   Updated: 2026/01/15 22:29:15 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
 */
char	*ft_strndup(char *src, size_t n)
{
	char	*dest;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(src);
	if (len > n)
		len = n;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/* 
#include <stdio.h>
#include <string.h>

int	main(int ac, char **av)
{
	char	*str;

	if (ac != 2)
		return (0);
	str = ft_strdup(av[1]);
	printf("%s\n", str);
	free(str);
	str = NULL;
	str = strdup(av[1]);
	printf("%s\n", str);
	free(str);
	return (0);
}
 */