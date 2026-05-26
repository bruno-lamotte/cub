/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 19:11:03 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/09 20:52:22 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current;

	current = lst;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

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

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
		return ;
	}
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
	return ;
}
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*to_del;

	current = *lst;
	to_del = NULL;
	while (current != NULL)
	{
		to_del = current->next;
		ft_lstdelone(current, del);
		current = to_del;
	}
	*lst = NULL;
	return ;
}
/* 
#include <stdio.h>

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
			new = ft_lstnew(malloc(1));
			if (!new)
				return (0);
			ft_lstadd_back(cursor, new);
		}
		i++;
	}
	// ft_display_list(*cursor);
	ft_lstclear(cursor, &free);
	free(cursor);
	return (0);
}
 */