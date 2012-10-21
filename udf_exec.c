#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;     /* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#if defined(MYSQL_SERVER)
#include <m_string.h>           /* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#define bzero(a,b) memset(a,0,b)
#define memcpy_fixed(a,b,c) memcpy(a,b,c)
#endif
#endif
#include <mysql.h>
#include <ctype.h>

#ifdef HAVE_DLOPEN

my_bool exec_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void exec_deinit(UDF_INIT *initid);
longlong exec(UDF_INIT *initid, UDF_ARGS *args, char *is_null,  char *error);

my_bool exec_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	if(args->arg_count != 2) {
		strcpy(message,"myfunc_double must have two arguments of type string");
		return 1;
	}
	args->arg_type[0] = STRING_RESULT;
	args->arg_type[1] = STRING_RESULT;
	initid->maybe_null = 1;
	initid->const_item = 0;
	return 0;
}

void exec_deinit(UDF_INIT *initid) {

}

longlong exec(UDF_INIT *initid, UDF_ARGS *args, char *is_null,  char *error) {
	FILE *fp;
	*error = 0;
	*is_null = 0;
	pid_t f = 0;
	char cmd[2048];
	char arg1[2048];
	int status;
	if(args->args[0] && args->args[1]) {
		fp = fopen("/tmp/debug.txt", "a");
		fwrite(args->args[0], args->lengths[0], 1, fp); 
		fwrite("\t", 1, 1, fp); 
		fwrite(args->args[1], args->lengths[1], 1, fp); 
		fwrite("\r\n", 2, 1, fp); 
		fclose(fp);
		if(args->lengths[0] > 2048 || args->lengths[1] > 2048) {
			strcpy(error, "Argument to long - limited to 2048 characters");
			return 0;
		}
		memset(cmd , 0, 2048);
		memset(arg1, 0, 2048);
		memcpy(cmd , args->args[0], args->lengths[0]);
		memcpy(arg1, args->args[1], args->lengths[1]);

		f = vfork();
		if(f == 0) {
			execl(cmd, cmd, arg1, NULL);
			_exit(0);
		} else if (f == -1) {
			strcpy(error, "fork failed");
			return 1;
		} else {
			waitpid(f, &status, 0);
		}
	}
	return 0;
}

#endif
