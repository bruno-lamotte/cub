#include "../../includes/cub.h"

/*

typedef struct	s_bdef_wall
{
	uint8_t tex_north;
	uint8_t tex_south;
	uint8_t tex_west;
	uint8_t tex_east;
}				t_bdef_wall;

typedef struct	s_bdef_air
{
	uint8_t tex_floor;
	uint8_t tex_ceiling;
}				t_bdef_air;

typedef struct	s_bdef_door
{
	uint8_t tex_front;
	uint8_t tex_back;
	uint8_t tex_side;
	uint8_t	padding;
}				t_bdef_door;

typedef struct	s_bdef_monster
{
	uint8_t	tex;
	uint8_t	initial_hp;
	uint8_t size_pc;
	uint8_t	padding;
}				t_bdef_monster;

typedef struct	s_bdef_obj
{
	uint8_t	tex;
	uint8_t size_pc;
}				t_bdef_obj;

*/

void build_wall_bdef(t_wall_def *def, t_bdef_wall *bdef, uint8_t count)
{
	uint8_t i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_north = def[i].tex_north;
		bdef[i].tex_west = def[i].tex_west;
		bdef[i].tex_south = def[i].tex_south;
		bdef[i].tex_east = def[i].tex_east;
		i++;
	}
}

void build_air_bdef(t_air_def *def, t_bdef_air *bdef, uint8_t count)
{
	uint8_t i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_ceiling = def[i].tex_ceiling;
		bdef[i].tex_floor = def[i].tex_floor;
		i++;
	}
}

void build_door_bdef(t_door_def *def, t_bdef_door *bdef, uint8_t count)
{
	uint8_t i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex_back = def[i].tex_back;
		bdef[i].tex_front = def[i].tex_front;
		bdef[i].tex_side = def[i].tex_side;
		i++;
	}
}

void build_mstr_bdef(t_monster_def *def, t_bdef_monster *bdef, uint8_t count)
{
	uint8_t i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex = def[i].tex;
		bdef[i].initial_hp = def[i].hp;
		bdef[i].size_pc = def[i].size_pc;
		i++;
	}
}

void build_obj_bdef(t_obj_def *def, t_bdef_obj *bdef, uint8_t count)
{
	uint8_t i;

	i = 0;
	while (i < count)
	{
		bdef[i].tex = def[i].tex;
		bdef[i].size_pc = def[i].size_pc;
		i++;
	}
}

/*
typedef struct	s_bdef_tex
{
	uint16_t	width;
	uint16_t	height;
	union
	{
		uint32_t	offset;
		uint32_t	color;
	};
}				t_bdef_tex;
*/

void build_tex_bdef(t_data *d, t_bdef_tex *bdef, uint8_t count)
{
	uint8_t i;
	uint32_t off;

	i = 0;
	off = 0;
	while (i < count)
	{
		if (d->textures_defs[i].is_color)
			bdef[i].color = d->textures_defs[i].color;
		else
		{
			// printf("%d | TEST off = %u\n", i, off);
			bdef[i].height = d->img_tab[i].height;
			bdef[i].width = d->img_tab[i].width;
			bdef[i].offset = off;
			off += (bdef[i].height * bdef[i].width * sizeof(uint32_t));
		}
		i++;
	}
	printf("build tex check allignement with hgeader offset -> END off = %u\n", off);
}

static void build_shade_lut(t_lut *lut)
{
    int     i;
    float   distance;
    float   intensity;
    float   max_visible_dist;
    float   factor;

    i = 0;
    max_visible_dist = 7.0f; // Completement noir a seulement 7 blocs !
    while (i < DIST_MAX)
    {
        distance = (float)i / 10.0f; // Resolution de 0.1 bloc
        if (distance <= 0.5f)
        {
            intensity = 1.0f; // Pleine lumiere immediate a bout portant
        }
        else if (distance >= max_visible_dist)
        {
            intensity = 0.0f; // Noir absolu au-dela de 7 blocs
        }
        else
        {
            // Attenuation quadratique realiste (style faisceau de lampe torche)
            factor = 1.0f - ((distance - 0.5f) / (max_visible_dist - 0.5f));
            intensity = factor * factor;
        }
        lut->shade_table[i] = intensity;
        i++;
    }
}

void    build_lut_bdef(t_lut *lut)
{
    build_shade_lut(lut);
}

void build_bdef(t_data *d, void *blob)
{
	build_wall_bdef(d->walls_defs, get_wall_bdef(blob), d->walls_len);
	build_air_bdef(d->airs_defs, get_air_bdef(blob), d->airs_len);
	build_door_bdef(d->doors_defs, get_door_bdef(blob), d->doors_len);
	build_mstr_bdef(d->monsters_defs, get_mstr_bdef(blob), d->monsters_len);
	build_obj_bdef(d->obj_defs, get_obj_bdef(blob), d->obj_len);
	build_tex_bdef(d, get_tex_bdef(blob), d->textures_len);
	build_lut_bdef(get_lut_bdef(blob));
}