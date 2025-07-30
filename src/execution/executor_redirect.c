/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/29 17:53:28 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_builtin(char *s)
{
	return (!ft_strcmp(s, "echo") || !ft_strcmp(s, "cd")
		|| !ft_strcmp(s, "pwd") || !ft_strcmp(s, "export")
		|| !ft_strcmp(s, "unset") || !ft_strcmp(s, "env")
		|| !ft_strcmp(s, "exit") || !ft_strcmp(s, "."));
}

int	classify_exec_error(const char *path)
{
	struct stat	st;

	if (!path || access(path, F_OK) != 0)
		return (127);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (126);
}

void	exec_node(t_minishell *sh, t_ast *node)
{
	char	*path;
	char	*underscore_entry;
	int		code;

	if (!node->cmd || !node->cmd[0])
		exit(0);
	if (is_builtin(node->cmd[0]))
		exit(execute_builtin(sh, node->cmd));
	path = resolve_command_path(sh, node->cmd[0]);
	if (path)
	{
		underscore_entry = gc_strjoin(sh, "_=", path);
		env_set(sh, underscore_entry);
		execve(path, node->cmd, sh->env);
	}
	code = classify_exec_error(path);
	write(2, "minishell: ", 11);
	write(2, node->cmd[0], ft_strlen(node->cmd[0]));
	if (code == 127)
		write(2, ": command not found\n", 20);
	else if (path && access(path, X_OK) != 0)
		write(2, ": Permission denied\n", 20);
	else
		write(2, ": Is a directory\n", 17);
	exit(code);
}

char	*replace_exit_status_marker(char *arg, char *temp_val, t_minishell *sh)
{
	char	*marker_pos;
	char	*before_marker;
	char	*after_marker;
	char	*val;
	char	*temp;

	marker_pos = ft_strnstr(arg, "$$EXIT_STATUS$$", ft_strlen(arg));
	if (!marker_pos)
		return (temp_val);
	before_marker = gc_strndup(sh, arg, marker_pos - arg);
	after_marker = marker_pos + 15;
	if (*after_marker)
	{
		temp = gc_strjoin(sh, before_marker, temp_val);
		val = gc_strjoin(sh, temp, after_marker);
		free(temp_val);
	}
	else
	{
		val = gc_strjoin(sh, before_marker, temp_val);
		free(temp_val);
	}
	return (val);
}
