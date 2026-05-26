/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 19:30:36 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 22:08:16 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* 
size_t	ft_test_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_test_rev_content(void *content)
{
	char	*str;
	size_t	i;
	size_t	y;
	size_t	len;
	char	temp;

	str = (char *)content;
	i = 0;
	len = ft_test_strlen(str);
	y = len - 1;
	while (i < len / 2)
	{
		temp = str[i];
		str[i] = str[y];
		str[y] = temp;
		y--;
		i++;
	}
	return ((void *)str);
}

void	*ft_test_content(void *content)
{
	char	*str;

	str = ft_strdup((char *)content);
	if (!str)
		return (NULL);
	return (str);
}

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
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*current;
	t_list	*new;
	t_list	*new_obj;
	char	*str;

	current = lst;
	new = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (current != NULL)
	{
		str = f(current->content);
		new_obj = ft_lstnew(str);
		if (!new_obj)
		{
			del(str);
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back(&new, new_obj);
		current = current->next;
	}
	return (new);
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

int	main(int ac, char **av)
{
	t_list	**cursor;
	t_list	*new;
	t_list	*new_lst;
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
			ft_lstadd_back(cursor, new);
		}
		i++;
	}
	ft_display_list(*cursor);
	printf("----------------------------\n");
	new_lst = ft_lstmap(*cursor, &ft_test_content, &free);
	ft_display_list(new_lst);
	ft_lstclear(&new_lst, &free);
	ft_lstclear(cursor, &free);
	free(cursor);
	return (0);
}
 */