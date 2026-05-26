/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 22:38:58 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/26 11:32:38 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

bool	gnl_have_nl(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (false);
	while (s[i])
	{
		if (s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

char	*gnl_free_stach_ret_null(char *stach)
{
	if (stach)
	{
		free(stach);
		stach = NULL;
	}
	return (NULL);
}

char	*gnl_extend_stach(int *nread, size_t *len, size_t *cap, char *stach)
{
	char	*new_stach;
	int		i;

	while ((size_t) * nread + *len + 1 > *cap)
		*cap *= 2;
	new_stach = malloc(sizeof(char) * (*cap + 1));
	if (!new_stach)
		return (gnl_free_stach_ret_null(stach), NULL);
	i = -1;
	while (++i < (long long)*len)
		new_stach[i] = stach[i];
	free(stach);
	stach = new_stach;
	return (stach);
}

void	gnl_copy_buf(int *nread, size_t *len, char *stach, char *buf)
{
	long long int	i;

	i = -1;
	while (++i < (long long) *nread)
		stach[*len + i] = buf[i];
	*len += *nread;
	stach[*len] = '\0';
	return ;
}
