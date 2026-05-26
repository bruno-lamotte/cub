/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:55:43 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 17:33:07 by rstarcev         ###   ########.fr       */
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
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	size_t	total;
	char	*str;

	total = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (total + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}
/* 
#include <stdio.h>

int	main(void)
{
	char *str = ft_strjoin("hello", NULL);
	printf("%s\n", str);
	free(str);
	return (0);
}
 */