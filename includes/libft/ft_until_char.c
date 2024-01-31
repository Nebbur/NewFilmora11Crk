/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_until_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 20:17:05 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 20:17:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char*	ft_until_char(const char* str, char caractere)
{
	const char	*index;
	size_t		len_str;
	char		*substring;

	if (str == NULL)
		return NULL;
	index = ft_strchr(str, caractere);
	if (index != NULL)
		len_str = (size_t)(index - str);
	else
		len_str = ft_strlen(str);
	substring = (char*)malloc(len_str + 1);
	ft_strncpy(substring, str, len_str);
	substring[len_str] = '\0';
	return (substring);
}
