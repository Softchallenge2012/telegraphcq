/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with plpgsql_yy or PLPGSQL_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define PLPGSQL_YYBISON 1

/* Skeleton name.  */
#define PLPGSQL_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define PLPGSQL_YYPURE 0

/* Using locations.  */
#define PLPGSQL_YYLSP_NEEDED 0



/* Tokens.  */
#ifndef PLPGSQL_YYTOKENTYPE
# define PLPGSQL_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum plpgsql_yytokentype {
     K_ALIAS = 258,
     K_ASSIGN = 259,
     K_BEGIN = 260,
     K_CLOSE = 261,
     K_CONSTANT = 262,
     K_CURSOR = 263,
     K_DEBUG = 264,
     K_DECLARE = 265,
     K_DEFAULT = 266,
     K_DIAGNOSTICS = 267,
     K_DOTDOT = 268,
     K_ELSE = 269,
     K_ELSIF = 270,
     K_END = 271,
     K_EXCEPTION = 272,
     K_EXECUTE = 273,
     K_EXIT = 274,
     K_FOR = 275,
     K_FETCH = 276,
     K_FROM = 277,
     K_GET = 278,
     K_IF = 279,
     K_IN = 280,
     K_INFO = 281,
     K_INTO = 282,
     K_IS = 283,
     K_LOG = 284,
     K_LOOP = 285,
     K_NEXT = 286,
     K_NOT = 287,
     K_NOTICE = 288,
     K_NULL = 289,
     K_OPEN = 290,
     K_PERFORM = 291,
     K_ROW_COUNT = 292,
     K_RAISE = 293,
     K_RECORD = 294,
     K_RENAME = 295,
     K_RESULT_OID = 296,
     K_RETURN = 297,
     K_RETURN_NEXT = 298,
     K_REVERSE = 299,
     K_SELECT = 300,
     K_THEN = 301,
     K_TO = 302,
     K_TYPE = 303,
     K_WARNING = 304,
     K_WHEN = 305,
     K_WHILE = 306,
     T_FUNCTION = 307,
     T_TRIGGER = 308,
     T_STRING = 309,
     T_NUMBER = 310,
     T_VARIABLE = 311,
     T_ROW = 312,
     T_RECORD = 313,
     T_DTYPE = 314,
     T_LABEL = 315,
     T_WORD = 316,
     T_ERROR = 317,
     O_OPTION = 318,
     O_DUMP = 319
   };
#endif
#define K_ALIAS 258
#define K_ASSIGN 259
#define K_BEGIN 260
#define K_CLOSE 261
#define K_CONSTANT 262
#define K_CURSOR 263
#define K_DEBUG 264
#define K_DECLARE 265
#define K_DEFAULT 266
#define K_DIAGNOSTICS 267
#define K_DOTDOT 268
#define K_ELSE 269
#define K_ELSIF 270
#define K_END 271
#define K_EXCEPTION 272
#define K_EXECUTE 273
#define K_EXIT 274
#define K_FOR 275
#define K_FETCH 276
#define K_FROM 277
#define K_GET 278
#define K_IF 279
#define K_IN 280
#define K_INFO 281
#define K_INTO 282
#define K_IS 283
#define K_LOG 284
#define K_LOOP 285
#define K_NEXT 286
#define K_NOT 287
#define K_NOTICE 288
#define K_NULL 289
#define K_OPEN 290
#define K_PERFORM 291
#define K_ROW_COUNT 292
#define K_RAISE 293
#define K_RECORD 294
#define K_RENAME 295
#define K_RESULT_OID 296
#define K_RETURN 297
#define K_RETURN_NEXT 298
#define K_REVERSE 299
#define K_SELECT 300
#define K_THEN 301
#define K_TO 302
#define K_TYPE 303
#define K_WARNING 304
#define K_WHEN 305
#define K_WHILE 306
#define T_FUNCTION 307
#define T_TRIGGER 308
#define T_STRING 309
#define T_NUMBER 310
#define T_VARIABLE 311
#define T_ROW 312
#define T_RECORD 313
#define T_DTYPE 314
#define T_LABEL 315
#define T_WORD 316
#define T_ERROR 317
#define O_OPTION 318
#define O_DUMP 319




/* Copy the first part of user declarations.  */
#line 1 "gram.y"

/**********************************************************************
 * gram.y				- Parser for the PL/pgSQL
 *						  procedural language
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/pl/plpgsql/src/gram.y,v 1.2 2004/03/24 08:11:35 chungwu Exp $
 *
 *	  This software is copyrighted by Jan Wieck - Hamburg.
 *
 *	  The author hereby grants permission  to  use,  copy,	modify,
 *	  distribute,  and	license this software and its documentation
 *	  for any purpose, provided that existing copyright notices are
 *	  retained	in	all  copies  and  that	this notice is included
 *	  verbatim in any distributions. No written agreement, license,
 *	  or  royalty  fee	is required for any of the authorized uses.
 *	  Modifications to this software may be  copyrighted  by  their
 *	  author  and  need  not  follow  the licensing terms described
 *	  here, provided that the new terms are  clearly  indicated  on
 *	  the first page of each file where they apply.
 *
 *	  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY
 *	  PARTY  FOR  DIRECT,	INDIRECT,	SPECIAL,   INCIDENTAL,	 OR
 *	  CONSEQUENTIAL   DAMAGES  ARISING	OUT  OF  THE  USE  OF  THIS
 *	  SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF, EVEN
 *	  IF  THE  AUTHOR  HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 *	  DAMAGE.
 *
 *	  THE  AUTHOR  AND	DISTRIBUTORS  SPECIFICALLY	 DISCLAIM	ANY
 *	  WARRANTIES,  INCLUDING,  BUT	NOT  LIMITED  TO,  THE	IMPLIED
 *	  WARRANTIES  OF  MERCHANTABILITY,	FITNESS  FOR  A  PARTICULAR
 *	  PURPOSE,	AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON
 *	  AN "AS IS" BASIS, AND THE AUTHOR	AND  DISTRIBUTORS  HAVE  NO
 *	  OBLIGATION   TO	PROVIDE   MAINTENANCE,	 SUPPORT,  UPDATES,
 *	  ENHANCEMENTS, OR MODIFICATIONS.
 *
 **********************************************************************/

#include "plpgsql.h"


static	PLpgSQL_expr	*read_sql_construct(int until,
											const char *expected,
											bool isexpression,
											const char *sqlstart);
static	PLpgSQL_expr	*read_sql_stmt(const char *sqlstart);
static	PLpgSQL_type	*read_datatype(int tok);
static	PLpgSQL_stmt	*make_select_stmt(void);
static	PLpgSQL_stmt	*make_fetch_stmt(void);
static	PLpgSQL_expr	*make_tupret_expr(PLpgSQL_row *row);
static	void check_assignable(PLpgSQL_datum *datum);



/* Enabling traces.  */
#ifndef PLPGSQL_YYDEBUG
# define PLPGSQL_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef PLPGSQL_YYERROR_VERBOSE
# undef PLPGSQL_YYERROR_VERBOSE
# define PLPGSQL_YYERROR_VERBOSE 1
#else
# define PLPGSQL_YYERROR_VERBOSE 0
#endif

#if ! defined (PLPGSQL_YYSTYPE) && ! defined (PLPGSQL_YYSTYPE_IS_DECLARED)
#line 55 "gram.y"
typedef union PLPGSQL_YYSTYPE {
		int32					ival;
		char					*str;
		struct
		{
			char *name;
			int  lineno;
		}						varname;
		struct
		{
			int  nalloc;
			int  nused;
			int	 *nums;
		}						intlist;
		struct
		{
			int  nalloc;
			int  nused;
			PLpgSQL_diag_item *dtitems;
		}						dtlist;
		struct
		{
			int  reverse;
			PLpgSQL_expr *expr;
		}						forilow;
		struct
		{
			char *label;
			int  n_initvars;
			int  *initvarnos;
		}						declhdr;
		PLpgSQL_type			*dtype;
		PLpgSQL_datum			*variable; /* a VAR, RECFIELD, or TRIGARG */
		PLpgSQL_var				*var;
		PLpgSQL_row				*row;
		PLpgSQL_rec				*rec;
		PLpgSQL_expr			*expr;
		PLpgSQL_stmt			*stmt;
		PLpgSQL_stmts			*stmts;
		PLpgSQL_stmt_block		*program;
		PLpgSQL_nsitem			*nsitem;
} PLPGSQL_YYSTYPE;
/* Line 191 of yacc.c.  */
#line 300 "y.tab.c"
# define plpgsql_yystype PLPGSQL_YYSTYPE /* obsolescent; will be withdrawn */
# define PLPGSQL_YYSTYPE_IS_DECLARED 1
# define PLPGSQL_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 312 "y.tab.c"

#if ! defined (plpgsql_yyoverflow) || PLPGSQL_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if PLPGSQL_YYSTACK_USE_ALLOCA
#  define PLPGSQL_YYSTACK_ALLOC alloca
# else
#  ifndef PLPGSQL_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define PLPGSQL_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define PLPGSQL_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef PLPGSQL_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define PLPGSQL_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define PLPGSQL_YYSIZE_T size_t
#  endif
#  define PLPGSQL_YYSTACK_ALLOC malloc
#  define PLPGSQL_YYSTACK_FREE free
# endif
#endif /* ! defined (plpgsql_yyoverflow) || PLPGSQL_YYERROR_VERBOSE */


