/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_blob.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_BLOB_H
# define CUB_BLOB_H

# include "cub_types.h"

typedef struct s_tmp_build_px
{
	t_bdef_tex	*tex_defs;
	uint32_t	*dest;
	t_img		*src_img;
	uint32_t	*tex_dest;
}				t_tmp_build_px;

bool			get_alloc_size(t_tmp_header *hdr, size_t *out);
bool			build_blob(t_data *d, void **blob);

bool			fill_tmp_hdr(t_data *d, t_tmp_header *hdr);
bool			build_hdr(t_data *d, void *blob, t_tmp_header *tmp);
bool			size_to_u32(size_t val, uint32_t *out);

void			build_rt(t_data *d, void *blob);
void			build_bdef(t_data *d, void *blob);
void			build_px(t_data *d, void *blob);

void			print_blob(void *blob);

/* build_helpers */
int16_t			get_block_id(char sym, t_data *d, t_block_type type);
int16_t			get_def_id(char *ids, char sym);

/* blob_helpers */
t_blob_hdr		*get_blob_hdr(void *blob);

uint16_t		get_map_height(t_blob_hdr *hdr);
uint16_t		get_map_width(t_blob_hdr *hdr);
uint16_t		get_map_size(t_blob_hdr *hdr);

uint8_t			*get_map_flags(void *blob);
uint8_t			*get_map_block_ids(void *blob);
uint8_t			*get_map_occ_ids(void *blob);

t_map_soa		get_map_soa(void *blob);

t_monster_rt	*get_monster_rt(void *blob);
t_door_rt		*get_door_rt(void *blob);

t_bdef_wall		*get_wall_bdef(void *blob);
t_bdef_air		*get_air_bdef(void *blob);
t_bdef_door		*get_door_bdef(void *blob);
t_bdef_monster	*get_mstr_bdef(void *blob);
t_bdef_obj		*get_obj_bdef(void *blob);
t_bdef_tex		*get_tex_bdef(void *blob);
t_lut			*get_lut_bdef(void *blob);

#endif
