/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 23:27:36 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/13 17:44:11 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_printf_putnbr_fd(int n, int fd)
{
	int			i;
	int long	nb;
	int			count;

	count = 0;
	i = 0;
	nb = (int long)n;
	if (nb < 0)
	{
		count += ft_printf_putchar_fd('-', fd);
		nb = -nb;
		i++;
	}
	if (nb >= 10)
		count += ft_printf_putnbr_fd(nb / 10, fd);
	count += ft_printf_putchar_fd((nb % 10) + '0', fd);
	return (count);
}

int	ft_printf_putstr_fd(char *str, int fd)
{
	int	count;

	if (!str)
		return (ft_printf_putstr_fd("(null)", fd));
	count = write(fd, str, ft_printf_strlen(str));
	return (count);
}

int	ft_printf_putnbr_base_fd(unsigned long long nb, char *base, int fd)
{
	int					count;
	unsigned long long	base_len;

	count = 0;
	base_len = (unsigned long long)ft_printf_strlen(base);
	if (nb >= base_len)
		count += ft_printf_putnbr_base_fd(nb / base_len, base, fd);
	count += ft_printf_putchar_fd(base[nb % base_len], fd);
	return (count);
}

int	ft_printf_put_addr(va_list *ap, int fd, char *lower_base)
{
	int					count;
	unsigned long long	ptr;

	count = 0;
	ptr = (uint64_t)va_arg(*ap, unsigned long int);
	if (ptr == 0)
		count += ft_printf_putstr_fd("(nil)", fd);
	else
	{
		count += ft_printf_putstr_fd("0x", fd);
		count += ft_printf_putnbr_base_fd(ptr, lower_base, fd);
	}
	return (count);
}
