/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rstarcev <rstarcev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 22:37:41 by rstarcev          #+#    #+#             */
/*   Updated: 2026/02/18 14:03:11 by rstarcev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

bool	gnl_have_nl(char *s);
size_t	gnl_strlen(const char *s);
char	*gnl_free_stach_ret_null(char *stach);
char	*gnl_extend_stach(int *nread, size_t *len, size_t *cap, char *stach);
void	gnl_copy_buf(int *nread, size_t *len, char *stach, char *buf);

char	*get_next_line(int fd);
char	*gnl_reader(char *stach, int fd);
char	*gnl_sub_reader(char *stach, size_t *len, size_t *cap, int fd);
char	*gnl_extract_line(char *s);
char	*gnl_trim_stach(char *s);

#endif