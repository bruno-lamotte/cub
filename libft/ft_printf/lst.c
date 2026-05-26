/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 22:54:17 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/13 15:47:19 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_node	*ft_printf_lstlast(t_node *lst)
{
	t_node	*current;

	current = lst;
	if (!lst)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	ft_printf_lstadd_back(t_node **lst, t_node *new)
{
	t_node	*last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	else
	{
		last = ft_printf_lstlast(*lst);
		last->next = new;
		return ;
	}
}

t_node	*ft_printf_lstnew(char c)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->spec = c;
	new->next = NULL;
	return (new);
}

void	ft_printf_lstclear(t_node **lst)
{
	t_node	*current;
	t_node	*to_del;

	current = *lst;
	to_del = NULL;
	while (current != NULL)
	{
		to_del = current->next;
		free(current);
		current = to_del;
	}
	*lst = NULL;
	return ;
}

t_node	*ft_printf_init_data(const char *fmt)
{
	t_node	*root;
	t_node	*new;

	root = NULL;
	new = NULL;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			while ((*fmt) && !ft_printf_is_set((*(++fmt)), "cspdiuxX%%"))
				;
			new = ft_printf_lstnew((char)*fmt);
			if (!new)
			{
				if (root)
					ft_printf_lstclear(&root);
				return (NULL);
			}
			ft_printf_lstadd_back(&root, new);
		}
		fmt++;
	}
	return (root);
}
