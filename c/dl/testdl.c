#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
	void (*fun)();
	void *handle = NULL;

	handle = dlopen("./libshare.so", RTLD_NOW);
	if (handle == NULL) {
		printf("DL open err\n");
		dlerror();
	}

	fun = (void (*)()) dlsym(handle, "lib_fun_a");
	if (fun == NULL) {
		printf("DL sym error\n");
		dlerror();
	}
	fun();
	dlclose(handle);

	return 0;
}

