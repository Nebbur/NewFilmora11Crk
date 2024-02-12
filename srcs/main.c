/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 20:17:05 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 20:17:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sort_list(t_env *head)
{
	t_env	*temp;
	char	*temp_key;
	char	*temp_value;

	temp = head;
	while (temp->next != NULL)
	{
		if (ft_strcmp(temp->key, temp->next->key) > 0)
		{
			temp_key = temp->key;
			temp_value = temp->value;
			temp->key = temp->next->key;
			temp->value = temp->next->value;
			temp->next->key = temp_key;
			temp->next->value = temp_value;
			temp = head;
		}
		else
			temp = temp->next;
	}
}

void	free_all(t_shell *shell)
{
	while (shell->env != NULL)
	{
		free(shell->env->key);
		free(shell->env);
		shell->env = shell->env->next;
	}
}

void	ft_echo(char **cmds)
{
	int	i;
	int	j;
	int	flag;

	printf("echo\n");
	flag = 0;
	i = 0;
	if (ft_strncmp(cmds[1], "-n", 2) == 0)
		if (!(cmds[1][2]) || cmds[1][2] == 'n')
			i++;
	if (i == 1)
		flag = 1;
	while (cmds[++i])
	{
		j = -1;
		while (cmds[i][++j] != '\0')
		{
			if (cmds[i][j] == '$')
			{
				if (cmds[i][j + 1] == '?')
				{
					ft_printf("%d", 0); // should expand to the exit status of the most recently executed foreground pipeline
					j++;
					continue ;
				}
				if (cmds[i][j + 1] == '$')
				{
					ft_printf("%d", getpid());
					j++;
				}
				ft_printf("%s", getenv(ft_until_char(cmds[i] + j + 1, '$')));
				while (cmds[i][++j + 1] > 32 && cmds[i][j + 1] != '$')
					;
			}
			else
				ft_printf("%c", cmds[i][j]);
		}
		if (cmds[i + 1])
			ft_printf(" ");
	}
	if (flag == 0)
		ft_printf("\n");
}

char	**ft_env_to_char(t_env *env)
{
	int		i;
	t_env	*temp;
	char	**env_char;

	i = 0;
	temp = env;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	env_char = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env != NULL)
	{
		env_char[i] = ft_strjoin(env->key, "=");
		env_char[i] = ft_strjoin(env_char[i], env->value);
		env = env->next;
		i++;
	}
	env_char[i] = NULL;
	return (env_char);
}

void	ft_exec(char **cmds, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**env;

	env = ft_env_to_char(shell->env);
	path = ft_strjoin("/bin/", cmds[0]);
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, cmds, env) == -1)
		{
			ft_printf("%s%s: %s: %s\n", RED, cmds[0], strerror(errno), RESET);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		ft_printf("%s%s: %s\n", RED, cmds[0], strerror(errno));
		exit(EXIT_FAILURE);
	}
	wait(&pid);
	free(path);
}

void	ft_cd(char **cmds, t_shell *shell)
{
	char *path;

	if (cmds[1] == NULL || ft_strcmp(cmds[1], "~") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			ft_printf("%s%s: %s\n", RED, "cd", strerror(errno));
			return ;
		}
	}
	else
		path = cmds[1];
	if (chdir(path) == -1)
		ft_printf("%s%s: %s: %s\n", RED, "cd", path, strerror(errno));
}

void	ft_export(char **cmds, t_shell *shell)
{
	t_env	*temp_exp;
	t_env	*temp_env;
	t_env	*new_node;
	char	*key;
	char	*value;

	temp_exp = shell->exp;
	temp_env = shell->env;
	if (cmds[1] == NULL)
	{
		while (temp_exp != NULL)
		{
			if (temp_exp->value != NULL)
				ft_printf("declare -x %s=\"%s\"\n", temp_exp->key, temp_exp->value);
			else
				ft_printf("declare -x %s\n", temp_exp->key);
			temp_exp = temp_exp->next;
		}
		return ;
	}
	if (ft_strchr(cmds[1], '=') == NULL)
	{
		key = cmds[1];
		value = NULL;
	}
	else
	{
		value = ft_strrchr(cmds[1], '=') + 1;
		key = ft_until_char(cmds[1], '=');
	}
	while (temp_exp->next != NULL && ft_strcmp(temp_exp->next->key, key) != 0)
		temp_exp = temp_exp->next;
	while (temp_env != NULL && ft_strcmp(temp_env->key, key) != 0)
		temp_env = temp_env->next;
	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->key = key;
	new_node->value = value;
	if (temp_exp->next == NULL)
		new_node->next = NULL;
	else
		new_node->next = temp_exp->next->next;
	temp_exp->next = new_node;
	if (temp_env != NULL)
	{
		temp_env->key = key;
		temp_env->value = value;
	}
	sort_list(shell->exp);
}

void	ft_unset(t_env **head, char *key)
{
	t_env	*temp;
	t_env	*prev;

	temp = *head;
	if (temp != NULL && ft_strcmp(temp->key, key) == 0)
	{
		*head = temp->next;
		free(temp->key);
		free(temp);
		return ;
	}
	while (temp != NULL && ft_strcmp(temp->key, key) != 0)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL)
		return ;
	prev->next = temp->next;
	free(temp->key);
	free(temp);
}

void	ft_env(char **cmds, t_shell *shell)
{
	t_env	*temp;

	temp = shell->env;
	while (temp != NULL)
	{
		ft_printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	if (ac != 1)
	{
		ft_printf("%sInvalid argument %s\nUsage: ./minishell%s\n", RED, GREEN, RESET);
		exit(EXIT_FAILURE);
	}
	shell = (t_shell){0};
	init_shell(&shell, envp);

	while (1)
	{
		shell.input = readline("\e[4;35mmini\e[4;34mshell\e[0;36m$\e[0m ");
		if (shell.input == NULL)
		{
			printf("exit\n");
			break ;
		}
		parser(&shell);
		if (ft_strcmp(shell.input, "exit") == 0)
		{
			free(shell.input);
			break ;
		}
		if (ft_strcmp(shell.input, "") != 0)
			add_history(shell.input);
		shell.cmds = ft_split(shell.input, ' ');
		free(shell.input);
		shell.input = NULL;
		if (shell.cmds[0] == NULL)
		{
			free(shell.cmds);
			continue ;
		}
		if (ft_strcmp(shell.cmds[0], "echo") == 0)
			ft_echo(shell.cmds);
		else if (ft_strcmp(shell.cmds[0], "cd") == 0)
			ft_cd(shell.cmds, &shell);
		else if (ft_strcmp(shell.cmds[0], "export") == 0)
			ft_export(shell.cmds, &shell);
		else if (ft_strcmp(shell.cmds[0], "env") == 0)
			ft_env(shell.cmds, &shell);
		else if (ft_strcmp(shell.cmds[0], "unset") == 0)
		{
			ft_unset(&shell.env, shell.cmds[1]);
			ft_unset(&shell.exp, shell.cmds[1]);
		}
		else
			ft_exec(shell.cmds, &shell);
		free(shell.cmds);
		shell.cmds = NULL;
	}

	free_all(&shell);
	return (0);
}
