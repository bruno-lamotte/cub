#include "../../includes/cub.h"

static void	print_sep_line(size_t width)
{
	size_t	i;

	printf("    +");
	i = 0;
	while (i < width)
	{
		printf("-----");
		i++;
	}
	printf("+\n");
}

static void	print_one_grid(const char *title, uint8_t *arr,
	size_t width, size_t height)
{
	size_t	l;
	size_t	c;
	size_t	idx;

	printf("\n%s\n", title);
	print_sep_line(width);
	l = 0;
	while (l < height)
	{
		printf("%3zu |", l);
		c = 0;
		while (c < width)
		{
			idx = l * width + c;
			printf(" %3u ", arr[idx]);
			c++;
		}
		printf("|\n");
		l++;
	}
	print_sep_line(width);
	printf("      ");
	c = 0;
	while (c < width)
	{
		printf("%3zu  ", c);
		c++;
	}
	printf("\n");
}

static char	get_flag_char(uint8_t flags)
{
	if (flags == 0)
		return ('.');
	if (flags & CELL_HAS_WALL)
		return ('W');
	if (flags & CELL_HAS_DOOR)
		return ('D');
	if (flags & CELL_HAS_OBJ)
		return ('O');
	if (flags & CELL_HAS_AIR)
		return ('A');
	return ('?');
}

void	print_map_flags_pretty(t_map_soa map, size_t width, size_t height)
{
	size_t	l;
	size_t	c;
	size_t	idx;

	printf("\nFLAG VIEW\n");
	print_sep_line(width);
	l = 0;
	while (l < height)
	{
		printf("%3zu |", l);
		c = 0;
		while (c < width)
		{
			idx = l * width + c;
			printf(" %3c ", get_flag_char(map.flags[idx]));
			c++;
		}
		printf("|\n");
		l++;
	}
	print_sep_line(width);
		printf("      ");
	c = 0;
	while (c < width)
	{
		printf("%3zu  ", c);
		c++;
	}
	printf("\n");
}

void	print_map_soa(t_map_soa map, size_t width, size_t height)
{
	print_map_flags_pretty(map, width, height);
	print_one_grid("FLAGS", map.flags, width, height);
	print_one_grid("BLOCK_ID", map.block_id, width, height);
	print_one_grid("OCC_ID", map.occ_id, width, height);
}

void print_hdr_seg_map(t_blob_seg *map)
{
	printf("start : %u\n", map->start);
	printf("end   : %u\n", map->end);
	printf("used  : %u\n", map->used);
	printf("width : %u\n", map->map_data.width);
	printf("height: %u\n", map->map_data.height);
}

void print_hdr_seg_normal(t_blob_seg *s)
{
	printf("start : %u\n", s->start);
	printf("end   : %u\n", s->end);
	printf("used  : %u\n", s->used);
	printf("count : %u\n", s->count);
}

