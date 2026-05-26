/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:56:36 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/08 22:40:49 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
/*
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int ac, char **av)
{
	printf("%d\n", ft_isprint(atoi(av[1])));
	printf("%d\n", isprint(atoi(av[1])));
	return (0);
}
*/