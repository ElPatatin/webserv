# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/28 12:37:08 by cpeset-c          #+#    #+#              #
#    Updated: 2024/08/21 20:12:21 by cpeset-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -=-=-=-=-	DEFINE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

# Here define the lenguage for the Makefile.
LANG	:= C++

SYSTEM	:= $(shell uname -s)

LOG_FLE	:= webserver.log

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

CFLAGS		:= -Wall -Wextra -Werror -W -g
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

# Classes directories
CNF_DIR	:= $(SRC_DIR)ConfigDataClass/
LOG_DIR	:= $(SRC_DIR)LogClass/

# Config Parser directories
CNP_DIR	:= $(SRC_DIR)config_parser/

# Webserver directories
WEB_DIR	:= $(SRC_DIR)webserver/

# Htpp directories
HTP_DIR	:= $(SRC_DIR)HttpClass/
HFS_DIR	:= $(HTP_DIR)HttpFileServingClass/
HRP_DIR	:= $(HTP_DIR)HttpRequestParserClass/
HDR_DIR	:= $(HTP_DIR)HttpHeadersClass/
MTD_DIR	:= $(HTP_DIR)HttpMethodsClass/
RSP_DIR	:= $(HTP_DIR)HttpResponseClass/
URL_DIR	:= $(HTP_DIR)HttpUrlClass/
VRS_DIR	:= $(HTP_DIR)HttpVersionClass/

OHT_DIR	:= $(SRC_DIR)old_http/

# -=-=-=-=-	FILE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

ifeq ($(LANG), C)
	EXT	:= .c
endif
ifeq ($(LANG), C++)
	EXT	:= .cpp
endif

INCLUDE	:= -I$(INC_DIR) -I$(CNF_DIR) -I$(LOG_DIR) -I$(CNP_DIR) -I$(HTP_DIR) -I$(HFS_DIR) -I$(HRP_DIR) -I$(HDR_DIR) -I$(MTD_DIR) -I$(RSP_DIR) -I$(URL_DIR) -I$(VRS_DIR)

# -----------------------------  SOURCE FILES  -------------------------------- #

# utils
SRCS	+= $(SRC_DIR)utils.cpp

# ConfigDataClass
SRCS	+= $(CNF_DIR)ConfigData.cpp

# LogClass
SRCS	+= $(LOG_DIR)Log.cpp

# Config Parser
SRCS	+= $(CNP_DIR)config_parse.cpp \
		$(CNP_DIR)config_get.cpp \
		$(CNP_DIR)config_validate.cpp \
		$(CNP_DIR)config_server_count.cpp \
		$(CNP_DIR)config_load.cpp \
		$(CNP_DIR)config_load_utils.cpp \
		$(CNP_DIR)config_load_validate.cpp

# Webserver files
SRCS	+= $(WEB_DIR)webserver.cpp \
		$(WEB_DIR)signals.cpp \
		$(WEB_DIR)webserver_start.cpp \
		$(WEB_DIR)webserver_run.cpp \
		$(WEB_DIR)webserver_stop.cpp \
		$(WEB_DIR)webserver_utils.cpp \
		$(WEB_DIR)sockets.cpp \
		$(WEB_DIR)epoll.cpp

SRCS	+= $(HTP_DIR)Http.cpp \
		$(HTP_DIR)HttpUtils.cpp \
		$(HTP_DIR)HttpCGI.cpp \
		$(HTP_DIR)HttpGet.cpp \
		$(HTP_DIR)HttpPost.cpp \
		$(HTP_DIR)HttpDelete.cpp \
		$(HFS_DIR)HttpFileServing.cpp \
		$(HFS_DIR)HttpDirListing.cpp \
		$(HFS_DIR)HttpSaveFile.cpp \
		$(HRP_DIR)HttpRequestParser.cpp \
		$(HDR_DIR)HttpHeaders.cpp \
		$(MTD_DIR)HttpMethods.cpp \
		$(RSP_DIR)HttpResponse.cpp \
		$(URL_DIR)HttpUrl.cpp \
		$(VRS_DIR)HttpVersion.cpp

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
	@echo "$(BLUE)	$(NAME) object and dependencies files cleaned.$(DEF_CLR)"

fclean: 
	@$(RM) -r $(OBJ_DIR) $(DEP_DIR) $(NAME) dummy ./tests/dummy_client.o $(LOG_FLE)
	@echo "$(WHITE)	All objects, dependencies and executables removed.$(DEF_CLR)"

re:
	@$(MAKE) fclean
	@$(MAKE)
	@echo "$(GREEN)	Cleaned and rebuilt everything for $(NAME) project.$(DEF_CLR)"

$(OBJ_DIR)%.o: %$(EXT) $(MKFL)
	@$(MK) $(dir $@) $(dir $(subst $(OBJ_DIR), $(DEP_DIR), $@))
	@printf "$(GREEN)\tCompiling: $(YELLOW)$< $(DEF_CLR)\n"
	@$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE) -c $< -o $@
	@mv $(patsubst %.o, %.d, $@) $(dir $(subst $(OBJ_DIR), $(DEP_DIR), $@))

dummy:
	@gcc -Wall -Werror -Wextra ./tests/dummy_client.c -o ./tests/dummy_client.o -c
	@gcc -Wall -Werror -Wextra ./tests/dummy_client.o -o dummy

.PHONY: all clean fclean re dummy