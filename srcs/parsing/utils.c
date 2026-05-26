#include "../../includes/cub.h"

bool is_char_fmt(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool is_space_fmt(char c)
{
	return (c == '\t' || c == ' ');
}

bool skip_space_tab(char *s, t_index *i, bool expect_eol)
{
	while(s[i->col] && is_space_fmt(s[i->col]))
		i->col++;
	if (!expect_eol && (!s[i->col] || s[i->col] == '\n'))
		return (i->err = PERR_UNEXPECTED_EOL, false);
	return (true);
}

bool skip_char_fmt(char *s, t_index *i)
{
	if (!s[i->col] || !is_char_fmt(s[i->col]))
		return (i->err = PERR_INVALID_CHAR, false);
	while(s[i->col] && is_char_fmt(s[i->col]))
		i->col++;
	if (s[i->col] && s[i->col] != '\n' && s[i->col] != ',' && !is_space_fmt(s[i->col]))
		return (i->err = PERR_INVALID_CHAR, false);
	return (true);
}

// bool skip_char_fmt(char *s, t_index *i, bool expect_eol)

	// if (!expect_eol && (!s[i->col] || s[i->col] == '\n'))
	// 	return (i->err = PERR_UNEXPECTED_EOL, false);
	// if (expect_eol && s[i->col] != '\0' && s[i->col] != '\n')
	// 	return (i->err = PERR_UNEXPECTED_REST_OF_LINE, false);

bool skip_separator(char *s, t_index *i, char sep)
{
	if (!skip_space_tab(s, i, false))
		return (false);
	if (s[i->col] != sep)
		return (i->err = PERR_BAD_SEPARATOR, false);
	i->col++;
	if (!skip_space_tab(s, i, false))
		return (false);
	return (true);
}

// must return PERR_BAD_KEYWORD if this func return false
bool skip_keyword(char *s, t_index *i, char *keyword)
{
    size_t len;

    len = ft_strlen(keyword);
    if (ft_strncmp(keyword, &s[i->col], len))
        return (false);
    i->col += len;
    return(true);
}

bool	is_hex_digit(char c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F'));
}

bool	isalpha_up(int c)
{
	if (c >= 'A' && c <= 'F')
		return (1);
	return (0);
}

bool skip_hex_color(char *s, t_index *i, int32_t *dest)
{
    size_t start;
    char old;

    start = i->col;

	if (!s[i->col] || s[i->col] != '0')
        return (i->err = PERR_BAD_HEX_PREFIX, false);

	i->col++;
	if (!s[i->col] || (s[i->col] != 'x' && s[i->col] != 'X'))
        return (i->err = PERR_BAD_HEX_PREFIX, false);
	i->col++;
    while (is_hex_digit(s[i->col]) && (i->col - start) < HEX_COLOR_LEN)
    {
        if (s[i->col] >= 'A' && s[i->col] <= 'F')
            s[i->col] = (char)ft_tolower((char)s[i->col]);
        i->col++;
    }
	if ((i->col - start) != HEX_COLOR_LEN)
		return (i->err = PERR_BAD_HEX_COLOR_LEN, false);
    old = s[i->col];
    s[i->col] = '\0';
    *dest = (int32_t)ft_atoi_base(&s[start + 2], "0123456789abcdef");
    s[i->col] = old;
	return (true);
}

size_t get_eol(char *s)
{
	size_t i;

	i = 0;
	while(s[i] && s[i] != '\n')
		i++;
	if (!s[i])
		return (0);
	return (i);
}

bool skip_file_path(char *s, t_index *i, char **dest)
{
	size_t eol;

	eol = get_eol(&s[i->col]);
	if (!eol)
		return (i->err = PERR_UNEXPECTED_EOL, false);
	*dest = ft_strndup(&s[i->col], eol);
	if (!(*dest))
		return (i->err = PERR_MALLOC_ERR, false);
	i->col += eol;
	return (true);
}

bool skip_tex_name(char *s, t_index *i, char **dest)
{
	size_t start;

	start = i->col;
	if(!skip_char_fmt(s, i))
		return (false);
	*dest = ft_strndup(&s[start], i->col - start);
	if (!(*dest))
		return (i->err = PERR_MALLOC_ERR);
	return (true);
}

bool skip_and_get_symbol(char *s, t_index *i, char *dest)
{
	skip_space_tab(s, i, false);
	if (!s[i->col] || s[i->col] == '\n')
		return (i->err = PERR_UNEXPECTED_EOL, false);
	*dest = s[i->col];
	i->col++;
	return (true);
}

ssize_t find_tex_from_str(char *s, size_t n, t_parsing *p)
{
	t_texture_def	*tex;
	ssize_t			j;

	tex = p->data->textures_defs;
	// write(1, s, 7);
	j = 0;
	// printf(" | TEX NAME = %s\n", tex[j].name);
	while (j < p->data->textures_len)
	{
		if (ft_strlen(tex[j].name) == n && !ft_strncmp(s, tex[j].name, n))
			return (j);
		j++;
	}
	return (-1);
}

/* bool	skip_uint16_value(char *s, t_index *i, uint16_t *out)
{
	uint32_t	val;

	if (!s[i->col] || !ft_isdigit((unsigned char)s[i->col]))
		return (i->err = PERR_NO_NUMBER, false);
	val = 0;
	while (s[i->col] && ft_isdigit((unsigned char)s[i->col]))
	{
		val = val * 10 + (s[i->col] - '0');
		if (val > UINT16_MAX)
			return (i->err = PERR_TOO_LARGE_NUMBER, false);
		i->col++;
	}
	*out = (uint16_t)val;
	return (true);
} */

bool	skip_uint16_value(char *s, t_index *i, uint16_t *out)
{
	uint32_t	val;
	uint32_t	digit;

	if (!s[i->col] || !ft_isdigit((unsigned char)s[i->col]))
		return (i->err = PERR_NO_NUMBER, false);
	val = 0;
	while (s[i->col] && ft_isdigit((unsigned char)s[i->col]))
	{
		digit = (uint32_t)(s[i->col] - '0');
		if (val > (UINT16_MAX - digit) / 10)
			return (i->err = PERR_TOO_LARGE_NUMBER, false);
		val = val * 10 + digit;
		i->col++;
	}
	*out = (uint16_t)val;
	return (true);
}

bool	skip_uint8_value(char *s, t_index *i, uint8_t *out)
{
	uint16_t	val;
	uint16_t	digit;

	if (!s[i->col] || !ft_isdigit((unsigned char)s[i->col]))
		return (i->err = PERR_NO_NUMBER, false);
	val = 0;
	while (s[i->col] && ft_isdigit((unsigned char)s[i->col]))
	{
		digit = (uint16_t)(s[i->col] - '0');
		if (val > (UINT8_MAX - digit) / 10)
			return (i->err = PERR_TOO_LARGE_NUMBER, false);
		val = val * 10 + digit;
		i->col++;
	}
	*out = (uint8_t)val;
	return (true);
}

bool	skip_percent_value(char *s, t_index *i, uint8_t *out)
{
	if (!skip_uint8_value(s, i, out))
		return (false);
	if (*out > 100)
		return (i->err = PERR_TOO_LARGE_NUMBER, false);
	if (s[i->col] != '%')
		return (i->err = PERR_INVALID_PERCENT, false);
	i->col++;
	return (true);
}