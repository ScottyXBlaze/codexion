/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:23 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 19:09:27 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	set_memory(t_all *all)
{
	all->dongles = malloc(sizeof(t_dongle) * all->params.nb_coders + 1);
	if (!all->dongles)
		return (0);
	all->dongles = memset(all->dongles, 0, sizeof(t_dongle)
			* all->params.nb_coders + 1);
	return (1);
}

static int	init_dongle(t_dongle *dongle)
{
	dongle->available_at = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL))
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL))
		return (0);
	if (!init_fifo(&dongle->fifo))
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	if (!init_heap(&dongle->edf))
	{
		pthread_mutex_destroy(&dongle->fifo.mutex);
		free(dongle->fifo.array);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	init_dongles(t_all *all)
{
	int	i;

	i = 0;
	if (!set_memory(all))
	{
		destroy_all(all);
		return (0);
	}
	while (i < all->params.nb_coders)
	{
		if (!init_dongle(&all->dongles[i]))
		{
			destroy_all(all);
		}
		i++;
	}
	return (1);
}
