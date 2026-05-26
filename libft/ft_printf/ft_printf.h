/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:40:02 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/13 17:44:21 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <limits.h>
# include <stdint.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_node
{
	char			spec;
	struct s_node	*next;
}					t_node;

int		ft_printf(const char *fmt, ...);
int		ft_printf_printer(const char *fmt, t_node *root, va_list *ap);
int		ft_printf_sub_printer(t_node *current, va_list *ap, int count, int fd);

t_node	*ft_printf_init_data(const char *fmt);
void	ft_printf_lstclear(t_node **lst);
t_node	*ft_printf_lstnew(char c);
void	ft_printf_lstadd_back(t_node **lst, t_node *new);
t_node	*ft_printf_lstlast(t_node *lst);

int		ft_printf_put_addr(va_list *ap, int fd, char *lower_base);
int		ft_printf_putnbr_base_fd(unsigned long long nb, char *base, int fd);
int		ft_printf_putstr_fd(char *str, int fd);
int		ft_printf_putnbr_fd(int n, int fd);
int		ft_printf_putchar_fd(char c, int fd);

int		ft_printf_strlen(const char *str);
int		ft_printf_is_set(char c, char const *set);
#endif