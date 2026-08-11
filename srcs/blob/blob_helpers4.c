/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blob_helpers4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

t_bdef_tex	*get_tex_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_tex *)((uint8_t *)blob + hdr->tex_def.start));
}

t_lut	*get_lut_bdef(void *blob)
{
	t_blob_hdr	*hdr;

	hdr = get_blob_hdr(blob);
	return ((t_lut *)((uint8_t *)blob + hdr->lut_def.start));
}
