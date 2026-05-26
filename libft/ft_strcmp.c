/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:41:26 by eretriha          #+#    #+#             */
/*   Updated: 2026/02/16 19:40:04 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	i = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	while (ss1[i] && ss2[i] && ss1[i] == ss2[i])
		i++;
	return (ss1[i] - ss2[i]);
}
/* 
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*s1 = "hellO";
	char	*s2 = "hello";
	size_t	n = 0;
	printf("%d\n", ft_strncmp("abc", "abc", 7));
	printf("%d\n", strncmp("abc", "abc", 7));
	return (0);
}
 */

/*
	if (n == 0 || !s1 || !s2)
		return (0);
*/