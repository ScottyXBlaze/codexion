/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 22:54:14 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:55:16 by nyramana         ###   ########.fr       */
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
