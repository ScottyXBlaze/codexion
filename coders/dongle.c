/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 21:46:57 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:52:10 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void		unlock_dongle(t_all *all, t_dongle *dongle);
static void	destroy_dongle(t_dongle *dongle);

int	can_take_dongle(t_all *all, t_dongle *dongle)
{
	return (get_time(all) >= dongle->available_at);
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
	pthread_cond_broadcast(&dongle->cond);
}

static void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->mutex);
	pthread_cond_destroy(&dongle->cond);
	free(dongle->scheduler.array);
}
