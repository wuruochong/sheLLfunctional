shell: main.c rpm.c
		gcc main.c -o "shell"
run: shell
	./shell
clean:
	rm shell
