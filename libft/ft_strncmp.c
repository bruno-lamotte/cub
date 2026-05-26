/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:41:26 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/10 20:43:52 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	i = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (ss1[i] && ss2[i] && i < n - 1 && ss1[i] == ss2[i])
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