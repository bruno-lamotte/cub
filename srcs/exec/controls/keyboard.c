#include "cub.h"

int	handle_terminal_key(int keycode, t_engine *engine);

int	key_press(int keycode, t_engine *engine)
{
	if (engine->terminal_mode)
		return (handle_terminal_key(keycode, engine));
	if (keycode == XK_w || keycode == 'w' || keycode == 'W'
		|| keycode == XK_z || keycode == 'z' || keycode == 'Z')
		engine->keys.w = true;
	else if (keycode == XK_s || keycode == 's' || keycode == 'S')
		engine->keys.s = true;
	else if (keycode == XK_a || keycode == 'a' || keycode == 'A'
		|| keycode == XK_q || keycode == 'q' || keycode == 'Q')
		engine->keys.a = true;
	else if (keycode == XK_d || keycode == 'd' || keycode == 'D')
		engine->keys.d = true;
	else if (keycode == XK_Left)
		engine->keys.left = true;
	else if (keycode == XK_Right)
		engine->keys.right = true;
	else if (keycode == 'e' || keycode == 'E')
		engine->keys.e = true;
	else if (keycode == XK_Escape)
		close_window(engine);
	return (0);
}

int	key_release(int keycode, t_engine *engine)
{
	if (keycode == XK_w || keycode == 'w' || keycode == 'W'
		|| keycode == XK_z || keycode == 'z' || keycode == 'Z')
		engine->keys.w = false;
	else if (keycode == XK_s || keycode == 's' || keycode == 'S')
		engine->keys.s = false;
	else if (keycode == XK_a || keycode == 'a' || keycode == 'A'
		|| keycode == XK_q || keycode == 'q' || keycode == 'Q')
		engine->keys.a = false;
	else if (keycode == XK_d || keycode == 'd' || keycode == 'D')
		engine->keys.d = false;
	else if (keycode == XK_Left)
		engine->keys.left = false;
	else if (keycode == XK_Right)
		engine->keys.right = false;
	else if (keycode == 'e' || keycode == 'E')
		engine->keys.e = false;
	return (0);
}
