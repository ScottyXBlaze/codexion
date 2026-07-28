/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 21:55:58 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:21:15 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	edf_push(t_heap *edf, t_coder *coder)
{
	t_coder	*tmp;

	pthread_mutex_lock(&edf->mutex);
	edf->array[edf->size] = coder;
	edf->size++;
	if (edf->size == 2)
	{
		if (is_higher_priority(edf->array[1], edf->array[0]))
		{
			tmp = edf->array[0];
			edf->array[0] = edf->array[1];
			edf->array[1] = tmp;
		}
	}
	pthread_mutex_unlock(&edf->mutex);
}

static void	edf_remove(t_heap *edf, t_coder *coder)
{
	pthread_mutex_lock(&edf->mutex);
	if (edf->size == 0)
	{
		pthread_mutex_unlock(&edf->mutex);
		return ;
	}
	if (edf->array[0] == coder)
	{
		edf->array[0] = edf->array[1];
		edf->array[1] = NULL;
		edf->size--;
	}
	else if (edf->size == 2 && edf->array[1] == coder)
	{
		edf->array[1] = NULL;
		edf->size--;
	}
	pthread_mutex_unlock(&edf->mutex);
}

int	lock_dongle_edf(t_coder *coder, t_dongle *dongle)
{
	long int	cooldown_remaining;

	edf_push(&dongle->edf, coder);
	pthread_mutex_lock(&dongle->mutex);
	while (coder->all->running)
	{
		if (is_my_turn_edf(&dongle->edf, coder))
		{
			if (can_take_dongle(coder->all, dongle))
				return (edf_remove(&dongle->edf, coder), 1);
			cooldown_remaining = dongle->available_at - get_time(coder->all);
			pthread_mutex_unlock(&dongle->mutex);
			if (cooldown_remaining > 0)
			{
				ft_sleep(cooldown_remaining, coder->all);
				pthread_mutex_lock(&dongle->mutex);
				continue ;
			}
			pthread_mutex_lock(&dongle->mutex);
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}
