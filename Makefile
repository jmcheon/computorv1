NAME = computorv1

SRCS_DIR = src/

INCLUDES = ./

SRCS =	main.cpp \
		utils.cpp \
		computorv.cpp \
		Token.cpp \
      	Lexer.cpp \
		Parser.cpp \
		RPNtree.cpp \


OBJS_DIR = obj/

OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix $(OBJS_DIR), $(OBJS))
MKDIR = /bin/mkdir -p
MKDIR_OBJS = $(MKDIR) $(OBJS_DIR)

CC = c++

SAM = -fsanitize=address 

CFLAGS = -Wall -Wextra -Werror -g #-std=c++98

RM = /bin/rm -rf

all :  $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -I $(INCLUDES)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
	@$(MKDIR_OBJS)
	$(CC) -c $< -o $@ 
#%.o:%.cpp
#	$(CC) -c -o $@ $<

clean :
	$(RM) $(OBJS) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean re

