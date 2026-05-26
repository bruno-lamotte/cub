/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:42:28 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/08 22:42:45 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
char	ft_switch_maj(unsigned int n, char c)
{
	if (n % 2 != 1)
		c -= 32;
	return (c);
}
 *//* 
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
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t		i;
	char		*str;

	i = 0;
	str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/* 
#include <stdio.h>

int	main(void)
{
	char	*s;
	char	*str;

	s = "abcdefghijklmnopqrstuvwxyz";
	str = ft_strmapi(s, &ft_switch_maj);
	printf("%s\n", str);
	free(str);
	return (0);
}
 */