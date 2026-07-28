/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:24:45 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/21 13:55:40 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long int	get_time(t_all *all)
{
	struct timeval	tv;
	long long		now;

	if (gettimeofday(&tv, NULL))
		return (-1);
	now = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (now - all->start_time);
}

void	ft_sleep(long long sleep_time, t_all *all)
{
	long long	start;

	start = get_time(all);
	while ((get_time(all) - start) < sleep_time)
	{
		if (!is_running(all))
			break ;
		usleep(500);
	}
}

int	is_running(t_all *all)
{
	int	status;

	pthread_mutex_lock(&all->running_mutex);
	status = all->running;
	pthread_mutex_unlock(&all->running_mutex);
	return (status);
}

void	stop_simulation(t_all *all)
{
	int	i;

	pthread_mutex_lock(&all->running_mutex);
	if (all->running)
		all->running = 0;
	pthread_mutex_unlock(&all->running_mutex);
	i = 0;
	while (i < all->params.nb_coders)
	{
		pthread_cond_broadcast(&all->dongles[i].cond);
		i++;
	}
}
