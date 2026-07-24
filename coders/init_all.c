/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:40:53 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/24 14:21:44 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static void	join_coders(t_coder *coders, int nb_coders);

int	init_all(t_all *all)
{
	if (pthread_mutex_init(&all->message_mutex, NULL))
	{
		destroy_all(all);
		return (0);
	}
	if (pthread_mutex_init(&all->running_mutex, NULL))
	{
		destroy_all(all);
		return (0);
	}
	if (pthread_mutex_init(&all->start_mutex, NULL))
	{
		destroy_all(all);
		return (0);
	}

	if (pthread_cond_init(&all->start_cond, NULL))
	{
		destroy_all(all);
		return (0);
	}
	all->start_time = get_time(all);
	ft_sleep(100000, all);
	pthread_mutex_lock(&all->running_mutex);
	all->running = 1;
	pthread_mutex_unlock(&all->running_mutex);
	join_coders(all->coders, all->params.nb_coders);
	pthread_join(all->monitor, NULL);
	return (1);
}

static void	join_coders(t_coder *coders, int nb_coders)
{
	int	i;

	i = 0;
	while (i < nb_coders)
	{
		if (pthread_join(coders[i].thread, NULL))
			return ;
		i++;
	}
}
