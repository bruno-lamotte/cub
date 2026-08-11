/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_menu.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blamotte <blamotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:33:49 by blamotte          #+#    #+#             */
/*   Updated: 2026/08/11 00:33:49 by blamotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_MENU_H
# define CUB_MENU_H

# define LINE_HEIGHT 35
# define MAP_FOLDER "./maps/"
# define MENU_BG_PATH "./bg.xpm"

# define KEY_ESC 65307
# define KEY_ENTER 65293
# define KEY_UP 65362
# define KEY_DOWN 65364

# define X_WHITE 0xFFFFFF
# define X_GREY 0x888888
# define X_GREEN 0x00FF00
# define X_RED 0xFF0000

typedef struct s_bg_data
{
	int32_t		scale;
	int32_t		scaled_w;
	int32_t		scaled_h;
	int32_t		off_x;
	int32_t		off_y;
}				t_bg_data;

# define MENU_PAUSE 0
# define MENU_ERROR 1
# define MENU_PLAY 2

typedef struct s_menu
{
	void		*mlx_ptr;
	void		*win_ptr;
	char		**maps;
	int32_t		map_count;
	int32_t		selected;
	int32_t		w_width;
	int32_t		w_height;
	t_img		bg_src;
	t_img		bg_scaled;
	int			ret_val;
}				t_menu;

int				cub(char *map);
void			close_menu(t_menu *menu);
char			*menu(void);
char			**load_maps(char *folder, int32_t *count);
bool			launch_selected_map(t_menu *menu);
void			destroy_menu_window(t_menu *menu);
bool			init_menu_bg(t_menu *m);
void			destroy_menu_bg(t_menu *m);
void			render_menu(t_menu *m);
int				menu_key_hook(int keycode, t_menu *menu);
void			menu_error(char *msg);

#endif