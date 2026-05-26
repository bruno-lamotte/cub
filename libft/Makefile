# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/10 17:29:33 by rstarcev          #+#    #+#              #
#    Updated: 2026/02/16 19:40:48 by rstarcev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                  VARIABLES                                   #
# **************************************************************************** #

NAME = libft.a

CC = cc
AR = ar rcs
RM = rm -f

CFLAGS = -Wall -Werror -Wextra

LIBC = ft_calloc.c ft_isascii.c ft_memchr.c ft_memmove.c ft_strdup.c ft_strlen.c ft_strrchr.c \
ft_atoi.c ft_isalnum.c ft_isdigit.c ft_isspace.c ft_isset.c ft_memcmp.c ft_memset.c ft_strlcat.c ft_strncmp.c ft_strcmp.c ft_tolower.c \
ft_bzero.c ft_isalpha.c ft_isprint.c ft_memcpy.c ft_strchr.c ft_strlcpy.c ft_strnstr.c ft_toupper.c

ADDITIONAL = ft_putchar_fd.c ft_putnbr_fd.c ft_split.c ft_strjoin.c ft_strjoin_3.c ft_strtrim.c ft_itoa.c \
ft_putendl_fd.c ft_putstr_fd.c ft_striteri.c ft_strmapi.c ft_substr.c

PERSO	= ft_strndup.c ft_atoi_base.c

GNL = gnl/get_next_line.c gnl/get_next_line_utils.c
FT_PRINTF = ft_printf/printers.c ft_printf/ft_printf.c ft_printf/utils.c ft_printf/lst.c

BONUS_SRC = ft_lstadd_back_bonus.c ft_lstlast_bonus.c ft_lstadd_front_bonus.c \
ft_lstmap_bonus.c ft_lstclear_bonus.c ft_lstnew_bonus.c ft_lstdelone_bonus.c \
ft_lstsize_bonus.c ft_lstiter_bonus.c

SRCS = $(LIBC) $(ADDITIONAL) $(GNL) $(FT_PRINTF) $(PERSO)
BONUS = $(BONUS_SRC)

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)


# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME) : $(OBJS)
	@$(AR) $(NAME) $(OBJS)

bonus: $(OBJS) $(BONUS_OBJS)
	@$(AR) $(NAME) $(OBJS) $(BONUS_OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	@$(RM) $(NAME)

rebonus: fclean bonus

re: fclean all

.PHONY: all bonus clean fclean rebonus re