#if (! defined (plpgsql_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (PLPGSQL_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union plpgsql_yyalloc
{
  short plpgsql_yyss;
  PLPGSQL_YYSTYPE plpgsql_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define PLPGSQL_YYSTACK_GAP_MAXIMUM (sizeof (union plpgsql_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define PLPGSQL_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (PLPGSQL_YYSTYPE))				\
      + PLPGSQL_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef PLPGSQL_YYCOPY
#  if 1 < __GNUC__
#   define PLPGSQL_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define PLPGSQL_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register PLPGSQL_YYSIZE_T plpgsql_yyi;		\
	  for (plpgsql_yyi = 0; plpgsql_yyi < (Count); plpgsql_yyi++)	\
	    (To)[plpgsql_yyi] = (From)[plpgsql_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables PLPGSQL_YYSIZE and PLPGSQL_YYSTACKSIZE give the old and new number of
   elements in the stack, and PLPGSQL_YYPTR gives the new location of the
   stack.  Advance PLPGSQL_YYPTR to a properly aligned location for the next
   stack.  */
# define PLPGSQL_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	PLPGSQL_YYSIZE_T plpgsql_yynewbytes;						\
	PLPGSQL_YYCOPY (&plpgsql_yyptr->Stack, Stack, plpgsql_yysize);				\
	Stack = &plpgsql_yyptr->Stack;						\
	plpgsql_yynewbytes = plpgsql_yystacksize * sizeof (*Stack) + PLPGSQL_YYSTACK_GAP_MAXIMUM; \
	plpgsql_yyptr += plpgsql_yynewbytes / sizeof (*plpgsql_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char plpgsql_yysigned_char;
#else
   typedef short plpgsql_yysigned_char;
#endif

/* PLPGSQL_YYFINAL -- State number of the termination state. */
#define PLPGSQL_YYFINAL  9
/* PLPGSQL_YYLAST -- Last index in PLPGSQL_YYTABLE.  */
#define PLPGSQL_YYLAST   203

/* PLPGSQL_YYNTOKENS -- Number of terminals. */
#define PLPGSQL_YYNTOKENS  71
/* PLPGSQL_YYNNTS -- Number of nonterminals. */
#define PLPGSQL_YYNNTS  74
/* PLPGSQL_YYNRULES -- Number of rules. */
#define PLPGSQL_YYNRULES  133
/* PLPGSQL_YYNRULES -- Number of states. */
#define PLPGSQL_YYNSTATES  232

/* PLPGSQL_YYTRANSLATE(PLPGSQL_YYLEX) -- Bison symbol number corresponding to PLPGSQL_YYLEX.  */
#define PLPGSQL_YYUNDEFTOK  2
#define PLPGSQL_YYMAXUTOK   319

#define PLPGSQL_YYTRANSLATE(PLPGSQL_YYX) 						\
  ((unsigned int) (PLPGSQL_YYX) <= PLPGSQL_YYMAXUTOK ? plpgsql_yytranslate[PLPGSQL_YYX] : PLPGSQL_YYUNDEFTOK)

/* PLPGSQL_YYTRANSLATE[PLPGSQL_YYLEX] -- Bison symbol number corresponding to PLPGSQL_YYLEX.  */
static const unsigned char plpgsql_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      68,    69,     2,     2,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    65,
      66,     2,    67,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if PLPGSQL_YYDEBUG
/* PLPGSQL_YYPRHS[PLPGSQL_YYN] -- Index of the first RHS symbol of rule number PLPGSQL_YYN in
   PLPGSQL_YYRHS.  */
static const unsigned short plpgsql_yyprhs[] =
{
       0,     0,     3,     8,    13,    14,    16,    19,    21,    24,
      25,    27,    33,    35,    38,    42,    44,    47,    49,    55,
      57,    59,    65,    69,    73,    79,    85,    86,    94,    95,
      96,   100,   102,   106,   109,   111,   113,   115,   117,   119,
     121,   122,   124,   125,   126,   129,   131,   133,   135,   137,
     138,   140,   143,   145,   148,   150,   152,   154,   156,   158,
     160,   162,   164,   166,   168,   170,   172,   174,   176,   178,
     180,   182,   184,   186,   190,   195,   201,   207,   211,   213,
     215,   217,   219,   228,   229,   235,   238,   243,   249,   258,
     260,   262,   264,   265,   274,   283,   285,   287,   290,   295,
     298,   301,   308,   314,   316,   318,   320,   322,   324,   326,
     328,   331,   333,   336,   341,   344,   348,   352,   357,   362,
     364,   366,   368,   370,   371,   372,   373,   374,   380,   381,
     383,   385,   388,   390
};

/* PLPGSQL_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short plpgsql_yyrhs[] =
{
      72,     0,    -1,    52,    73,    77,    76,    -1,    53,    73,
      77,    76,    -1,    -1,    74,    -1,    74,    75,    -1,    75,
      -1,    63,    64,    -1,    -1,    65,    -1,    78,     5,   144,
      98,    16,    -1,   140,    -1,   140,    79,    -1,   140,    79,
      80,    -1,    10,    -1,    80,    81,    -1,    81,    -1,    66,
      66,   143,    67,    67,    -1,    10,    -1,    82,    -1,    91,
      93,    94,    95,    96,    -1,    91,    39,    65,    -1,    91,
      90,    65,    -1,    91,     3,    20,    89,    65,    -1,    40,
      92,    47,    92,    65,    -1,    -1,    91,     8,    83,    85,
      88,    45,    84,    -1,    -1,    -1,    68,    86,    69,    -1,
      87,    -1,    86,    70,    87,    -1,    91,    94,    -1,    28,
      -1,    20,    -1,    61,    -1,    57,    -1,    61,    -1,    61,
      -1,    -1,     7,    -1,    -1,    -1,    32,    34,    -1,    65,
      -1,    97,    -1,     4,    -1,    11,    -1,    -1,    99,    -1,
      99,   100,    -1,   100,    -1,    77,    65,    -1,   102,    -1,
     108,    -1,   110,    -1,   111,    -1,   112,    -1,   116,    -1,
     119,    -1,   120,    -1,   121,    -1,   122,    -1,   123,    -1,
     129,    -1,   130,    -1,   117,    -1,   101,    -1,   103,    -1,
     131,    -1,   132,    -1,   133,    -1,    36,   144,   137,    -1,
     107,   144,     4,   137,    -1,    23,    12,   144,   104,    65,
      -1,   104,    70,   106,     4,   105,    -1,   106,     4,   105,
      -1,    37,    -1,    41,    -1,    56,    -1,    56,    -1,    24,
     144,   138,    98,   109,    16,    24,    65,    -1,    -1,    15,
     144,   138,    98,   109,    -1,    14,    98,    -1,   140,    30,
     144,   128,    -1,   140,    51,   144,   139,   128,    -1,   140,
      20,   144,   113,    25,   115,   139,   128,    -1,   114,    -1,
      56,    -1,    61,    -1,    -1,   140,    20,   144,   118,    25,
      45,   139,   128,    -1,   140,    20,   144,   118,    25,    18,
     139,   128,    -1,    58,    -1,    57,    -1,    45,   144,    -1,
      19,   144,   141,   142,    -1,    42,   144,    -1,    43,   144,
      -1,    38,   144,   125,   124,   126,    65,    -1,    38,   144,
     125,   124,    65,    -1,    54,    -1,    17,    -1,    49,    -1,
      33,    -1,    26,    -1,    29,    -1,     9,    -1,   126,   127,
      -1,   127,    -1,    70,    56,    -1,    98,    16,    30,    65,
      -1,   136,   144,    -1,    18,   144,   137,    -1,    35,   144,
     134,    -1,    21,   144,   135,    27,    -1,     6,   144,   135,
      65,    -1,    56,    -1,    56,    -1,    61,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    66,    66,   143,    67,    67,    -1,
      -1,    60,    -1,    65,    -1,    50,   137,    -1,    61,    -1,
      -1
};

/* PLPGSQL_YYRLINE[PLPGSQL_YYN] -- source line where rule number PLPGSQL_YYN was defined.  */
static const unsigned short plpgsql_yyrline[] =
{
       0,   211,   211,   215,   221,   222,   225,   226,   229,   235,
     236,   239,   260,   268,   276,   287,   293,   295,   299,   301,
     303,   307,   327,   341,   351,   356,   361,   360,   412,   424,
     427,   451,   477,   488,   511,   512,   514,   539,   545,   557,
     568,   569,   574,   585,   586,   590,   592,   643,   644,   648,
     655,   659,   670,   687,   689,   691,   693,   695,   697,   699,
     701,   703,   705,   707,   709,   711,   713,   715,   717,   719,
     721,   723,   725,   729,   745,   761,   778,   793,   803,   807,
     813,   821,   828,   846,   853,   896,   902,   920,   939,   961,
     987,   997,  1010,  1028,  1058,  1088,  1090,  1096,  1103,  1119,
    1162,  1193,  1209,  1226,  1232,  1236,  1240,  1244,  1248,  1252,
    1258,  1271,  1280,  1286,  1290,  1303,  1316,  1430,  1442,  1455,
    1470,  1485,  1487,  1492,  1496,  1500,  1504,  1508,  1516,  1517,
    1521,  1523,  1527,  1538
};
#endif

#if PLPGSQL_YYDEBUG || PLPGSQL_YYERROR_VERBOSE
/* PLPGSQL_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at PLPGSQL_YYNTOKENS, nonterminals. */
static const char *const plpgsql_yytname[] =
{
  "$end", "error", "$undefined", "K_ALIAS", "K_ASSIGN", "K_BEGIN", 
  "K_CLOSE", "K_CONSTANT", "K_CURSOR", "K_DEBUG", "K_DECLARE", 
  "K_DEFAULT", "K_DIAGNOSTICS", "K_DOTDOT", "K_ELSE", "K_ELSIF", "K_END", 
  "K_EXCEPTION", "K_EXECUTE", "K_EXIT", "K_FOR", "K_FETCH", "K_FROM", 
  "K_GET", "K_IF", "K_IN", "K_INFO", "K_INTO", "K_IS", "K_LOG", "K_LOOP", 
  "K_NEXT", "K_NOT", "K_NOTICE", "K_NULL", "K_OPEN", "K_PERFORM", 
  "K_ROW_COUNT", "K_RAISE", "K_RECORD", "K_RENAME", "K_RESULT_OID", 
  "K_RETURN", "K_RETURN_NEXT", "K_REVERSE", "K_SELECT", "K_THEN", "K_TO", 
  "K_TYPE", "K_WARNING", "K_WHEN", "K_WHILE", "T_FUNCTION", "T_TRIGGER", 
  "T_STRING", "T_NUMBER", "T_VARIABLE", "T_ROW", "T_RECORD", "T_DTYPE", 
  "T_LABEL", "T_WORD", "T_ERROR", "O_OPTION", "O_DUMP", "';'", "'<'", 
  "'>'", "'('", "')'", "','", "$accept", "pl_function", "comp_optsect", 
  "comp_options", "comp_option", "opt_semi", "pl_block", "decl_sect", 
  "decl_start", "decl_stmts", "decl_stmt", "decl_statement", "@1", 
  "decl_cursor_query", "decl_cursor_args", "decl_cursor_arglist", 
  "decl_cursor_arg", "decl_is_from", "decl_aliasitem", "decl_rowtype", 
  "decl_varname", "decl_renname", "decl_const", "decl_datatype", 
  "decl_notnull", "decl_defval", "decl_defkey", "proc_sect", "proc_stmts", 
  "proc_stmt", "stmt_perform", "stmt_assign", "stmt_getdiag", 
  "getdiag_list", "getdiag_item", "getdiag_target", "assign_var", 
  "stmt_if", "stmt_else", "stmt_loop", "stmt_while", "stmt_fori", 
  "fori_var", "fori_varname", "fori_lower", "stmt_fors", "stmt_dynfors", 
  "fors_target", "stmt_select", "stmt_exit", "stmt_return", 
  "stmt_return_next", "stmt_raise", "raise_msg", "raise_level", 
  "raise_params", "raise_param", "loop_body", "stmt_execsql", 
  "stmt_dynexecute", "stmt_open", "stmt_fetch", "stmt_close", 
  "cursor_varptr", "cursor_variable", "execsql_start", "expr_until_semi", 
  "expr_until_then", "expr_until_loop", "opt_label", "opt_exitlabel", 
  "opt_exitcond", "opt_lblname", "lno", 0
};
#endif

# ifdef PLPGSQL_YYPRINT
/* PLPGSQL_YYTOKNUM[PLPGSQL_YYLEX-NUM] -- Internal token number corresponding to
   token PLPGSQL_YYLEX-NUM.  */
static const unsigned short plpgsql_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    59,    60,    62,    40,    41,
      44
};
# endif

/* PLPGSQL_YYR1[PLPGSQL_YYN] -- Symbol number of symbol that rule PLPGSQL_YYN derives.  */
static const unsigned char plpgsql_yyr1[] =
{
       0,    71,    72,    72,    73,    73,    74,    74,    75,    76,
      76,    77,    78,    78,    78,    79,    80,    80,    81,    81,
      81,    82,    82,    82,    82,    82,    83,    82,    84,    85,
      85,    86,    86,    87,    88,    88,    89,    90,    91,    92,
      93,    93,    94,    95,    95,    96,    96,    97,    97,    98,
      98,    99,    99,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   101,   102,   103,   104,   104,   105,   105,
     106,   107,   108,   109,   109,   109,   110,   111,   112,   113,
     114,   114,   115,   116,   117,   118,   118,   119,   120,   121,
     122,   123,   123,   124,   125,   125,   125,   125,   125,   125,
     126,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   136,   137,   138,   139,   140,   140,   141,   141,
     142,   142,   143,   144
};

/* PLPGSQL_YYR2[PLPGSQL_YYN] -- Number of symbols composing right hand side of rule PLPGSQL_YYN.  */
static const unsigned char plpgsql_yyr2[] =
{
       0,     2,     4,     4,     0,     1,     2,     1,     2,     0,
       1,     5,     1,     2,     3,     1,     2,     1,     5,     1,
       1,     5,     3,     3,     5,     5,     0,     7,     0,     0,
       3,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     0,     2,     1,     1,     1,     1,     0,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     5,     5,     3,     1,     1,
       1,     1,     8,     0,     5,     2,     4,     5,     8,     1,
       1,     1,     0,     8,     8,     1,     1,     2,     4,     2,
       2,     6,     5,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     4,     2,     3,     3,     4,     4,     1,
       1,     1,     1,     0,     0,     0,     0,     5,     0,     1,
       1,     2,     1,     0
};

/* PLPGSQL_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when PLPGSQL_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char plpgsql_yydefact[] =
{
       0,     4,     4,     0,     0,   126,     5,     7,   126,     1,
       8,     0,     9,     0,    12,     6,     9,     0,    10,     2,
     133,    15,    13,     3,   132,     0,   126,    19,     0,    38,
       0,    14,    17,    20,    40,     0,   133,   133,   133,   133,
       0,   133,   133,   133,   133,   133,   133,   133,    81,   121,
     122,     0,     0,   126,    52,    68,    54,    69,   133,    55,
      56,    57,    58,    59,    67,    60,    61,    62,    63,    64,
      65,    66,    70,    71,    72,   133,    12,    39,     0,     0,
      16,     0,    41,    26,     0,    37,     0,    42,   127,     0,
     123,   128,     0,   133,   124,     0,   123,     0,    99,   100,
      97,    53,    11,    51,     0,   114,   133,   133,   133,     0,
       0,     0,    29,    22,    23,    43,   120,     0,   115,   129,
       0,     0,     0,   126,   119,   116,    73,   109,   104,   107,
     108,   106,   105,     0,   123,     0,   126,   125,     0,     0,
      36,     0,     0,     0,     0,     0,   118,   123,   130,    98,
     117,    80,     0,     0,    83,   103,     0,    74,    90,    96,
      95,    91,     0,    89,     0,     0,    86,   126,    25,    18,
      24,     0,    31,    42,    35,    34,     0,    44,    47,    48,
      45,    21,    46,   131,    75,     0,     0,   126,   133,     0,
     102,     0,     0,   111,    92,     0,     0,    87,    30,     0,
      33,    28,     0,    78,    79,    77,    85,   124,     0,   112,
     101,   110,   125,   125,   125,     0,    32,    27,     0,   126,
       0,   126,   126,   126,   113,    76,    83,    82,    88,    94,
      93,    84
};

/* PLPGSQL_YYDEFGOTO[NTERM-NUM]. */
static const short plpgsql_yydefgoto[] =
{
      -1,     3,     5,     6,     7,    19,    51,    13,    22,    31,
      32,    33,   112,   217,   143,   171,   172,   176,   141,    86,
      34,    78,    87,   115,   145,   181,   182,   165,    53,    54,
      55,    56,    57,   152,   205,   153,    58,    59,   189,    60,
      61,    62,   162,   163,   212,    63,    64,   164,    65,    66,
      67,    68,    69,   156,   133,   192,   193,   166,    70,    71,
      72,    73,    74,   125,   117,    75,   118,   123,   167,    76,
     120,   149,    25,    26
};

/* PLPGSQL_YYPACT[STATE-NUM] -- Index in PLPGSQL_YYTABLE of the portion describing
   STATE-NUM.  */
#define PLPGSQL_YYPACT_NINF -161
static const short plpgsql_yypact[] =
{
       3,   -44,   -44,    39,   -14,    16,   -44,  -161,    16,  -161,
    -161,    18,    21,    84,    81,  -161,    21,    37,  -161,  -161,
    -161,  -161,     4,  -161,  -161,    32,   128,  -161,    42,  -161,
      36,     4,  -161,  -161,    28,    38,  -161,  -161,  -161,  -161,
      92,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,    49,   100,    45,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,    27,  -161,    72,    37,
    -161,   101,  -161,  -161,    55,  -161,    57,  -161,  -161,    67,
    -161,    64,    67,  -161,  -161,    69,  -161,   148,  -161,  -161,
    -161,  -161,  -161,  -161,   122,  -161,  -161,  -161,  -161,    42,
      60,    70,    65,  -161,  -161,    96,  -161,    73,  -161,  -161,
     -33,   108,    85,    94,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,    86,  -161,   -31,   128,  -161,    77,    76,
    -161,    80,    87,    -4,   120,     9,  -161,  -161,  -161,  -161,
    -161,  -161,   -37,   154,    79,  -161,   -36,  -161,  -161,  -161,
    -161,  -161,   134,  -161,   137,   151,  -161,   128,  -161,  -161,
    -161,    26,  -161,  -161,  -161,  -161,   123,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,    85,   -19,   128,  -161,   153,
    -161,   116,   -27,  -161,  -161,    -3,   145,  -161,  -161,    87,
    -161,  -161,   172,  -161,  -161,  -161,  -161,  -161,   155,  -161,
    -161,  -161,  -161,  -161,  -161,   113,  -161,  -161,   -19,    94,
     115,   128,   128,   128,  -161,  -161,    79,  -161,  -161,  -161,
    -161,  -161
};

/* PLPGSQL_YYPGOTO[NTERM-NUM].  */
static const short plpgsql_yypgoto[] =
{
    -161,  -161,   180,  -161,   177,   169,    40,  -161,  -161,  -161,
     156,  -161,  -161,  -161,  -161,  -161,   -13,  -161,  -161,  -161,
    -137,    82,  -161,    15,  -161,  -161,  -161,   -26,  -161,   139,
    -161,  -161,  -161,  -161,   -23,    11,  -161,  -161,   -28,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,     7,  -146,  -161,  -161,
    -161,  -161,  -161,  -161,   109,  -161,   -55,    -7,  -160,    41,
    -161,  -161,   124,   -35
};

/* PLPGSQL_YYTABLE[PLPGSQL_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what PLPGSQL_YYDEFACT says.
   If PLPGSQL_YYTABLE_NINF, syntax error.  */
#define PLPGSQL_YYTABLE_NINF -51
static const short plpgsql_yytable[] =
{
      52,    89,    90,    91,    92,   173,    94,    95,    96,    97,
      98,    99,   100,   178,    27,   213,   174,   147,   203,     4,
     179,   197,   204,   104,   175,   158,   159,   160,   184,   190,
     161,    81,   148,   185,   191,    82,    83,    21,   210,     9,
     105,   126,   214,   191,    28,    12,    14,   106,    16,    14,
      10,    36,   221,   222,   223,     1,     2,   107,   122,   -50,
     -50,   -50,   173,    37,    38,    29,    39,    84,    40,    41,
      30,   135,   136,   137,   180,   228,   229,   230,   108,   157,
      42,    43,    11,    44,    17,    85,    18,    45,    46,    20,
      47,    21,   183,   187,   188,   198,   199,   154,    24,    35,
      36,    48,    79,    77,    93,    88,    49,    50,   -49,   -49,
     -49,    11,    37,    38,   101,    39,   102,    40,    41,   109,
     113,   111,   114,   116,   119,   124,   134,   139,   144,    42,
      43,   140,    44,   142,    36,   150,    45,    46,   146,    47,
     155,   151,   168,   169,   -49,   170,    37,    38,    29,    39,
      48,    40,    41,   207,   177,    49,    50,   127,   186,   194,
      11,   206,   195,    42,    43,   128,    44,   196,   201,   208,
      45,    46,   209,    47,   129,   215,   218,   130,   224,   220,
     227,   131,     8,    15,    48,    23,   216,    80,   200,    49,
      50,   138,   103,   226,    11,   225,   202,   132,   231,   211,
     219,   121,     0,   110
};

static const short plpgsql_yycheck[] =
{
      26,    36,    37,    38,    39,   142,    41,    42,    43,    44,
      45,    46,    47,     4,    10,    18,    20,    50,    37,    63,
      11,   167,    41,    58,    28,    56,    57,    58,    65,    65,
      61,     3,    65,    70,    70,     7,     8,    10,    65,     0,
      75,    96,    45,    70,    40,     5,     5,    20,     8,     8,
      64,     6,   212,   213,   214,    52,    53,    30,    93,    14,
      15,    16,   199,    18,    19,    61,    21,    39,    23,    24,
      66,   106,   107,   108,    65,   221,   222,   223,    51,   134,
      35,    36,    66,    38,    66,    57,    65,    42,    43,     5,
      45,    10,   147,    14,    15,    69,    70,   123,    61,    67,
       6,    56,    66,    61,    12,    67,    61,    62,    14,    15,
      16,    66,    18,    19,    65,    21,    16,    23,    24,    47,
      65,    20,    65,    56,    60,    56,     4,    67,    32,    35,
      36,    61,    38,    68,     6,    27,    42,    43,    65,    45,
      54,    56,    65,    67,    16,    65,    18,    19,    61,    21,
      56,    23,    24,   188,    34,    61,    62,     9,     4,    25,
      66,   187,    25,    35,    36,    17,    38,    16,    45,    16,
      42,    43,    56,    45,    26,    30,     4,    29,    65,    24,
      65,    33,     2,     6,    56,    16,   199,    31,   173,    61,
      62,   109,    53,   219,    66,   218,   185,    49,   226,   192,
     207,    92,    -1,    79
};

/* PLPGSQL_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char plpgsql_yystos[] =
{
       0,    52,    53,    72,    63,    73,    74,    75,    73,     0,
      64,    66,    77,    78,   140,    75,    77,    66,    65,    76,
       5,    10,    79,    76,    61,   143,   144,    10,    40,    61,
      66,    80,    81,    82,    91,    67,     6,    18,    19,    21,
      23,    24,    35,    36,    38,    42,    43,    45,    56,    61,
      62,    77,    98,    99,   100,   101,   102,   103,   107,   108,
     110,   111,   112,   116,   117,   119,   120,   121,   122,   123,
     129,   130,   131,   132,   133,   136,   140,    61,    92,    66,
      81,     3,     7,     8,    39,    57,    90,    93,    67,   144,
     144,   144,   144,    12,   144,   144,   144,   144,   144,   144,
     144,    65,    16,   100,   144,   144,    20,    30,    51,    47,
     143,    20,    83,    65,    65,    94,    56,   135,   137,    60,
     141,   135,   144,   138,    56,   134,   137,     9,    17,    26,
      29,    33,    49,   125,     4,   144,   144,   144,    92,    67,
      61,    89,    68,    85,    32,    95,    65,    50,    65,   142,
      27,    56,   104,   106,    98,    54,   124,   137,    56,    57,
      58,    61,   113,   114,   118,    98,   128,   139,    65,    67,
      65,    86,    87,    91,    20,    28,    88,    34,     4,    11,
      65,    96,    97,   137,    65,    70,     4,    14,    15,   109,
      65,    70,   126,   127,    25,    25,    16,   128,    69,    70,
      94,    45,   106,    37,    41,   105,    98,   144,    16,    56,
      65,   127,   115,    18,    45,    30,    87,    84,     4,   138,
      24,   139,   139,   139,    65,   105,    98,    65,   128,   128,
     128,   109
};

#if ! defined (PLPGSQL_YYSIZE_T) && defined (__SIZE_TYPE__)
# define PLPGSQL_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (PLPGSQL_YYSIZE_T) && defined (size_t)
# define PLPGSQL_YYSIZE_T size_t
#endif
#if ! defined (PLPGSQL_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define PLPGSQL_YYSIZE_T size_t
# endif
#endif
#if ! defined (PLPGSQL_YYSIZE_T)
# define PLPGSQL_YYSIZE_T unsigned int
#endif

#define plpgsql_yyerrok		(plpgsql_yyerrstatus = 0)
#define plpgsql_yyclearin	(plpgsql_yychar = PLPGSQL_YYEMPTY)
#define PLPGSQL_YYEMPTY		(-2)
#define PLPGSQL_YYEOF		0

#define PLPGSQL_YYACCEPT	goto plpgsql_yyacceptlab
#define PLPGSQL_YYABORT		goto plpgsql_yyabortlab
#define PLPGSQL_YYERROR		goto plpgsql_yyerrlab1


/* Like PLPGSQL_YYERROR except do call plpgsql_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of PLPGSQL_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define PLPGSQL_YYFAIL		goto plpgsql_yyerrlab

#define PLPGSQL_YYRECOVERING()  (!!plpgsql_yyerrstatus)

#define PLPGSQL_YYBACKUP(Token, Value)					\
do								\
  if (plpgsql_yychar == PLPGSQL_YYEMPTY && plpgsql_yylen == 1)				\
    {								\
      plpgsql_yychar = (Token);						\
      plpgsql_yylval = (Value);						\
      plpgsql_yytoken = PLPGSQL_YYTRANSLATE (plpgsql_yychar);				\
      PLPGSQL_YYPOPSTACK;						\
      goto plpgsql_yybackup;						\
    }								\
  else								\
    { 								\
      plpgsql_yyerror ("syntax error: cannot back up");\
      PLPGSQL_YYERROR;							\
    }								\
while (0)

#define PLPGSQL_YYTERROR	1
#define PLPGSQL_YYERRCODE	256

/* PLPGSQL_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef PLPGSQL_YYLLOC_DEFAULT
# define PLPGSQL_YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* PLPGSQL_YYLEX -- calling `plpgsql_yylex' with the right arguments.  */

#ifdef PLPGSQL_YYLEX_PARAM
# define PLPGSQL_YYLEX plpgsql_yylex (PLPGSQL_YYLEX_PARAM)
#else
# define PLPGSQL_YYLEX plpgsql_yylex ()
#endif

/* Enable debugging if requested.  */
#if PLPGSQL_YYDEBUG

# ifndef PLPGSQL_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define PLPGSQL_YYFPRINTF fprintf
# endif

# define PLPGSQL_YYDPRINTF(Args)			\
do {						\
  if (plpgsql_yydebug)					\
    PLPGSQL_YYFPRINTF Args;				\
} while (0)

# define PLPGSQL_YYDSYMPRINT(Args)			\
do {						\
  if (plpgsql_yydebug)					\
    plpgsql_yysymprint Args;				\
} while (0)

# define PLPGSQL_YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (plpgsql_yydebug)							\
    {								\
      PLPGSQL_YYFPRINTF (stderr, "%s ", Title);				\
      plpgsql_yysymprint (stderr, 					\
                  Token, Value);	\
      PLPGSQL_YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| plpgsql_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
plpgsql_yy_stack_print (short *bottom, short *top)
#else
static void
plpgsql_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  PLPGSQL_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    PLPGSQL_YYFPRINTF (stderr, " %d", *bottom);
  PLPGSQL_YYFPRINTF (stderr, "\n");
}

# define PLPGSQL_YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (plpgsql_yydebug)							\
    plpgsql_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the PLPGSQL_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
plpgsql_yy_reduce_print (int plpgsql_yyrule)
#else
static void
plpgsql_yy_reduce_print (plpgsql_yyrule)
    int plpgsql_yyrule;
#endif
{
  int plpgsql_yyi;
  unsigned int plpgsql_yylineno = plpgsql_yyrline[plpgsql_yyrule];
  PLPGSQL_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             plpgsql_yyrule - 1, plpgsql_yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (plpgsql_yyi = plpgsql_yyprhs[plpgsql_yyrule]; 0 <= plpgsql_yyrhs[plpgsql_yyi]; plpgsql_yyi++)
    PLPGSQL_YYFPRINTF (stderr, "%s ", plpgsql_yytname [plpgsql_yyrhs[plpgsql_yyi]]);
  PLPGSQL_YYFPRINTF (stderr, "-> %s\n", plpgsql_yytname [plpgsql_yyr1[plpgsql_yyrule]]);
}

# define PLPGSQL_YY_REDUCE_PRINT(Rule)		\
do {					\
  if (plpgsql_yydebug)				\
    plpgsql_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int plpgsql_yydebug;
#else /* !PLPGSQL_YYDEBUG */
# define PLPGSQL_YYDPRINTF(Args)
# define PLPGSQL_YYDSYMPRINT(Args)
# define PLPGSQL_YYDSYMPRINTF(Title, Token, Value, Location)
# define PLPGSQL_YY_STACK_PRINT(Bottom, Top)
# define PLPGSQL_YY_REDUCE_PRINT(Rule)
#endif /* !PLPGSQL_YYDEBUG */


/* PLPGSQL_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	PLPGSQL_YYINITDEPTH
# define PLPGSQL_YYINITDEPTH 200
#endif

/* PLPGSQL_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < PLPGSQL_YYSTACK_BYTES (PLPGSQL_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if PLPGSQL_YYMAXDEPTH == 0
# undef PLPGSQL_YYMAXDEPTH
#endif

#ifndef PLPGSQL_YYMAXDEPTH
# define PLPGSQL_YYMAXDEPTH 10000
#endif



#if PLPGSQL_YYERROR_VERBOSE

# ifndef plpgsql_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define plpgsql_yystrlen strlen
#  else
/* Return the length of PLPGSQL_YYSTR.  */
static PLPGSQL_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
plpgsql_yystrlen (const char *plpgsql_yystr)
#   else
plpgsql_yystrlen (plpgsql_yystr)
     const char *plpgsql_yystr;
#   endif
{
  register const char *plpgsql_yys = plpgsql_yystr;

  while (*plpgsql_yys++ != '\0')
    continue;

  return plpgsql_yys - plpgsql_yystr - 1;
}
#  endif
# endif

# ifndef plpgsql_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define plpgsql_yystpcpy stpcpy
#  else
/* Copy PLPGSQL_YYSRC to PLPGSQL_YYDEST, returning the address of the terminating '\0' in
   PLPGSQL_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
plpgsql_yystpcpy (char *plpgsql_yydest, const char *plpgsql_yysrc)
#   else
plpgsql_yystpcpy (plpgsql_yydest, plpgsql_yysrc)
     char *plpgsql_yydest;
     const char *plpgsql_yysrc;
#   endif
{
  register char *plpgsql_yyd = plpgsql_yydest;
  register const char *plpgsql_yys = plpgsql_yysrc;

  while ((*plpgsql_yyd++ = *plpgsql_yys++) != '\0')
    continue;

  return plpgsql_yyd - 1;
}
#  endif
# endif

#endif /* !PLPGSQL_YYERROR_VERBOSE */



#if PLPGSQL_YYDEBUG
/*--------------------------------.
| Print this symbol on PLPGSQL_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
plpgsql_yysymprint (FILE *plpgsql_yyoutput, int plpgsql_yytype, PLPGSQL_YYSTYPE *plpgsql_yyvaluep)
#else
static void
plpgsql_yysymprint (plpgsql_yyoutput, plpgsql_yytype, plpgsql_yyvaluep)
    FILE *plpgsql_yyoutput;
    int plpgsql_yytype;
    PLPGSQL_YYSTYPE *plpgsql_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) plpgsql_yyvaluep;

  if (plpgsql_yytype < PLPGSQL_YYNTOKENS)
    {
      PLPGSQL_YYFPRINTF (plpgsql_yyoutput, "token %s (", plpgsql_yytname[plpgsql_yytype]);
# ifdef PLPGSQL_YYPRINT
      PLPGSQL_YYPRINT (plpgsql_yyoutput, plpgsql_yytoknum[plpgsql_yytype], *plpgsql_yyvaluep);
# endif
    }
  else
    PLPGSQL_YYFPRINTF (plpgsql_yyoutput, "nterm %s (", plpgsql_yytname[plpgsql_yytype]);

  switch (plpgsql_yytype)
    {
      default:
        break;
    }
  PLPGSQL_YYFPRINTF (plpgsql_yyoutput, ")");
}

#endif /* ! PLPGSQL_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
plpgsql_yydestruct (int plpgsql_yytype, PLPGSQL_YYSTYPE *plpgsql_yyvaluep)
#else
static void
plpgsql_yydestruct (plpgsql_yytype, plpgsql_yyvaluep)
    int plpgsql_yytype;
    PLPGSQL_YYSTYPE *plpgsql_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) plpgsql_yyvaluep;

  switch (plpgsql_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef PLPGSQL_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int plpgsql_yyparse (void *PLPGSQL_YYPARSE_PARAM);
# else
int plpgsql_yyparse ();
# endif
#else /* ! PLPGSQL_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int plpgsql_yyparse (void);
#else
int plpgsql_yyparse ();
#endif
#endif /* ! PLPGSQL_YYPARSE_PARAM */



/* The lookahead symbol.  */
int plpgsql_yychar;

/* The semantic value of the lookahead symbol.  */
PLPGSQL_YYSTYPE plpgsql_yylval;

/* Number of syntax errors so far.  */
int plpgsql_yynerrs;



/*----------.
| plpgsql_yyparse.  |
`----------*/

#ifdef PLPGSQL_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int plpgsql_yyparse (void *PLPGSQL_YYPARSE_PARAM)
# else
int plpgsql_yyparse (PLPGSQL_YYPARSE_PARAM)
  void *PLPGSQL_YYPARSE_PARAM;
# endif
#else /* ! PLPGSQL_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
plpgsql_yyparse (void)
#else
int
plpgsql_yyparse ()

#endif
#endif
{
  
  register int plpgsql_yystate;
  register int plpgsql_yyn;
  int plpgsql_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int plpgsql_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int plpgsql_yytoken = 0;

  /* Three stacks and their tools:
     `plpgsql_yyss': related to states,
     `plpgsql_yyvs': related to semantic values,
     `plpgsql_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow plpgsql_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	plpgsql_yyssa[PLPGSQL_YYINITDEPTH];
  short *plpgsql_yyss = plpgsql_yyssa;
  register short *plpgsql_yyssp;

  /* The semantic value stack.  */
  PLPGSQL_YYSTYPE plpgsql_yyvsa[PLPGSQL_YYINITDEPTH];
  PLPGSQL_YYSTYPE *plpgsql_yyvs = plpgsql_yyvsa;
  register PLPGSQL_YYSTYPE *plpgsql_yyvsp;



#define PLPGSQL_YYPOPSTACK   (plpgsql_yyvsp--, plpgsql_yyssp--)

  PLPGSQL_YYSIZE_T plpgsql_yystacksize = PLPGSQL_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  PLPGSQL_YYSTYPE plpgsql_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int plpgsql_yylen;

  PLPGSQL_YYDPRINTF ((stderr, "Starting parse\n"));

  plpgsql_yystate = 0;
  plpgsql_yyerrstatus = 0;
  plpgsql_yynerrs = 0;
  plpgsql_yychar = PLPGSQL_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  plpgsql_yyssp = plpgsql_yyss;
  plpgsql_yyvsp = plpgsql_yyvs;

  goto plpgsql_yysetstate;

/*------------------------------------------------------------.
| plpgsql_yynewstate -- Push a new state, which is found in plpgsql_yystate.  |
`------------------------------------------------------------*/
 plpgsql_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  plpgsql_yyssp++;

 plpgsql_yysetstate:
  *plpgsql_yyssp = plpgsql_yystate;

  if (plpgsql_yyss + plpgsql_yystacksize - 1 <= plpgsql_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      PLPGSQL_YYSIZE_T plpgsql_yysize = plpgsql_yyssp - plpgsql_yyss + 1;

#ifdef plpgsql_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	PLPGSQL_YYSTYPE *plpgsql_yyvs1 = plpgsql_yyvs;
	short *plpgsql_yyss1 = plpgsql_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if plpgsql_yyoverflow is a macro.  */
	plpgsql_yyoverflow ("parser stack overflow",
		    &plpgsql_yyss1, plpgsql_yysize * sizeof (*plpgsql_yyssp),
		    &plpgsql_yyvs1, plpgsql_yysize * sizeof (*plpgsql_yyvsp),

		    &plpgsql_yystacksize);

	plpgsql_yyss = plpgsql_yyss1;
	plpgsql_yyvs = plpgsql_yyvs1;
      }
#else /* no plpgsql_yyoverflow */
# ifndef PLPGSQL_YYSTACK_RELOCATE
      goto plpgsql_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (PLPGSQL_YYMAXDEPTH <= plpgsql_yystacksize)
	goto plpgsql_yyoverflowlab;
      plpgsql_yystacksize *= 2;
      if (PLPGSQL_YYMAXDEPTH < plpgsql_yystacksize)
	plpgsql_yystacksize = PLPGSQL_YYMAXDEPTH;

      {
	short *plpgsql_yyss1 = plpgsql_yyss;
	union plpgsql_yyalloc *plpgsql_yyptr =
	  (union plpgsql_yyalloc *) PLPGSQL_YYSTACK_ALLOC (PLPGSQL_YYSTACK_BYTES (plpgsql_yystacksize));
	if (! plpgsql_yyptr)
	  goto plpgsql_yyoverflowlab;
	PLPGSQL_YYSTACK_RELOCATE (plpgsql_yyss);
	PLPGSQL_YYSTACK_RELOCATE (plpgsql_yyvs);

#  undef PLPGSQL_YYSTACK_RELOCATE
	if (plpgsql_yyss1 != plpgsql_yyssa)
	  PLPGSQL_YYSTACK_FREE (plpgsql_yyss1);
      }
# endif
#endif /* no plpgsql_yyoverflow */

      plpgsql_yyssp = plpgsql_yyss + plpgsql_yysize - 1;
      plpgsql_yyvsp = plpgsql_yyvs + plpgsql_yysize - 1;


      PLPGSQL_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) plpgsql_yystacksize));

      if (plpgsql_yyss + plpgsql_yystacksize - 1 <= plpgsql_yyssp)
	PLPGSQL_YYABORT;
    }

  PLPGSQL_YYDPRINTF ((stderr, "Entering state %d\n", plpgsql_yystate));

  goto plpgsql_yybackup;

/*-----------.
| plpgsql_yybackup.  |
`-----------*/
plpgsql_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* plpgsql_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];
  if (plpgsql_yyn == PLPGSQL_YYPACT_NINF)
    goto plpgsql_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* PLPGSQL_YYCHAR is either PLPGSQL_YYEMPTY or PLPGSQL_YYEOF or a valid lookahead symbol.  */
  if (plpgsql_yychar == PLPGSQL_YYEMPTY)
    {
      PLPGSQL_YYDPRINTF ((stderr, "Reading a token: "));
      plpgsql_yychar = PLPGSQL_YYLEX;
    }

  if (plpgsql_yychar <= PLPGSQL_YYEOF)
    {
      plpgsql_yychar = plpgsql_yytoken = PLPGSQL_YYEOF;
      PLPGSQL_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      plpgsql_yytoken = PLPGSQL_YYTRANSLATE (plpgsql_yychar);
      PLPGSQL_YYDSYMPRINTF ("Next token is", plpgsql_yytoken, &plpgsql_yylval, &plpgsql_yylloc);
    }

  /* If the proper action on seeing token PLPGSQL_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  plpgsql_yyn += plpgsql_yytoken;
  if (plpgsql_yyn < 0 || PLPGSQL_YYLAST < plpgsql_yyn || plpgsql_yycheck[plpgsql_yyn] != plpgsql_yytoken)
    goto plpgsql_yydefault;
  plpgsql_yyn = plpgsql_yytable[plpgsql_yyn];
  if (plpgsql_yyn <= 0)
    {
      if (plpgsql_yyn == 0 || plpgsql_yyn == PLPGSQL_YYTABLE_NINF)
	goto plpgsql_yyerrlab;
      plpgsql_yyn = -plpgsql_yyn;
      goto plpgsql_yyreduce;
    }

  if (plpgsql_yyn == PLPGSQL_YYFINAL)
    PLPGSQL_YYACCEPT;

  /* Shift the lookahead token.  */
  PLPGSQL_YYDPRINTF ((stderr, "Shifting token %s, ", plpgsql_yytname[plpgsql_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (plpgsql_yychar != PLPGSQL_YYEOF)
    plpgsql_yychar = PLPGSQL_YYEMPTY;

  *++plpgsql_yyvsp = plpgsql_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (plpgsql_yyerrstatus)
    plpgsql_yyerrstatus--;

  plpgsql_yystate = plpgsql_yyn;
  goto plpgsql_yynewstate;


/*-----------------------------------------------------------.
| plpgsql_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
plpgsql_yydefault:
  plpgsql_yyn = plpgsql_yydefact[plpgsql_yystate];
  if (plpgsql_yyn == 0)
    goto plpgsql_yyerrlab;
  goto plpgsql_yyreduce;


/*-----------------------------.
| plpgsql_yyreduce -- Do a reduction.  |
`-----------------------------*/
plpgsql_yyreduce:
  /* plpgsql_yyn is the number of a rule to reduce with.  */
  plpgsql_yylen = plpgsql_yyr2[plpgsql_yyn];

  /* If PLPGSQL_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets PLPGSQL_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to PLPGSQL_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that PLPGSQL_YYVAL may be used uninitialized.  */
  plpgsql_yyval = plpgsql_yyvsp[1-plpgsql_yylen];


  PLPGSQL_YY_REDUCE_PRINT (plpgsql_yyn);
  switch (plpgsql_yyn)
    {
        case 2:
#line 212 "gram.y"
    {
						plpgsql_yylval.program = (PLpgSQL_stmt_block *)plpgsql_yyvsp[-1].stmt;
					}
    break;

  case 3:
#line 216 "gram.y"
    {
						plpgsql_yylval.program = (PLpgSQL_stmt_block *)plpgsql_yyvsp[-1].stmt;
					}
    break;

  case 8:
#line 230 "gram.y"
    {
						plpgsql_DumpExecTree = 1;
					}
    break;

  case 11:
#line 240 "gram.y"
    {
						PLpgSQL_stmt_block *new;

						new = malloc(sizeof(PLpgSQL_stmt_block));
						memset(new, 0, sizeof(PLpgSQL_stmt_block));

						new->cmd_type	= PLPGSQL_STMT_BLOCK;
						new->lineno		= plpgsql_yyvsp[-2].ival;
						new->label		= plpgsql_yyvsp[-4].declhdr.label;
						new->n_initvars = plpgsql_yyvsp[-4].declhdr.n_initvars;
						new->initvarnos = plpgsql_yyvsp[-4].declhdr.initvarnos;
						new->body		= plpgsql_yyvsp[-1].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 12:
#line 261 "gram.y"
    {
						plpgsql_ns_setlocal(false);
						plpgsql_yyval.declhdr.label	  = plpgsql_yyvsp[0].str;
						plpgsql_yyval.declhdr.n_initvars = 0;
						plpgsql_yyval.declhdr.initvarnos = NULL;
						plpgsql_add_initdatums(NULL);
					}
    break;

  case 13:
#line 269 "gram.y"
    {
						plpgsql_ns_setlocal(false);
						plpgsql_yyval.declhdr.label	  = plpgsql_yyvsp[-1].str;
						plpgsql_yyval.declhdr.n_initvars = 0;
						plpgsql_yyval.declhdr.initvarnos = NULL;
						plpgsql_add_initdatums(NULL);
					}
    break;

  case 14:
#line 277 "gram.y"
    {
						plpgsql_ns_setlocal(false);
						if (plpgsql_yyvsp[0].str != NULL)
							plpgsql_yyval.declhdr.label = plpgsql_yyvsp[0].str;
						else
							plpgsql_yyval.declhdr.label = plpgsql_yyvsp[-2].str;
						plpgsql_yyval.declhdr.n_initvars = plpgsql_add_initdatums(&(plpgsql_yyval.declhdr.initvarnos));
					}
    break;

  case 15:
#line 288 "gram.y"
    {
						plpgsql_ns_setlocal(true);
					}
    break;

  case 16:
#line 294 "gram.y"
    {	plpgsql_yyval.str = plpgsql_yyvsp[0].str;	}
    break;

  case 17:
#line 296 "gram.y"
    {	plpgsql_yyval.str = plpgsql_yyvsp[0].str;	}
    break;

  case 18:
#line 300 "gram.y"
    {	plpgsql_yyval.str = plpgsql_yyvsp[-2].str;	}
    break;

  case 19:
#line 302 "gram.y"
    {	plpgsql_yyval.str = NULL;	}
    break;

  case 20:
#line 304 "gram.y"
    {	plpgsql_yyval.str = NULL;	}
    break;

  case 21:
#line 308 "gram.y"
    {
						PLpgSQL_var		*new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-4].varname.name;
						new->lineno		= plpgsql_yyvsp[-4].varname.lineno;

						new->datatype	= plpgsql_yyvsp[-2].dtype;
						new->isconst	= plpgsql_yyvsp[-3].ival;
						new->notnull	= plpgsql_yyvsp[-1].ival;
						new->default_val = plpgsql_yyvsp[0].expr;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
												plpgsql_yyvsp[-4].varname.name);
					}
    break;

  case 22:
#line 328 "gram.y"
    {
						PLpgSQL_rec		*new;

						new = malloc(sizeof(PLpgSQL_rec));

						new->dtype		= PLPGSQL_DTYPE_REC;
						new->refname	= plpgsql_yyvsp[-2].varname.name;
						new->lineno		= plpgsql_yyvsp[-2].varname.lineno;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_REC, new->recno,
												plpgsql_yyvsp[-2].varname.name);
					}
    break;

  case 23:
#line 342 "gram.y"
    {
						plpgsql_yyvsp[-1].row->dtype		= PLPGSQL_DTYPE_ROW;
						plpgsql_yyvsp[-1].row->refname		= plpgsql_yyvsp[-2].varname.name;
						plpgsql_yyvsp[-1].row->lineno		= plpgsql_yyvsp[-2].varname.lineno;

						plpgsql_adddatum((PLpgSQL_datum *)plpgsql_yyvsp[-1].row);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_ROW, plpgsql_yyvsp[-1].row->rowno,
												plpgsql_yyvsp[-2].varname.name);
					}
    break;

  case 24:
#line 352 "gram.y"
    {
						plpgsql_ns_additem(plpgsql_yyvsp[-1].nsitem->itemtype,
										plpgsql_yyvsp[-1].nsitem->itemno, plpgsql_yyvsp[-4].varname.name);
					}
    break;

  case 25:
#line 357 "gram.y"
    {
						plpgsql_ns_rename(plpgsql_yyvsp[-3].str, plpgsql_yyvsp[-1].str);
					}
    break;

  case 26:
#line 361 "gram.y"
    { plpgsql_ns_push(NULL); }
    break;

  case 27:
#line 363 "gram.y"
    {
						PLpgSQL_var *new;
						PLpgSQL_expr *curname_def;
						char		buf[1024];
						char		*cp1;
						char		*cp2;

						/* pop local namespace for cursor args */
						plpgsql_ns_pop();

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						curname_def = malloc(sizeof(PLpgSQL_expr));
						memset(curname_def, 0, sizeof(PLpgSQL_expr));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-6].varname.name;
						new->lineno		= plpgsql_yyvsp[-6].varname.lineno;

						curname_def->dtype = PLPGSQL_DTYPE_EXPR;
						strcpy(buf, "SELECT '");
						cp1 = new->refname;
						cp2 = buf + strlen(buf);
						while (*cp1 != '\0')
						{
							if (*cp1 == '\\' || *cp1 == '\'')
								*cp2++ = '\\';
							*cp2++ = *cp1++;
						}
						strcpy(cp2, "'::refcursor");
						curname_def->query = strdup(buf);
						new->default_val = curname_def;

						new->datatype = plpgsql_parse_datatype("refcursor");

						new->cursor_explicit_expr = plpgsql_yyvsp[0].expr;
						if (plpgsql_yyvsp[-3].row == NULL)
							new->cursor_explicit_argrow = -1;
						else
							new->cursor_explicit_argrow = plpgsql_yyvsp[-3].row->rowno;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
										   plpgsql_yyvsp[-6].varname.name);
					}
    break;

  case 28:
