/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kafouber <kafouber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:47:45 by eretriha          #+#    #+#             */
/*   Updated: 2026/02/17 04:07:46 by kafouber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*str;
	size_t	len;

	str = "hello";
	len = ft_strlen(str);
	printf("%ld\n", len);
	len = strlen(str);
	printf("%ld\n", len);
	return (0);
}
 */
