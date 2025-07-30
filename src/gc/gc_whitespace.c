/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_whitespace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/29 18:06:52 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	count_words(char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (!is_whitespace(*str))
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		str++;
	}
	return (count);
}

char	*extract_word(t_minishell *sh, char *str, int *start)
{
	int		end;
	int		len;
	int		i;
	char	*word;

	while (str[*start] && is_whitespace(str[*start]))
		(*start)++;
	if (!str[*start])
		return (NULL);
	end = *start;
	while (str[end] && !is_whitespace(str[end]))
		end++;
	len = end - *start;
	word = gc_malloc(sh, len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[*start + i];
		i++;
	}
	word[len] = '\0';
	*start = end;
	return (word);
}

static	char	**handle_quoted_string(t_minishell *sh, char *str)
{
	char	**words;

	words = gc_malloc(sh, sizeof(char *) * 2);
	if (!words)
		return (NULL);
	words[0] = gc_strdup(sh, str);
	words[1] = NULL;
	return (words);
}

char	**split_whitespace(t_minishell *sh, char *str)
{
	char	**words;
	int		word_count;
	int		start;
	int		i;

	if (!str)
		return (NULL);
	if (ft_strchr(str, '"') || ft_strchr(str, '\''))
		return (handle_quoted_string(sh, str));
	word_count = count_words(str);
	words = gc_malloc(sh, sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	start = 0;
	i = 0;
	while (i < word_count)
	{
		words[i] = extract_word(sh, str, &start);
		if (!words[i])
			return (NULL);
		i++;
	}
	words[i] = NULL;
	return (words);
}
