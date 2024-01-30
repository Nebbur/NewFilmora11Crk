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

void	init_shell(t_shell *shell, char **envp)
{
	char **env;

	env = envp;
	init_env(shell, env);
	init_export(shell, env);
	/* while(shell->exp != NULL)
	{
		//exp sorted
		printf("%s=%s\n", shell->exp->key, shell->exp->value);
		shell->exp = shell->exp->next;
	} */
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
