# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/28 12:37:08 by cpeset-c          #+#    #+#              #
#    Updated: 2024/06/21 23:17:33 by cpeset-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -=-=-=-=-	DEFINE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

# Here define the lenguage for the Makefile.
LANG	:= C++

SYSTEM	:= $(shell uname -s)

# -=-=-=-=-	NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

NAME    	:= webserv

MKFL		:= Makefile

# -=-=-=-=-	CLRS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

DEF_CLR		:= \033[0;39m
GRAY		:= \033[0;90m
RED			:= \033[0;91m
GREEN		:= \033[0;92m
YELLOW		:= \033[0;93m
BLUE		:= \033[0;94m
MAGENTA		:= \033[0;95m
CYAN		:= \033[0;96m
WHITE		:= \033[0;97m

# -=-=-=-=-	CMND -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

# Define Compiler for MacOS or Linux system.

ifeq ($(LANG), C)
	ifeq ($(SYSTEM), Darwin)
		CC	:= clang
	endif
	ifeq ($(SYSTEM), Linux)
		CC	:= gcc
	endif
else ifeq ($(LANG), C++)
	ifeq ($(SYSTEM), Darwin)
		CC	:= clang++
	endif
	ifeq ($(SYSTEM), Linux)
		CC	:= g++
	endif
endif

# Define compiler flags.

CFLAGS		:= -Wall -Wextra -Werror -W
ifeq ($(LANG), C++)
	CFLAGS	+= -std=c++98
endif

XFLAGS		= #-fsanitize=address -g
DFLAGS		= -MT $@ -MMD -MP

AR		= ar -rcs
RM		= rm -f
MK		= mkdir -p
CP		= cp -f

# -=-=-=-=-	PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

OBJ_DIR	:= .objs/
DEP_DIR	:= .deps/

SRC_DIR	:= ./sources/
INC_DIR	:= ./includes/

# LoadConfigClass
LCC_DIR	:= $(SRC_DIR)LoadConfigClass/
SOC_DIR	:= $(SRC_DIR)SockClass/
CNF_DIR	:= $(SRC_DIR)ConfigDataClass/
LOG_DIR	:= $(SRC_DIR)LogClass/

# -=-=-=-=-	FILE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

ifeq ($(LANG), C)
	EXT	:= .c
endif
ifeq ($(LANG), C++)
	EXT	:= .cpp
endif

INCLUDE	:= -I$(INC_DIR) -I$(LCC_DIR) -I$(SOC_DIR) -I$(CNF_DIR) -I$(LOG_DIR)

# -----------------------------  SOURCE FILES  -------------------------------- #

# FileUtils
SRCS	+= $(SRC_DIR)fileUtils.cpp

# LoadConfigClass
SRCS	+= $(LCC_DIR)LoadConfig.cpp

# ConfigDataClass
SRCS	+= $(CNF_DIR)ConfigData.cpp

# LogClass
SRCS	+= $(LOG_DIR)Log.cpp

# SockClass
SRCS	+= $(SOC_DIR)Sock.cpp \
			$(SOC_DIR)SockException.cpp

# -----------------------------  MAIN FILES  ---------------------------------- #

SRCS	+= $(SRC_DIR)main.cpp

OBJS	= $(addprefix $(OBJ_DIR), $(SRCS:$(EXT)=.o))
DEPS	= $(addprefix $(DEP_DIR), $(addsuffix .d, $(basename $(SRCS))))

# -=-=-=-=-	RULE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=s-=-=-=-=-=-=-=-=-=-=-=-=- #

all: $(NAME)

$(NAME):: $(OBJS)
	@$(CC) $(CFLAGS) $(XFLAGS) $(OBJS) -o $(NAME)
	@printf "\n\t$(WHITE)Program \033[1;31m$(NAME) $(WHITE)has been compiled!$(DEF_CLR)\n"

$(NAME)::
	@printf "\t$(WHITE)Nothing more to be done for program \033[1;31m$(NAME)$(DEF_CLR)\n"

-include $(DEPS)

clean:
	@$(RM) -r $(OBJ_DIR) $(DEP_DIR) ./tests/dummy_client.o
	@echo "$(BLUE)	Cub3D object and dependencies files cleaned.$(DEF_CLR)"

fclean: 
	@$(RM) -r $(OBJ_DIR) $(DEP_DIR) $(NAME) dummy ./tests/dummy_client.o
	@echo "$(WHITE)	All objects, dependencies and executables removed.$(DEF_CLR)"

re:
	@$(MAKE) fclean
	@$(MAKE)
	@echo "$(GREEN)	Cleaned and rebuilt everything for $(NAME) project.$(DEF_CLR)"

$(OBJ_DIR)%.o: %$(EXT) $(MKFL)
	@$(MK) $(dir $@) $(dir $(subst $(OBJ_DIR), $(DEP_DIR), $@))
	@printf "\r$(GREEN)\tCompiling: $(YELLOW)$< $(DEF_CLR)                   \r"
	@$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE) -c $< -o $@
	@mv $(patsubst %.o, %.d, $@) $(dir $(subst $(OBJ_DIR), $(DEP_DIR), $@))

dummy:
	@gcc -Wall -Werror -Wextra ./tests/dummy_client.c -o ./tests/dummy_client.o -c
	@gcc -Wall -Werror -Wextra ./tests/dummy_client.o -o dummy

.PHONY: all clean fclean re dummy