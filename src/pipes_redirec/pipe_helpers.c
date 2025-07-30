/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:33:19 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/29 14:41:46 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_ast *n)
{
	if (!n)
		return (0);
	if (n->cmd && n->cmd[0])
		return (1);
	return (count_cmds(n->left) + count_cmds(n->right));
}

void	fill_cmds(t_ast *n, t_ast **arr, int *idx)
{
	if (!n)
		return ;
	fill_cmds(n->left, arr, idx);
	if (n->cmd && n->cmd[0])
	{
		arr[*idx] = n;
		++(*idx);
	}
	fill_cmds(n->right, arr, idx);
}

int	flatten_pipeline(t_ast *root, t_ast ***out, t_minishell *sh)
{
	int	count;
	int	i;

	(void)sh;
	if (!root || !out)
		return (0);
	count = count_cmds(root);
	if (count <= 0)
		return (0);
	*out = malloc(sizeof(t_ast *) * count);
	if (!*out)
		return (-1);
	i = 0;
	fill_cmds(root, *out, &i);
	return (count);
}
