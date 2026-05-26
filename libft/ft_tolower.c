/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:10:00 by eretriha          #+#    #+#             */
/*   Updated: 2025/11/09 03:55:18 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
/* 
int main(void)
{
	char c;
	int i = 'A';
    while (i <= 'Z')
    {
        c = ft_tolower(i);
        write(1, &c, 1);
        i++;
    }

	return (0);
}
 */