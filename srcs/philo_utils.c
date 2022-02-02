/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 14:30:49 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/01 12:54:55 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print(int id, char *msg, t_philo *philo)
{
	long int	time;
	int			ret;

	ret = 0;
	while (!ret && !check_death(philo))
	{
		pthread_mutex_lock(&(philo->arg->print.mutex));
		ret = philo->arg->print.data == 1;
		if (ret)
			philo->arg->print.data = 0;
		pthread_mutex_unlock(&(philo->arg->print.mutex));
	}
	time = actual_time();
	if (time < 0)
		return (0);
	if (!check_death(philo))
	{	
		printf("%ld : philo %d %s\n", time - philo->arg->time, id + 1, msg);
		pthread_mutex_lock(&(philo->arg->print.mutex));
		philo->arg->print.data = 1;
		pthread_mutex_unlock(&(philo->arg->print.mutex));
	}
	return (1);
}

int	time_sleep(t_philo *philo)
{
	long int	time;
	long int	sleep;

	time = actual_time();
	print(philo->id, "start sleep !", philo);
	while ((actual_time() - time < philo->arg->t_sleep) && !check_death(philo))
	{
		if (philo->arg->t_sleep - (actual_time() - time) > philo->arg->t_die)
			sleep = philo->arg->t_die;
		else
			sleep = ((philo->arg->t_sleep - (actual_time() - time)) / 2);
		if (check_death(philo))
			return (0);
		usleep(sleep);
	}
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
	while ((actual_time() - time) < (philo->arg->t_eat))
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
