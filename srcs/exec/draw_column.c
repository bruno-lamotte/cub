#include "cub.h"

uint8_t get_wall_tex_id(t_ray_data *ray, t_vec2_fp *ray_dir, void *blob)
{
    t_bdef_wall *walls;
    t_bdef_wall hit_wall;
    uint8_t     *flags;
    int         width;

    flags = get_map_flags(blob);
    width = get_map_width(get_blob_hdr(blob));
    int height = get_map_height(get_blob_hdr(blob));
    if (ray->map_x < 0 || ray->map_x >= width || ray->map_y < 0 || ray->map_y >= height)
        return (0);
    if (flags[ray->map_y * width + ray->map_x] & CELL_HAS_DOOR)
    {
        t_bdef_door *doors = get_door_bdef(blob);
        t_bdef_door hit_door = doors[ray->hit_type];
        if (ray->side == 0)
            return (hit_door.tex_side);
        return (hit_door.tex_front);
    }
    walls = get_wall_bdef(blob);
    hit_wall = walls[ray->hit_type]; 
    if (ray->side == 0)
    {
        if (ray_dir->x > 0)
            return (hit_wall.tex_west);
        return (hit_wall.tex_east);
    }
    else
    {
        if (ray_dir->y > 0)
            return (hit_wall.tex_north);
        return (hit_wall.tex_south);
    }
}

void init_projection(t_draw *d, t_ray_data *ray)
{
    int64_t height_fp;

    if (ray->perp_wall_dist <= 0)
        ray->perp_wall_dist = INT_TO_FP(1);    
    height_fp = ((int64_t)INT_TO_FP(WINDOW_HEIGHT) << FP_SHIFT) / ray->perp_wall_dist;
    height_fp = height_fp >> FP_SHIFT;
    if (height_fp > WINDOW_HEIGHT * 64)
        d->line_height = WINDOW_HEIGHT * 64;
    else if (height_fp <= 0)
        d->line_height = 1;
    else
        d->line_height = (int)height_fp;
    d->draw_start = -d->line_height / 2 + WINDOW_HEIGHT / 2;
    if (d->draw_start < 0)
        d->draw_start = 0;   
    d->draw_end = d->line_height / 2 + WINDOW_HEIGHT / 2;
    if (d->draw_end >= WINDOW_HEIGHT)
        d->draw_end = WINDOW_HEIGHT - 1;
}

static void init_texture_mapping(t_draw *d, t_ray_data *ray, t_vec2_fp *dir, void *blob)
{
    uint8_t     tex_id;
    t_bdef_tex  *tex_defs;

    tex_id = get_wall_tex_id(ray, dir, blob);
    tex_defs = get_tex_bdef(blob);
    d->tex = tex_defs[tex_id];
    if (d->tex.width > 0 && d->tex.height > 0)
    {
        d->pixels = (uint32_t *)((uint8_t *)blob + d->tex.offset);
        d->tex_x = FP_TO_INT(d->tex.width * ray->wall_x);
        if (ray->side == 0 && dir->x > 0)
            d->tex_x = d->tex.width - d->tex_x - 1;
        if (ray->side == 1 && dir->y < 0)
            d->tex_x = d->tex.width - d->tex_x - 1;
        if (d->tex_x < 0)
            d->tex_x = 0;
        else if (d->tex_x >= d->tex.width)
            d->tex_x = d->tex.width - 1;
        d->step = INT_TO_FP(d->tex.height) / d->line_height;
        d->tex_pos = (d->draw_start - WINDOW_HEIGHT / 2 + d->line_height / 2) * d->step;
    }
    else
    {
        d->pixels = NULL;
        d->tex_x = 0;
        d->step = 0;
        d->tex_pos = 0;
    }
}

void render_pixels(t_draw *d, t_screen *screen, t_ray_data *ray, t_lut *lut)
{
    int         y;
    int         tex_y;
    uint32_t    color;
    int         r, g, b;

    int       wall_dist_idx = (int)(FP_TO_FLOAT(ray->perp_wall_dist) * 10.0f);
    if (wall_dist_idx < 0) wall_dist_idx = 0;
    else if (wall_dist_idx >= DIST_MAX) wall_dist_idx = DIST_MAX - 1;
    float     wall_shade = lut->shade_table[wall_dist_idx];

    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        if (y < d->draw_start)
        {
            float ceil_dist = (float)WINDOW_HEIGHT / ((float)WINDOW_HEIGHT - 2.0f * y);
            int dist_idx = (int)(ceil_dist * 10.0f);
            if (dist_idx < 0) dist_idx = 0;
            else if (dist_idx >= DIST_MAX) dist_idx = DIST_MAX - 1;
            float ceil_shade = lut->shade_table[dist_idx];

            uint32_t ceil_color = 0x000000;
            int cr = (ceil_color >> 16) & 0xFF;
            int cg = (ceil_color >> 8) & 0xFF;
            int cb = ceil_color & 0xFF;
            cr = (int)((float)cr * ceil_shade);
            cg = (int)((float)cg * ceil_shade);
            cb = (int)((float)cb * ceil_shade);
            put_pixel(&screen->img, d->x, y, (cr << 16) | (cg << 8) | cb);
        }
        else if (y > d->draw_end)
        {
            float floor_dist = (float)WINDOW_HEIGHT / (2.0f * y - (float)WINDOW_HEIGHT);
            int dist_idx = (int)(floor_dist * 10.0f);
            if (dist_idx < 0) dist_idx = 0;
            else if (dist_idx >= DIST_MAX) dist_idx = DIST_MAX - 1;
            float floor_shade = lut->shade_table[dist_idx];

            uint32_t floor_color = 0x333333;
            int fr = (floor_color >> 16) & 0xFF;
            int fg = (floor_color >> 8) & 0xFF;
            int fb = floor_color & 0xFF;
            fr = (int)((float)fr * floor_shade);
            fg = (int)((float)fg * floor_shade);
            fb = (int)((float)fb * floor_shade);
            put_pixel(&screen->img, d->x, y, (fr << 16) | (fg << 8) | fb);
        }
        else
        {
            if (d->tex.width == 0 && d->tex.height == 0)
            {
                color = d->tex.color;
            }
            else
            {
                tex_y = FP_TO_INT(d->tex_pos);
                if (tex_y < 0)
                    tex_y = 0;
                else
                    tex_y = tex_y % d->tex.height;
                d->tex_pos += d->step;
                color = d->pixels[tex_y * d->tex.width + d->tex_x];
            }
            r = (color >> 16) & 0xFF;
            g = (color >> 8) & 0xFF;
            b = color & 0xFF;
            if (ray->side == 1)
            {
                r >>= 1;
                g >>= 1;
                b >>= 1;
            }
            r = (int)((float)r * wall_shade);
            g = (int)((float)g * wall_shade);
            b = (int)((float)b * wall_shade);
            color = (r << 16) | (g << 8) | b;
            put_pixel(&screen->img, d->x, y, color);
        }
        y++;
    }
}

void    draw_column(int x, t_worker *w, t_ray_data *ray)
{
    t_draw    d;
    t_lut     *lut = get_lut_bdef(w->blob);

    d.x = x;
    init_projection(&d, ray);
    init_texture_mapping(&d, ray, &ray->ray_dir, w->blob);
    render_pixels(&d, w->screen, ray, lut);
}

