all:
	gcc -Wall raidutil.c lib/string.c lib/status.c lib/get.c -o raid
