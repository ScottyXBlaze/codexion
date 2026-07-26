/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 21:46:57 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/26 23:33:16 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	unlock_dongle(t_all *all, t_dongle *dongle);
static void	destroy_dongle(t_dongle *dongle);

int	can_take_dongle(t_all *all, t_dongle *dongle)
{
	return (get_time(all) >= dongle->available_at);
}

int	lock_dongle(t_coder *coder, t_dongle *dongle)
{
	if (coder->all->params.scheduler == FIFO)
		return (lock_dongle_fifo(coder, dongle));
	return (lock_dongle_edf(coder, dongle));
}


int	destroy_dongles(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->params.nb_coders)
	{
		destroy_dongle(&all->dongles[i]);
		i++;
	}
	free(all->dongles);
	return (1);
}

void	unlock_dongle(t_all *all, t_dongle *dongle)
{
	dongle->available_at = get_time(all) + all->params.dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}


static void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->mutex);
	pthread_mutex_destroy(&dongle->fifo.mutex);
	free(dongle->fifo.array);
	pthread_mutex_destroy(&dongle->edf.mutex);
	free(dongle->edf.array);
}

