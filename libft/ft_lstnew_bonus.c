/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:02:46 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/08 22:41:30 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
/* 
#include <stdio.h>

int	main(void)
{
	t_list	*root;
	char	str[6] = "12345";

	root = ft_lstnew(str);
	if (!root)
		return (0);
	printf("%s\n", (char *)root->content);
	free(root);
	return (0);
}
 */