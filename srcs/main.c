/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 00:12:30 by rstarcev          #+#    #+#             */
/*   Updated: 2026/06/22 18:50:00 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	main(int ac, char **av)
{
	if (ac == 1)
		return (cub_menu());
	else
		return (cub(av[1]));
}

// int main(void)
// {
// 	return (cub("./maps/1.cub"));
// }
