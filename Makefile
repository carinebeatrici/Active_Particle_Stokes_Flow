CC = gcc-4.8
SRC = src
INC = include
CFLAGS = -O3 -fbounds-check -Wall -Wextra -I$(INC) -lm -g
EXEC = cpu-box-lt

all:  
	$(CC) $(SRC)/main.c $(SRC)/initialize.c $(SRC)/distance.c $(SRC)/save_position.c $(SRC)/update.c $(SRC)/calc_box.c $(SRC)/box_list.c -o $(EXEC) $(CFLAGS)

clean:
	rm src/*~	
	rm $(INC)/*~

