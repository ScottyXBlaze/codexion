/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 22:14:42 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:29:48 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_scheduler(t_heap2 *heap)
{
	heap->array = malloc(sizeof(t_coder *) * 2);
	if (!heap->array)
		return (0);
	heap->count = 0;
	if (pthread_mutex_init(&heap->mutex, NULL))
	{
		free(heap->array);
		return (0);
	}
	return (1);
}

void	destroy_scheduler(t_heap2 *fifo)
{
	free(fifo->array);
	pthread_mutex_destroy(&fifo->mutex);
}

void	scheduler_push(t_heap2 *scheduler, t_coder *coder)
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

void	scheduler_pop(t_heap2 *scheduler)
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

int	is_my_turn(t_heap2 *scheduler, t_coder *coder)
{
	int	my_turn;

	pthread_mutex_lock(&scheduler->mutex);
	my_turn = (scheduler->count > 0 && scheduler->array[0] == coder);
	pthread_mutex_unlock(&scheduler->mutex);
	return (my_turn);
}

int	lock_dongle_orig(t_coder *coder, t_dongle *dongle)
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
