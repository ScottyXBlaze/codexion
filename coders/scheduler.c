/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 22:14:42 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:56:21 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	scheduler_push(t_heap *scheduler, t_coder *coder)
{
	t_coder	*tmp;

	pthread_mutex_lock(&scheduler->mutex);
	if (coder->all->params.scheduler == FIFO)
	{
		if (scheduler->count < 2)
			scheduler->array[scheduler->count] = coder;
		if (scheduler->count < 2)
			scheduler->count++;
	}
	else
	{
		scheduler->array[scheduler->count] = coder;
		scheduler->count++;
		if (scheduler->count == 2)
		{
			if (is_higher_priority(scheduler->array[1], scheduler->array[0]))
			{
				tmp = scheduler->array[0];
				scheduler->array[0] = scheduler->array[1];
				scheduler->array[1] = tmp;
			}
		}
	}
	pthread_mutex_unlock(&scheduler->mutex);
}

static void	scheduler_pop(t_heap *scheduler)
{
	pthread_mutex_lock(&scheduler->mutex);
	if (scheduler->count > 0)
	{
		scheduler->array[0] = scheduler->array[1];
		scheduler->array[1] = NULL;
		scheduler->count--;
	}
	pthread_mutex_unlock(&scheduler->mutex);
}

static int	is_my_turn(t_heap *scheduler, t_coder *coder)
{
	int	my_turn;

	pthread_mutex_lock(&scheduler->mutex);
	my_turn = (scheduler->count > 0 && scheduler->array[0] == coder);
	pthread_mutex_unlock(&scheduler->mutex);
	return (my_turn);
}

int	lock_dongle(t_coder *coder, t_dongle *dongle)
{
	long int	remaining_cooldown;

	scheduler_push(&dongle->scheduler, coder);
	pthread_mutex_lock(&dongle->mutex);
	while (is_running(coder->all))
	{
		if (is_my_turn(&dongle->scheduler, coder))
		{
			if (can_take_dongle(coder->all, dongle))
				return (scheduler_pop(&dongle->scheduler), 1);
			remaining_cooldown = dongle->available_at - get_time(coder->all);
			pthread_mutex_unlock(&dongle->mutex);
			if (remaining_cooldown > 0)
			{
				ft_sleep(remaining_cooldown, coder->all);
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
