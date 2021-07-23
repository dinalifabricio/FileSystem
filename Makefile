all:
	cc main.c utils/utils.c fat/fat.c fat_table/fat_table.c -o main -lreadline
	./main
