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

void init_env(t_shell *shell, char **envp)
{
	t_env *temp = NULL;
	t_env *new_node;

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

void	init_shell(t_shell *shell, char **envp)
{
	init_env(shell, envp);
}

void	free_all(t_shell *shell)
{
	printf("Free to All\n");
	while (shell->env != NULL)
	{
		free(shell->env->key);
		free(shell->env);
		shell->env = shell->env->next;
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

	free_all(&shell);
	return (0);
}
