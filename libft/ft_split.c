/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 19:02:59 by rstarcev          #+#    #+#             */
/*   Updated: 2025/11/10 22:43:57 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_len(char const *s, char sep)
{
	int	i;

	i = 0;
	if (s[i] == sep)
		while (s[i] && s[i] == sep)
			i++;
	else if (s[i] != sep)
		while (s[i] && s[i] != sep)
			i++;
	return (i);
}

static void	ft_free_chartab(int *itab, char **tab)
{
	int	y;

	y = 0;
	while (y <= itab[1])
	{
		if (tab[y])
		{
			free(tab[y]);
			tab[y] = NULL;
		}
		y++;
	}
	itab[0] = -1;
	itab[1] = -1;
	free(tab);
}

static int	ft_count_words(char const *s, char sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] && s[i] == sep)
			i += ft_get_len(&s[i], sep);
		else if (s[i] && s[i] != sep)
		{
			i += ft_get_len(&s[i], sep);
			count++;
		}
	}
	return (count);
}

static char	*ft_cpy_word(char const *s, char sep)
{
	int		i;
	int		len;
	char	*out;

	i = 0;
	len = ft_get_len(s, sep);
	out = malloc(sizeof(char) * (len + 1));
	if (!out)
		return (NULL);
	while (s[i] && i < len)
	{
		out[i] = s[i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

char	**ft_split(char const *s, char c)
{
	int		itab[3];
	char	**tab;

	tab = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	itab[0] = 0;
	itab[1] = 0;
	while (s[itab[0]])
	{
		if (s[itab[0]] == c)
			itab[0] += ft_get_len(&s[itab[0]], c);
		else if (s[itab[0]] != c)
		{
			tab[itab[1]] = ft_cpy_word(&s[itab[0]], c);
			if (!tab[itab[1]])
				ft_free_chartab(itab, tab);
			if (itab[1] == -1 && itab[0] == -1)
				return (NULL);
			itab[1]++;
			itab[0] += ft_get_len(&s[itab[0]], c);
		}
	}
	tab[itab[1]] = NULL;
	return (tab);
}
/*
#include <stdio.h>

int	main(void)
{
	char	**tab;
	int		y;

	// tab = ft_split("              .    hello bonjour    ca va ?      ", ' ');
	tab = ft_split("xxxxxxxxhello!", ('x'));
	y = 0;
	while (tab[y] != NULL)
	{
		printf("%s\n", tab[y]);
		y++;
	}
	ft_free_chartab(y, tab);
	return (0);
}
 */

/*
static void	ft_free_chartab(int size, char **tab)
{
	int	y;

	y = 0;
	while (y <= size)
	{
		if (tab[y])
		{
			free(tab[y]);
			tab[y] = NULL;
		}
		y++;
	}
	free(tab);
}
*/
/*
char	**ft_split(char const *s, char c)
{
	int		i;
	int		y;
	char	**tab;

	tab = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	y = 0;
	while (s[i])
	{
		if (s[i] == c)
			i += ft_get_len(&s[i], c);
		else if (s[i] != c)
		{
			tab[y] = ft_cpy_word(&s[i], c);
			if (!tab[y])
			{
				ft_free_chartab(y, tab);
				return (NULL);
			}
			y++;
			i += ft_get_len(&s[i], c);
		}
	}
	tab[y] = NULL;
	return (tab);
}
 */
/*
char	**ft_split(char const *s, char c)
{
	int		itab[3];
	char	**tab;

	tab = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	itab[0] = 0;
	itab[1] = 0;
	while (s[itab[0]])
	{
		if (s[itab[0]] == c)
			itab[0] += ft_get_len(&s[itab[0]], c);
		else if (s[itab[0]] != c)
		{
			tab[itab[1]] = ft_cpy_word(&s[itab[0]], c);
			if (!tab[itab[1]])
			{
				ft_free_chartab(itab[1], tab);
				return (NULL);
			}
			itab[1]++;
			itab[0] += ft_get_len(&s[itab[0]], c);
		}
	}
	tab[itab[1]] = NULL;
	return (tab);
}
 */