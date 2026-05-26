/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:40:56 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/09 04:24:35 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		if (big[i] == little[j])
		{
			while (little[j] && big[i + j] == little[j] && i + j < len)
				j++;
			if (little[j] == '\0')
				return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
/* 
// COMPILE WITH : cc ft_strnstr.c -lbsd 

#include <stdio.h>
#include <bsd/string.h>

int	main(int ac, char **av)
{
	char *big = "bonjour mon pote a la compote comment ca va ?";
	char *little = "ca";
	size_t len = 100;

	printf("%s\n", ft_strnstr(big, little, len));
	printf("%p\n", ft_strnstr(big, little, len));
	printf("%s\n", strnstr(big, little, len));
	printf("%p\n", strnstr(big, little, len));

	return (0);
}
 */