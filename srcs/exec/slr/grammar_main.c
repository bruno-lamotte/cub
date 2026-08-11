/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 01:39:07 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 01:39:07 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include <stdlib.h>
#include <unistd.h>

void	get_rule_from_grammar(t_slr1 *data, t_rule **new_rule, char *line);

int	parse_grammar(t_slr1 *data)
{
	int		fd;
	char	*line;
	t_rule	*new_rule;

	fd = open("grammars/grammar_bonus++.txt", O_RDONLY);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line && *line != '\n' && *line != '\0' && *line != '\r')
	{
		new_rule = malloc(sizeof(t_rule));
		if (!new_rule)
			return (close(fd), free(line), 0);
		ft_bzero(new_rule, sizeof(t_rule));
		get_rule_from_grammar(data, &new_rule, line);
		ft_lstadd_back(&data->rules, ft_lstnew(new_rule));
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	return (close(fd), 1);
}
