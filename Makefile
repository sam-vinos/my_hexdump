CC=gcc -std=c99
NAME_PROGRAM=my_hexdump
FILE =main.c
CLEAN =rm -f



all: $(NAME_PROGRAM)


rebuild: clean all


$(NAME_PROGRAM): $(FILE)
	$(CC) -o $@ $^

install: $(NAME_FILE)
	$(CLEAN) /usr/local/bin/hexdump2
	ln /home/huawei/myprograms/hexdump2/hexdump2 /usr/local/bin/

clean:
	$(CLEAN) $(NAME_PROGRAM)
