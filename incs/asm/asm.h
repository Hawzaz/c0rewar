/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:19:41 by dbaffier          #+#    #+#             */
/*   Updated: 2019/09/23 03:04:47 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# define OK		0
# define NOT_OK	1

# include "op.h"

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# define ERR_FILE			1
# define ERR_EXTENSION		2
# define ERR_NOFILE			3
# define ERR_OVERFLOW		4
# define ERR_OPEN			5
# define ERR_MALLOC			6

# define ERR_NAME_H			1
# define ERR_NAME_LEN		2
# define ERR_COMMENT_H		3
# define ERR_COMMENT_LEN	4
# define ERR_LAB			5
# define ERR_LAB_FOUND		6

# define ERR_STR_FILE		"filename too short"
# define ERR_STR_EXTENSION	"file extension is not correct"
# define ERR_STR_NOFILE		"no given file"
# define ERR_STR_OVERFLOW	"too much argument given"
# define ERR_STR_OPEN		"open failed "
# define ERR_STR_MALLOC		"malloc failed to allocate memory"

# define ERR_STR_NAME		"lexical error on name ->`.name`"
# define ERR_STR_NAME_LEN	"name too long ( < 128 )"
# define ERR_STR_COMMENT	"lexical error on comment -> `.comment`"
# define ERR_STR_COMMENT_LEN "comment too long ( < 2048 )"

enum {
	LABEL = 1,
	OP_CODE,
	PARAM,
	DOT,
	DOT_ARG,
	DIRECT,
	UNDIRECT,
	REGISTER,
};

typedef struct		s_token
{
	int					bin;
	int					type;
	int					arg_n;
	int					val;
	char				*lab;
	struct s_token		*next;
}					t_token;

typedef struct		s_aolist
{
	int					id;
	int					line;
	size_t				size;
	size_t				mem_addr;
	t_token				*tok;
	struct s_aolist		*lab;
	char				*comment;
	struct s_aolist		*next;
}					t_aolist;

typedef struct		s_env
{
	int			fd;
	int			line;
	size_t		size;
	char		*fd_name;
	t_aolist	*aolist;
}					t_env;

extern t_op			g_op_tab[17];

int			asm_file_open(t_env *e, char *file);
int			asm_file(int ac, char **av);

int			asm_comment(t_aolist *head, char **line);
char		*str_s_e(char *line, int s, int e);
char		**ft_strsplitws(const char *s);
int			create_label(t_token **head, char *val, size_t *i);
int			create_dot(t_token **head, char *val, size_t *i);
int			grep_opcode(t_token **head, char *line, size_t st, size_t *i);
int			grep_arg(t_token *head, char *line, size_t *i);
int			is_label(char *line, int start);
int			set_id(t_token *head);
size_t		chunk_size(t_token *tok, int id);
int			tok_create(t_aolist *head, char **line);
int			parser(t_env *e, char *file);
void		ft_freetab(char ***table);
void		push_front(void *head, void *new, int type);
int			syntax_analysis(t_env *e, t_aolist *aolist);
int			asm_syntax_label(t_env *e, t_aolist *head, t_token *curr);
int			evaluate_header(t_env *e, t_aolist *head);
int			asm_syntax_header(t_env *e, t_aolist *head);
int			asm_lexical_label(char *lab);
int			syntax_error(t_env *e, int error, t_token *tok, int line);

#endif