#line 412 "gram.y"
    {
						PLpgSQL_expr *query;

						plpgsql_ns_setlocal(false);
						query = read_sql_stmt("SELECT ");
						plpgsql_ns_setlocal(true);
						
						plpgsql_yyval.expr = query;
					}
    break;

  case 29:
#line 424 "gram.y"
    {
						plpgsql_yyval.row = NULL;
					}
    break;

  case 30:
#line 428 "gram.y"
    {
						/* Copy the temp arrays to malloc'd storage */
						int nfields = plpgsql_yyvsp[-1].row->nfields;
						char **ftmp;
						int *vtmp;

						ftmp = malloc(nfields * sizeof(char *));
						vtmp = malloc(nfields * sizeof(int));
						memcpy(ftmp, plpgsql_yyvsp[-1].row->fieldnames, nfields * sizeof(char *));
						memcpy(vtmp, plpgsql_yyvsp[-1].row->varnos, nfields * sizeof(int));

						pfree((char *)(plpgsql_yyvsp[-1].row->fieldnames));
						pfree((char *)(plpgsql_yyvsp[-1].row->varnos));

						plpgsql_yyvsp[-1].row->fieldnames = ftmp;
						plpgsql_yyvsp[-1].row->varnos = vtmp;

						plpgsql_adddatum((PLpgSQL_datum *)plpgsql_yyvsp[-1].row);

						plpgsql_yyval.row = plpgsql_yyvsp[-1].row;
					}
    break;

  case 31:
