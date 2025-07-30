/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:19:03 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/29 17:04:56 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_till_pipe(char **words)
{
	int	count;

	count = 0;
	while (words[count] && ft_strcmp(words[count], "|") != 0)
		count++;
	return (count);
}

t_ast	*handle_pipe_error(t_minishell *sh)
{
	ft_putstr_fd("minishell:"
		" syntax error near unexpected token `|'\n", STDERR_FILENO);
	if (sh)
		sh->last_exit = 2;
	return (NULL);
}

static t_ast	*process_pipeline_loop(t_pipeline_params params)
{
	while (params.words[*params.i])
	{
		if (ft_strcmp(params.words[*params.i], "|") == 0)
		{
			if (!process_pipeline_segment(params))
			{
				if (*params.node)
					free_ast(*params.node);
				return (NULL);
			}
		}
		(*params.i)++;
	}
	return (*params.node);
}

static int	validate_pipeline_segments(char **words, t_minishell *sh)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	while (words[i])
	{
		if (ft_strcmp(words[i], "|") == 0)
		{
			if (!words[i + 1] || ft_strcmp(words[i + 1], "|") == 0)
				return (0);
			start = i + 1;
		}
		else if (i == start)
		{
			if (!parse_segment(words + start,
					count_till_pipe(words + start), sh))
				return (0);
		}
		i++;
	}
	return (1);
}

t_ast	*parse_pipeline(char **words, t_minishell *sh)
{
	t_ast				*node;
	int					i;
	int					start;
	t_pipeline_params	params;

	i = 0;
	start = 0;
	node = NULL;
	if (words[0] && ft_strcmp(words[0], "|") == 0)
		return (handle_pipe_error(sh));
	if (!validate_pipeline_segments(words, sh))
		return (handle_pipe_error(sh));
	params = (t_pipeline_params){words, &i, &start, &node, sh};
	node = process_pipeline_loop(params);
	if (!node)
		node = parse_segment(words, i, sh);
	return (node);
}
