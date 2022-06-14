/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:51:08 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 14:52:35 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <pthread.h>
# include "libft.h"
# include <stdio.h>
# include <sys/time.h>

typedef struct s_mutex
{
	long int		data;
	pthread_mutex_t	mutex;
}	t_mutex;

typedef struct s_arg
{
	int				t_eat;
	int				t_die;
	int				c_eat;
	int				t_sleep;
	int				max_eat;
	long int		time;
	struct s_mutex	dead;
	struct s_mutex	print;
}				t_arg;

typedef struct s_philo
{
	int				id;
	int				eat;
	struct s_mutex	fork;
	struct s_mutex	*next_fork;
	int				eat_c;
	struct s_mutex	full;
	struct s_mutex	last_eat;
	pthread_t		thread;
	int				dead;
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
void		a_table(t_philo *philo);
int			time_sleep(t_philo *philo);
int			check_full(t_philo *philo);
int			check_all_full(t_env *env);
int			pair_one(t_philo *philo);
int			not_pair_one(t_philo *philo);
void		custom_exit(t_philo *philo, t_env *env, int err);
int			init_philo(t_env *env);
void		custom_close(t_env *env);
int			rout_part(t_philo *philo);

#endif