#line 452 "gram.y"
    {
						PLpgSQL_row *new;

						new = malloc(sizeof(PLpgSQL_row));
						memset(new, 0, sizeof(PLpgSQL_row));

						new->dtype = PLPGSQL_DTYPE_ROW;
						new->refname = strdup("*internal*");
						new->lineno = plpgsql_yylineno;
						new->rowtypeclass = InvalidOid;
						/*
						 * We make temporary fieldnames/varnos arrays that
						 * are much bigger than necessary.  We will resize
						 * them to just the needed size in the
						 * decl_cursor_args production.
						 */
						new->fieldnames = palloc(1024 * sizeof(char *));
						new->varnos = palloc(1024 * sizeof(int));
						new->nfields = 1;

						new->fieldnames[0] = plpgsql_yyvsp[0].var->refname;
						new->varnos[0] = plpgsql_yyvsp[0].var->varno;

						plpgsql_yyval.row = new;
					}
    break;

  case 32:
#line 478 "gram.y"
    {
						int i = plpgsql_yyvsp[-2].row->nfields++;

						plpgsql_yyvsp[-2].row->fieldnames[i] = plpgsql_yyvsp[0].var->refname;
						plpgsql_yyvsp[-2].row->varnos[i] = plpgsql_yyvsp[0].var->varno;

						plpgsql_yyval.row = plpgsql_yyvsp[-2].row;
					}
    break;

  case 33:
