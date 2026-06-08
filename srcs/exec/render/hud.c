#include "cub.h"
#include <stdio.h>

static int	get_hud_color(int hp)
{
	if (hp > PLAYER_MAX_HP / 2)
		return (0x2ECC71);
	if (hp > PLAYER_MAX_HP / 4)
		return (0xF1C40F);
	return (0xE74C3C);
}

void	draw_hud_pixels(t_engine *eng)
{
	int		bar_y;
	int		hp;
	int		fill_w;

	bar_y = eng->screen->win_height - 35;
	draw_rect_blend(&eng->screen->img2,
		(t_vec2){.i = {20, eng->screen->win_height - 70}},
		(t_vec2){.i = {220, 55}}, 0x000000);
	draw_rect(&eng->screen->img2, (t_vec2){.i = {30, bar_y}},
		(t_vec2){.i = {200, 12}}, 0x333333);
	hp = eng->player->hp;
	if (hp < 0)
		hp = 0;
	if (hp > PLAYER_MAX_HP)
		hp = PLAYER_MAX_HP;
	fill_w = (hp * 198) / PLAYER_MAX_HP;
	if (fill_w > 0)
	{
		draw_rect(&eng->screen->img2, (t_vec2){.i = {31, bar_y + 1}},
			(t_vec2){.i = {fill_w, 10}}, get_hud_color(hp));
	}
}

void	draw_hud_text(t_engine *eng)
{
	int		card_y;
	char	hp_str[32];
	int		hp;

	card_y = eng->screen->win_height - 70;
	hp = eng->player->hp;
	if (hp < 0)
		hp = 0;
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		30, card_y + 18, 0xFFFFFF, "LIFE (HP)");
	sprintf(hp_str, "%d / %d", hp, PLAYER_MAX_HP);
	mlx_string_put(eng->screen->mlx_ptr, eng->screen->win_ptr,
		160, card_y + 18, 0xCCCCCC, hp_str);
}
