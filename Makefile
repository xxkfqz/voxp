PROJECT = sunvoxp
CC = gcc
CFLAGS = -DPACKAGE_NAME=\"$(PROJECT)\" --std=c11 -O3 -I./include/
LDFLAGS = -ldl -lm
SRCS = src/
RM = rm -f

all: $(PROJECT)

$(PROJECT): $(SRCS)errexit.c $(SRCS)export.c $(SRCS)sunapi.c $(SRCS)main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -s

clean:
	$(RM) $(PROJECT) $(SRCS)/*.o
