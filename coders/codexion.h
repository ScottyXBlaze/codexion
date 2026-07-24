/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:38:48 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/24 14:21:37 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# ifndef MAX_CODER
#  define MAX_CODER 500

# endif

typedef struct s_coder	t_coder;

typedef struct s_heap
{
	t_coder				**array;
	int					size;
	int					capacity;
	pthread_mutex_t		mutex;
}						t_heap;

typedef struct s_fifo
{
	t_coder				**array;
	int					head;
	int					tail;
	int					size;
	int					capacity;
	pthread_mutex_t		mutex;
}						t_fifo;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	long int			available_at;
	t_heap				edf;
	t_fifo				fifo;
}						t_dongle;

typedef enum s_scheduler
{
	fifo,
	edf,
}						t_scheduler;

typedef enum s_state
{
	compile,
	debug,
	refactor,
	take_dongle,
	burned_out,
}						t_state;

typedef struct s_params
{
	int					nb_coders;
	int					burnout;
	int					compile;
	int					debug;
	int					refactor;
	int					compiles_required;
	int					dongle_cooldown;
	t_scheduler			scheduler;
}						t_params;

typedef struct s_all
{
	t_params			params;
	long long			start_time;
	int					running;
	pthread_mutex_t		running_mutex;
	pthread_mutex_t		message_mutex;

	pthread_mutex_t		start_mutex;
	pthread_cond_t		start_cond;
	pthread_t			monitor;
	t_dongle			*dongles;
	t_coder				*coders;

}						t_all;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		mutex;

	long long			last_compile;
	int					compile_count;

	int					is_finished;

	t_dongle			*l_dongle;
	t_dongle			*r_dongle;

	t_all				*all;
}						t_coder;

int						ft_isspace(char c);

int						validate_int(char *str);
int						validate_scheduler(char *str);

int						parsers(int argc, char **argv, t_all *all);

int						is_running(t_all *all);
void					stop_simulation(t_all *all);
void					ft_sleep(long long sleep_time, t_all *all);
long int				get_time(t_all *all);

int						init_all(t_all *all);
int						init_dongles(t_all *all);
int						init_fifo(t_fifo *fifo, int capacity);
int						init_heap(t_heap *heap, int capacity);
int						init_coders(t_all *all);

int						destroy_dongles(t_all *all);
void					destroy_all(t_all *all);
void					destroy_fifo(t_fifo *fifo);
void					destroy_heap(t_heap *heap);
int						destroy_coders(t_coder *coders);

void					*coder_loop(void *args);
void					*monitor_loop(void *args);

int						can_take_dongle(t_all *all, t_dongle *dongle);
void					unlock_dongle(t_all *all, t_dongle *dongle);
int						lock_dongle(t_coder *coder, t_dongle *dongle);

int						lock_dongle_fifo(t_coder *coder, t_dongle *dongle);
int						lock_dongle_edf(t_coder *coder, t_dongle *dongle);

void					print_state(t_coder *coder, t_state state);

void					coder_compile(t_coder *coder);
void					coder_debug(t_coder *coder);
void					coder_refactor(t_coder *coder);


void					heap_push(t_heap *heap, t_coder *coder);
t_coder					*heap_peek(t_heap *heap);
void					heap_pop(t_heap *heap);

#endif