#line 489 "gram.y"
    {
						PLpgSQL_var *new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-1].varname.name;
						new->lineno		= plpgsql_yyvsp[-1].varname.lineno;

						new->datatype	= plpgsql_yyvsp[0].dtype;
						new->isconst	= false;
						new->notnull	= false;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
										   plpgsql_yyvsp[-1].varname.name);
						
						plpgsql_yyval.var = new;
					}
    break;

  case 36:
#line 515 "gram.y"
    {
						char	*name;
						PLpgSQL_nsitem *nsi;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						if (name[0] != '$')
							plpgsql_yyerror("can only alias positional parameters");

						plpgsql_ns_setlocal(false);
						nsi = plpgsql_ns_lookup(name, NULL);
						if (nsi == NULL)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "function has no parameter %s", name);
						}

						plpgsql_ns_setlocal(true);

						pfree(name);

						plpgsql_yyval.nsitem = nsi;
					}
    break;

  case 37:
#line 540 "gram.y"
    {
						plpgsql_yyval.row = plpgsql_yylval.row;
					}
    break;

  case 38:
#line 546 "gram.y"
    {
						char	*name;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						/* name should be malloc'd for use as varname */
						plpgsql_yyval.varname.name = strdup(name);
						plpgsql_yyval.varname.lineno  = plpgsql_yylineno;
						pfree(name);
					}
    break;

  case 39:
#line 558 "gram.y"
    {
						char	*name;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						/* the result must be palloc'd, see plpgsql_ns_rename */
						plpgsql_yyval.str = name;
					}
    break;

  case 40:
#line 568 "gram.y"
    { plpgsql_yyval.ival = 0; }
    break;

  case 41:
#line 570 "gram.y"
    { plpgsql_yyval.ival = 1; }
    break;

  case 42:
#line 574 "gram.y"
    {
						/*
						 * If there's a lookahead token, read_datatype
						 * should consume it.
						 */
						plpgsql_yyval.dtype = read_datatype(plpgsql_yychar);
						plpgsql_yyclearin;
					}
    break;

  case 43:
#line 585 "gram.y"
    { plpgsql_yyval.ival = 0; }
    break;

  case 44:
#line 587 "gram.y"
    { plpgsql_yyval.ival = 1; }
    break;

  case 45:
#line 591 "gram.y"
    { plpgsql_yyval.expr = NULL; }
    break;

  case 46:
#line 593 "gram.y"
    {
						int				tok;
						int				lno;
						PLpgSQL_dstring ds;
						PLpgSQL_expr	*expr;

						lno = plpgsql_yylineno;
						expr = malloc(sizeof(PLpgSQL_expr));
						plpgsql_dstring_init(&ds);
						plpgsql_dstring_append(&ds, "SELECT ");

						expr->dtype   = PLPGSQL_DTYPE_EXPR;
						expr->plan	  = NULL;
						expr->nparams = 0;

						tok = plpgsql_yylex();
						switch (tok)
						{
							case 0:
								plpgsql_yyerror("unexpected end of file");
							case K_NULL:
								if (plpgsql_yylex() != ';')
									plpgsql_yyerror("expected ; after NULL");

								free(expr);
								plpgsql_dstring_free(&ds);

								plpgsql_yyval.expr = NULL;
								break;

							default:
								plpgsql_dstring_append(&ds, plpgsql_yytext);
								while ((tok = plpgsql_yylex()) != ';')
								{
									if (tok == 0)
										plpgsql_yyerror("unterminated default value");

									if (plpgsql_SpaceScanned)
										plpgsql_dstring_append(&ds, " ");
									plpgsql_dstring_append(&ds, plpgsql_yytext);
								}
								expr->query = strdup(plpgsql_dstring_get(&ds));
								plpgsql_dstring_free(&ds);

								plpgsql_yyval.expr = expr;
								break;
						}
					}
    break;

  case 49:
#line 648 "gram.y"
    {
							PLpgSQL_stmts	*new;

							new = malloc(sizeof(PLpgSQL_stmts));
							memset(new, 0, sizeof(PLpgSQL_stmts));
							plpgsql_yyval.stmts = new;
					}
    break;

  case 50:
#line 656 "gram.y"
    { plpgsql_yyval.stmts = plpgsql_yyvsp[0].stmts; }
    break;

  case 51:
#line 660 "gram.y"
    {
								if (plpgsql_yyvsp[-1].stmts->stmts_used == plpgsql_yyvsp[-1].stmts->stmts_alloc)
								{
									plpgsql_yyvsp[-1].stmts->stmts_alloc *= 2;
									plpgsql_yyvsp[-1].stmts->stmts = realloc(plpgsql_yyvsp[-1].stmts->stmts, sizeof(PLpgSQL_stmt *) * plpgsql_yyvsp[-1].stmts->stmts_alloc);
								}
								plpgsql_yyvsp[-1].stmts->stmts[plpgsql_yyvsp[-1].stmts->stmts_used++] = (struct PLpgSQL_stmt *)plpgsql_yyvsp[0].stmt;

								plpgsql_yyval.stmts = plpgsql_yyvsp[-1].stmts;
						}
    break;

  case 52:
#line 671 "gram.y"
    {
								PLpgSQL_stmts	*new;

								new = malloc(sizeof(PLpgSQL_stmts));
								memset(new, 0, sizeof(PLpgSQL_stmts));

								new->stmts_alloc = 64;
								new->stmts_used  = 1;
								new->stmts = malloc(sizeof(PLpgSQL_stmt *) * new->stmts_alloc);
								new->stmts[0] = (struct PLpgSQL_stmt *)plpgsql_yyvsp[0].stmt;

								plpgsql_yyval.stmts = new;

						}
    break;

  case 53:
#line 688 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[-1].stmt; }
    break;

  case 54:
