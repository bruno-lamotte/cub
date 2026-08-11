/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blob_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

t_blob_hdr	*get_blob_hdr(void *blob)
{
	return ((t_blob_hdr *)(uint8_t *)blob);
}

uint16_t	get_map_height(t_blob_hdr *hdr)
{
	return (hdr->map.u_data.map_data.height);
}

uint16_t	get_map_width(t_blob_hdr *hdr)
{
	return (hdr->map.u_data.map_data.width);
}

uint16_t	get_map_size(t_blob_hdr *hdr)
{
	return (get_map_width(hdr) * get_map_height(hdr));
}

uint8_t	*get_map_flags(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start);
}
