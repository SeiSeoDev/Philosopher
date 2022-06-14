/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 11:25:44 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 14:53:54 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	long int	i;
	int			neg;
	long int	nbr;
	long int	tmp;

	nbr = 0;
	i = 0;
	neg = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			neg = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		tmp = nbr;
		nbr = nbr * 10 + str[i++] - '0';
		if (nbr - tmp > 2147483647 || tmp > nbr)
			return (-1);
	}
	if (neg)
		nbr = neg * nbr;
	return (nbr);
}
