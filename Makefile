# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = -lX11
LIBSDIR = 
#-L/usr/X11R6/lib
IDIR = 
#-I/opt/X11/include

# fichiers du projet 
# TODO à compléter
SRC =  clientAPI.c labyrinthAPI.c main.c 
OBJ = $(SRC:.c=.o)
EXEC = laby.out


# règle initiale
all: $(EXEC)

# règles de compilation
# TODO Compléter
clientAPI.o: clientAPI.c
labyrinthAPI.o: labyrinthAPI.c
main.o : main.c
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $< $(IDIR)
	
# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	#rm -f *.o
	ls $(OBJ)
mproper:
	rm -f $(EXEC) *.o