void print_hdr(t_blob_hdr *h)
{
	printf("--------------------------\n");
	printf("BLOB HEADER :\n");
	printf("--------------------------\n");
	printf("map:\n");
	print_hdr_seg_map(&h->map);
	printf("--------------------------\n");
	printf("door_rt:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_door_rt));
	print_hdr_seg_normal(&h->door_rt);
	printf("--------------------------\n");
	printf("mstr_rt:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_monster_rt));
	print_hdr_seg_normal(&h->mstr_rt);
	printf("--------------------------\n");
	printf("wall_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_wall));
	print_hdr_seg_normal(&h->wall_def);
	printf("--------------------------\n");
	printf("air_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_air));
	print_hdr_seg_normal(&h->air_def);
	printf("--------------------------\n");
	printf("door_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_door));
	print_hdr_seg_normal(&h->door_def);
	printf("--------------------------\n");
	printf("mstr_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_monster));
	print_hdr_seg_normal(&h->mstr_def);
	printf("--------------------------\n");
	printf("obj_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_obj));
	print_hdr_seg_normal(&h->obj_def);
	printf("--------------------------\n");
	printf("tex_def:\n");
	printf("sizeof struct -> %zd\n", sizeof(t_bdef_tex));
	print_hdr_seg_normal(&h->tex_def);
	printf("--------------------------\n");
	printf("pixels_data:\n");
	print_hdr_seg_normal(&h->pixels_data);
	printf("--------------------------\n");
	printf("end offset = %u\n", h->end_offset);
	printf("--------------------------\n");
}

void print_one_door_rt(t_door_rt *d, size_t width)
{
	size_t x;
	size_t y;

	x = d->map_id % width;
	y = d->map_id / width;
	printf("X = %zd | Y = %zd\n", x, y);
	printf("open ratio %d/255\n", d->open_ratio_255);
	printf("def id -> %d\n", d->def_id);
	printf("-------\n");
}

void print_door_rt(void *blob)
{
	t_blob_hdr *hdr;
	size_t width;
	t_door_rt *d;
	size_t i;

	printf("\nDOOR RT :\n");
	printf("-------\n");
	hdr = get_blob_hdr(blob);
	width = get_map_width(hdr);
	d = get_door_rt(blob);
	i = 0;
	while (i < hdr->door_rt.count)
	{
		print_one_door_rt(&d[i], width);
		i++;
	}
}

void print_one_mstr_rt(t_monster_rt *d, size_t width)
{
	size_t x;
	size_t y;

	x = d->map_id % width;
	y = d->map_id / width;
	printf("X = %zd | Y = %zd\n", x, y);
	printf("Xf = %f | Yf = %f\n", d->pos.x, d->pos.y);
	printf("hp %d\n", d->current_hp);
	printf("def id -> %d\n", d->def_id);
	printf("-------\n");
}

void print_mstr_rt(void *blob)
{
	t_blob_hdr *hdr;
	size_t width;
	t_monster_rt *d;
	size_t i;

	printf("\nMSTR RT :\n");
	printf("-------\n");
	hdr = get_blob_hdr(blob);
	width = get_map_width(hdr);
	d = get_monster_rt(blob);
	i = 0;
	while (i < hdr->mstr_rt.count)
	{
		print_one_mstr_rt(&d[i], width);
		i++;
	}
}

static void	print_title(const char *title)
{
	printf("\n=== %s ===\n", title);
}

void	print_bdef_wall_arr(t_bdef_wall *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_WALL");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | north south west east\n");
	printf("-----+------------------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %5u %4u %4u\n",
			i,
			arr[i].tex_north,
			arr[i].tex_south,
			arr[i].tex_west,
			arr[i].tex_east);
		i++;
	}
}

void	print_bdef_air_arr(t_bdef_air *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_AIR");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | floor ceiling\n");
	printf("-----+---------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %7u\n",
			i,
			arr[i].tex_floor,
			arr[i].tex_ceiling);
		i++;
	}
}

void	print_bdef_door_arr(t_bdef_door *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_DOOR");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | front back side\n");
	printf("-----+----------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %5u %4u %4u\n",
			i,
			arr[i].tex_front,
			arr[i].tex_back,
			arr[i].tex_side);
		i++;
	}
}

void	print_bdef_monster_arr(t_bdef_monster *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_MONSTER");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | tex hp size_pc\n");
	printf("-----+---------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %3u %3u %7u\n",
			i,
			arr[i].tex,
			arr[i].initial_hp,
			arr[i].size_pc);
		i++;
	}
}

void	print_bdef_obj_arr(t_bdef_obj *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_OBJ");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | tex size_pc\n");
	printf("-----+------------\n");
	i = 0;
	while (i < len)
	{
		printf(" %3u | %3u %7u\n",
			i,
			arr[i].tex,
			arr[i].size_pc);
		i++;
	}
}

void	print_bdef_tex_arr(t_bdef_tex *arr, uint8_t len)
{
	uint8_t	i;

	print_title("BDEF_TEX");
	if (!arr || len == 0)
	{
		printf("(empty)\n");
		return ;
	}
	printf(" idx | width height | kind   | value\n");
	printf("-----+--------------+--------+------------\n");
	i = 0;
	while (i < len)
	{
		if (arr[i].width == 0 && arr[i].height == 0)
		{
			printf(" %3u | %5u %6u | color  | 0x%08X\n",
				i,
				arr[i].width,
				arr[i].height,
				arr[i].color);
		}
		else
		{
			printf(" %3u | %5u %6u | offset | %10u\n",
				i,
				arr[i].width,
				arr[i].height,
				arr[i].offset);
		}
		i++;
	}
}

void print_bdefs(void *blob)
{
	t_blob_hdr *h;

	h = get_blob_hdr(blob);
	print_bdef_wall_arr(get_wall_bdef(blob), h->wall_def.count);
	print_bdef_air_arr(get_air_bdef(blob), h->air_def.count);
	print_bdef_door_arr(get_door_bdef(blob), h->door_def.count);
	print_bdef_monster_arr(get_mstr_bdef(blob), h->mstr_def.count);
	print_bdef_obj_arr(get_obj_bdef(blob), h->obj_def.count);
}

void print_blob(void *blob)
{
	print_hdr(get_blob_hdr(blob));
	print_map_soa(get_map_soa(blob), get_map_width(blob), get_map_height(blob));
	print_door_rt(blob);
	print_mstr_rt(blob);
	print_bdefs(blob);
}