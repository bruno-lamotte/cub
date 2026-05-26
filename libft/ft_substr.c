/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:53:36 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/09 19:45:06 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
size_t	ft_strlen(char const *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*out;
	size_t	i;

	out = NULL;
	i = 0;
	if (!s || len == 0 || start >= ft_strlen(s))
		return (ft_strdup(""));
	while (s[i + start] && i < len)
		i++;
	out = malloc(sizeof(char) * (i + 1));
	if (!out)
		return (NULL);
	i = 0;
	while (s[i + start] && i < len)
	{
		out[i] = s[i + start];
		i++;
	}
	out[i] = '\0';
	return (out);
}
/* 
#include <stdio.h>

int	main(void)
{
	char *str;
	str = ft_substr("tripouille", 100, 1);
	printf("%s\n", str);
	if (str)
		free(str);
	return (0);
}
 */
