/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 14:30:49 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 15:01:44 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print(int id, char *msg, t_philo *philo)
{
	long int	time;
	int			ret;

	ret = 0;
	while (!ret)
	{
		pthread_mutex_lock(&(philo->arg->print.mutex));
		time = actual_time();
		if (check_death(philo) || !philo->arg->print.data)
		{	
			pthread_mutex_unlock(&(philo->arg->print.mutex));
			return (0);
		}
		else
		{
			printf("%ld : philo %d %s\n", time - philo->arg->time, id + 1, msg);
			ret = 1;
		}
		pthread_mutex_unlock(&(philo->arg->print.mutex));
	}
	return (1);
}

int	time_sleep(t_philo *philo)
{
	long int	time;
	long int	sleep;

	time = actual_time();
	print(philo->id, "is sleeping", philo);
	while ((actual_time() - time < philo->arg->t_sleep) && !check_death(philo))
	{
		if (philo->arg->t_sleep - (actual_time() - time) > philo->arg->t_die)
			sleep = philo->arg->t_die;
		else
			sleep = ((philo->arg->t_sleep - (actual_time() - time)) / 2);
		if (sleep < 0)
			return (0);
		if (check_death(philo))
		{
			return (0);
		}
		usleep(sleep);
	}
	print(philo->id, "is thinking", philo);
	return (1);
}

void	a_table(t_philo *philo)
{
	long int	time;
	long int	eating;

	time = actual_time();
	pthread_mutex_lock(&(philo->last_eat.mutex));
	philo->last_eat.data = time;
	pthread_mutex_unlock(&(philo->last_eat.mutex));
	print(philo->id, "is eating", philo);
	while ((actual_time() - time) < (philo->arg->t_eat) && !check_death(philo))
	{
		if (philo->arg->t_eat - (actual_time() - time) > philo->arg->t_die)
			eating = philo->arg->t_die;
		else
			eating = ((philo->arg->t_eat - (actual_time() - time)) / 2);
		usleep(10);
	}
	if (!check_death(philo))
	{
		pthread_mutex_lock(&(philo->fork.mutex));
		philo->fork.data = 1;
		pthread_mutex_unlock(&(philo->fork.mutex));
		pthread_mutex_lock(&(philo->next_fork->mutex));
		philo->next_fork->data = 1;
		pthread_mutex_unlock(&(philo->next_fork->mutex));
	}
}

long int	actual_time(void)
{
	struct timeval	tv;
	int				err;

	err = gettimeofday(&tv, NULL);
	if (err != 0)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	custom_close(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		pthread_join(env->philo[i].thread, NULL);
		i++;
	}
}
