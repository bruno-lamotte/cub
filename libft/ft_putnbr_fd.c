/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 21:48:12 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/09 04:10:31 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
 */
void	ft_putnbr_fd(int n, int fd)
{
	int			i;
	int long	nb;

	i = 0;
	nb = (int long) n;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
		i++;
	}
	if (nb >= 10)
		ft_putnbr_fd(nb / 10, fd);
	ft_putchar_fd((nb % 10) + '0', fd);
}
/* 
#include <limits.h>
#include <stdio.h>

int	main(void)
{
	int	val;
	val = INT_MIN;
	ft_putnbr_fd(val, 1);
	printf("\n%d\n", val);
	return (0);
}
 */