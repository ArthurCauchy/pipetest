/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acauchy <acauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 11:55:40 by acauchy           #+#    #+#             */
/*   Updated: 2018/04/11 13:38:12 by acauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void exit_error(char *errmsg)
{
	fprintf(stderr, "errmsg\n");
	exit(1);
}

/*
** role :
** 1 : read
** 2 : write
*/
static int	start_process(char *path, int *pipefd, int role)
{
	pid_t	pid;
	char	*args[2] = {strdup(path), NULL};

	if ((pid = fork()) == -1)
		exit_error("fork() error");
	if (pid == 0)
	{
		if (role == 1)
		{
			close(pipefd[1]);
			close(0);
			dup(pipefd[0]);
		}
		else if (role == 2)
		{
			close(pipefd[0]);
			close(1);
			dup(pipefd[1]);
		}
		execve(path, args, NULL);
	}
	return (pid);
}

int			main(void)
{
	int		pipefd[2];
	pid_t	ls_pid;
	pid_t	cat_pid;

	if (pipe(pipefd) == -1)
		exit_error("pipe() error");
	printf("read end : %d, write end : %d\n", pipefd[0], pipefd[1]);
	ls_pid = start_process("/bin/ls", pipefd, 2);
	cat_pid = start_process("/bin/cat", pipefd, 1);
	return (0);
}
