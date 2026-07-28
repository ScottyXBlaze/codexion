/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:37:43 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/28 22:04:04 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_heap *heap)
{
	heap->array = malloc(sizeof(t_coder *) * 2);
	if (!heap->array)
		return (0);
	heap->size = 0;
	heap->capacity = 2;
	if (pthread_mutex_init(&heap->mutex, NULL))
	{
		free(heap->array);
		return (0);
	}
	return (1);
}

void	destroy_heap(t_heap *heap)
{
	free(heap->array);
	pthread_mutex_destroy(&heap->mutex);
}

int	is_my_turn_edf(t_heap *edf, t_coder *coder)
{
	int	my_turn;

	my_turn = 0;
	pthread_mutex_lock(&edf->mutex);
	if (edf->size > 0 && edf->array[0] == coder)
		my_turn = 1;
	pthread_mutex_unlock(&edf->mutex);
	return (my_turn);
}

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
