#ifndef CUB_EXEC_H
# define CUB_EXEC_H

# include "cub_types.h"

int				get_num_threads(void);

// srcs/exec/raycasting.c
void			cast_ray(t_worker *w, int x, t_ray_data *out);

// srcs/exec/render/render_main.c
void			render_frame(t_engine *engine);
void			init_thread_pool(t_engine *engine);
void			shutdown_thread_pool(t_engine *engine);
void			submit_job(t_thread_pool *pool, void (*func)(void *, t_worker *), void *arg);
void			wait_for_jobs(t_thread_pool *pool);
void			worker_render_slice(void *arg, t_worker *w);
void			put_pixel(t_img *img, int x, int y, int color);
int				clamp_idx(int idx);
float			clamp_float(float val);
int				clamp_color(int val);
unsigned int	blend_colors(unsigned int src, unsigned int dst, unsigned char alpha);
void			draw_minimap(t_engine *engine);
void			draw_minimap_monsters(t_engine *eng, t_vec2 c, int radius,
					double px_per_tile);
void			draw_minimap_player(t_engine *eng, t_vec2 c);
void			draw_hacking_prompt(t_engine *eng);
void			draw_hacking_overlay(t_engine *eng);
void			draw_hacking_text(t_engine *eng);
void			draw_terminal_hacking_screen(t_engine *eng);
void			draw_terminal_hacking_text(t_engine *eng);
int				is_near_terminal(t_engine *eng);
void			draw_rect(t_img *img, t_vec2 p, t_vec2 sz, int color);
void			draw_rect_blend(t_img *img, t_vec2 p, t_vec2 sz, int color);
void			draw_filled_circle(t_img *img, t_vec2 c, int r, int color);
void			draw_circle_outline(t_img *img, t_vec2 c, int r, int color);
void			draw_single_map_obj(t_engine *eng, t_interact_obj *obj, t_vec2 p, int is_sel);

// srcs/exec/render/hud.c
void			draw_hud_pixels(t_engine *eng);
void			draw_hud_text(t_engine *eng);

// srcs/exec/draw_column.c
uint8_t			get_wall_tex_id(t_ray_data *ray, t_vec2 *ray_dir, void *blob);
void			init_projection(t_draw *d, t_ray_data *ray, int win_height);
void			render_pixels(t_draw *d, t_worker *w, t_ray_data *ray, t_lut *lut);
void			render_floor_slice(t_worker *w);
void			draw_column(int x, t_worker *w, t_ray_data *ray);

// srcs/exec/draw_floor_utils.c
void			init_row_params(t_worker *w, int y, t_floor_row *r, t_lut *lut);
void			draw_floor_pixel(t_worker *w, t_floor_row *r, int x, float sh[2]);

float			get_diff(float a, float b);


float			fast_inv_sqrt(float number);
double			get_door_ratio(int mx, int my, int width, void *blob);
int				is_door_horiz(int mx, int my, int w, void *blob);
int				check_door_hit(t_ray_data *ray, t_player_rt *p, void *blob);

// srcs/exec/player.c
void			init_player_rt(t_data *data, t_player_rt *p);

// srcs/exec/movement/
int				is_walkable(int x, int y, void *blob);
int				is_walkable_for_routing(int x, int y, void *blob);
int				is_valid_position(double x, double y, void *blob);
void			update_position(t_engine *eng, t_keys *keys);
void			update_rotation(t_player_rt *p, t_keys *keys);
int				check_player_mstr_collision(double x, double y, t_engine *eng);

// srcs/exec/exit.c
bool			is_player_on_exit(t_engine *engine);

// srcs/exec/doors.c

void			update_doors(t_engine *engine);

// srcs/exec/controls/
int				key_press(int keycode, t_engine *engine);
int				key_release(int keycode, t_engine *engine);
int	close_window(t_engine *engine, int ret_val);
void			free_engine_graphics(t_engine *engine);
int				game_loop(t_engine *engine);
void			update_global_alarm_state(t_engine *eng);
void			init_terminal_mode(t_engine *eng);
void			update_interaction(t_engine *eng);

// srcs/exec/light_sources/
void			init_static_lights(t_engine *engine);
int				check_los(t_vec2 p1, t_vec2 p2, void *blob);
int				check_thick_los(t_vec2 p1, t_vec2 p2, double radius, void *blob);
int				check_los_shadow(float mx, float my, float px, float py);
int				run_dda(int step[4], float d[4], float side[2], t_map_data *map);
float			compute_light_at_point(double wx, double wy, void *blob, t_engine *eng);
float			get_alarm_light_at_point(double wx, double wy, void *blob, t_engine *eng);

