/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:38:25 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 14:45:23 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&(philo->arg->print.mutex));
	pthread_mutex_lock(&(philo->last_eat.mutex));
	philo->last_eat.data = actual_time();
	pthread_mutex_unlock(&(philo->last_eat.mutex));
	pthread_mutex_unlock(&(philo->arg->print.mutex));
	philo->eat_c = 0;
	while (!check_full(philo) && !check_death(philo) && (philo->arg->c_eat < 0
			|| philo->arg->c_eat > philo->eat_c))
	{
		if (philo->next_fork)
		{
			rout_part(philo);
		}
	}	
	return (NULL);
}

int	launch_philo(t_env *env)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&(env->arg->print.mutex));
	while (++i < env->count)
	{
		if (i % 2)
			pthread_create(&(env->philo[i].thread), NULL, routine,
				&(env->philo[i]));
	}
	i = -1;
	while (++i < env->count)
	{
		if (!(i % 2))
			pthread_create(&(env->philo[i].thread), NULL, routine,
				&(env->philo[i]));
	}
	pthread_mutex_unlock(&(env->arg->print.mutex));
	return (1);
}

void	init_values(t_env *env, int i)
{
	pthread_mutex_init(&(env->philo[i].full.mutex), NULL);
	env->philo[i].full.data = 0;
	env->philo[i].dead = 0;
	pthread_mutex_init(&(env->philo[i].fork.mutex), NULL);
	env->philo[i].fork.data = 1;
	env->philo[i].arg = env->arg;
	pthread_mutex_init(&(env->philo[i].last_eat.mutex), NULL);
	env->philo[i].last_eat.data = actual_time();
	env->philo[i].id = i;
}

int	init_philo(t_env *env)
{
	int	i;

	i = -1;
	while (++i < env->count)
	{
		init_values(env, i);
	}
	i = -1;
	while (++i < env->count)
	{
		if (env->count == 1)
			env->philo[i].next_fork = NULL;
		else if (i == env->count - 1)
			env->philo[i].next_fork = &env->philo[0].fork;
		else
			env->philo[i].next_fork = &env->philo[i + 1].fork;
	}
	return (launch_philo(env));
}
