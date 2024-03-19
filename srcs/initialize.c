/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_env(t_shell *shell, char **envp)
{
	t_env *temp;
	t_env *new_node;

	temp = NULL;
	while (envp[0])
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		new_node->key = ft_until_char((const char *)*envp, '=');
		new_node->value = ft_strrchr((const char *)*envp, '=') + 1;
		if (temp == NULL)
		{
			shell->env = new_node;
			temp = shell->env;
		}
		else
		{
			temp->next = new_node;
			temp = temp->next;
		}
		(envp)++;
	}
	temp->next = NULL;
}

void	init_export(t_shell *shell, char **envp)
{
	t_env *temp;
	t_env *new_node;

	temp = NULL;
	while (envp[0])
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		new_node->key = ft_until_char((const char *)*envp, '=');
		new_node->value = ft_strrchr((const char *)*envp, '=') + 1;
		if (temp == NULL)
		{
			shell->exp = new_node;
			temp = shell->exp;
		}
		else
		{
			if (ft_strncmp(envp[0], "_=", 2) == 0)
			{
				envp++;
				continue ;
			}
			temp->next = new_node;
			temp = temp->next;
		}
		envp++;
	}
	sort_list(shell->exp);
	temp->next = NULL;

}

t_token	*init_token(t_token *token)
{
	if (token)
		free(token);
	token = (t_token *)malloc(sizeof(t_token));
	token->type = 0;
	token->error = 0;
	token->prev = NULL;
	token->next = NULL;
	token->value = NULL;
	token->same_word = false;
	token->delimiter[S_QUOTE] = false;
	token->delimiter[D_QUOTE] = false;
	token->delimiter[PARENTHESIS] = false;
	return (token);
}

t_cmds	*init_cmds(t_cmds *cmds)
{
	cmds = (t_cmds *)malloc(sizeof(t_cmds));
	cmds->cmds = NULL;
	cmds->input = NULL;
	cmds->count_cmds = 0;
	return (cmds);
}

t_pipe	*init_pipe(t_pipe *pipe)
{
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	pipe->fd_out = dup(STDOUT_FILENO);
	pipe->fd_in = dup(STDIN_FILENO);
	pipe->index = 0;
	pipe->cmds = NULL;
	return (pipe);
}

void	init_redir(t_redir **redir)
{
	*redir = (t_redir *)malloc(sizeof(t_redir));
	(*redir)->file = NULL;
	(*redir)->type = 0;
}

void	init_shell(t_shell *shell, char **envp)
{
	init_env(shell, envp);
	init_export(shell, envp);
	init_redir(&shell->redir);
	shell->pipe = NULL; //init_pipe(shell->pipe);
	shell->cmds = init_cmds(shell->cmds);
	shell->token = init_token(shell->token);
}
