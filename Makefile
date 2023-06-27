NAME = computorv1

SRCS_DIR = src/

INCLUDES = ./

SRCS =	main.cpp \
		computorv.cpp \
		utils.cpp \
		Token.cpp \
      	Lexer.cpp \
		Parser.cpp \
		RPNtree.cpp \

BONUS_SRCS = main_bonus.cpp \
		computorv_bonus.cpp \
		utils.cpp \
		Token.cpp \
      	Lexer.cpp \
		Parser.cpp \
		RPNtree.cpp \

OBJS_DIR = obj/

OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix $(OBJS_DIR), $(OBJS))

BONUS_OBJS = $(BONUS_SRCS:.cpp=.o)
BONUS_OBJS := $(addprefix $(OBJS_DIR), $(BONUS_OBJS))


MKDIR = /bin/mkdir -p
MKDIR_OBJS = $(MKDIR) $(OBJS_DIR)

CC = c++

SAM = -fsanitize=address 

CFLAGS = -Wall -Wextra -Werror -g

RM = /bin/rm -rf

all :  $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -I $(INCLUDES)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
	@$(MKDIR_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean :
	$(RM) $(OBJS) $(BONUS_OBJS) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: $(BONUS_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(BONUS_OBJS) -I $(INCLUDES)

.PHONY : all clean fclean re bonus