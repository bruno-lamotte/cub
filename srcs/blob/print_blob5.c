/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blob5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 02:00:00 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 02:00:00 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	print_blob(void *blob)
{
	print_hdr(get_blob_hdr(blob));
	print_map_soa(get_map_soa(blob), get_map_width(blob),
		get_map_height(blob));
	print_door_rt(blob);
	print_mstr_rt(blob);
	print_bdefs(blob);
}
