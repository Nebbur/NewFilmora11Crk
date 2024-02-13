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
	token = (t_token *)malloc(sizeof(t_token));
	token->value = NULL;
	token->type = 0;
	token->quote[S_QUOTE] = false;
	token->quote[D_QUOTE] = false;
	token->error = 0;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void	init_shell(t_shell *shell, char **envp)
{
	char **env;

	env = envp;
	init_env(shell, env);
	init_export(shell, env);
	shell->token = init_token(shell->token);
}

