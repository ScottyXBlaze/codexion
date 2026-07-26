/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:44:58 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/26 23:33:57 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	one_coder_case(t_coder *coder);
static int	reserve_dongle(t_coder *coder);
static void	run_loop(t_coder *coder);
static void	wait_running(t_coder *coder);

void	*coder_loop(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	wait_running(coder);
	if (coder->all->params.nb_coders == 1)
		return (one_coder_case(coder), NULL);
	while (is_running(coder->all))
	{
		if (coder->all->params.compiles_required == 0 || !reserve_dongle(coder))
			break ;
		run_loop(coder);
		if (coder->compile_count >= coder->all->params.compiles_required)
		{
			pthread_mutex_lock(&coder->mutex);
			coder->is_finished = 1;
			pthread_mutex_unlock(&coder->mutex);
			break ;
		}
	}
	return (NULL);
}

static int	one_coder_case(t_coder *coder)
{
	if (!lock_dongle(coder, coder->l_dongle))
		return (0);
	print_state(coder, TAKE_DONGLE);
	while (is_running(coder->all))
		usleep(500);
	unlock_dongle(coder->all, coder->l_dongle);
	return (1);
}

static int	reserve_dongle(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (!lock_dongle(coder, coder->r_dongle))
			return (0);
		print_state(coder, TAKE_DONGLE);
		if (!lock_dongle(coder, coder->l_dongle))
		{
			unlock_dongle(coder->all, coder->r_dongle);
			return (0);
		}
		print_state(coder, TAKE_DONGLE);
	}
	else
	{
		if (!lock_dongle(coder, coder->l_dongle))
			return (0);
		print_state(coder, TAKE_DONGLE);
		if (!lock_dongle(coder, coder->r_dongle))
		{
			unlock_dongle(coder->all, coder->l_dongle);
			return (0);
		}
		print_state(coder, TAKE_DONGLE);
	}
	return (1);
}

static void	run_loop(t_coder *coder)
{
	coder_compile(coder);
	unlock_dongle(coder->all, coder->l_dongle);
	unlock_dongle(coder->all, coder->r_dongle);
	coder_debug(coder);
	coder_refactor(coder);
}

static void	wait_running(t_coder *coder)
{
	while (!is_running(coder->all))
	{
		usleep(500);
	}
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
}
