#include "cub.h"

bool	is_player_on_exit(t_engine *engine)
{
	uint8_t		*flags;
	uint8_t		*occ;
	int			idx;
	t_blob_hdr	*hdr;

	if (!engine || !engine->player || !engine->blob || !engine->data)
		return (false);
	hdr = get_blob_hdr(engine->blob);
	if (!hdr)
		return (false);
	if ((uint16_t)engine->player->pos.d.x >= get_map_width(hdr)
		|| (uint16_t)engine->player->pos.d.y >= get_map_height(hdr))
		return (false);
	flags = get_map_flags(engine->blob);
	idx = get_map_width(hdr) * (uint16_t)engine->player->pos.d.y
		+ (uint16_t)engine->player->pos.d.x;
	if (!(flags[idx] & CELL_HAS_OBJ))
		return (false);
	occ = get_map_occ_ids(engine->blob);
	if (engine->data->obj_defs[occ[idx]].symbol != 'X')
		return (false);
	return (true);
}

