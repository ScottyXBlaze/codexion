/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:49:13 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/22 14:55:06 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_thread_coders(t_coder *coders);
static int	set_memory(t_all *all);
static void	set_coder(t_all *all, t_coder *coders, int i);

int	init_coders(t_all *all)
{
	int	i;

	i = 0;
	set_memory(all);
	while (i < all->params.nb_coders)
	{
		set_coder(all, all->coders, i);
		if (pthread_mutex_init(&all->coders[i].mutex, NULL))
		{
			destroy_dongles(all);
			return (0);
		}
		i++;
	}
	if (pthread_create(&all->monitor, NULL, monitor_loop, all))
	{
		destroy_all(all);
		return (0);
	}
	if (!init_thread_coders(all->coders))
		return (destroy_dongles(all), 0);
	return (1);
}

static int	set_memory(t_all *all)
{
	all->coders = malloc(sizeof(t_coder) * all->params.nb_coders);
	if (all->coders == NULL)
	{
		destroy_all(all);
		return (0);
	}
	all->coders = memset(all->coders, 0, sizeof(t_coder)
			* all->params.nb_coders);
	return (1);
}

static void	set_coder(t_all *all, t_coder *coders, int i)
{
	coders[i].id = i + 1;
	coders[i].all = all;
	coders[i].l_dongle = &all->dongles[i];
	coders[i].r_dongle = &all->dongles[(i + 1) % all->params.nb_coders];
	coders[i].compile_count = 0;
	coders[i].last_compile = get_time(all);
	coders[i].is_finished = 0;
}

static int	init_thread_coders(t_coder *coders)
{
	int	i;
	int	nb_coders;

	i = 0;
	nb_coders = coders[0].all->params.nb_coders;
	while (i < nb_coders)
	{
		if (pthread_create(&coders[i].thread, NULL, coder_loop, &coders[i]))
			return (0);
		i++;
	}
	return (1);
}

int	destroy_coders(t_coder *coders)
{
	free(coders);
	coders = NULL;
	return (1);
}
