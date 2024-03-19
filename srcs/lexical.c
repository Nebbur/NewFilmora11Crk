/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_special_char(char c);
char	*get_token_type(int type);
int	ft_isnot_theend(char *input, int i);
int	ft_isnotspace(char c);
bool	is_same_word(char *input, int i);
void remove_last_node(t_token **head) ;
bool	next_command_exists(char *input);

static bool	is_special_char(char c)
{
	if (c == '|' || c == ';' || c == '\'' || c == '\"' || \
	c == '>' || c == '<' || c == '$' || c == '&' || \
	c == '(' || c == ')' || c == '\\')
		return (true);
	return (false);
}

void	handle_backslash(t_token *token, char *input, int *i, char *special_char[2])
{
	int	begin;
	int	j;
	int	count_char;
	bool	trig[2];

	token->type = BACKSLASH;
	trig[1] = false;
	if (input[*i + 1] == '\0')
		token->error = 1;		// Error: Backslash isn't followed by anything
	if (*i % 2 == 0)
		trig[0] = true;
	else
		trig[0] = false;

	if (ft_strchr(*special_char, input[*i + 1]) != NULL)
	{
		begin = *i;
		count_char = 0;
		j = -1;
		while (input[++j] && input[j] > 32 && trig[1] == false)
		{
			if (input[j] == '\\')
			{
				if(j % 2 == 0 && input[j + 2] && input[j + 2] != '\\')
					trig[1] = !trig[1] ;
				j++;
				if (ft_strchr(*special_char, input[*i]) != NULL)
					count_char++;
			}
			else
				count_char++;
		}
		token->value = ft_calloc(1, sizeof(char *) * (count_char + 1));;
		trig[1] = false;
		(*i)--;
		while (input[++(*i)] && input[*i] > 32 && trig[1] == false)
		{
			if (input[*i] == '\\')
			{
				if (trig[0] == true)
					if(*i % 2 == 0 && input[*i + 2] && input[*i + 2] != '\\')
						trig[1] = !trig[1] ;
				if (trig[0] == false)
					if(*i % 2 != 0 && input[*i + 2] && input[*i + 2] != '\\')
						trig[1] = !trig[1] ;
				if (ft_strchr(*special_char, input[++(*i)]) != NULL)
					token->value = ft_strjoin(token->value, ft_substr(input, *i, 1));
				else if (*special_char[1] == '1')
					token->value = ft_strjoin(token->value, ft_substr(input, (*i), 1));
			}
			else
				token->value = ft_strjoin(token->value, ft_substr(input, *i, 1));
		}
		if (is_same_word(input, (*i)) == true)
			token->same_word = true;
		(*i)--;
		printf("Input: %s\n", input + *i);
	}
	else
		printf("Error: Backslash isn't followed by anything\n");
}

bool	is_same_word(char *input, int i)
{
	if (next_command_exists(input + (i + 1)) == false)
	{
		return (false);
	}
	if (input[i] && input[i + 1] && (input[i + 1] == '\"' || input[i + 1] == '\'') || \
	ft_isnotspace(input[i + 1]) == 1)
		return (true);
	else if (input[i] && input[i + 1] && input[i + 1] == '\\' && input[i + 2] && \
	(input[i + 2] == '\'' || input[i + 2] == '\\' || input[i + 2] == '\"'))
		return (true);
	return (false);
}

int	ft_isnotspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (0);
	return (1);
}

int	ft_isnot_theend(char *input, int i)
{
	if (input[i] && input[i + 1] && \
	input[i] == '\\' && input[i + 1] != '\"')
	{
		if (input[i + 2] && input[i + 2] == '\"')
			return (3);
		return (2);
	}
	return (1);
}

void	handle_backslah_in_quote(t_token *token, char *input, int *i_beg[2], char quote_char)
{
	char	*temp;
	char	*temp2;
	int		j;
	int		k;

	//printf("Handle Backslash in quote\n");
	j = -1;
	while (token->value[++j])
		;
	if (token->value[j - 1] == quote_char)
		token->value[j - 1] = '\0';
	j = -1;
	while (token->value[++j] && quote_char == '\"' && token->value[j] != quote_char)
	{
		if (token->value[j + 1] && token->value[j] == '\\')
		{
			temp = ft_substr(token->value, j, 2);
			temp[2] = '\0';
		//	printf("Temp value: %s\n", temp);
			temp2 = ft_strdup(token->value + 2);
		//	printf("Temp2 value: %s\n", temp2);
			handle_backslash(token, temp, &j, (char *[]){"\\\"\'$&|<>();?*!`", "1"});
			token->value = ft_strjoin(token->value, temp2);
		//	printf("Token value after: %s\n", token->value);
			free(temp);
			free(temp2);
			/* printf("Temp value: %s\n", temp);
			printf("Before while\n");
			printf("Token value: %s\n", token->value);
			//while (token->value[j + k] && token->value[j + k + 1] && \
			//ft_isnotspace(token->value[j + k]))
			//	k = k + ft_isnot_theend(token->value, j + k);
			temp2 = ft_strdup(token->value + j + 2);
			printf("Temp2: %s\n", temp2);
			printf("Token value before: %s\n", token->value);
			handle_backslash(token, temp, &j, (char *[]){"\\\"$`", "1"});
			printf("Token value after: %s\n", token->value);
			temp = ft_strjoin(temp, token->value);
			printf("Temp: %s\n", temp);
			token->value = ft_strjoin(temp, temp2);
			free(temp);
			free(temp2); */
		}
	}
}

