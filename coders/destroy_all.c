/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:30:13 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/24 14:22:38 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_all(t_all *all)
{
	pthread_mutex_destroy(&all->running_mutex);
	pthread_mutex_destroy(&all->message_mutex);
	pthread_mutex_destroy(&all->start_mutex);
	pthread_cond_destroy(&all->start_cond);
	destroy_dongles(all);
	destroy_coders(all->coders);
}
