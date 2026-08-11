/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_section7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

bool	check_s(char *n, bool (*f)(t_parsing *, size_t), t_parsing *p, size_t l)
{
	char	**file;
	t_index	*i;

	file = p->file_content;
	i = &p->idx;
	if (!file[i->line] || ft_strcmp(n, file[i->line++]))
		return (i->err = PERR_SECTION_TITLE, false);
	if (!file[i->line] || ft_strcmp("{\n", file[i->line++]))
		return (i->err = PERR_OPEN_BRACE, false);
	while (file[i->line])
	{
		if (!ft_strcmp("}\n", file[i->line]) || !i->line++)
			break ;
		l++;
	}
	if (!file[i->line] || ft_strcmp("}\n", file[i->line]))
		return (i->err = PERR_CLOSE_BRACE, false);
	if (!l)
		return (i->err = PERR_NO_CONTENT, false);
	if (l > MAX_SECTION_ITEMS)
		return (i->err = PERR_MAX_CONTENT, false);
	i->line -= l;
	if (!f(p, l))
		return (false);
	return (i->line++, true);
}