// srcs/exec/monsters_ai/
void			update_monsters(t_engine *engine);
void			update_single_monster(t_monster_rt *m, t_engine *eng, t_worker *w);
void			print_monsters_debug(t_monster_rt *mstr, int count);
void			update_alarm_disarm(t_engine *eng);
int				detect_player(t_monster_rt *m, t_engine *eng);
void			init_all_ai_behaviors(t_slr1 *slr, t_engine *eng);
int				check_monster_collision(double x, double y, int self_idx, t_engine *eng);
int				get_reachable_cells(t_engine *eng, t_vec2 start, t_vec2 *cells, t_worker *w);
int				is_bottleneck(int x, int y, void *blob);
void			find_closest_alarm(t_monster_rt *m, t_engine *eng,
					t_vec2 *alarm_pos);
void			mstr_go_to_alarm(t_monster_rt *m, t_engine *eng,
					t_vec2 alarm_pos, t_worker *w);
t_vec2			find_next_step(t_monster_rt *m, t_engine *eng, t_vec2 target, t_worker *w);
void			mstr_move_towards(t_monster_rt *m, t_engine *eng,
					t_vec2 target);
void			mstr_go_to_guard(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_chase(t_monster_rt *m, t_engine *eng, t_worker *w);
int				get_fsm_state_from_slr_state(t_engine *eng, int slr_state_id);
void			mstr_patrol_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_chase_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_scan_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_reloc_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_alarm_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			mstr_attack_behavior(t_monster_rt *m, t_engine *eng, t_worker *w);
void			execute_slr_transition(t_engine *eng, t_monster_rt *m, char *stim);
void			execute_slr_transition_by_id(t_engine *eng, t_monster_rt *m, int sym_nbr);
int				check_monster_self_collision(t_monster_rt *self, t_engine *eng);
int				is_bottleneck_cell(t_engine *eng, int x, int y);
void			assign_new_patrol_target(t_monster_rt *m, t_engine *eng, t_worker *w);
void			run_grammar_transitions(t_monster_rt *m, t_engine *eng,
					double d2, int spotted);
void			init_monster_targets(t_engine *eng);
int				is_any_monster_near(t_engine *eng, t_light *l);
int				select_patrol_cell(t_engine *eng, t_monster_rt *m,
					t_vec2 *cells, int total);
int				score_patrol_cell(t_engine *eng, t_monster_rt *m, t_vec2 cell);
void			init_bfs_arrays(int *parent, int size);
int				bfs_run(t_vec2 start, t_vec2 target, t_engine *eng, t_worker *w);
int				bfs_backtrack(int start_idx, int target_idx, int *parent);

// srcs/exec/monsters_render/
void			draw_monsters_3d(t_engine *eng);
void			draw_objects_3d(t_engine *eng);
double			get_sprite_coords(t_engine *eng, t_vec2 pos,
					t_vec2 *sp, t_vec2 *trans);
int				collect_monsters(t_engine *eng, t_sprite *sprites, int count);
int				collect_objects(t_engine *eng, t_sprite *sprites, int count);
void			sort_sprites(t_sprite *sprites, int count);
void			draw_monster_stripe(t_engine *eng, int stripe, int *w_h,
					t_sprite *s);
void			draw_object_stripe(t_engine *eng, int stripe, int *w_h,
					t_sprite *s);
void			load_monster_anim(t_engine *eng);
t_mstr_anim_type	get_monster_active_anim(t_engine *eng, t_sprite *s, int *mirror);
int				sh_idx(double t);
void			calc_monster_bounds(t_engine *eng, t_sprite *s,
					t_vec2 params, int bounds[2]);
unsigned int	get_color_from_hex(char *hex);
int				get_tok_index(char *tok, int cpp);
t_img			copy_image_frame(void *mlx, t_img *src);
void			parse_dxpm_row(char *line, t_img *dst, unsigned int *palette,
					int cpp);
unsigned int	get_transparent_color(t_img *img);
void			parse_dxpm_header_line(char *line, int *cpp,
					unsigned int *palette, t_img *prev);
void			load_single_dxpm(void *mlx, char *path, t_img *prev,
					t_img *dst);

#endif
