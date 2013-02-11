
/* 
Author: Brent Arias
*/

#ifndef   PRINTABLE_ENUM
#define PRINTABLE_ENUM

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/control/iif.hpp> 
#include <boost/preprocessor/tuple/eat.hpp>


/******************************************************************************
BOOST_PP_EXPR_IIF requires precompiler definition arguments (viz. PRINTABLE_ENUM_ENUMS, 
PRINTABLE_ENUM_STRINGS, PRINTABLE_ENUM_DEBUG) to be either 0 or 1.   This will not build otherwise!!!!
*******************************************************************************/

/*Usually the ENUM macro should actually expand into an enum, "define ENUM 1" sets that default*/
#ifndef PRINTABLE_ENUM_ENUMS       /*By default, turn enums on*/
#define PRINTABLE_ENUM_ENUMS 1
#endif
#ifndef PRINTABLE_ENUM_STRINGS     /*By default, turn printable strings off*/
#define PRINTABLE_ENUM_STRINGS 0
#endif
#ifndef PRINTABLE_ENUM_DEBUG
#define PRINTABLE_ENUM_DEBUG 0     /*By default, turn debuggin "compliment" condition off */
#endif

/*Used internally by the ENUM macro*/
#ifdef ENUM_STR
#undef ENUM_STR
#endif
#define ENUM_STR(r, data, elem) BOOST_PP_STRINGIZE(elem)

/*Make sure there is no space after the '\' line wrap - or you'll spend
  hours pursuing "too few arguments" or "too many arguments" compiler errors.*/

#define ENUM(name, start, entries)                       \
BOOST_PP_IIF(PRINTABLE_ENUM_ENUMS,                       \
   BOOST_PP_SEQ_ENUM, BOOST_PP_TUPLE_EAT(1))             \
   (                                                     \
      (typedef enum{ BOOST_PP_SEQ_HEAD(entries) = start) \
      BOOST_PP_SEQ_TAIL(entries)                         \
      (LAST_##name } name;)                              \
   )                                                     \
BOOST_PP_IIF(BOOST_PP_BITOR(PRINTABLE_ENUM_DEBUG,PRINTABLE_ENUM_STRINGS), \
   BOOST_PP_SEQ_ENUM, BOOST_PP_TUPLE_EAT(1))             \
   (                                                     \
      (static const char* name##_Str[]={ BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(entries)) ) \
      BOOST_PP_SEQ_TRANSFORM(ENUM_STR, _, BOOST_PP_SEQ_TAIL(entries) ) \
      (};)                                               \
   )


#if 0
***************************************************
*********** EXAMPLE "ENUM" MACRO USAGE ************
*This example starts the enum count at zero. The  *
*enumerations themselves are delimited with parens*
*instead of commas, as such:                      *
***************************************************

#define PRINTABLE_ENUM_STRINGS 1
#include "PrintableEnum.h"
ENUM(myEnumDefinition, 0,
  (MYENUM_YELLOW)           /*enums delimited with parens instead of commas*/
  (MYENUM_RED) 
  (GREEN)
  (BLUE)
  (MAGENTA)
  (ETC)
)

/*The above ENUM macro expansion:

  1) creates the "myEnumDefinition" typedef
  2) starts the enumeration at zero
  3) creates the actual enum values (e.g. GREEN, BLUE, etc)
  4) creates the string array "myEnumDefinition_Str[]'
  5) fills the string array with values such as "GREEN", "BLUE", etc
  6) creates a "tail" enum value called "LAST_myEnumDefinition"

To illustrate example (6), consider the last entry in this example's corresponding enum expansion:

typedef enum{ MYENUM_YELLOW = 0, MYENUM_RED, GREEN, BLUE, MAGENTA, ETC, LAST_myEnumDefinition } myEnumDefinition; 

*/
#endif

#endif