void handle_quote(int quote_type, int *i, char *input, t_token *token)
{
	char	*temp;
	char	quote_char;
	int		begin;
	int		len;
	int		j;

	len = 0;
	token->type = WORD;
	begin = *i + 1;
	quote_char = input[*i];
	if (input[*i + 1] && input[*i + 1] == quote_char && input[*i] == quote_char)
	{
		token->type = EMPTY;
		(*i) = *i + 2;
		return ;
	}
	if (input[*i + 1] == '$')
	{
		while (input[++(*i)])
			if (input[*i] == quote_char)
				break ;
		token->type = ENV;
		token->value = ft_substr(input, begin, *i - begin);
		token->delimiter[quote_type] = true;
		return ;
	}
	if (quote_type == S_QUOTE && input[*i] == quote_char)
	{
		while (input[++(*i)] && input[*i] != quote_char)
			;
		token->value = ft_substr(input, begin, *i - begin);
		if (is_same_word(input, (*i)) == true && is_special_char(input[*i + 1]) == false)
				token->same_word = true;
		(*i)--;
		return ;
	}
	while (input[++(*i)])
	{
		if (quote_type == D_QUOTE && input[*i + 1] && \
		input[*i] != '\\' && input[*i + 1] == quote_char)
		{
			len = 1;
			break;
		}
		else if (input[*i] && input[*i + 1] && input[*i + 2] && \
		input[*i] == '\\' && input[*i + 1] == '\\' && input[*i + 2] == quote_char)
		{
			len = 3;
			break ;
		}
	}
	token->value = ft_substr(input, begin, (*i  + len) - begin);
	if ((is_same_word(input, (*i)) == true && input[*i + 2] && /*ft_isalnum(input[*i + 2]) != 0) && */
	(input[*i + 2] == '\"' || input[*i + 2] == '\'')))
		token->same_word = true;
}

void	delete_node(t_token *token)
{
	t_token	*temp;

	temp = token;
	if (temp != NULL)
	{
		temp->delimiter[S_QUOTE] = false;
		temp->delimiter[D_QUOTE] = false;
		free(temp->value);
		free(temp);
		temp = NULL;
	}
}

t_token	*special_char(char *input, t_token *token, int *i, bool delimiter_flag[2])
{
	int	begin;

	if (input[*i] == ' ')
		(*i)++;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (input[*i] == '\'')
		{
			delimiter_flag[S_QUOTE] = !delimiter_flag[S_QUOTE];
			if (delimiter_flag[S_QUOTE] == false)
			{
				if (token->value == NULL)
				{
					token->prev->delimiter[S_QUOTE] = delimiter_flag[S_QUOTE];
					token = token->prev;
				}
				token->delimiter[S_QUOTE] = true;
				return (token);
			}
			handle_quote(S_QUOTE, i, input, token);
		}
		else
		{
			delimiter_flag[D_QUOTE] = !delimiter_flag[D_QUOTE];
			if (delimiter_flag[D_QUOTE] == false)
			{
				if (token->value == NULL)
				{
					token->prev->delimiter[D_QUOTE] = delimiter_flag[D_QUOTE];
					token = token->prev;
				}
				token->delimiter[D_QUOTE] = true;
				return (token);
			}
			handle_quote(D_QUOTE, i, input, token);
		}
	}
	else if (input[*i] == '$')
	{
		begin = *i;
		while (input[++(*i)] && input[*i] > 32)
			;
		token->type = ENV;
		token->value = ft_substr(input, begin, *i - begin);
	}
	else if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			token->type = OR;
			token->value = ft_strdup("||");
			(*i)++;
		}
		else
		{
			token->type = PIPE;
			token->value = ft_strdup("|");
		}
	}
	else if (input[*i] == ';')
	{
		token->type = SEMICOLON;
		token->value = ft_strdup(";");
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			token->value = ft_strdup(">>");
			token->type = APPEND;
		}
		else
		{
			token->type = REDIR_OUT;
			token->value = ft_strdup(">");
		}
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			token->type = HEREDOC;
			(*i)++;
			token->value = ft_strdup("<<");
		}
		else
		{
			token->type = REDIR_IN;
			token->value = ft_strdup("<");
		}
	}
	else if (input[*i] == '&')
	{
		if (input[*i + 1] == '&')
		{
			token->type = AND;
			token->value = ft_strdup("&&");
			(*i)++;
		}
		else
		{
			token->type = AMPERSAND;
			token->value = ft_strdup("&");
		}
	}
	else if (input[*i] == '\\')
		handle_backslash(token, input, i, (char *[]){"\\\"\'$&|<>();?*!`", "0"});
	else if (input[*i] == '(')
	{
		token->delimiter[PARENTHESIS] = true;
		token->type = WORD;
		int	counter_parenthesis;
		counter_parenthesis = 1;
		begin = *i;
		while (input[++(*i)] && counter_parenthesis != 0)
		{
			if (input[*i] == '(')
				counter_parenthesis++;
			else if (input[*i] == ')')
				counter_parenthesis--;
		}
		token->value = ft_substr(input, begin, *i - begin);
		*i = *i - 1;
	}
	return (token);
}

