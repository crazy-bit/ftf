#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

#include "ftf_config.h"
#include "ftf_util.h"

static struct termios org_opts;

void ftf_debug(int level, const char *fname, const char *fmt, ...)
{
	va_list ap;
	char logbuf[1024];

	va_start(ap, fmt);
	vsnprintf(logbuf, sizeof(logbuf), fmt, ap);
	va_end(ap);

	if (level <= FTF_LOG_LEVEL) {
		time_t now;
		struct tm *tm_now;

		if ((now = time(NULL)) < 0)
			exit(1);
		tm_now = localtime(&now);

		fprintf(stderr, "%02d:%02d:%02d: %s:%s",
			tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec,
			fname, logbuf);
	}
}

void *signal_set(int signo, void (*func)(int))
{
	int ret;
	struct sigaction sig;
	struct sigaction osig;

	sig.sa_handler = func;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
#ifdef SA_RESTART
	sig.sa_flags |= SA_RESTART;
#endif /* SA_RESTART */

	ret = sigaction(signo, &sig, &osig);

	if (ret < 0)
		return (SIG_ERR);
	else
		return (osig.sa_handler);
}

void set_terminal()
{
	int res = 0;
	struct termios new_opts;
	
	//-----  store old settings -----------
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);
	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
}

void restore_terminal()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
}


