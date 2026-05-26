/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:06:50 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 20:33:09 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_get_alloc_len(char const *s1, char const *set)
{
	int	len;
	int	i;
	int	y;

	len = ft_strlen(s1);
	i = 0;
	y = len - 1;
	while (is_set(s1[i], set))
		i++;
	if (i == len)
		return (0);
	while (is_set(s1[y], set))
	{
		i++;
		y--;
	}
	len -= i;
	return (len);
}

static int	ft_begin_last_set(char const *s1, char const *set)
{
	int	len;
	int	i;
	int	y;

	len = ft_strlen(s1);
	i = 0;
	y = len - 1;
	while (is_set(s1[y], set))
	{
		i++;
		y--;
	}
	len -= i;
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*out;
	int		i;
	int		y;
	int		n;
	int		alloc_len;

	i = 0;
	y = 0;
	n = ft_begin_last_set(s1, set);
	alloc_len = ft_get_alloc_len(s1, set);
	out = malloc(sizeof(char) * (alloc_len + 1));
	if (!out)
		return (NULL);
	while (is_set(s1[i], set))
		i++;
	while (y < n - i)
	{
		out[y] = s1[i + y];
		y++;
	}
	out[y] = '\0';
	return (out);
}
/* 
#include <stdio.h>

int	main(void)
{
	char *str = " bonjour bonjour ";
	char *set = " ";
	char	*dest;
	// printf("%d\n", ft_get_alloc_len(str, set));
	// printf("%d\n", ft_begin_last_set(str, set));
	dest = ft_strtrim(str, " bonjour");
	if (!dest)
		return (0);
	ft_putstr_fd(dest, 1);
	printf("\n%p\n", str);

	free(dest);
	return (0);
}
 */