/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 22:14:42 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:52:38 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long int	get_coder_deadline(t_coder *coder)
{
	long int	last;

	pthread_mutex_lock(&coder->mutex);
	last = coder->last_compile;
	pthread_mutex_unlock(&coder->mutex);
	return (last + coder->all->params.burnout);
}

int	is_higher_priority(t_coder *a, t_coder *b)
{
	long int	a_deadline;
	long int	b_deadline;

	a_deadline = get_coder_deadline(a);
	b_deadline = get_coder_deadline(b);
	if (a_deadline != b_deadline)
		return (a_deadline < b_deadline);
	return (a->id < b->id);
}

int	init_scheduler(t_heap *heap)
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

void	destroy_scheduler(t_heap *fifo)
{
	free(fifo->array);
	pthread_mutex_destroy(&fifo->mutex);
}

void	scheduler_push(t_heap *scheduler, t_coder *coder)
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

void	scheduler_pop(t_heap *scheduler)
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

int	is_my_turn(t_heap *scheduler, t_coder *coder)
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
