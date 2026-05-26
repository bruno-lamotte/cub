#include "../../includes/cub.h"

t_blob_hdr *get_blob_hdr(void *blob)
{
	return ((t_blob_hdr *)(uint8_t *)blob);
}

/*
typedef struct	s_map_soa
{
	uint8_t		*flags;
	uint8_t		*block_id; // wall and air
	uint8_t		*bonus_id; // door and obj
}				t_map_soa;
*/

uint16_t get_map_height(t_blob_hdr *hdr)
{
	return (hdr->map.map_data.height);
}

uint16_t get_map_width(t_blob_hdr *hdr)
{
	return (hdr->map.map_data.width);
}

uint16_t get_map_size(t_blob_hdr *hdr)
{
	return (get_map_width(hdr) * get_map_height(hdr));
}

uint8_t *get_map_flags(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start);
}

uint8_t *get_map_block_ids(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start + hdr->map.used / MAP_SOA_LAYERS);
}

uint8_t *get_map_occ_ids(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((uint8_t *)blob + hdr->map.start + (hdr->map.used / MAP_SOA_LAYERS) * 2);
}

t_map_soa get_map_soa(void *blob)
{
	t_map_soa m;

	m.flags = get_map_flags(blob);
	m.block_id = get_map_block_ids(blob);
	m.occ_id = get_map_occ_ids(blob);
	return (m);
}

t_monster_rt *get_monster_rt(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_monster_rt *)((uint8_t *)blob + hdr->mstr_rt.start));
}

t_door_rt *get_door_rt(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_door_rt *)((uint8_t *)blob + hdr->door_rt.start));
}

t_bdef_wall *get_wall_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_wall *)((uint8_t *)blob + hdr->wall_def.start));
}

t_bdef_air *get_air_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_air *)((uint8_t *)blob + hdr->air_def.start));
}

t_bdef_door *get_door_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_door *)((uint8_t *)blob + hdr->door_def.start));
}

t_bdef_monster *get_mstr_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_monster *)((uint8_t *)blob + hdr->mstr_def.start));
}

t_bdef_obj *get_obj_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_obj *)((uint8_t *)blob + hdr->obj_def.start));
}

t_bdef_tex *get_tex_bdef(void *blob)
{
	t_blob_hdr *hdr;

	hdr = get_blob_hdr(blob);
	return ((t_bdef_tex *)((uint8_t *)blob + hdr->tex_def.start));
}
t_lut   *get_lut_bdef(void *blob)
{
    t_blob_hdr  *hdr;

    hdr = get_blob_hdr(blob);
    return ((t_lut *)((uint8_t *)blob + hdr->lut_def.start)); 
}