#line 690 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 55:
#line 692 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 56:
#line 694 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 57:
#line 696 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 58:
#line 698 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 59:
#line 700 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 60:
#line 702 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 61:
#line 704 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 62:
#line 706 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 63:
#line 708 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 64:
#line 710 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 65:
#line 712 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 66:
#line 714 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 67:
#line 716 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 68:
#line 718 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 69:
#line 720 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 70:
#line 722 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 71:
#line 724 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 72:
#line 726 "gram.y"
    { plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;

  case 73:
#line 730 "gram.y"
    {
						PLpgSQL_stmt_assign *new;

						new = malloc(sizeof(PLpgSQL_stmt_assign));
						memset(new, 0, sizeof(PLpgSQL_stmt_assign));

						new->cmd_type = PLPGSQL_STMT_ASSIGN;
						new->lineno   = plpgsql_yyvsp[-1].ival;
						new->varno = -1;
						new->expr  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 74:
#line 746 "gram.y"
    {
						PLpgSQL_stmt_assign *new;

						new = malloc(sizeof(PLpgSQL_stmt_assign));
						memset(new, 0, sizeof(PLpgSQL_stmt_assign));

						new->cmd_type = PLPGSQL_STMT_ASSIGN;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->varno = plpgsql_yyvsp[-3].ival;
						new->expr  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 75:
#line 762 "gram.y"
    {
						PLpgSQL_stmt_getdiag	 *new;

						new = malloc(sizeof(PLpgSQL_stmt_getdiag));
						memset(new, 0, sizeof(PLpgSQL_stmt_getdiag));

						new->cmd_type = PLPGSQL_STMT_GETDIAG;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->ndtitems = plpgsql_yyvsp[-1].dtlist.nused;
						new->dtitems  = malloc(sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-1].dtlist.nused);
						memcpy(new->dtitems, plpgsql_yyvsp[-1].dtlist.dtitems, sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-1].dtlist.nused);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 76:
#line 779 "gram.y"
    {
						if (plpgsql_yyvsp[-4].dtlist.nused == plpgsql_yyvsp[-4].dtlist.nalloc)
						{
							plpgsql_yyvsp[-4].dtlist.nalloc *= 2;
							plpgsql_yyvsp[-4].dtlist.dtitems = repalloc(plpgsql_yyvsp[-4].dtlist.dtitems, sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-4].dtlist.nalloc);
						}
						plpgsql_yyvsp[-4].dtlist.dtitems[plpgsql_yyvsp[-4].dtlist.nused].target = plpgsql_yyvsp[-2].ival;
						plpgsql_yyvsp[-4].dtlist.dtitems[plpgsql_yyvsp[-4].dtlist.nused].item   = plpgsql_yyvsp[0].ival;
						plpgsql_yyvsp[-4].dtlist.nused++;

						plpgsql_yyval.dtlist.nalloc = plpgsql_yyvsp[-4].dtlist.nalloc;
						plpgsql_yyval.dtlist.nused  = plpgsql_yyvsp[-4].dtlist.nused;
						plpgsql_yyval.dtlist.dtitems = plpgsql_yyvsp[-4].dtlist.dtitems;
					}
    break;

  case 77:
#line 794 "gram.y"
    {
						plpgsql_yyval.dtlist.nalloc = 1;
						plpgsql_yyval.dtlist.nused  = 1;
						plpgsql_yyval.dtlist.dtitems = palloc(sizeof(PLpgSQL_diag_item) * plpgsql_yyval.dtlist.nalloc);
						plpgsql_yyval.dtlist.dtitems[0].target = plpgsql_yyvsp[-2].ival;
						plpgsql_yyval.dtlist.dtitems[0].item   = plpgsql_yyvsp[0].ival;
					}
    break;

  case 78:
#line 804 "gram.y"
    {
						plpgsql_yyval.ival = PLPGSQL_GETDIAG_ROW_COUNT;
					}
    break;

  case 79:
#line 808 "gram.y"
    {
						plpgsql_yyval.ival = PLPGSQL_GETDIAG_RESULT_OID;
					}
    break;

  case 80:
#line 814 "gram.y"
    {
						check_assignable(plpgsql_yylval.variable);
						plpgsql_yyval.ival = plpgsql_yylval.variable->dno;
					}
    break;

  case 81:
#line 822 "gram.y"
    {
						check_assignable(plpgsql_yylval.variable);
						plpgsql_yyval.ival = plpgsql_yylval.variable->dno;
					}
    break;

  case 82:
#line 829 "gram.y"
    {
						PLpgSQL_stmt_if *new;

						new = malloc(sizeof(PLpgSQL_stmt_if));
						memset(new, 0, sizeof(PLpgSQL_stmt_if));

						new->cmd_type	= PLPGSQL_STMT_IF;
						new->lineno		= plpgsql_yyvsp[-6].ival;
						new->cond		= plpgsql_yyvsp[-5].expr;
						new->true_body	= plpgsql_yyvsp[-4].stmts;
						new->false_body = plpgsql_yyvsp[-3].stmts;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 83:
#line 846 "gram.y"
    {
							PLpgSQL_stmts	*new;

							new = malloc(sizeof(PLpgSQL_stmts));
							memset(new, 0, sizeof(PLpgSQL_stmts));
							plpgsql_yyval.stmts = new;
					}
    break;

  case 84:
#line 854 "gram.y"
    {
					  /*
					   * Translate the structure:	   into:
					   *
					   * IF c1 THEN					   IF c1 THEN		 
					   *	 ...						   ...				 
					   * ELSIF c2 THEN				   ELSE 
					   *								   IF c2 THEN	
					   *	 ...							   ...				 
					   * ELSE							   ELSE				 
					   *	 ...							   ...				 
					   * END IF							   END IF			 
					   *							   END IF
					   * 
					   */

						PLpgSQL_stmts	*new;
						PLpgSQL_stmt_if *new_if;

						/* first create a new if-statement */
						new_if = malloc(sizeof(PLpgSQL_stmt_if));
						memset(new_if, 0, sizeof(PLpgSQL_stmt_if));

						new_if->cmd_type	= PLPGSQL_STMT_IF;
						new_if->lineno		= plpgsql_yyvsp[-3].ival;
						new_if->cond		= plpgsql_yyvsp[-2].expr;
						new_if->true_body	= plpgsql_yyvsp[-1].stmts;
						new_if->false_body	= plpgsql_yyvsp[0].stmts;
						
						/* this is a 'container' for the if-statement */
						new = malloc(sizeof(PLpgSQL_stmts));
						memset(new, 0, sizeof(PLpgSQL_stmts));
						
						new->stmts_alloc = 64;
						new->stmts_used	 = 1;
						new->stmts = malloc(sizeof(PLpgSQL_stmt *) * new->stmts_alloc);
						new->stmts[0] = (struct PLpgSQL_stmt *)new_if;

						plpgsql_yyval.stmts = new;
						
					}
    break;

  case 85:
#line 897 "gram.y"
    {
						plpgsql_yyval.stmts = plpgsql_yyvsp[0].stmts;				
					}
    break;

  case 86:
#line 903 "gram.y"
    {
						PLpgSQL_stmt_loop *new;

						new = malloc(sizeof(PLpgSQL_stmt_loop));
						memset(new, 0, sizeof(PLpgSQL_stmt_loop));

						new->cmd_type = PLPGSQL_STMT_LOOP;
						new->lineno   = plpgsql_yyvsp[-1].ival;
						new->label	  = plpgsql_yyvsp[-3].str;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 87:
#line 921 "gram.y"
    {
						PLpgSQL_stmt_while *new;

						new = malloc(sizeof(PLpgSQL_stmt_while));
						memset(new, 0, sizeof(PLpgSQL_stmt_while));

						new->cmd_type = PLPGSQL_STMT_WHILE;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->label	  = plpgsql_yyvsp[-4].str;
						new->cond	  = plpgsql_yyvsp[-1].expr;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 88:
#line 940 "gram.y"
    {
						PLpgSQL_stmt_fori		*new;

						new = malloc(sizeof(PLpgSQL_stmt_fori));
						memset(new, 0, sizeof(PLpgSQL_stmt_fori));

						new->cmd_type = PLPGSQL_STMT_FORI;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						new->var	  = plpgsql_yyvsp[-4].var;
						new->reverse  = plpgsql_yyvsp[-2].forilow.reverse;
						new->lower	  = plpgsql_yyvsp[-2].forilow.expr;
						new->upper	  = plpgsql_yyvsp[-1].expr;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 89:
#line 962 "gram.y"
    {
						PLpgSQL_var		*new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[0].varname.name;
						new->lineno		= plpgsql_yyvsp[0].varname.lineno;

						new->datatype	= plpgsql_parse_datatype("integer");
						new->isconst	= false;
						new->notnull	= false;
						new->default_val = NULL;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
												plpgsql_yyvsp[0].varname.name);

						plpgsql_add_initdatums(NULL);

						plpgsql_yyval.var = new;
					}
    break;

  case 90:
#line 988 "gram.y"
    {
						char	*name;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						/* name should be malloc'd for use as varname */
						plpgsql_yyval.varname.name = strdup(name);
						plpgsql_yyval.varname.lineno  = plpgsql_yylineno;
						pfree(name);
					}
    break;

  case 91:
#line 998 "gram.y"
    {
						char	*name;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						/* name should be malloc'd for use as varname */
						plpgsql_yyval.varname.name = strdup(name);
						plpgsql_yyval.varname.lineno  = plpgsql_yylineno;
						pfree(name);
					}
    break;

  case 92:
#line 1010 "gram.y"
    {
						int			tok;

						tok = plpgsql_yylex();
						if (tok == K_REVERSE)
						{
							plpgsql_yyval.forilow.reverse = 1;
						}
						else
						{
							plpgsql_yyval.forilow.reverse = 0;
							plpgsql_push_back_token(tok);
						}

						plpgsql_yyval.forilow.expr = plpgsql_read_expression(K_DOTDOT, "..");
					}
    break;

  case 93:
#line 1029 "gram.y"
    {
						PLpgSQL_stmt_fors		*new;

						new = malloc(sizeof(PLpgSQL_stmt_fors));
						memset(new, 0, sizeof(PLpgSQL_stmt_fors));

						new->cmd_type = PLPGSQL_STMT_FORS;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						switch (plpgsql_yyvsp[-4].rec->dtype)
						{
							case PLPGSQL_DTYPE_REC:
								new->rec = plpgsql_yyvsp[-4].rec;
								break;
							case PLPGSQL_DTYPE_ROW:
								new->row = (PLpgSQL_row *)plpgsql_yyvsp[-4].rec;
								break;
							default:
								elog(ERROR, "unknown dtype %d in stmt_fors", plpgsql_yyvsp[-4].rec->dtype);
						}
						new->query = plpgsql_yyvsp[-1].expr;
						new->body  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 94:
#line 1059 "gram.y"
    {
						PLpgSQL_stmt_dynfors	*new;

						new = malloc(sizeof(PLpgSQL_stmt_dynfors));
						memset(new, 0, sizeof(PLpgSQL_stmt_dynfors));

						new->cmd_type = PLPGSQL_STMT_DYNFORS;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						switch (plpgsql_yyvsp[-4].rec->dtype)
						{
							case PLPGSQL_DTYPE_REC:
								new->rec = plpgsql_yyvsp[-4].rec;
								break;
							case PLPGSQL_DTYPE_ROW:
								new->row = (PLpgSQL_row *)plpgsql_yyvsp[-4].rec;
								break;
							default:
								elog(ERROR, "unknown dtype %d in stmt_dynfors", plpgsql_yyvsp[-4].rec->dtype);
						}
						new->query = plpgsql_yyvsp[-1].expr;
						new->body  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 95:
#line 1089 "gram.y"
    { plpgsql_yyval.rec = plpgsql_yylval.rec; }
    break;

  case 96:
#line 1091 "gram.y"
    {
						plpgsql_yyval.rec = (PLpgSQL_rec *)(plpgsql_yylval.row);
					}
    break;

  case 97:
#line 1097 "gram.y"
    {
						plpgsql_yyval.stmt = make_select_stmt();
						plpgsql_yyval.stmt->lineno = plpgsql_yyvsp[0].ival;
					}
    break;

  case 98:
#line 1104 "gram.y"
    {
						PLpgSQL_stmt_exit *new;

						new = malloc(sizeof(PLpgSQL_stmt_exit));
						memset(new, 0, sizeof(PLpgSQL_stmt_exit));

						new->cmd_type = PLPGSQL_STMT_EXIT;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->label	  = plpgsql_yyvsp[-1].str;
						new->cond	  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 99:
#line 1120 "gram.y"
    {
						PLpgSQL_stmt_return *new;

						new = malloc(sizeof(PLpgSQL_stmt_return));
						memset(new, 0, sizeof(PLpgSQL_stmt_return));

						if (plpgsql_curr_compile->fn_retistuple &&
							!plpgsql_curr_compile->fn_retset)
						{
							new->retrecno	= -1;
							switch (plpgsql_yylex())
							{
								case K_NULL:
									new->expr = NULL;
									break;

								case T_ROW:
									new->expr = make_tupret_expr(plpgsql_yylval.row);
									break;

								case T_RECORD:
									new->retrecno = plpgsql_yylval.rec->recno;
									new->expr = NULL;
									break;

								default:
									plpgsql_yyerror("return type mismatch in function returning tuple");
									break;
							}
							if (plpgsql_yylex() != ';')
								plpgsql_yyerror("expected ';'");
						}
						else
							new->expr = plpgsql_read_expression(';', ";");

						new->cmd_type = PLPGSQL_STMT_RETURN;
						new->lineno   = plpgsql_yyvsp[0].ival;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 100:
#line 1163 "gram.y"
    {
						PLpgSQL_stmt_return_next *new;

						new = malloc(sizeof(PLpgSQL_stmt_return_next));
						memset(new, 0, sizeof(PLpgSQL_stmt_return_next));

						new->cmd_type	= PLPGSQL_STMT_RETURN_NEXT;
						new->lineno		= plpgsql_yyvsp[0].ival;

						if (plpgsql_curr_compile->fn_retistuple)
						{
							int tok = plpgsql_yylex();

							if (tok == T_RECORD)
								new->rec = plpgsql_yylval.rec;
							else if (tok == T_ROW)
								new->row = plpgsql_yylval.row;
							else
								plpgsql_yyerror("Incorrect argument to RETURN NEXT");

							if (plpgsql_yylex() != ';')
								plpgsql_yyerror("Expected ';'");
						}
						else
							new->expr = plpgsql_read_expression(';', ";");

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 101:
#line 1194 "gram.y"
    {
						PLpgSQL_stmt_raise		*new;

						new = malloc(sizeof(PLpgSQL_stmt_raise));

						new->cmd_type	= PLPGSQL_STMT_RAISE;
						new->lineno		= plpgsql_yyvsp[-4].ival;
						new->elog_level = plpgsql_yyvsp[-3].ival;
						new->message	= plpgsql_yyvsp[-2].str;
						new->nparams	= plpgsql_yyvsp[-1].intlist.nused;
						new->params		= malloc(sizeof(int) * plpgsql_yyvsp[-1].intlist.nused);
						memcpy(new->params, plpgsql_yyvsp[-1].intlist.nums, sizeof(int) * plpgsql_yyvsp[-1].intlist.nused);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 102:
#line 1210 "gram.y"
    {
						PLpgSQL_stmt_raise		*new;

						new = malloc(sizeof(PLpgSQL_stmt_raise));

						new->cmd_type	= PLPGSQL_STMT_RAISE;
						new->lineno		= plpgsql_yyvsp[-3].ival;
						new->elog_level = plpgsql_yyvsp[-2].ival;
						new->message	= plpgsql_yyvsp[-1].str;
						new->nparams	= 0;
						new->params		= NULL;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 103:
#line 1227 "gram.y"
    {
						plpgsql_yyval.str = strdup(plpgsql_yytext);
					}
    break;

  case 104:
#line 1233 "gram.y"
    {
						plpgsql_yyval.ival = ERROR;
					}
    break;

  case 105:
#line 1237 "gram.y"
    {
						plpgsql_yyval.ival = WARNING;
					}
    break;

  case 106:
#line 1241 "gram.y"
    {
						plpgsql_yyval.ival = NOTICE;
					}
    break;

  case 107:
#line 1245 "gram.y"
    {
						plpgsql_yyval.ival = INFO;
					}
    break;

  case 108:
#line 1249 "gram.y"
    {
						plpgsql_yyval.ival = LOG;
					}
    break;

  case 109:
#line 1253 "gram.y"
    {
						plpgsql_yyval.ival = DEBUG1;
					}
    break;

  case 110:
#line 1259 "gram.y"
    {
						if (plpgsql_yyvsp[-1].intlist.nused == plpgsql_yyvsp[-1].intlist.nalloc)
						{
							plpgsql_yyvsp[-1].intlist.nalloc *= 2;
							plpgsql_yyvsp[-1].intlist.nums = repalloc(plpgsql_yyvsp[-1].intlist.nums, sizeof(int) * plpgsql_yyvsp[-1].intlist.nalloc);
						}
						plpgsql_yyvsp[-1].intlist.nums[plpgsql_yyvsp[-1].intlist.nused++] = plpgsql_yyvsp[0].ival;

						plpgsql_yyval.intlist.nalloc = plpgsql_yyvsp[-1].intlist.nalloc;
						plpgsql_yyval.intlist.nused  = plpgsql_yyvsp[-1].intlist.nused;
						plpgsql_yyval.intlist.nums   = plpgsql_yyvsp[-1].intlist.nums;
					}
    break;

  case 111:
#line 1272 "gram.y"
    {
						plpgsql_yyval.intlist.nalloc = 1;
						plpgsql_yyval.intlist.nused  = 1;
						plpgsql_yyval.intlist.nums   = palloc(sizeof(int) * plpgsql_yyval.intlist.nalloc);
						plpgsql_yyval.intlist.nums[0] = plpgsql_yyvsp[0].ival;
					}
    break;

  case 112:
#line 1281 "gram.y"
    {
						plpgsql_yyval.ival = plpgsql_yylval.variable->dno;
					}
    break;

  case 113:
#line 1287 "gram.y"
    { plpgsql_yyval.stmts = plpgsql_yyvsp[-3].stmts; }
    break;

  case 114:
#line 1291 "gram.y"
    {
						PLpgSQL_stmt_execsql	*new;

						new = malloc(sizeof(PLpgSQL_stmt_execsql));
						new->cmd_type = PLPGSQL_STMT_EXECSQL;
						new->lineno   = plpgsql_yyvsp[0].ival;
						new->sqlstmt  = read_sql_stmt(plpgsql_yyvsp[-1].str);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 115:
#line 1304 "gram.y"
    {
							PLpgSQL_stmt_dynexecute *new;

							new = malloc(sizeof(PLpgSQL_stmt_dynexecute));
							new->cmd_type = PLPGSQL_STMT_DYNEXECUTE;
							new->lineno   = plpgsql_yyvsp[-1].ival;
							new->query	  = plpgsql_yyvsp[0].expr;

							plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
						}
    break;

  case 116:
#line 1317 "gram.y"
    {
						PLpgSQL_stmt_open *new;
						int				  tok;

						new = malloc(sizeof(PLpgSQL_stmt_open));
						memset(new, 0, sizeof(PLpgSQL_stmt_open));

						new->cmd_type = PLPGSQL_STMT_OPEN;
						new->lineno = plpgsql_yyvsp[-1].ival;
						new->curvar = plpgsql_yyvsp[0].var->varno;

						if (plpgsql_yyvsp[0].var->cursor_explicit_expr == NULL)
						{
						    tok = plpgsql_yylex();

							if (tok != K_FOR)
							{
								plpgsql_error_lineno = plpgsql_yyvsp[-1].ival;
								elog(ERROR, "syntax error at \"%s\" - expected FOR to open a reference cursor", plpgsql_yytext);
							}

							tok = plpgsql_yylex();
							switch (tok)
							{
								case K_SELECT:
									new->query = read_sql_stmt("SELECT ");
									break;

								case K_EXECUTE:
									new->dynquery = read_sql_stmt("SELECT ");
									break;

								default:
									plpgsql_error_lineno = plpgsql_yyvsp[-1].ival;
									elog(ERROR, "syntax error at \"%s\"", plpgsql_yytext);
							}

						}
						else
						{
							if (plpgsql_yyvsp[0].var->cursor_explicit_argrow >= 0)
							{
								char   *cp;

								tok = plpgsql_yylex();

								if (tok != '(')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "cursor %s has arguments",
										 plpgsql_yyvsp[0].var->refname);
								}

								/*
								 * Push back the '(', else read_sql_stmt
								 * will complain about unbalanced parens.
								 */
								plpgsql_push_back_token(tok);

								new->argquery = read_sql_stmt("SELECT ");

								/*
								 * Now remove the leading and trailing parens,
								 * because we want "select 1, 2", not
								 * "select (1, 2)".
								 */
								cp = new->argquery->query;

								if (strncmp(cp, "SELECT", 6) != 0)
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "expected 'SELECT (', got '%s' (internal error)",
										 new->argquery->query);
								}
								cp += 6;
								while (*cp == ' ') /* could be more than 1 space here */
									cp++;
								if (*cp != '(')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "expected 'SELECT (', got '%s' (internal error)",
										 new->argquery->query);
								}
								*cp = ' ';

								cp += strlen(cp) - 1;

								if (*cp != ')')
									plpgsql_yyerror("missing )");
								*cp = '\0';
							}
							else
							{
								tok = plpgsql_yylex();

								if (tok == '(')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "cursor %s has no arguments", plpgsql_yyvsp[0].var->refname);
								}
								
								if (tok != ';')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "syntax error at \"%s\"", plpgsql_yytext);
								}
							}
						}

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 117:
#line 1431 "gram.y"
    {
						PLpgSQL_stmt_fetch *new;

						new = (PLpgSQL_stmt_fetch *)make_fetch_stmt();
						new->curvar = plpgsql_yyvsp[-1].ival;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
						plpgsql_yyval.stmt->lineno = plpgsql_yyvsp[-2].ival;
					}
    break;

  case 118:
#line 1443 "gram.y"
    {
						PLpgSQL_stmt_close *new;

						new = malloc(sizeof(PLpgSQL_stmt_close));
						new->cmd_type = PLPGSQL_STMT_CLOSE;
						new->lineno = plpgsql_yyvsp[-2].ival;
						new->curvar = plpgsql_yyvsp[-1].ival;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;

  case 119:
#line 1456 "gram.y"
    {
						if (plpgsql_yylval.variable->dtype != PLPGSQL_DTYPE_VAR)
							plpgsql_yyerror("cursor variable must be a simple variable");

						if (((PLpgSQL_var *) plpgsql_yylval.variable)->datatype->typoid != REFCURSOROID)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s must be of type cursor or refcursor",
								 ((PLpgSQL_var *) plpgsql_yylval.variable)->refname);
						}
						plpgsql_yyval.var = (PLpgSQL_var *) plpgsql_yylval.variable;
					}
    break;

  case 120:
#line 1471 "gram.y"
    {
						if (plpgsql_yylval.variable->dtype != PLPGSQL_DTYPE_VAR)
							plpgsql_yyerror("cursor variable must be a simple variable");

						if (((PLpgSQL_var *) plpgsql_yylval.variable)->datatype->typoid != REFCURSOROID)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s must be of type refcursor",
								 ((PLpgSQL_var *) plpgsql_yylval.variable)->refname);
						}
						plpgsql_yyval.ival = plpgsql_yylval.variable->dno;
					}
    break;

  case 121:
