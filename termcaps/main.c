#include <curses.h>
#include <term.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int	main(int ac, char **av)
{
	int		val;
	char	*bp;
	char	*name;

	name = getenv("TERM");
	val = tgetent(NULL, name);
	printf("%d\n", val);
	printf("%s\n", bp);
	printf("%s\n", name);
	val = setupterm(NULL, STDOUT_FILENO, &errno);
	val = tgetnum("co");
	printf("%d\n", val);
	val = tgetflag("os");
	printf("%d\n", val);
	bp = tgetstr("cl", NULL);
	val = tputs(bp, 90, putchar);
}
