/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:59:34 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/01 13:19:26 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//valgrind --tool=helgrind
// fsanitize=thread

void	custom_exit(t_philo *philo, t_env *env, int err)
{
	if (env)
	{
		if (env->arg)
		{
			free(env->arg);
		}
		free(env);
	}
	if (philo)
		free(philo);
	exit(err);
}

void	print_time(int time, int philnb, char *str)
{
	printf("%dms |Philo%d| :%s", time, philnb, str);
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

t_env	*init(t_env *env, char **av, int ac)
{
	env = malloc(sizeof(t_env));
	env->arg = malloc(sizeof(t_arg));
	if (ac == 6)
		env->arg->c_eat = ft_atoi(av[5]);
	else
		env->arg->c_eat = -1;
	env->arg->t_eat = ft_atoi(av[3]);
	env->arg->t_sleep = ft_atoi(av[4]);
	env->arg->t_die = ft_atoi(av[2]);
	env->count = ft_atoi(av[1]);
	env->philo = malloc(sizeof(t_philo) * env->count);
	if (env->arg->t_eat < 0 || env->arg->t_sleep < 0 || env->arg->t_die < 0
		|| env->count < 0)
		custom_exit(env->philo, env, EXIT_FAILURE);
	env->arg->time = actual_time();
	pthread_mutex_init(&(env->arg->dead.mutex), NULL);
	pthread_mutex_init(&(env->arg->print.mutex), NULL);
	env->arg->dead.data = 0;
	env->arg->print.data = 1;
	return (env);
}

int	give_fork(t_philo *philo, int nb)
{
	int	i;

	if (nb == 1)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&(philo->fork.mutex));
			i = philo->fork.data == 1;
			if (i)
				philo->fork.data = 0;
			pthread_mutex_unlock(&(philo->fork.mutex));
		}
		else
		{
			pthread_mutex_lock(&(philo->next_fork->mutex));
			i = philo->next_fork->data == 1;
			if (i)
				philo->next_fork->data = 0;
			pthread_mutex_unlock(&(philo->next_fork->mutex));
		}
	}
	else
	{
		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(&(philo->fork.mutex));
			i = philo->fork.data == 1;
			if (i)
				philo->fork.data = 0;
			pthread_mutex_unlock(&(philo->fork.mutex));
		}
		else
		{
			pthread_mutex_lock(&(philo->next_fork->mutex));
			i = philo->next_fork->data == 1;
			if (i)
				philo->next_fork->data = 0;
			pthread_mutex_unlock(&(philo->next_fork->mutex));
		}
	}
	return (i);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->last_eat.mutex);
	philo->last_eat.data = actual_time();
	pthread_mutex_unlock(&philo->last_eat.mutex);
	philo->eat_c = 0;
	while (!check_full(philo) && !check_death(philo) && (philo->arg->c_eat < 0
		|| philo->arg->c_eat > philo->eat_c))
	{
		if (philo->next_fork)
		{
			while (!give_fork(philo, 1) && !check_death(philo))
				usleep(1);
			while (!give_fork(philo, 2) && !check_death(philo))
				usleep(1);
			if (!check_death(philo))
			{
				a_table(philo);
				philo->eat_c += 1;
				if ((philo->eat_c >= philo->arg->c_eat) && philo->arg->c_eat != -1)
				{
					pthread_mutex_lock(&(philo->full.mutex));
					philo->full.data = 1;
					pthread_mutex_unlock(&(philo->full.mutex));
				}
				time_sleep(philo);
			}
			print(philo->id, "has taken a fork", philo);
		}
	}
	return (NULL);
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

int	main_thread(t_env *env)
{
	int			i;
	long int	time;

	while (1)
	{
		i = 0;
		while (i < env->count && env->philo[i].thread)
		{
			time = actual_time();
			pthread_mutex_lock(&env->philo[i].last_eat.mutex);
			if ((time - env->philo[i].last_eat.data > env->arg->t_die)
				|| check_all_full(env))
			{
				pthread_mutex_unlock(&env->philo[i].last_eat.mutex);
				if (!check_all_full(env))
					print(env->philo[i].id, "died\n", &env->philo[i]);
				pthread_mutex_lock(&env->arg->dead.mutex);
				env->arg->dead.data = 1;
				pthread_mutex_unlock(&env->arg->dead.mutex);
				custom_close(env);
				custom_exit(env->philo, env, EXIT_FAILURE);
				i++;
			}
			pthread_mutex_unlock(&env->philo[i].last_eat.mutex);
			usleep(10);
		}
	}
}

int	launch_philo(t_env *env)
{
	int	i;

	i = -1;
	while (++i < env->count)
		if (i % 2)
			pthread_create(&(env->philo[i].thread), NULL, routine,
			&(env->philo[i]));
	i = -1;
	usleep((env->arg->t_eat / 2) * 1000);
	while (++i < env->count)
		if (!(i % 2))
			pthread_create(&(env->philo[i].thread), NULL, routine,
			&(env->philo[i]));
	return (1);
}

int	init_philo(t_env *env)
{
	int	i;

	i = -1;
	while (++i < env->count)
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

int	main(int ac, char **av)
{
	t_env	*env;
	int		i;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("USAGE: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	env = NULL;
	env = init(env, av, ac);
	init_philo(env);
	main_thread(env);
	free(env->philo);
	free(env);
	return (0);
}