#line 1486 "gram.y"
    { plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;

  case 122:
#line 1488 "gram.y"
    { plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;

  case 123:
#line 1492 "gram.y"
    { plpgsql_yyval.expr = plpgsql_read_expression(';', ";"); }
    break;

  case 124:
#line 1496 "gram.y"
    { plpgsql_yyval.expr = plpgsql_read_expression(K_THEN, "THEN"); }
    break;

  case 125:
#line 1500 "gram.y"
    { plpgsql_yyval.expr = plpgsql_read_expression(K_LOOP, "LOOP"); }
    break;

  case 126:
#line 1504 "gram.y"
    {
						plpgsql_ns_push(NULL);
						plpgsql_yyval.str = NULL;
					}
    break;

  case 127:
#line 1509 "gram.y"
    {
						plpgsql_ns_push(plpgsql_yyvsp[-2].str);
						plpgsql_yyval.str = plpgsql_yyvsp[-2].str;
					}
    break;

  case 128:
#line 1516 "gram.y"
    { plpgsql_yyval.str = NULL; }
    break;

  case 129:
#line 1518 "gram.y"
    { plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;

  case 130:
#line 1522 "gram.y"
    { plpgsql_yyval.expr = NULL; }
    break;

  case 131:
#line 1524 "gram.y"
    { plpgsql_yyval.expr = plpgsql_yyvsp[0].expr; }
    break;

  case 132:
#line 1528 "gram.y"
    {
						char	*name;

						plpgsql_convert_ident(plpgsql_yytext, &name, 1);
						plpgsql_yyval.str = strdup(name);
						pfree(name);
					}
    break;

  case 133:
#line 1538 "gram.y"
    {
						plpgsql_error_lineno = plpgsql_yylineno;
						plpgsql_yyval.ival = plpgsql_yylineno;
					}
    break;


    }

/* Line 999 of yacc.c.  */
#line 2970 "y.tab.c"

  plpgsql_yyvsp -= plpgsql_yylen;
  plpgsql_yyssp -= plpgsql_yylen;


  PLPGSQL_YY_STACK_PRINT (plpgsql_yyss, plpgsql_yyssp);

  *++plpgsql_yyvsp = plpgsql_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  plpgsql_yyn = plpgsql_yyr1[plpgsql_yyn];

  plpgsql_yystate = plpgsql_yypgoto[plpgsql_yyn - PLPGSQL_YYNTOKENS] + *plpgsql_yyssp;
  if (0 <= plpgsql_yystate && plpgsql_yystate <= PLPGSQL_YYLAST && plpgsql_yycheck[plpgsql_yystate] == *plpgsql_yyssp)
    plpgsql_yystate = plpgsql_yytable[plpgsql_yystate];
  else
    plpgsql_yystate = plpgsql_yydefgoto[plpgsql_yyn - PLPGSQL_YYNTOKENS];

  goto plpgsql_yynewstate;


/*------------------------------------.
| plpgsql_yyerrlab -- here on detecting error |
`------------------------------------*/
plpgsql_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!plpgsql_yyerrstatus)
    {
      ++plpgsql_yynerrs;
#if PLPGSQL_YYERROR_VERBOSE
      plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];

      if (PLPGSQL_YYPACT_NINF < plpgsql_yyn && plpgsql_yyn < PLPGSQL_YYLAST)
	{
	  PLPGSQL_YYSIZE_T plpgsql_yysize = 0;
	  int plpgsql_yytype = PLPGSQL_YYTRANSLATE (plpgsql_yychar);
	  char *plpgsql_yymsg;
	  int plpgsql_yyx, plpgsql_yycount;

	  plpgsql_yycount = 0;
	  /* Start PLPGSQL_YYX at -PLPGSQL_YYN if negative to avoid negative indexes in
	     PLPGSQL_YYCHECK.  */
	  for (plpgsql_yyx = plpgsql_yyn < 0 ? -plpgsql_yyn : 0;
	       plpgsql_yyx < (int) (sizeof (plpgsql_yytname) / sizeof (char *)); plpgsql_yyx++)
	    if (plpgsql_yycheck[plpgsql_yyx + plpgsql_yyn] == plpgsql_yyx && plpgsql_yyx != PLPGSQL_YYTERROR)
	      plpgsql_yysize += plpgsql_yystrlen (plpgsql_yytname[plpgsql_yyx]) + 15, plpgsql_yycount++;
	  plpgsql_yysize += plpgsql_yystrlen ("syntax error, unexpected ") + 1;
	  plpgsql_yysize += plpgsql_yystrlen (plpgsql_yytname[plpgsql_yytype]);
	  plpgsql_yymsg = (char *) PLPGSQL_YYSTACK_ALLOC (plpgsql_yysize);
	  if (plpgsql_yymsg != 0)
	    {
	      char *plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yymsg, "syntax error, unexpected ");
	      plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yytname[plpgsql_yytype]);

	      if (plpgsql_yycount < 5)
		{
		  plpgsql_yycount = 0;
		  for (plpgsql_yyx = plpgsql_yyn < 0 ? -plpgsql_yyn : 0;
		       plpgsql_yyx < (int) (sizeof (plpgsql_yytname) / sizeof (char *));
		       plpgsql_yyx++)
		    if (plpgsql_yycheck[plpgsql_yyx + plpgsql_yyn] == plpgsql_yyx && plpgsql_yyx != PLPGSQL_YYTERROR)
		      {
			const char *plpgsql_yyq = ! plpgsql_yycount ? ", expecting " : " or ";
			plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yyq);
			plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yytname[plpgsql_yyx]);
			plpgsql_yycount++;
		      }
		}
	      plpgsql_yyerror (plpgsql_yymsg);
	      PLPGSQL_YYSTACK_FREE (plpgsql_yymsg);
	    }
	  else
	    plpgsql_yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* PLPGSQL_YYERROR_VERBOSE */
	plpgsql_yyerror ("syntax error");
    }



  if (plpgsql_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (plpgsql_yychar == PLPGSQL_YYEOF)
        {
	  /* Pop the error token.  */
          PLPGSQL_YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (plpgsql_yyss < plpgsql_yyssp)
	    {
	      PLPGSQL_YYDSYMPRINTF ("Error: popping", plpgsql_yystos[*plpgsql_yyssp], plpgsql_yyvsp, plpgsql_yylsp);
	      plpgsql_yydestruct (plpgsql_yystos[*plpgsql_yyssp], plpgsql_yyvsp);
	      PLPGSQL_YYPOPSTACK;
	    }
	  PLPGSQL_YYABORT;
        }

      PLPGSQL_YYDSYMPRINTF ("Error: discarding", plpgsql_yytoken, &plpgsql_yylval, &plpgsql_yylloc);
      plpgsql_yydestruct (plpgsql_yytoken, &plpgsql_yylval);
      plpgsql_yychar = PLPGSQL_YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto plpgsql_yyerrlab1;


/*----------------------------------------------------.
| plpgsql_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
plpgsql_yyerrlab1:
  plpgsql_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];
      if (plpgsql_yyn != PLPGSQL_YYPACT_NINF)
	{
	  plpgsql_yyn += PLPGSQL_YYTERROR;
	  if (0 <= plpgsql_yyn && plpgsql_yyn <= PLPGSQL_YYLAST && plpgsql_yycheck[plpgsql_yyn] == PLPGSQL_YYTERROR)
	    {
	      plpgsql_yyn = plpgsql_yytable[plpgsql_yyn];
	      if (0 < plpgsql_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (plpgsql_yyssp == plpgsql_yyss)
	PLPGSQL_YYABORT;

      PLPGSQL_YYDSYMPRINTF ("Error: popping", plpgsql_yystos[*plpgsql_yyssp], plpgsql_yyvsp, plpgsql_yylsp);
      plpgsql_yydestruct (plpgsql_yystos[plpgsql_yystate], plpgsql_yyvsp);
      plpgsql_yyvsp--;
      plpgsql_yystate = *--plpgsql_yyssp;

      PLPGSQL_YY_STACK_PRINT (plpgsql_yyss, plpgsql_yyssp);
    }

  if (plpgsql_yyn == PLPGSQL_YYFINAL)
    PLPGSQL_YYACCEPT;

  PLPGSQL_YYDPRINTF ((stderr, "Shifting error token, "));

  *++plpgsql_yyvsp = plpgsql_yylval;


  plpgsql_yystate = plpgsql_yyn;
  goto plpgsql_yynewstate;


/*-------------------------------------.
| plpgsql_yyacceptlab -- PLPGSQL_YYACCEPT comes here.  |
`-------------------------------------*/
plpgsql_yyacceptlab:
  plpgsql_yyresult = 0;
  goto plpgsql_yyreturn;

/*-----------------------------------.
| plpgsql_yyabortlab -- PLPGSQL_YYABORT comes here.  |
`-----------------------------------*/
plpgsql_yyabortlab:
  plpgsql_yyresult = 1;
  goto plpgsql_yyreturn;

#ifndef plpgsql_yyoverflow
/*----------------------------------------------.
| plpgsql_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
plpgsql_yyoverflowlab:
  plpgsql_yyerror ("parser stack overflow");
  plpgsql_yyresult = 2;
  /* Fall through.  */
#endif

plpgsql_yyreturn:
#ifndef plpgsql_yyoverflow
  if (plpgsql_yyss != plpgsql_yyssa)
    PLPGSQL_YYSTACK_FREE (plpgsql_yyss);
#endif
  return plpgsql_yyresult;
}


