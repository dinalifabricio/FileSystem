all:
	cc main.c utils/utils.c cluster/cluster.c fat/fat.c fat_table/fat_table.c -o main -lreadline
	./main
