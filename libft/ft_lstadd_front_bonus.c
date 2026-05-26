/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:10:43 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/08 22:40:59 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	else
	{
		new->next = *lst;
		*lst = new;
		return ;
	}
}
/* 
#include <stdio.h>

void	ft_clear_lst(t_list **lst)
{
	t_list	*current;
	t_list	*to_del;

	if (!lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		to_del = current->next;
		free(current->content);
		free(current);
		current = NULL;
		current = to_del;
	}
}

void	ft_display_list(t_list *root)
{
	t_list	*current;

	current = root;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(src);
	dest = malloc(sizeof(char) * (len + 1));
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	main(int ac, char **av)
{
	t_list	**cursor;
	t_list	*new;
	int		i;

	cursor = malloc(sizeof(t_list *));
	if (!cursor)
		return (0);
	*cursor = NULL;
	i = 1;
	while (i < ac)
	{
		if (av[i] != NULL)
		{
			new = ft_lstnew(ft_strdup(av[i]));
			if (!new)
				return (0);
			ft_lstadd_front(cursor, new);
		}
		i++;
	}
	ft_display_list(*cursor);
	ft_clear_lst(cursor);
	free(cursor);
	return (0);
}
 */