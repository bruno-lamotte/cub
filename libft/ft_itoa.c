/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:18:06 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 15:33:26 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int nbr)
{
	long int	n;
	int			count;

	n = (long int)nbr;
	count = 1;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n / 10 > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static char	*put_buff(char *buffer, int i, long int nb)
{
	buffer[i] = '\0';
	while (nb > 0)
	{
		buffer[--i] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (buffer);
}

char	*ft_itoa(int n)
{
	long int	nb;
	int			sign;
	char		*buffer;
	int			i;

	sign = 1;
	nb = (long int)n;
	if (n == 0)
		return (ft_strdup("0"));
	i = int_len(n);
	if (nb < 0)
		sign = -sign;
	if (sign < 0)
		nb = -nb;
	buffer = malloc(sizeof(char) * (i + 1));
	if (!buffer)
		return (NULL);
	if (sign < 0)
		buffer[0] = '-';
	buffer = put_buff(buffer, i, nb);
	return (buffer);
}
/* 
#include <stdio.h>
#include <limits.h>

int	main(void)
{
	int nb = 10;
	char *str = ft_itoa(nb);
	printf("%p\n", str);
	printf("----------------\n");
	printf("%s\n", str);
	printf("%d\n", nb);
	free(str);
	return (0);
}
 */