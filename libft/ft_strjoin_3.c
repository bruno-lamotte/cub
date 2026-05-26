/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kafouber <kafouber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:57:00 by rstarcev          #+#    #+#             */
/*   Updated: 2026/02/13 06:30:11 by kafouber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_3(const char *a, const char *b, const char *c)
{
	char	*ab;
	char	*abc;

	ab = ft_strjoin(a, b);
	if (!ab)
		return (NULL);
	abc = ft_strjoin(ab, c);
	free(ab);
	return (abc);
}
