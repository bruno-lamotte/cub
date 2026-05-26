/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:12:21 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/09 05:53:57 by rstarcev         ###   ########.fr       */
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
char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	cc;

	i = 0;
	cc = (unsigned char) c;
	if (cc == '\0')
		return ((char *)&s[ft_strlen(s)]);
	while (s[i])
	{
		if (s[i] == cc)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
/* 
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*str;
	char 	*str2;

	str2 = strchr("comment ca va mon pote", '\0' + 256);
	str = ft_strchr("comment ca va mon pote", '\0' + 256);
	printf("%p\n", str);
	printf("%p\n", str2);
	return (0);
}
 */