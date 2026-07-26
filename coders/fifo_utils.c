/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 00:36:11 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/27 00:36:38 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_fifo(t_fifo *fifo)
{
	fifo->array = malloc(sizeof(t_coder *) * 2);
	if (!fifo->array)
		return (0);
	fifo->count = 0;
	if (pthread_mutex_init(&fifo->mutex, NULL))
	{
		free(fifo->array);
		return (0);
	}
	return (1);
}

void	destroy_fifo(t_fifo *fifo)
{
	free(fifo->array);
	pthread_mutex_destroy(&fifo->mutex);
}
