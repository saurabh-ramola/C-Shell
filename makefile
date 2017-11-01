shell: main.c input.c builtin.c
	gcc -o main main.c input.c builtin.c redirection.c piping.c
