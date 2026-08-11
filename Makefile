NAME = cub

CC = cc
MAKEFLAGS += -j16
CFLAGS = -O3 -ffast-math -g -Wall -Wextra -lpthread -flto
RM = rm -rf

OBJ_DIR = objects
INCLUDES_DIR = includes

SRCS = \
	srcs/main.c \
	srcs/engine_init.c \
	srcs/parsing/parsing.c \
	srcs/parsing/get_file.c \
	srcs/parsing/free_def.c \
	srcs/parsing/print_defs.c \
	srcs/parsing/print_defs2.c \
	srcs/parsing/print_defs3.c \
	srcs/parsing/print_defs4.c \
	srcs/parsing/utils.c \
	srcs/parsing/utils2.c \
	srcs/parsing/utils3.c \
	srcs/parsing/utils4.c \
	srcs/parsing/check_section.c \
	srcs/parsing/check_section2.c \
	srcs/parsing/check_section3.c \
	srcs/parsing/check_section4.c \
	srcs/parsing/check_section5.c \
	srcs/parsing/check_section6.c \
	srcs/parsing/check_section7.c \
	srcs/parsing/check_id.c \
	srcs/parsing/check_id2.c \
	srcs/parsing/get_tex.c \
	srcs/parsing/validate_map.c \
	srcs/parsing/validate_map2.c \
	srcs/parsing/init_mlx.c \
	srcs/parsing/print_tex_lib.c \
	srcs/parsing/print_tex_lib2.c \
	srcs/blob/build_blob.c \
	srcs/blob/blob_helpers.c \
	srcs/blob/blob_helpers2.c \
	srcs/blob/blob_helpers3.c \
	srcs/blob/blob_helpers4.c \
	srcs/blob/build_rt.c \
	srcs/blob/build_rt2.c \
	srcs/blob/build_def.c \
	srcs/blob/build_def2.c \
	srcs/blob/print_blob.c \
	srcs/blob/print_blob2.c \
	srcs/blob/print_blob3.c \
	srcs/blob/print_blob4.c \
	srcs/blob/print_blob5.c \
	srcs/blob/build_helpers.c \
	srcs/blob/build_hdr.c \
	srcs/blob/build_hdr2.c \
	srcs/exec/controls/window.c \
	srcs/exec/controls/keyboard.c \
	srcs/exec/controls/terminal.c \
	srcs/exec/controls/interaction.c \
	srcs/exec/controls/mouse.c \
	srcs/exec/doors.c \
	srcs/exec/player.c \
	srcs/exec/movement/player_move.c \
	srcs/exec/movement/collision.c \
	srcs/exec/render/thread_pool.c \
	srcs/exec/render/thread_pool2.c \
	srcs/exec/render/fps.c \
	srcs/exec/render/draw_utils.c \
	srcs/exec/render/draw_shapes.c \
	srcs/exec/render/hacking_overlay.c \
	srcs/exec/render/hacking_terminal.c \
	srcs/exec/render/hacking_terminal2.c \
	srcs/exec/render/render_main.c \
	srcs/exec/render/hud.c \
	srcs/exec/raycasting.c \
	srcs/exec/raycasting2.c \
	srcs/exec/draw_column.c \
	srcs/exec/draw_pixels.c \
	srcs/exec/draw_floor.c \
	srcs/exec/draw_floor_utils.c \
	srcs/exec/exec_utils.c \
	srcs/exec/fp_utils.c \
	srcs/exec/fp_utils2.c \
	srcs/exec/exit.c \
	srcs/exec/light_sources/light_init.c \
	srcs/exec/light_sources/light_los.c \
	srcs/exec/light_sources/light_los_utils.c \
	srcs/exec/light_sources/light_compute.c \
	srcs/exec/light_sources/light_compute2.c \
	srcs/exec/minimap/minimap_draw.c \
	srcs/exec/minimap/minimap_utils.c \
	srcs/exec/minimap_entities.c \
	srcs/exec/minimap_entities2.c \
	srcs/exec/monsters_render/sprite_collect.c \
	srcs/exec/monsters_render/sprite_proj.c \
	srcs/exec/monsters_render/sprite_proj_utils.c \
	srcs/exec/monsters_render/dxpm_loader.c \
	srcs/exec/monsters_render/dxpm_parser.c \
	srcs/exec/monsters_render/sprite_anim_loader.c \
	srcs/exec/monsters_render/sprite_anim_loader2.c \
	srcs/exec/monsters_render/sprite_object_draw.c \
	srcs/exec/monsters_render/sprite_draw.c \
	srcs/exec/monsters_ai/player_detection.c \
	srcs/exec/monsters_ai/collision.c \
	srcs/exec/monsters_ai/patrol_select.c \
	srcs/exec/monsters_ai/patrol_assign.c \
	srcs/exec/monsters_ai/bfs_utils.c \
	srcs/exec/monsters_ai/bfs_utils2.c \
	srcs/exec/monsters_ai/bfs_path.c \
	srcs/exec/monsters_ai/bfs_path2.c \
	srcs/exec/monsters_ai/bfs_move.c \
	srcs/exec/monsters_ai/mstr_behaviors_move.c \
	srcs/exec/monsters_ai/slr_states.c \
	srcs/exec/monsters_ai/slr_transitions.c \
	srcs/exec/monsters_ai/mstr_transitions.c \
	srcs/exec/monsters_ai/mstr_behaviors_actions.c \
	srcs/exec/monsters_ai/mstr_behaviors_alarm.c \
	srcs/exec/monsters_ai/mstr_update.c \
	srcs/exec/monsters_ai/mstr_update_utils.c \
	srcs/exec/vector_arithmetic.c \
	srcs/exec/slr/item.c \
	srcs/exec/slr/closure.c \
	srcs/exec/slr/free_slr.c \
	srcs/exec/slr/free_utils.c \
	srcs/exec/slr/firsts.c \
	srcs/exec/slr/firsts2.c \
	srcs/exec/slr/follows.c \
	srcs/exec/slr/follows_main.c \
	srcs/exec/slr/grammar.c \
	srcs/exec/slr/grammar_main.c \
	srcs/exec/slr/symbols_utils.c \
	srcs/exec/slr/symbols_main.c \
	srcs/exec/slr/table_fill.c \
	srcs/exec/slr/table_init.c \
	srcs/exec/slr/table_init2.c \
	srcs/exec/slr/states_utils.c \
	srcs/exec/slr/states_main.c \
	srcs/exec/slr/print.c \
	srcs/exec/slr/print2.c \
	srcs/menu/menu.c \
	srcs/menu/menu_render.c \
	srcs/menu/menu_maps.c \
	srcs/menu/menu_hooks.c \
	srcs/menu/menu_background.c \
	srcs/menu/menu_background2.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

MLX_DIR = mlx
MLX = $(MLX_DIR)/libmlx.a

INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

all: $(MLX) $(LIBFT_LIB) $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR) bonus

$(MLX):
	printf "INC=/usr/include\n" > $(MLX_DIR)/Makefile.gen
	grep -v '%%%%' $(MLX_DIR)/Makefile.mk >> $(MLX_DIR)/Makefile.gen
	$(MAKE) -C $(MLX_DIR) -f Makefile.gen CC=gcc CFLAGS="-O3 -std=gnu17 -I/usr/include"

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) -f Makefile.gen clean || true

re: fclean all

norminette:
	norminette $(INCLUDES_DIR) srcs

.PHONY: all clean fclean re norminette