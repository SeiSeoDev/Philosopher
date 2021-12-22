
#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H

#include <stdlib.h>
#include <pthread.h>
#include "libft.h"
#include <stdio.h>

typedef struct s_mutex
{
	int				data;
	pthread_mutex_t	mutex;
}	t_mutex;

typedef struct s_arg
{
	int				t_eat;
	int				t_die;
	int				c_eat;
	int				t_sleep;
	int				max_eat;
	long	int		time;
	struct s_mutex	dead;
}				t_arg;

typedef struct s_philo
{
	int				id;
	int				eat;
	struct s_mutex	fork;
	struct s_mutex	*next_fork;
	int				eat_c;
	struct s_mutex	full;
	long int		last_eat;
	pthread_t		thread;
	int 			dead;
	t_arg			*arg;

}				t_philo;

typedef struct s_env
{
	int				count;
	int				t_eat;
	int				t_die;
	int				t_sleep;
	int				c_eat;
	t_philo			*philo;
	t_arg			*arg;
}	t_env;

int			print(int id, char *msg, t_philo *philo);
long int	actual_time(void);
int			check_death(t_philo *philo);
int			a_table(t_philo *philo);
int			time_sleep(t_philo *philo);
int		check_full(t_philo *philo);
#endif