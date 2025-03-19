#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFOLT	16

extern char *optarg;
extern int optind;

const unsigned char short_opts[] = "hs:l:e:";
const unsigned char help_str[] = "<Пародия на hexdump>\n\n--help -h  \tвыводит эту строку\n--str -s   \tдлинна выводимой строки\n--len -l   \tсколько дaнных вывести на экран\n--format -e\tформат вывода данных (в битовом (b),\n           \tв шеснадцатиричном виде(x), в десятиричном виде(t))";
const struct option long_opts[5] = {
	{"help", 0, 0, 'h'},
	{"str", 1, 0, 's'},
	{"len", 1, 0, 'l'},
	{"format", 1, 0, 'e'},
	{NULL, 0, NULL, 0}
};

const unsigned char format_str1[] = "%2x ";
const unsigned char format_str2[] = "%8b ";
const unsigned char format_str3[] = "%3d ";

unsigned char *format_str = format_str1;
char number_str = DEFOLT;
signed len_print_data = 0;
signed len_data_file;
unsigned char *data;



int main(int argc, char **argv)
{
	for (signed char opt; (opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1;) {
		switch (opt) {
			case 'h':
				puts(help_str);
				return 0;
			case 's':
				number_str = (unsigned)atoi(optarg);
				number_str = number_str < 0 ? -number_str : number_str;
				if (!number_str) {
					fprintf(stderr, "%s\n", "A string cannot be of zero length");
					return 6;
				}
				break;
			case 'l':
				len_print_data = (signed)atoi(optarg);
				break;
			case 'e':
				if (!optarg[1]) {
					switch (optarg[0]) {
						case 'b':
							format_str = format_str2;
							break;
						case 'x':
							format_str = format_str1;
							break;
						case 't':
							format_str = format_str3;
							break;
						default:
							fprintf(stderr, "%s [%c]\n",\
									"Invalid argument to option --format",\
									optarg[0]);
							return 1;
					}
				}
				break;
			case '?':
				fprintf(stderr, "%s\n", "Wrong option");
				return 2;
			default:
				fprintf(stderr, "%s\n", "Unknown error");
				return 3;
		}
	}
	if (argc == optind) {
		fprintf(stderr, "%s\n", "No file named to read");
		return 4;
	}
	int fd = open(argv[optind], 0, 0);
	if (fd < 0) {
		fprintf(stderr, "%s\n", "Error opening file");
		return 5;
	}
	len_data_file = lseek(fd, 0, 2);
	if (!len_print_data || len_print_data > len_data_file || -len_print_data > len_data_file) {
		len_print_data = len_data_file;
		lseek(fd, 0, 0);
	}
	else if (len_print_data < 0)
		lseek(fd, len_print_data, 2);
	else
		lseek(fd, 0, 0);
	len_print_data = len_print_data < 0 ? -len_print_data : len_print_data;
	data = (unsigned char *)malloc(len_print_data);
	read(fd, data, len_print_data);
	close(fd);
	{
		len_print_data++;
		unsigned int ind;
		for (ind = 1; ind != len_print_data; ind++) {
			printf(format_str, data[ind - 1]);
			if (!(ind % number_str))
				putchar('\n');
		}
		if (--ind % number_str)
			putchar('\n');
	}
	free(data);
	return 0;
}
