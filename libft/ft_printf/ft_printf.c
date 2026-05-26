/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 22:59:38 by rstarcev          #+#    #+#             */
/*   Updated: 2026/01/10 22:54:55 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_sub_printer(t_node *current, va_list *ap, int count, int fd)
{
	char				*l;
	char				*u;
	char				*t;

	l = "0123456789abcdef";
	u = "0123456789ABCDEF";
	t = "0123456789";
	if (current->spec == 'c')
		count += ft_printf_putchar_fd((char)va_arg(*ap, int), fd);
	else if (current->spec == 's')
		count += ft_printf_putstr_fd(va_arg(*ap, char *), fd);
	else if (current->spec == 'p')
		count += ft_printf_put_addr(ap, fd, l);
	else if (current->spec == 'd' || current->spec == 'i')
		count += ft_printf_putnbr_fd(va_arg(*ap, int), fd);
	else if (current->spec == 'u')
	count += ft_printf_putnbr_base_fd((uint64_t)va_arg(*ap, uint32_t), t, fd);
	else if (current->spec == 'x')
	count += ft_printf_putnbr_base_fd((uint64_t)va_arg(*ap, uint32_t), l, fd);
	else if (current->spec == 'X')
	count += ft_printf_putnbr_base_fd((uint64_t)va_arg(*ap, uint32_t), u, fd);
	else if (current->spec == '%')
		count += ft_printf_putchar_fd('%', fd);
	return (count);
}

int	ft_printf_printer(const char *fmt, t_node *root, va_list *ap)
{
	int		count;
	t_node	*current;
	int		fd;

	fd = 1;
	count = 0;
	current = root;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			count = ft_printf_sub_printer(current, ap, count, fd);
			while ((*fmt) && !ft_printf_is_set((*(++fmt)), "cspdiuxX%%"))
				;
			current = current->next;
		}
		else
			count += ft_printf_putchar_fd(*fmt, fd);
		fmt++;
	}
	return (count);
}

int	ft_printf(const char *fmt, ...)
{
	int		count;
	va_list	ap;
	t_node	*root;

	count = 0;
	if (!fmt)
		return (-1);
	root = ft_printf_init_data(fmt);
	if (!root)
		return (0);
	va_start(ap, fmt);
	count += ft_printf_printer(fmt, root, &ap);
	va_end(ap);
	ft_printf_lstclear(&root);
	return (count);
}
