all:
	gcc -Wall raidutil.c lib/string.c lib/status.c lib/get.c lib/replace.c -o raid
