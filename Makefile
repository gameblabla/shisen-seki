CC		?= gcc
STRIP		?= strip
CFLAGS		?= -lSDL -DNO_FRAMELIMIT -DNO_SCALING
LDFLAGS		?= -lSDL -lm -Wl,--as-needed -Wl,--gc-sections -flto
TARGET		?= shisen.elf
SRCDIR		:= src
OBJDIR		:= obj
SRC		:= $(wildcard $(SRCDIR)/*.c)
OBJ		:= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS	+= -O0 -g
	
.PHONY: all opk clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -Rf $(TARGET) $(OBJDIR) $(RELEASEDIR)