void remove_last_node(t_token **head)
{
	t_token	*current;

	if (*head == NULL)
		return;
	current = *head;
	if (current->next == NULL)
	{
		free(current->value);
		free(current);
		*head = NULL;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->prev->next = NULL;
		free(current->value);
		free(current);
	}
}

bool	next_command_exists(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '\0')
		return (false);
	while (input[i] && input[i] < 33)
		i++;
	if (input[i] == '\0')
		return (false);
	return (true);
}

int	lexical(char *input , t_shell *shell)
{
	t_token	*token;
	int		i;
	int		j;
	bool	is_echo;
	bool	delimiter_flag[2];

	j = -1;
	i = -1;
	is_echo = false;
	token = shell->token;
	delimiter_flag[S_QUOTE] = false;
	delimiter_flag[D_QUOTE] = false;
	token->delimiter[S_QUOTE] = false;
	token->delimiter[D_QUOTE] = false;
	token->delimiter[PARENTHESIS] = false;
	while (input[++i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		if (is_special_char(input[i]) == true)
		{
			token = special_char(input, token, &i, delimiter_flag);
			if (next_command_exists(input + i + 1) == true)
			{
				token->next = init_token(token->next);
				token->next->prev = token;
				token->next->delimiter[S_QUOTE] = delimiter_flag[S_QUOTE];
				token->next->delimiter[D_QUOTE] = delimiter_flag[D_QUOTE];
				token = token->next;
			}
			else
				token->next = NULL;
		}
		else
		{
			token->type = WORD;
			token->value = ft_calloc(1, sizeof(char *));
			while (input[i] && input[i] > 32 && is_special_char(input[i]) == false)
				token->value = ft_strjoin(token->value, ft_substr(input, i++, 1));
			i--;
			if (ft_strcmp(token->value, "echo") == 0)
				is_echo = true;
			if (is_echo == true && is_same_word(input, i) == true)
				token->same_word = true;
			if (next_command_exists(input + i + 1) == true)
			{
				token->next = init_token(token->next);
				token->next->prev = token;
				token = token->next;
			}
			else
				token->next = NULL;
		}
	}
	print_token(shell->token);
	printf("%s______________________________________\n", GREEN);
	printf("|                                    |\n");
	printf("%s|_-_-_-_-_-_%sLEAVING LEXICAL%s-_-_-_-_-_|%s\n", GREEN, RED, GREEN, RESET);
	printf("%s|____________________________________|\n", GREEN);
	return (0);
}

void	print_token(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp != NULL)
	{
		printf("%s_______________________\n", RED);
		char *delimiter;
		if (temp->delimiter[S_QUOTE] == true)
			delimiter = "SINGLE";
		else if (temp->delimiter[D_QUOTE] == true)
			delimiter = "DOUBLE";
		else if (temp->delimiter[PARENTHESIS] == true)
			delimiter = "PARENTHESIS";
		else
			delimiter = "NONE";
		printf("%s_______________________\n", RED);
		printf("|%s Value: |%s%s%s|\n", BLUE, YELLOW, temp->value, BLUE);
		printf("%s|%s Type: %s%s\n", RED, BLUE, YELLOW, get_token_type(temp->type));
		printf("%s|%s Delimiter: %s%s\n", RED, BLUE, YELLOW, delimiter);
		printf("%s|%s Same word: %s%s\n", RED, BLUE, YELLOW, (temp->same_word == true) ? "TRUE" : "FALSE");
		printf("%s|______________________\n", RED);
		printf("%s\n", RESET);
		temp = temp->next;
	}
}

char	*get_token_type(int type)
{
	if (type == S_QUOTE)
		return ("S_QUOTE");
	else if (type == D_QUOTE)
		return ("D_QUOTE");
	else if (type == BACKSLASH)
		return ("BACKSLASH");
	else if (type == AMPERSAND)
		return ("AMPERSAND");
	else if (type == OR)
		return ("OR");
	else if (type == AND)
		return ("AND");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == SEMICOLON)
		return ("SEMICOLON");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == ENV)
		return ("ENV");
	else if (type == PARENTHESIS)
		return ("PARENTHESIS");
	else if (type == WORD)
		return ("WORD");
	return ("ERROR");
}