#line 1544 "gram.y"



PLpgSQL_expr *
plpgsql_read_expression(int until, const char *expected)
{
	return read_sql_construct(until, expected, true, "SELECT ");
}

static PLpgSQL_expr *
read_sql_stmt(const char *sqlstart)
{
	return read_sql_construct(';', ";", false, sqlstart);
}

static PLpgSQL_expr *
read_sql_construct(int until,
				   const char *expected,
				   bool isexpression,
				   const char *sqlstart)
{
	int					tok;
	int					lno;
	PLpgSQL_dstring		ds;
	int					parenlevel = 0;
	int					nparams = 0;
	int					params[1024];
	char				buf[32];
	PLpgSQL_expr		*expr;

	lno = plpgsql_yylineno;
	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, (char *) sqlstart);

	for (;;)
	{
		tok = plpgsql_yylex();
		if (tok == '(')
			parenlevel++;
		else if (tok == ')')
		{
			parenlevel--;
			if (parenlevel < 0)
				elog(ERROR, "mismatched parentheses");
		}
		else if (parenlevel == 0 && tok == until)
			break;
		/*
		 * End of function definition is an error, and we don't expect to
		 * hit a semicolon either (unless it's the until symbol, in which
		 * case we should have fallen out above).
		 */
		if (tok == 0 || tok == ';')
		{
			plpgsql_error_lineno = lno;
			if (parenlevel != 0)
				elog(ERROR, "mismatched parentheses");
			if (isexpression)
				elog(ERROR, "missing %s at end of SQL expression",
					 expected);
			else
				elog(ERROR, "missing %s at end of SQL statement",
					 expected);
			break;
		}
		if (plpgsql_SpaceScanned)
			plpgsql_dstring_append(&ds, " ");
		switch (tok)
		{
			case T_VARIABLE:
				params[nparams] = plpgsql_yylval.variable->dno;
				snprintf(buf, sizeof(buf), " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			default:
				plpgsql_dstring_append(&ds, plpgsql_yytext);
				break;
		}
	}

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;
	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->nparams		= nparams;
	while(nparams-- > 0)
		expr->params[nparams] = params[nparams];
	plpgsql_dstring_free(&ds);

	return expr;
}

static PLpgSQL_type *
read_datatype(int tok)
{
	int					lno;
	PLpgSQL_dstring		ds;
	PLpgSQL_type		*result;
	bool				needspace = false;
	int					parenlevel = 0;

	lno = plpgsql_yylineno;

	/* Often there will be a lookahead token, but if not, get one */
	if (tok == PLPGSQL_YYEMPTY)
		tok = plpgsql_yylex();

	if (tok == T_DTYPE)
	{
		/* lexer found word%TYPE and did its thing already */
		return plpgsql_yylval.dtype;
	}

	plpgsql_dstring_init(&ds);

	while (tok != ';')
	{
		if (tok == 0)
		{
			plpgsql_error_lineno = lno;
			if (parenlevel != 0)
				elog(ERROR, "mismatched parentheses");
			elog(ERROR, "incomplete datatype declaration");
		}
		/* Possible followers for datatype in a declaration */
		if (tok == K_NOT || tok == K_ASSIGN || tok == K_DEFAULT)
			break;
		/* Possible followers for datatype in a cursor_arg list */
		if ((tok == ',' || tok == ')') && parenlevel == 0)
			break;
		if (tok == '(')
			parenlevel++;
		else if (tok == ')')
			parenlevel--;
		if (needspace)
			plpgsql_dstring_append(&ds, " ");
		needspace = true;
		plpgsql_dstring_append(&ds, plpgsql_yytext);

		tok = plpgsql_yylex();
	}

	plpgsql_push_back_token(tok);

	plpgsql_error_lineno = lno;	/* in case of error in parse_datatype */

	result = plpgsql_parse_datatype(plpgsql_dstring_get(&ds));

	plpgsql_dstring_free(&ds);

	return result;
}


static PLpgSQL_stmt *
make_select_stmt(void)
{
	PLpgSQL_dstring		ds;
	int					nparams = 0;
	int					params[1024];
	char				buf[32];
	PLpgSQL_expr		*expr;
	PLpgSQL_row			*row = NULL;
	PLpgSQL_rec			*rec = NULL;
	int					tok = 0;
	int					have_nexttok = 0;
	int					have_into = 0;

	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, "SELECT ");

	while(1)
	{
		if (!have_nexttok)
			tok = plpgsql_yylex();
		have_nexttok = 0;
		if (tok == ';')
			break;
		if (tok == 0)
		{
			plpgsql_error_lineno = plpgsql_yylineno;
			elog(ERROR, "unexpected end of file");
		}
		if (tok == K_INTO)
		{
			if (have_into)
			{
				plpgsql_error_lineno = plpgsql_yylineno;
				elog(ERROR, "INTO specified more than once");
			}
			tok = plpgsql_yylex();
			switch (tok)
			{
				case T_ROW:
					row = plpgsql_yylval.row;
					have_into = 1;
					break;

				case T_RECORD:
					rec = plpgsql_yylval.rec;
					have_into = 1;
					break;

				case T_VARIABLE:
				{
					int				nfields = 1;
					char			*fieldnames[1024];
					int				varnos[1024];

					check_assignable(plpgsql_yylval.variable);
					fieldnames[0] = strdup(plpgsql_yytext);
					varnos[0]	  = plpgsql_yylval.variable->dno;

					while ((tok = plpgsql_yylex()) == ',')
					{
						tok = plpgsql_yylex();
						switch(tok)
						{
							case T_VARIABLE:
								check_assignable(plpgsql_yylval.variable);
								fieldnames[nfields] = strdup(plpgsql_yytext);
								varnos[nfields++]	= plpgsql_yylval.variable->dno;
								break;

							default:
								plpgsql_error_lineno = plpgsql_yylineno;
								elog(ERROR, "plpgsql: %s is not a variable",
									 plpgsql_yytext);
						}
					}
					have_nexttok = 1;

					row = malloc(sizeof(PLpgSQL_row));
					row->dtype = PLPGSQL_DTYPE_ROW;
					row->refname = strdup("*internal*");
					row->lineno = plpgsql_yylineno;
					row->rowtypeclass = InvalidOid;
					row->nfields = nfields;
					row->fieldnames = malloc(sizeof(char *) * nfields);
					row->varnos = malloc(sizeof(int) * nfields);
					while (--nfields >= 0)
					{
						row->fieldnames[nfields] = fieldnames[nfields];
						row->varnos[nfields] = varnos[nfields];
					}

					plpgsql_adddatum((PLpgSQL_datum *)row);

					have_into = 1;
				}
				break;

				default:
					/* Treat the INTO as non-special */
					plpgsql_dstring_append(&ds, " INTO ");
					have_nexttok = 1;
					break;
			}
			continue;
		}

		if (plpgsql_SpaceScanned)
			plpgsql_dstring_append(&ds, " ");
		switch (tok)
		{
			case T_VARIABLE:
				params[nparams] = plpgsql_yylval.variable->dno;
				snprintf(buf, sizeof(buf), " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			default:
				plpgsql_dstring_append(&ds, plpgsql_yytext);
				break;
		}
	}

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;
	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->nparams		= nparams;
	while(nparams-- > 0)
		expr->params[nparams] = params[nparams];
	plpgsql_dstring_free(&ds);

	if (have_into)
	{
		PLpgSQL_stmt_select *select;

		select = malloc(sizeof(PLpgSQL_stmt_select));
		memset(select, 0, sizeof(PLpgSQL_stmt_select));
		select->cmd_type = PLPGSQL_STMT_SELECT;
		select->rec		 = rec;
		select->row		 = row;
		select->query	 = expr;

		return (PLpgSQL_stmt *)select;
	}
	else
	{
		PLpgSQL_stmt_execsql *execsql;

		execsql = malloc(sizeof(PLpgSQL_stmt_execsql));
		execsql->cmd_type = PLPGSQL_STMT_EXECSQL;
		execsql->sqlstmt  = expr;

		return (PLpgSQL_stmt *)execsql;
	}
}


static PLpgSQL_stmt *
make_fetch_stmt(void)
{
	int					tok;
	PLpgSQL_row		   *row = NULL;
	PLpgSQL_rec		   *rec = NULL;
	PLpgSQL_stmt_fetch *fetch;
	int					have_nexttok = 0;

	/* We have already parsed everything through the INTO keyword */

	tok = plpgsql_yylex();
	switch (tok)
	{
		case T_ROW:
			row = plpgsql_yylval.row;
			break;

		case T_RECORD:
			rec = plpgsql_yylval.rec;
			break;

		case T_VARIABLE:
			{
				int				nfields = 1;
				char			*fieldnames[1024];
				int				varnos[1024];

				check_assignable(plpgsql_yylval.variable);
				fieldnames[0] = strdup(plpgsql_yytext);
				varnos[0]	  = plpgsql_yylval.variable->dno;

				while ((tok = plpgsql_yylex()) == ',')
				{
					tok = plpgsql_yylex();
					switch(tok)
					{
						case T_VARIABLE:
							check_assignable(plpgsql_yylval.variable);
							fieldnames[nfields] = strdup(plpgsql_yytext);
							varnos[nfields++]	= plpgsql_yylval.variable->dno;
							break;

						default:
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "plpgsql: %s is not a variable",
								 plpgsql_yytext);
					}
				}
				have_nexttok = 1;

				row = malloc(sizeof(PLpgSQL_row));
				row->dtype = PLPGSQL_DTYPE_ROW;
				row->refname = strdup("*internal*");
				row->lineno = plpgsql_yylineno;
				row->rowtypeclass = InvalidOid;
				row->nfields = nfields;
				row->fieldnames = malloc(sizeof(char *) * nfields);
				row->varnos = malloc(sizeof(int) * nfields);
				while (--nfields >= 0)
				{
					row->fieldnames[nfields] = fieldnames[nfields];
					row->varnos[nfields] = varnos[nfields];
				}

				plpgsql_adddatum((PLpgSQL_datum *)row);
			}
			break;

		default:
			plpgsql_yyerror("syntax error");
	}

	if (!have_nexttok)
		tok = plpgsql_yylex();

	if (tok != ';')
		plpgsql_yyerror("syntax error");

	fetch = malloc(sizeof(PLpgSQL_stmt_select));
	memset(fetch, 0, sizeof(PLpgSQL_stmt_fetch));
	fetch->cmd_type = PLPGSQL_STMT_FETCH;
	fetch->rec		 = rec;
	fetch->row		 = row;

	return (PLpgSQL_stmt *)fetch;
}


static PLpgSQL_expr *
make_tupret_expr(PLpgSQL_row *row)
{
	PLpgSQL_dstring		ds;
	PLpgSQL_expr		*expr;
	int					i;
	char				buf[16];

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * (row->nfields - 1));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;

	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, "SELECT ");

	for (i = 0; i < row->nfields; i++)
	{
		sprintf(buf, "%s$%d", (i > 0) ? "," : "", i + 1);
		plpgsql_dstring_append(&ds, buf);
		expr->params[i] = row->varnos[i];
	}

	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->plan_argtypes = NULL;
	expr->nparams		= row->nfields;

	plpgsql_dstring_free(&ds);
	return expr;
}

static void
check_assignable(PLpgSQL_datum *datum)
{
	switch (datum->dtype)
	{
		case PLPGSQL_DTYPE_VAR:
			if (((PLpgSQL_var *) datum)->isconst)
			{
				plpgsql_error_lineno = plpgsql_yylineno;
				elog(ERROR, "%s is declared CONSTANT",
					 ((PLpgSQL_var *) datum)->refname);
			}
			break;
		case PLPGSQL_DTYPE_RECFIELD:
			/* always assignable? */
			break;
		case PLPGSQL_DTYPE_TRIGARG:
			plpgsql_yyerror("cannot assign to tg_argv");
			break;
		default:
			plpgsql_yyerror("check_assignable: unexpected datum type");
			break;
	}
}

#include "pl_scan.c"

