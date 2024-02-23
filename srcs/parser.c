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

void	new_input(t_shell *shell)
{
	t_token	*token;
	char	*input;

	/* while (1)
	{
		input = readline("> ");
		break;
	} */
}

int	validation(t_shell *shell)
{
	t_token	*token;

	token = shell->token;
	while (token)
	{
		if (token->type == S_QUOTE || token->type == D_QUOTE)
			token->type = WORD;
		if (token->type == BACKSLASH)
			if (token->next == NULL)
				new_input(shell);
		if (token->type == PIPE || token->type == AND || token->type == OR)
		{
			if (token->next == NULL)
				new_input(shell);
			if (token->next && token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Operator not followed by any command\n");
				return (1);
			}
		}
		if (token->type == REDIR_OUT || token->type == REDIR_IN || token->type == REDIR_APPEND || token->type == REDIR_HEREDOC)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Redirection not followed by any file\n");
				return (1);
			}
		}
		if (token->next != NULL)
			token = token->next;
		else
			break ;
	}
	return (0);
}

int	parsing(t_shell *shell)
{
	t_token	*temp;
	int		i;
	int		count_cmds_for_pipe[2];

	temp = shell->token;
	i = 0;
	count_cmds_for_pipe[0] = 0;
	count_cmds_for_pipe[1] = 0;
	while (temp != NULL)
	{
		if (temp->type == PIPE)
			count_cmds_for_pipe[1] = count_cmds_for_pipe[0];
		else if (temp->type == WORD)
		{
			count_cmds_for_pipe[0]++;
			i++;
		}
		temp = temp->next;
	}
	shell->cmds->cmds = (char **)malloc(sizeof(char *) * (i + 2));
	shell->cmds->type = (int *)malloc(sizeof(int) * (i + 1));
	shell->cmds->count_cmds = count_cmds_for_pipe[1];
	temp = shell->token;
	i = 0;
	while (temp != NULL)
	{
		shell->cmds->type[i] = temp->type;
		if (temp->type == WORD)
			shell->cmds->cmds[i++] = ft_strdup(temp->value);
		if (!temp->next->next)
			break ;
		temp = temp->next;
	}
	shell->cmds->cmds[i] = NULL;
	return (0);
}

int process_tokens(t_shell *shell)
{
	t_token	*token;

	token = shell->token;
	while (token->next)
	{
		if (token->same_word == true && token->next != NULL && token->next->next != NULL)
		{
			token->next->value = ft_strjoin(token->value, token->next->value);
			token = token->prev;
			token->next = token->next->next;
			token->next->next->prev = token->next;
			token->next->prev = token;
		}
		if (token->next->next != NULL)
			token = token->next;
		else
			break ;
	}
	return (0);
}
void	print_cmds(char **cmds)
{
	int		i;

	i = 0;
	while (cmds[i])
	{
		printf("Cmds[%d]: %s\n", i, cmds[i]);
		i++;
	}
}

int	check_valid_pipes(t_shell *shell)
{
	t_token	*token;

	token = shell->token;
	while (token->next != NULL)
	{
		if (token->type == PIPE)
		{
			if (token->prev == NULL || (token->next && token->next->value == NULL))
			{
				printf("Error: Pipe not followed by any command\n");
				return (1);
			}
		}
		token = token->next;
	}
}

int	count_pipes(t_shell *shell)
{
	t_token	*token;
	int		i;

	token = shell->token;
	i = 0;
	while (token->next != NULL)
	{
		if (token->type == PIPE)
			i++;
		token = token->next;
	}
	return (i);
}

int	**create_pipes(t_shell *shell, int nb_pipes)
{
	int	**pipes;
	int	i;

	i = -1;
	pipes = (int **)malloc(sizeof(int *) * (nb_pipes + 1));
	while (++i < nb_pipes)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		pipe(pipes[i]);
	}
	pipes[i] = NULL;
	return (pipes);
}

int	process_pipes(t_shell *shell)
{
	t_pipe	*pipe;
	int	i;

	if (check_valid_pipes(shell) == 1)
		return (1);
	i = count_pipes(shell);
	shell->pipe = (t_pipe *)malloc(sizeof(t_pipe) * (i + 1));
	shell->pipe->pipe = create_pipes(shell, i);
	pipe = shell->pipe;
	pipe = init_pipe(pipe);
	if (pipe->cmds == NULL)
		pipe->cmds = (char **)malloc(sizeof(char *) * (shell->cmds->count_cmds + 1));
	i = 0;
	pipe->index--;
	while (++pipe->index < shell->cmds->count_cmds)
	{
		if (shell->cmds->type[pipe->index] != WORD)
			i++;
		else
			pipe->cmds[pipe->index] = shell->cmds->cmds[pipe->index + i];
	}
	print_cmds(pipe->cmds);
	return (0);
}

int	parser(t_shell *shell)
{
	if (lexical(shell->cmds->input , shell) == 1)
		return (1);
	if (process_tokens(shell) == 1)
		return (1);
	if (validation(shell) == 1)
		return (1);
	if (parsing(shell) == 1)
		return (1);
	if (process_pipes(shell) == 1)
		return (1);
	print_token(shell->token);
	print_cmds(shell->cmds->cmds);
	return (0);
}
