/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:08:02 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 18:05:00 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
void	*ft_memset(void *ptr, int value, size_t count)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)ptr;
	while (i < count)
	{
		str[i] = value;
		i++;
	}
	return (ptr);
}
 */

void	*ft_calloc(size_t n, size_t size)
{
	void	*out;

	if (n <= 0 || size <= 0)
	{
		out = malloc(0);
		if (!out)
			return (NULL);
		return (out);
	}
	if ((size_t)-1 / n < size)
		return (NULL);
	out = malloc(n * size);
	if (!out)
		return (NULL);
	ft_memset(out, 0, n * size);
	return (out);
}
/* 
void	*ft_calloc(size_t n, size_t size)
{
	void	*out;

	if ((size_t)-1 / n < size)
		return (NULL);
	if (n == 0 || size == 0)
		return (malloc(1));
	out = malloc(n * size);
	if (!out)
		return (NULL);
	ft_memset(out, 0, n * size);
	return (out);
}
 */
/* 
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	main(void)
{
	// size_t		n;
	// size_t		size;
	// void		*ptr;

	// n = 2;
	// size = sizeof(int);
	// ptr = calloc(n, size);
	// write(1, ptr, n * size);
	// free(ptr);
	// ptr = NULL;
	// ft_putchar('\n');
	// ptr = ft_calloc(n, size);
	// write(1, ptr, n * size);
	// free(ptr);

	printf("%zu\n", (size_t)INT_MIN);
	
	// printf("%ld\n", (size_t)-1 / 2);
	return (0);
}
 */