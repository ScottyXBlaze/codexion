/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:52:03 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/26 22:46:08 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_isnumber(char c);
static int	ft_isspace(char c);
static char	*remove_unnecessary(char *str);

int	validate_int(char *str)
{
	int	i;
	int	len;

	i = 0;
	str = remove_unnecessary(str);
	if (!str[i])
		printf("Invalid parameter: '%s'\n", str);
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isnumber(str[i]))
		{
			printf("Should be number: '%s'\n", str);
			return (0);
		}
		i++;
	}
	len = i;
	if (len > 10 || (len == 10 && strcmp(str, "2147483647") > 0))
	{
		printf("Number too large: '%s'\n", str);
		return (0);
	}
	return (1);
}

static char	*remove_unnecessary(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] == '0' && ft_isnumber(str[i + 1]))
		i++;
	return (&str[i]);
}

static int	ft_isspace(char c)
{
	return (c == ' ' || (9 <= c && c <= 13));
}

static int	ft_isnumber(char c)
{
	return ('0' <= c && c <= '9');
}

int	validate_scheduler(char *str)
{
	while (ft_isspace(*str))
		str++;
	if (strcmp(str, "fifo") == 0 || strcmp(str, "edf") == 0)
		return (1);
	printf("[ERROR] Invalid scheduler parameter %s\n", str);
	printf("Should be 'fifo' of 'edf'\n");
	return (0);
}
