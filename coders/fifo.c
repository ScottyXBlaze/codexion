/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 00:36:57 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/27 00:38:04 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo_push(t_fifo *fifo, t_coder *coder)
{
	pthread_mutex_lock(&fifo->mutex);
	if (fifo->count < 2)
	{
		fifo->array[fifo->count] = coder;
		fifo->count++;
	}
	pthread_mutex_unlock(&fifo->mutex);
}

void	fifo_pop(t_fifo *fifo)
{
	pthread_mutex_lock(&fifo->mutex);
	if (fifo->count > 0)
	{
		fifo->array[0] = fifo->array[1];
		fifo->array[1] = NULL;
		fifo->count--;
	}
	pthread_mutex_unlock(&fifo->mutex);
}

int	is_my_turn_fifo(t_fifo *fifo, t_coder *coder)
{
	int	my_turn;

	pthread_mutex_lock(&fifo->mutex);
	my_turn = (fifo->count > 0 && fifo->array[0] == coder);
	pthread_mutex_unlock(&fifo->mutex);
	return (my_turn);
}

int	lock_dongle_fifo(t_coder *coder, t_dongle *dongle)
{
	long int	remaining_cooldown;

	fifo_push(&dongle->fifo, coder);
	while (is_running(coder->all))
	{
		if (is_my_turn_fifo(&dongle->fifo, coder))
		{
			pthread_mutex_lock(&dongle->mutex);
			if (can_take_dongle(coder->all, dongle))
			{
				fifo_pop(&dongle->fifo);
				return (1);
			}
			remaining_cooldown = dongle->available_at - get_time(coder->all);
			pthread_mutex_unlock(&dongle->mutex);
			if (remaining_cooldown > 0)
			{
				ft_sleep(remaining_cooldown, coder->all);
				continue ;
			}
		}
		usleep(500);
	}
	return (0);
}
