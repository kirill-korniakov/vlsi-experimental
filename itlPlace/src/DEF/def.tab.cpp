/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse defyyparse
#define yylex   defyylex
#define yyerror defyyerror
#define yylval  defyylval
#define yychar  defyychar
#define yydebug defyydebug
#define yynerrs defyynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     QSTRING = 258,
     T_STRING = 259,
     SITE_PATTERN = 260,
     NUMBER = 261,
     K_HISTORY = 262,
     K_NAMESCASESENSITIVE = 263,
     K_DESIGN = 264,
     K_VIAS = 265,
     K_TECH = 266,
     K_UNITS = 267,
     K_ARRAY = 268,
     K_FLOORPLAN = 269,
     K_SITE = 270,
     K_CANPLACE = 271,
     K_CANNOTOCCUPY = 272,
     K_DIEAREA = 273,
     K_PINS = 274,
     K_DEFAULTCAP = 275,
     K_MINPINS = 276,
     K_WIRECAP = 277,
     K_TRACKS = 278,
     K_GCELLGRID = 279,
     K_DO = 280,
     K_BY = 281,
     K_STEP = 282,
     K_LAYER = 283,
     K_ROW = 284,
     K_RECT = 285,
     K_COMPS = 286,
     K_COMP_GEN = 287,
     K_SOURCE = 288,
     K_WEIGHT = 289,
     K_EEQMASTER = 290,
     K_FIXED = 291,
     K_COVER = 292,
     K_UNPLACED = 293,
     K_PLACED = 294,
     K_FOREIGN = 295,
     K_REGION = 296,
     K_REGIONS = 297,
     K_NETS = 298,
     K_START_NET = 299,
     K_MUSTJOIN = 300,
     K_ORIGINAL = 301,
     K_USE = 302,
     K_STYLE = 303,
     K_PATTERN = 304,
     K_PATTERNNAME = 305,
     K_ESTCAP = 306,
     K_ROUTED = 307,
     K_NEW = 308,
     K_SNETS = 309,
     K_SHAPE = 310,
     K_WIDTH = 311,
     K_VOLTAGE = 312,
     K_SPACING = 313,
     K_NONDEFAULTRULE = 314,
     K_NONDEFAULTRULES = 315,
     K_N = 316,
     K_S = 317,
     K_E = 318,
     K_W = 319,
     K_FN = 320,
     K_FE = 321,
     K_FS = 322,
     K_FW = 323,
     K_GROUPS = 324,
     K_GROUP = 325,
     K_SOFT = 326,
     K_MAXX = 327,
     K_MAXY = 328,
     K_MAXHALFPERIMETER = 329,
     K_CONSTRAINTS = 330,
     K_NET = 331,
     K_PATH = 332,
     K_SUM = 333,
     K_DIFF = 334,
     K_SCANCHAINS = 335,
     K_START = 336,
     K_FLOATING = 337,
     K_ORDERED = 338,
     K_STOP = 339,
     K_IN = 340,
     K_OUT = 341,
     K_RISEMIN = 342,
     K_RISEMAX = 343,
     K_FALLMIN = 344,
     K_FALLMAX = 345,
     K_WIREDLOGIC = 346,
     K_MAXDIST = 347,
     K_ASSERTIONS = 348,
     K_DISTANCE = 349,
     K_MICRONS = 350,
     K_END = 351,
     K_IOTIMINGS = 352,
     K_RISE = 353,
     K_FALL = 354,
     K_VARIABLE = 355,
     K_SLEWRATE = 356,
     K_CAPACITANCE = 357,
     K_DRIVECELL = 358,
     K_FROMPIN = 359,
     K_TOPIN = 360,
     K_PARALLEL = 361,
     K_TIMINGDISABLES = 362,
     K_THRUPIN = 363,
     K_MACRO = 364,
     K_PARTITIONS = 365,
     K_TURNOFF = 366,
     K_FROMCLOCKPIN = 367,
     K_FROMCOMPPIN = 368,
     K_FROMIOPIN = 369,
     K_TOCLOCKPIN = 370,
     K_TOCOMPPIN = 371,
     K_TOIOPIN = 372,
     K_SETUPRISE = 373,
     K_SETUPFALL = 374,
     K_HOLDRISE = 375,
     K_HOLDFALL = 376,
     K_VPIN = 377,
     K_SUBNET = 378,
     K_XTALK = 379,
     K_PIN = 380,
     K_SYNTHESIZED = 381,
     K_DEFINE = 382,
     K_DEFINES = 383,
     K_DEFINEB = 384,
     K_IF = 385,
     K_THEN = 386,
     K_ELSE = 387,
     K_FALSE = 388,
     K_TRUE = 389,
     K_EQ = 390,
     K_NE = 391,
     K_LE = 392,
     K_LT = 393,
     K_GE = 394,
     K_GT = 395,
     K_OR = 396,
     K_AND = 397,
     K_NOT = 398,
     K_SPECIAL = 399,
     K_DIRECTION = 400,
     K_RANGE = 401,
     K_FPC = 402,
     K_HORIZONTAL = 403,
     K_VERTICAL = 404,
     K_ALIGN = 405,
     K_MIN = 406,
     K_MAX = 407,
     K_EQUAL = 408,
     K_BOTTOMLEFT = 409,
     K_TOPRIGHT = 410,
     K_ROWS = 411,
     K_TAPER = 412,
     K_TAPERRULE = 413,
     K_VERSION = 414,
     K_DIVIDERCHAR = 415,
     K_BUSBITCHARS = 416,
     K_PROPERTYDEFINITIONS = 417,
     K_STRING = 418,
     K_REAL = 419,
     K_INTEGER = 420,
     K_PROPERTY = 421,
     K_BEGINEXT = 422,
     K_ENDEXT = 423,
     K_NAMEMAPSTRING = 424,
     K_ON = 425,
     K_OFF = 426,
     K_X = 427,
     K_Y = 428,
     K_COMPONENT = 429,
     K_PINPROPERTIES = 430,
     K_TEST = 431,
     K_COMMONSCANPINS = 432,
     K_SNET = 433,
     K_COMPONENTPIN = 434,
     K_REENTRANTPATHS = 435,
     K_SHIELD = 436,
     K_SHIELDNET = 437,
     K_NOSHIELD = 438,
     K_ANTENNAPINPARTIALMETALAREA = 439,
     K_ANTENNAPINPARTIALMETALSIDEAREA = 440,
     K_ANTENNAPINGATEAREA = 441,
     K_ANTENNAPINDIFFAREA = 442,
     K_ANTENNAPINMAXAREACAR = 443,
     K_ANTENNAPINMAXSIDEAREACAR = 444,
     K_ANTENNAPINPARTIALCUTAREA = 445,
     K_ANTENNAPINMAXCUTCAR = 446,
     K_SIGNAL = 447,
     K_POWER = 448,
     K_GROUND = 449,
     K_CLOCK = 450,
     K_TIEOFF = 451,
     K_ANALOG = 452,
     K_SCAN = 453,
     K_RESET = 454,
     K_RING = 455,
     K_STRIPE = 456,
     K_FOLLOWPIN = 457,
     K_IOWIRE = 458,
     K_COREWIRE = 459,
     K_BLOCKWIRE = 460,
     K_FILLWIRE = 461,
     K_BLOCKAGEWIRE = 462,
     K_PADRING = 463,
     K_BLOCKRING = 464,
     K_BLOCKAGES = 465,
     K_PLACEMENT = 466,
     K_SLOTS = 467,
     K_FILLS = 468,
     K_PUSHDOWN = 469,
     K_NETLIST = 470,
     K_DIST = 471,
     K_USER = 472,
     K_TIMING = 473,
     K_BALANCED = 474,
     K_STEINER = 475,
     K_TRUNK = 476,
     K_FIXEDBUMP = 477,
     K_FENCE = 478,
     K_FREQUENCY = 479,
     K_GUIDE = 480,
     K_MAXBITS = 481,
     K_PARTITION = 482,
     K_TYPE = 483,
     K_ANTENNAMODEL = 484,
     K_DRCFILL = 485,
     K_OXIDE1 = 486,
     K_OXIDE2 = 487,
     K_OXIDE3 = 488,
     K_OXIDE4 = 489,
     K_CUTSIZE = 490,
     K_CUTSPACING = 491,
     K_DESIGNRULEWIDTH = 492,
     K_DIAGWIDTH = 493,
     K_ENCLOSURE = 494,
     K_HALO = 495,
     K_GROUNDSENSITIVITY = 496,
     K_HARDSPACING = 497,
     K_LAYERS = 498,
     K_MINCUTS = 499,
     K_NETEXPR = 500,
     K_OFFSET = 501,
     K_ORIGIN = 502,
     K_ROWCOL = 503,
     K_STYLES = 504,
     K_POLYGON = 505,
     K_PORT = 506,
     K_SUPPLYSENSITIVITY = 507,
     K_VIA = 508,
     K_VIARULE = 509,
     K_WIREEXT = 510,
     K_EXCEPTPGNET = 511,
     K_FILLWIREOPC = 512,
     K_OPC = 513,
     K_PARTIAL = 514,
     K_ROUTEHALO = 515
   };
#endif
/* Tokens.  */
#define QSTRING 258
#define T_STRING 259
#define SITE_PATTERN 260
#define NUMBER 261
#define K_HISTORY 262
#define K_NAMESCASESENSITIVE 263
#define K_DESIGN 264
#define K_VIAS 265
#define K_TECH 266
#define K_UNITS 267
#define K_ARRAY 268
#define K_FLOORPLAN 269
#define K_SITE 270
#define K_CANPLACE 271
#define K_CANNOTOCCUPY 272
#define K_DIEAREA 273
#define K_PINS 274
#define K_DEFAULTCAP 275
#define K_MINPINS 276
#define K_WIRECAP 277
#define K_TRACKS 278
#define K_GCELLGRID 279
#define K_DO 280
#define K_BY 281
#define K_STEP 282
#define K_LAYER 283
#define K_ROW 284
#define K_RECT 285
#define K_COMPS 286
#define K_COMP_GEN 287
#define K_SOURCE 288
#define K_WEIGHT 289
#define K_EEQMASTER 290
#define K_FIXED 291
#define K_COVER 292
#define K_UNPLACED 293
#define K_PLACED 294
#define K_FOREIGN 295
#define K_REGION 296
#define K_REGIONS 297
#define K_NETS 298
#define K_START_NET 299
#define K_MUSTJOIN 300
#define K_ORIGINAL 301
#define K_USE 302
#define K_STYLE 303
#define K_PATTERN 304
#define K_PATTERNNAME 305
#define K_ESTCAP 306
#define K_ROUTED 307
#define K_NEW 308
#define K_SNETS 309
#define K_SHAPE 310
#define K_WIDTH 311
#define K_VOLTAGE 312
#define K_SPACING 313
#define K_NONDEFAULTRULE 314
#define K_NONDEFAULTRULES 315
#define K_N 316
#define K_S 317
#define K_E 318
#define K_W 319
#define K_FN 320
#define K_FE 321
#define K_FS 322
#define K_FW 323
#define K_GROUPS 324
#define K_GROUP 325
#define K_SOFT 326
#define K_MAXX 327
#define K_MAXY 328
#define K_MAXHALFPERIMETER 329
#define K_CONSTRAINTS 330
#define K_NET 331
#define K_PATH 332
#define K_SUM 333
#define K_DIFF 334
#define K_SCANCHAINS 335
#define K_START 336
#define K_FLOATING 337
#define K_ORDERED 338
#define K_STOP 339
#define K_IN 340
#define K_OUT 341
#define K_RISEMIN 342
#define K_RISEMAX 343
#define K_FALLMIN 344
#define K_FALLMAX 345
#define K_WIREDLOGIC 346
#define K_MAXDIST 347
#define K_ASSERTIONS 348
#define K_DISTANCE 349
#define K_MICRONS 350
#define K_END 351
#define K_IOTIMINGS 352
#define K_RISE 353
#define K_FALL 354
#define K_VARIABLE 355
#define K_SLEWRATE 356
#define K_CAPACITANCE 357
#define K_DRIVECELL 358
#define K_FROMPIN 359
#define K_TOPIN 360
#define K_PARALLEL 361
#define K_TIMINGDISABLES 362
#define K_THRUPIN 363
#define K_MACRO 364
#define K_PARTITIONS 365
#define K_TURNOFF 366
#define K_FROMCLOCKPIN 367
#define K_FROMCOMPPIN 368
#define K_FROMIOPIN 369
#define K_TOCLOCKPIN 370
#define K_TOCOMPPIN 371
#define K_TOIOPIN 372
#define K_SETUPRISE 373
#define K_SETUPFALL 374
#define K_HOLDRISE 375
#define K_HOLDFALL 376
#define K_VPIN 377
#define K_SUBNET 378
#define K_XTALK 379
#define K_PIN 380
#define K_SYNTHESIZED 381
#define K_DEFINE 382
#define K_DEFINES 383
#define K_DEFINEB 384
#define K_IF 385
#define K_THEN 386
#define K_ELSE 387
#define K_FALSE 388
#define K_TRUE 389
#define K_EQ 390
#define K_NE 391
#define K_LE 392
#define K_LT 393
#define K_GE 394
#define K_GT 395
#define K_OR 396
#define K_AND 397
#define K_NOT 398
#define K_SPECIAL 399
#define K_DIRECTION 400
#define K_RANGE 401
#define K_FPC 402
#define K_HORIZONTAL 403
#define K_VERTICAL 404
#define K_ALIGN 405
#define K_MIN 406
#define K_MAX 407
#define K_EQUAL 408
#define K_BOTTOMLEFT 409
#define K_TOPRIGHT 410
#define K_ROWS 411
#define K_TAPER 412
#define K_TAPERRULE 413
#define K_VERSION 414
#define K_DIVIDERCHAR 415
#define K_BUSBITCHARS 416
#define K_PROPERTYDEFINITIONS 417
#define K_STRING 418
#define K_REAL 419
#define K_INTEGER 420
#define K_PROPERTY 421
#define K_BEGINEXT 422
#define K_ENDEXT 423
#define K_NAMEMAPSTRING 424
#define K_ON 425
#define K_OFF 426
#define K_X 427
#define K_Y 428
#define K_COMPONENT 429
#define K_PINPROPERTIES 430
#define K_TEST 431
#define K_COMMONSCANPINS 432
#define K_SNET 433
#define K_COMPONENTPIN 434
#define K_REENTRANTPATHS 435
#define K_SHIELD 436
#define K_SHIELDNET 437
#define K_NOSHIELD 438
#define K_ANTENNAPINPARTIALMETALAREA 439
#define K_ANTENNAPINPARTIALMETALSIDEAREA 440
#define K_ANTENNAPINGATEAREA 441
#define K_ANTENNAPINDIFFAREA 442
#define K_ANTENNAPINMAXAREACAR 443
#define K_ANTENNAPINMAXSIDEAREACAR 444
#define K_ANTENNAPINPARTIALCUTAREA 445
#define K_ANTENNAPINMAXCUTCAR 446
#define K_SIGNAL 447
#define K_POWER 448
#define K_GROUND 449
#define K_CLOCK 450
#define K_TIEOFF 451
#define K_ANALOG 452
#define K_SCAN 453
#define K_RESET 454
#define K_RING 455
#define K_STRIPE 456
#define K_FOLLOWPIN 457
#define K_IOWIRE 458
#define K_COREWIRE 459
#define K_BLOCKWIRE 460
#define K_FILLWIRE 461
#define K_BLOCKAGEWIRE 462
#define K_PADRING 463
#define K_BLOCKRING 464
#define K_BLOCKAGES 465
#define K_PLACEMENT 466
#define K_SLOTS 467
#define K_FILLS 468
#define K_PUSHDOWN 469
#define K_NETLIST 470
#define K_DIST 471
#define K_USER 472
#define K_TIMING 473
#define K_BALANCED 474
#define K_STEINER 475
#define K_TRUNK 476
#define K_FIXEDBUMP 477
#define K_FENCE 478
#define K_FREQUENCY 479
#define K_GUIDE 480
#define K_MAXBITS 481
#define K_PARTITION 482
#define K_TYPE 483
#define K_ANTENNAMODEL 484
#define K_DRCFILL 485
#define K_OXIDE1 486
#define K_OXIDE2 487
#define K_OXIDE3 488
#define K_OXIDE4 489
#define K_CUTSIZE 490
#define K_CUTSPACING 491
#define K_DESIGNRULEWIDTH 492
#define K_DIAGWIDTH 493
#define K_ENCLOSURE 494
#define K_HALO 495
#define K_GROUNDSENSITIVITY 496
#define K_HARDSPACING 497
#define K_LAYERS 498
#define K_MINCUTS 499
#define K_NETEXPR 500
#define K_OFFSET 501
#define K_ORIGIN 502
#define K_ROWCOL 503
#define K_STYLES 504
#define K_POLYGON 505
#define K_PORT 506
#define K_SUPPLYSENSITIVITY 507
#define K_VIA 508
#define K_VIARULE 509
#define K_WIREEXT 510
#define K_EXCEPTPGNET 511
#define K_FILLWIREOPC 512
#define K_OPC 513
#define K_PARTIAL 514
#define K_ROUTEHALO 515




/* Copy the first part of user declarations.  */
#line 45 "def.y"

#include <stdlib.h>
#include <string.h>
//#include <malloc.h> //KNOTE: commented by Kirill
#include "defrReader.hpp"
#include "DEF/lex.h"
#include "defiUser.hpp"
#include "defrCallBacks.hpp"

#define YYDEBUG 1         /* this is temp fix for pcr 755132 */
// TX_DIR:TRANSLATION ON

extern int dumb_mode;     // Read next N tokens in dumb mode
extern int no_num;        // No numbers for the next N tokens
extern int real_num;      // Next token will be a real number
extern int by_is_keyword;
extern int bit_is_keyword;
extern int do_is_keyword;
extern int new_is_keyword;
extern int nondef_is_keyword;
extern int step_is_keyword;
extern int mustjoin_is_keyword;
extern int routed_is_keyword;
extern int cover_is_keyword;
extern int fixed_is_keyword;
extern int orient_is_keyword;
extern char* History_text;
extern int parsing_property;
extern int doneDesign;

extern int errors;        // from lex.cpph
extern int nlines;        // from lex.cpph
extern int defInPropDef;  // from lex.cpph, tracking if insided propDef

extern char* defrFileName;

double save_x;
double save_y;

static double defVersionNum = 5.7;   // default is 5.7

static int defIgnoreVersion = 0; // ignore checking version number
                                 // this is redundant, since def does not have
                                 // either 5.3 or 5.4 code

static int hasVer = 0;        // keep track VERSION is in the file
static int hasNameCase = 0;   // keep track NAMESCASESENSITIVE is in the file
static int hasDivChar = 0;    // keep track DIVIDERCHAR is in the file
static int hasBusBit = 0;     // keep track BUSBITCHARS is in the file
static int hasDes = 0;        // keep track DESIGN is in the file
static int regTypeDef;        // keep track that region type is defined 
static int aOxide = 0;        // keep track for oxide
static int netOsnet = 0;      // net = 1 & snet = 2
static int viaRule = 0;       // keep track the viarule has called first
static int hasPort = 0;       // keep track is port defined in a Pin

static int               needNPCbk = 0;     // if cbk for net path is needed
static int               needSNPCbk = 0;    // if cbk for snet path is needed
static double            xStep, yStep;
static int               hasDoStep;
static int               hasBlkLayerComp;     // only 1 BLOCKAGE/LAYER/COMP
static int               hasBlkLayerSpac;     // only 1 BLOCKAGE/LAYER/SPACING
static int               hasBlkPlaceComp;     // only 1 BLOCKAGE/PLACEMENT/COMP

// the following variables to keep track the number of warnings printed.
static int assertionWarnings = 0;
static int blockageWarnings = 0;
static int caseSensitiveWarnings = 0;
static int componentWarnings = 0;
static int constraintWarnings = 0;
static int defaultCapWarnings = 0;
static int fillWarnings = 0;
static int gcellGridWarnings = 0;
static int iOTimingWarnings = 0;
static int netWarnings = 0;
static int nonDefaultWarnings = 0;
static int pinExtWarnings = 0;
static int pinWarnings = 0;
static int regionWarnings = 0;
static int rowWarnings = 0;
static int scanchainWarnings = 0;
static int sNetWarnings = 0;
static int stylesWarnings = 0;
static int trackWarnings = 0;
static int unitsWarnings = 0;
static int versionWarnings = 0;
static int viaWarnings = 0;

int names_case_sensitive = 1; // always true in 5.6
int reader_case_sensitive = 0; // save what the reader prefer
int defRetVal;

int shield = FALSE;       // To identify if the path is shield for 5.3

static char defPropDefType;   // save the current type of the property
static char* shieldName;      // to hold the shieldNetName
static char* rowName;         // to hold the rowName for message

int bitsNum;                  // Scanchain Bits value
char *warningMsg;
char *defMsg;

/* From def_keywords.cpp */
extern char* ringCopy(const char* string);


/* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */
#define CALLBACK(func, typ, data) \
    if (!errors) {\
      if (func) { \
        if ((defRetVal = (*func)(typ, data, defrUserData)) == PARSE_OK) { \
        } else if (defRetVal == STOP_PARSE) { \
	  return defRetVal; \
        } else { \
          defError(6010, "Error in callback"); \
	  return defRetVal; \
        } \
      } \
    }

#define CHKERR() \
    if (errors > 20) {\
      defError(6011, "Too many syntax errors"); \
      errors = 0; \
      return 1; \
    }

#define CHKPROPTYPE(propType, propName, name) \
    if (propType == 'N') { \
       warningMsg = (char*)defMalloc(strlen(propName)+strlen(name)+35); \
       sprintf(warningMsg, "PropName %s is not defined for %s.", \
               propName, name); \
       defWarning(7010, warningMsg); \
       defFree(warningMsg); \
    }

double convert_defname2num(char *versionName)
{
    char majorNm[80];
    char minorNm[80];
    char *subMinorNm = NULL;
    char *p1;
    char *versionNm = strdup(versionName);

    double major = 0, minor = 0, subMinor = 0;
    double version;

    sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
    if ((p1 = strchr(minorNm, '.'))) {
       subMinorNm = p1+1;
       *p1 = '\0';
    }
    major = atof(majorNm);
    minor = atof(minorNm);
    if (subMinorNm)
       subMinor = atof(subMinorNm);

    version = major;

    if (minor > 0)
       version = major + minor/10;

    if (subMinor > 0)
       version = version + subMinor/1000;

    free(versionNm);
    return version;
}

int numIsInt (char* volt) {
    if (strchr(volt, '.'))  // a floating point
       return 0;
    else
       return 1;
}

int defValidNum(int values) {
    char *outMsg;
    switch (values) {
        case 100:
        case 200:
        case 1000:
        case 2000:
                return 1;
        case 10000:
        case 20000:
             if (defVersionNum < 5.6) {
                if (defrUnitsCbk) {
                  if (unitsWarnings++ < defrUnitsWarnings) {
                    outMsg = (char*)defMalloc(1000);
                    sprintf (outMsg,
                    "Error found when processing DEF file '%s'\nUnit %d is a 5.6 or later syntax\nYour def file is not defined as 5.6.",
                    defrFileName, values);
                    defError(6501, outMsg);
                    defFree(outMsg);
                  }
                }
                return 0;
             } else
                return 1;
    }
    if (defrUnitsCbk) {
      if (unitsWarnings++ < defrUnitsWarnings) {
        outMsg = (char*)defMalloc(10000);
        sprintf (outMsg,
          "The value %d defined for DEF UNITS DISTANCE MICRON is invalid\n. Correct value is 100, 200, 1000, 2000, 10000, or 20000.", values);
        defError(6502, outMsg);
        defFree(outMsg);
        CHKERR();
      }
    }
    return 0;
}

#define FIXED 1
#define COVER 2
#define PLACED 3
#define UNPLACED 4


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 269 "def.y"
typedef union YYSTYPE {
        double dval ;
        int    integer ;
        char * string ;
        int    keyword ;  /* really just a nop */
        struct defpoint pt;
        defTOKEN *tk;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 846 "def.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 858 "def.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1400

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  268
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  497
/* YYNRULES -- Number of rules. */
#define YYNRULES  898
/* YYNRULES -- Number of states. */
#define YYNSTATES  1568

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   515

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     264,   265,   266,   263,   267,   262,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   261,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     8,     9,    10,    15,    16,    20,    24,
      25,    28,    30,    32,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    52,    54,    56,    58,    60,    62,    64,
      66,    68,    70,    72,    74,    76,    78,    80,    82,    84,
      86,    88,    90,    92,    94,    96,    98,   100,   102,   104,
     106,   108,   109,   114,   117,   118,   123,   124,   129,   130,
     135,   137,   138,   144,   145,   148,   149,   155,   156,   162,
     163,   169,   170,   176,   177,   183,   184,   190,   191,   197,
     198,   204,   205,   211,   212,   218,   221,   222,   227,   228,
     233,   235,   238,   241,   242,   244,   250,   254,   258,   259,
     274,   275,   290,   291,   306,   308,   310,   312,   314,   316,
     318,   320,   322,   323,   330,   334,   337,   338,   341,   347,
     350,   354,   358,   359,   362,   363,   364,   365,   376,   377,
     380,   383,   385,   389,   393,   394,   399,   400,   405,   409,
     412,   413,   414,   423,   424,   425,   436,   437,   446,   450,
     455,   460,   465,   470,   471,   478,   479,   486,   491,   492,
     499,   503,   504,   507,   510,   511,   514,   517,   519,   521,
     523,   525,   527,   529,   531,   533,   535,   537,   539,   541,
     542,   543,   547,   550,   551,   552,   564,   565,   571,   572,
     576,   577,   580,   581,   586,   587,   590,   593,   596,   599,
     600,   610,   613,   615,   617,   618,   619,   624,   625,   628,
     630,   639,   641,   644,   648,   652,   653,   656,   657,   658,
     665,   666,   669,   670,   677,   678,   679,   689,   690,   695,
     696,   697,   722,   724,   726,   731,   736,   743,   744,   749,
     751,   753,   754,   757,   762,   767,   772,   777,   780,   785,
     789,   790,   793,   794,   795,   803,   806,   810,   811,   814,
     815,   820,   824,   825,   828,   831,   834,   837,   839,   841,
     845,   849,   850,   853,   857,   860,   861,   866,   867,   870,
     873,   874,   877,   879,   881,   883,   885,   887,   889,   891,
     893,   895,   897,   899,   901,   902,   907,   908,   914,   915,
     917,   921,   923,   925,   927,   929,   932,   935,   938,   942,
     943,   952,   953,   955,   956,   963,   964,   969,   971,   974,
     977,   980,   983,   986,   987,   994,   996,   999,  1003,  1006,
    1011,  1014,  1017,  1020,  1024,  1027,  1031,  1035,  1036,  1039,
    1043,  1045,  1046,  1050,  1051,  1055,  1056,  1063,  1064,  1067,
    1068,  1075,  1076,  1083,  1084,  1091,  1092,  1094,  1097,  1098,
    1101,  1102,  1103,  1109,  1113,  1116,  1117,  1122,  1123,  1128,
    1132,  1136,  1140,  1144,  1148,  1152,  1153,  1158,  1160,  1161,
    1166,  1167,  1168,  1174,  1175,  1176,  1177,  1186,  1187,  1192,
    1194,  1196,  1199,  1202,  1205,  1208,  1210,  1212,  1214,  1216,
    1218,  1219,  1226,  1227,  1232,  1233,  1234,  1238,  1239,  1243,
    1245,  1247,  1249,  1251,  1253,  1255,  1257,  1260,  1261,  1265,
    1266,  1267,  1274,  1275,  1278,  1280,  1283,  1292,  1294,  1299,
    1304,  1309,  1314,  1320,  1326,  1332,  1338,  1339,  1342,  1344,
    1346,  1348,  1349,  1353,  1356,  1357,  1360,  1364,  1368,  1371,
    1373,  1375,  1377,  1379,  1381,  1383,  1385,  1387,  1389,  1391,
    1393,  1395,  1399,  1400,  1403,  1407,  1408,  1411,  1413,  1415,
    1417,  1419,  1422,  1423,  1424,  1430,  1431,  1432,  1439,  1440,
    1441,  1451,  1452,  1459,  1463,  1466,  1470,  1471,  1476,  1480,
    1484,  1488,  1492,  1496,  1497,  1502,  1504,  1505,  1506,  1509,
    1510,  1516,  1517,  1522,  1523,  1524,  1532,  1534,  1537,  1540,
    1543,  1546,  1547,  1551,  1552,  1556,  1558,  1560,  1562,  1564,
    1566,  1569,  1570,  1574,  1575,  1576,  1584,  1586,  1590,  1593,
    1597,  1601,  1602,  1605,  1610,  1611,  1615,  1616,  1619,  1621,
    1622,  1625,  1629,  1630,  1635,  1636,  1641,  1643,  1646,  1648,
    1649,  1652,  1655,  1658,  1661,  1662,  1665,  1668,  1671,  1674,
    1677,  1681,  1685,  1689,  1693,  1694,  1697,  1699,  1701,  1706,
    1707,  1711,  1712,  1719,  1724,  1729,  1731,  1735,  1736,  1745,
    1746,  1748,  1749,  1752,  1756,  1760,  1764,  1768,  1771,  1774,
    1778,  1782,  1783,  1786,  1790,  1791,  1795,  1796,  1799,  1800,
    1802,  1803,  1809,  1810,  1815,  1816,  1821,  1822,  1828,  1829,
    1834,  1835,  1841,  1843,  1844,  1849,  1858,  1859,  1862,  1863,
    1867,  1868,  1873,  1882,  1895,  1896,  1899,  1900,  1904,  1905,
    1910,  1919,  1932,  1933,  1936,  1939,  1943,  1947,  1948,  1951,
    1955,  1956,  1963,  1964,  1967,  1973,  1979,  1983,  1984,  1985,
    1992,  1994,  1995,  1996,  2003,  2004,  2005,  2009,  2010,  2013,
    2015,  2017,  2020,  2025,  2029,  2030,  2033,  2034,  2035,  2044,
    2046,  2048,  2050,  2053,  2056,  2059,  2060,  2063,  2064,  2069,
    2070,  2075,  2076,  2079,  2080,  2086,  2087,  2093,  2097,  2101,
    2102,  2105,  2106,  2107,  2118,  2119,  2126,  2127,  2134,  2138,
    2139,  2140,  2147,  2148,  2152,  2155,  2159,  2163,  2164,  2167,
    2171,  2172,  2177,  2178,  2182,  2183,  2185,  2187,  2188,  2190,
    2192,  2193,  2196,  2197,  2205,  2206,  2213,  2214,  2220,  2221,
    2229,  2230,  2237,  2238,  2244,  2246,  2247,  2252,  2253,  2256,
    2260,  2264,  2265,  2268,  2269,  2272,  2275,  2278,  2281,  2284,
    2286,  2289,  2293,  2297,  2301,  2305,  2308,  2309,  2312,  2313,
    2320,  2321,  2324,  2325,  2328,  2329,  2333,  2334,  2338,  2340,
    2342,  2344,  2346,  2350,  2354,  2355,  2357,  2360,  2361,  2364,
    2365,  2366,  2374,  2375,  2378,  2379,  2384,  2385,  2388,  2391,
    2394,  2397,  2401,  2405,  2408,  2409,  2412,  2417,  2418,  2419,
    2426,  2427,  2432,  2433,  2436,  2440,  2444,  2446,  2447,  2452,
    2455,  2458,  2461,  2464,  2465,  2466,  2471,  2474,  2477,  2481,
    2482,  2485,  2489,  2490,  2497,  2501,  2505,  2508,  2509,  2512,
    2516,  2517,  2518,  2525,  2526,  2529,  2533,  2534,  2541,  2545,
    2549,  2552,  2553,  2556,  2560,  2561,  2562,  2572,  2573,  2574,
    2582,  2583,  2586,  2590,  2591,  2598,  2599,  2602,  2603,  2606,
    2611,  2615,  2618,  2619,  2622,  2623,  2624,  2631,  2632,  2635,
    2638,  2639,  2640,  2649,  2650,  2655,  2656,  2661,  2662,  2668,
    2670,  2671,  2674,  2677,  2680,  2683,  2684,  2689,  2690,  2693,
    2696,  2699,  2702,  2706,  2710,  2713,  2714,  2717,  2718
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     269,     0,    -1,   270,   272,   273,   278,    -1,    -1,    -1,
     159,   271,     4,   261,    -1,    -1,     8,   170,   261,    -1,
       8,   171,   261,    -1,    -1,   273,   274,    -1,     1,    -1,
     275,    -1,   555,    -1,   695,    -1,   402,    -1,   556,    -1,
     366,    -1,   724,    -1,   617,    -1,   537,    -1,   599,    -1,
     439,    -1,   739,    -1,   646,    -1,   682,    -1,   389,    -1,
     573,    -1,   713,    -1,   499,    -1,   759,    -1,   633,    -1,
     368,    -1,   281,    -1,   306,    -1,   309,    -1,   311,    -1,
     276,    -1,   314,    -1,   305,    -1,   283,    -1,   365,    -1,
     285,    -1,   316,    -1,   321,    -1,   286,    -1,   347,    -1,
     307,    -1,   279,    -1,   357,    -1,   304,    -1,    -1,     9,
     277,     4,   261,    -1,    96,     9,    -1,    -1,    11,   280,
       4,   261,    -1,    -1,    13,   282,     4,   261,    -1,    -1,
      14,   284,     4,   261,    -1,     7,    -1,    -1,   162,   287,
     288,    96,   162,    -1,    -1,   288,   289,    -1,    -1,     9,
     290,     4,   300,   261,    -1,    -1,    76,   291,     4,   300,
     261,    -1,    -1,   178,   292,     4,   300,   261,    -1,    -1,
      41,   293,     4,   300,   261,    -1,    -1,    70,   294,     4,
     300,   261,    -1,    -1,   174,   295,     4,   300,   261,    -1,
      -1,    29,   296,     4,   300,   261,    -1,    -1,   125,   297,
       4,   300,   261,    -1,    -1,   179,   298,     4,   300,   261,
      -1,    -1,    59,   299,     4,   300,   261,    -1,     1,   261,
      -1,    -1,   165,   301,   526,   303,    -1,    -1,   164,   302,
     526,   303,    -1,   163,    -1,   163,     3,    -1,   169,     4,
      -1,    -1,     6,    -1,    12,    94,    95,     6,   261,    -1,
     160,     3,   261,    -1,   161,     3,   261,    -1,    -1,    15,
     308,     4,     6,     6,   313,    25,     6,    26,     6,    27,
       6,     6,   261,    -1,    -1,    16,   310,     4,     6,     6,
     313,    25,     6,    26,     6,    27,     6,     6,   261,    -1,
      -1,    17,   312,     4,     6,     6,   313,    25,     6,    26,
       6,    27,     6,     6,   261,    -1,    61,    -1,    64,    -1,
      62,    -1,    63,    -1,    65,    -1,    68,    -1,    67,    -1,
      66,    -1,    -1,    18,   315,   384,   385,   386,   261,    -1,
     317,   318,   320,    -1,    20,     6,    -1,    -1,   318,   319,
      -1,    21,     6,    22,     6,   261,    -1,    96,    20,    -1,
     322,   323,   346,    -1,    19,     6,   261,    -1,    -1,   323,
     324,    -1,    -1,    -1,    -1,   262,   325,     4,   263,    76,
     326,     4,   327,   328,   261,    -1,    -1,   328,   329,    -1,
     263,   144,    -1,   367,    -1,   263,   145,     4,    -1,   263,
     245,     3,    -1,    -1,   263,   252,   330,     4,    -1,    -1,
     263,   241,   331,     4,    -1,   263,    47,   343,    -1,   263,
     251,    -1,    -1,    -1,   263,    28,   332,     4,   333,   340,
     387,   387,    -1,    -1,    -1,   263,   250,   334,     4,   335,
     341,   384,   385,   385,   386,    -1,    -1,   263,   253,   336,
       4,   264,     6,     6,   265,    -1,   436,   387,   313,    -1,
     263,   184,     6,   344,    -1,   263,   185,     6,   344,    -1,
     263,   186,     6,   344,    -1,   263,   187,     6,   344,    -1,
      -1,   263,   188,     6,    28,   337,     4,    -1,    -1,   263,
     189,     6,    28,   338,     4,    -1,   263,   190,     6,   344,
      -1,    -1,   263,   191,     6,    28,   339,     4,    -1,   263,
     229,   342,    -1,    -1,    58,     6,    -1,   237,     6,    -1,
      -1,    58,     6,    -1,   237,     6,    -1,   231,    -1,   232,
      -1,   233,    -1,   234,    -1,   192,    -1,   193,    -1,   194,
      -1,   195,    -1,   196,    -1,   197,    -1,   198,    -1,   199,
      -1,    -1,    -1,    28,   345,     4,    -1,    96,    19,    -1,
      -1,    -1,    29,   348,     4,     4,     6,     6,   313,   349,
     350,   352,   261,    -1,    -1,    25,     6,    26,     6,   351,
      -1,    -1,    27,     6,     6,    -1,    -1,   352,   353,    -1,
      -1,   263,   166,   354,   355,    -1,    -1,   355,   356,    -1,
       4,     6,    -1,     4,     3,    -1,     4,     4,    -1,    -1,
     359,     6,   358,    25,     6,    27,     6,   361,   261,    -1,
      23,   360,    -1,   172,    -1,   173,    -1,    -1,    -1,    28,
     362,   364,   363,    -1,    -1,   364,   363,    -1,     4,    -1,
      24,   360,     6,    25,     6,    27,     6,   261,    -1,   167,
      -1,   263,   167,    -1,   369,   370,   388,    -1,    10,     6,
     261,    -1,    -1,   370,   371,    -1,    -1,    -1,   262,   372,
       4,   373,   374,   261,    -1,    -1,   374,   375,    -1,    -1,
     263,    30,   376,     4,   387,   387,    -1,    -1,    -1,   263,
     250,   377,     4,   378,   384,   385,   385,   386,    -1,    -1,
     263,    50,   379,     4,    -1,    -1,    -1,   263,   254,   380,
       4,   263,   235,     6,     6,   263,   243,   381,     4,     4,
       4,   263,   236,     6,     6,   263,   239,     6,     6,     6,
       6,    -1,   382,    -1,   367,    -1,   263,   248,     6,     6,
      -1,   263,   247,     6,     6,    -1,   263,   246,     6,     6,
       6,     6,    -1,    -1,   263,    49,   383,     4,    -1,   387,
      -1,   387,    -1,    -1,   385,   386,    -1,   264,     6,     6,
     265,    -1,   264,   266,     6,   265,    -1,   264,     6,   266,
     265,    -1,   264,   266,   266,   265,    -1,    96,    10,    -1,
     390,   391,    96,    42,    -1,    42,     6,   261,    -1,    -1,
     391,   392,    -1,    -1,    -1,   262,   393,     4,   394,   395,
     396,   261,    -1,   387,   387,    -1,   395,   387,   387,    -1,
      -1,   396,   397,    -1,    -1,   263,   166,   398,   399,    -1,
     263,   228,   401,    -1,    -1,   399,   400,    -1,     4,     6,
      -1,     4,     3,    -1,     4,     4,    -1,   223,    -1,   225,
      -1,   403,   404,   438,    -1,    31,     6,   261,    -1,    -1,
     404,   405,    -1,   406,   410,   261,    -1,   407,   409,    -1,
      -1,   262,   408,     4,     4,    -1,    -1,   409,   266,    -1,
     409,     4,    -1,    -1,   410,   411,    -1,   415,    -1,   418,
      -1,   435,    -1,   437,    -1,   432,    -1,   420,    -1,   413,
      -1,   422,    -1,   425,    -1,   427,    -1,   412,    -1,   367,
      -1,    -1,   263,    35,   414,     4,    -1,    -1,   263,    32,
     416,     4,   417,    -1,    -1,     4,    -1,   263,    33,   419,
      -1,   215,    -1,   216,    -1,   217,    -1,   218,    -1,   431,
     421,    -1,   431,     4,    -1,   387,   387,    -1,   421,   387,
     387,    -1,    -1,   263,   240,   423,   424,     6,     6,     6,
       6,    -1,    -1,    71,    -1,    -1,   263,   260,     6,   426,
       4,     4,    -1,    -1,   263,   166,   428,   429,    -1,   430,
      -1,   429,   430,    -1,     4,     6,    -1,     4,     3,    -1,
       4,     4,    -1,   263,    41,    -1,    -1,   263,    40,   433,
       4,   434,   313,    -1,   387,    -1,     6,     6,    -1,   436,
     387,   313,    -1,   263,    38,    -1,   263,    38,   387,   313,
      -1,   263,    36,    -1,   263,    37,    -1,   263,    39,    -1,
     263,    34,     6,    -1,    96,    31,    -1,   440,   441,   497,
      -1,    43,     6,   261,    -1,    -1,   441,   442,    -1,   443,
     455,   261,    -1,   444,    -1,    -1,   262,   445,   446,    -1,
      -1,     4,   447,   449,    -1,    -1,    45,   264,     4,   448,
       4,   265,    -1,    -1,   449,   450,    -1,    -1,   264,     4,
     451,     4,   454,   265,    -1,    -1,   264,   266,   452,     4,
     454,   265,    -1,    -1,   264,   125,   453,     4,   454,   265,
      -1,    -1,   367,    -1,   263,   126,    -1,    -1,   455,   456,
      -1,    -1,    -1,   263,   480,   457,   458,   481,    -1,   263,
      33,   471,    -1,   263,   222,    -1,    -1,   263,   224,   459,
       6,    -1,    -1,   263,    46,   460,     4,    -1,   263,    49,
     527,    -1,   263,    34,     6,    -1,   263,   124,     6,    -1,
     263,    51,     6,    -1,   263,    47,   343,    -1,   263,    48,
       6,    -1,    -1,   263,    59,   461,     4,    -1,   472,    -1,
      -1,   263,   182,   462,     4,    -1,    -1,    -1,   263,   183,
     463,   464,   481,    -1,    -1,    -1,    -1,   263,   123,   465,
       4,   466,   673,   467,   677,    -1,    -1,   263,   166,   468,
     469,    -1,   367,    -1,   470,    -1,   469,   470,    -1,     4,
       6,    -1,     4,     3,    -1,     4,     4,    -1,   215,    -1,
     216,    -1,   217,    -1,   218,    -1,   176,    -1,    -1,   474,
     476,   387,   387,   473,   478,    -1,    -1,   263,   122,   475,
       4,    -1,    -1,    -1,    28,   477,     4,    -1,    -1,   479,
     387,   313,    -1,    39,    -1,    36,    -1,    37,    -1,    36,
      -1,    37,    -1,    52,    -1,   484,    -1,   481,   482,    -1,
      -1,    53,   483,   484,    -1,    -1,    -1,     4,   485,   490,
     489,   486,   487,    -1,    -1,   487,   488,    -1,     4,    -1,
       4,   313,    -1,     4,    25,     6,    26,     6,    27,     6,
       6,    -1,   489,    -1,   264,     6,     6,   265,    -1,   264,
     266,     6,   265,    -1,   264,     6,   266,   265,    -1,   264,
     266,   266,   265,    -1,   264,     6,     6,     6,   265,    -1,
     264,   266,     6,     6,   265,    -1,   264,     6,   266,     6,
     265,    -1,   264,   266,   266,     6,   265,    -1,    -1,   490,
     491,    -1,   494,    -1,   492,    -1,   157,    -1,    -1,   158,
     493,     4,    -1,    48,     6,    -1,    -1,   495,   496,    -1,
     263,    55,   498,    -1,   263,    48,     6,    -1,    96,    43,
      -1,   200,    -1,   201,    -1,   202,    -1,   203,    -1,   204,
      -1,   205,    -1,   206,    -1,   257,    -1,   230,    -1,   207,
      -1,   208,    -1,   209,    -1,   535,   500,   536,    -1,    -1,
     500,   501,    -1,   443,   502,   261,    -1,    -1,   502,   503,
      -1,   516,    -1,   518,    -1,   520,    -1,   504,    -1,   263,
     480,    -1,    -1,    -1,   263,   480,   505,   506,   528,    -1,
      -1,    -1,   263,   181,   507,     4,   508,   514,    -1,    -1,
      -1,   263,   250,   509,     4,   510,   384,   385,   385,   386,
      -1,    -1,   263,    30,   511,     4,   387,   387,    -1,   263,
      33,   419,    -1,   263,   222,    -1,   263,   224,     6,    -1,
      -1,   263,    46,   512,     4,    -1,   263,    49,   527,    -1,
     263,    34,     6,    -1,   263,    51,     6,    -1,   263,    47,
     343,    -1,   263,    48,     6,    -1,    -1,   263,   166,   513,
     523,    -1,   367,    -1,    -1,    -1,   515,   528,    -1,    -1,
     263,    56,   517,     4,     6,    -1,    -1,   263,    57,   519,
       4,    -1,    -1,    -1,   263,    58,   521,     4,     6,   522,
     525,    -1,   524,    -1,   523,   524,    -1,     4,     6,    -1,
       4,     3,    -1,     4,     4,    -1,    -1,   146,     6,     6,
      -1,    -1,   146,     6,     6,    -1,   219,    -1,   220,    -1,
     221,    -1,    91,    -1,   531,    -1,   528,   529,    -1,    -1,
      53,   530,   531,    -1,    -1,    -1,     4,   532,   534,   495,
     489,   533,   487,    -1,     6,    -1,    54,     6,   261,    -1,
      96,    54,    -1,   538,   539,   554,    -1,    69,     6,   261,
      -1,    -1,   539,   540,    -1,   541,   543,   545,   261,    -1,
      -1,   262,   542,     4,    -1,    -1,   543,   544,    -1,     4,
      -1,    -1,   545,   546,    -1,   263,    71,   552,    -1,    -1,
     263,   166,   547,   550,    -1,    -1,   263,    41,   548,   549,
      -1,   367,    -1,   387,   387,    -1,     4,    -1,    -1,   550,
     551,    -1,     4,     6,    -1,     4,     3,    -1,     4,     4,
      -1,    -1,   552,   553,    -1,    72,     6,    -1,    73,     6,
      -1,    74,     6,    -1,    96,    69,    -1,   557,   559,   572,
      -1,   558,   559,   571,    -1,    93,     6,   261,    -1,    75,
       6,   261,    -1,    -1,   559,   560,    -1,   561,    -1,   566,
      -1,   262,   562,   569,   261,    -1,    -1,    76,   563,     4,
      -1,    -1,    77,   564,     4,     4,     4,     4,    -1,    78,
     264,   565,   265,    -1,    79,   264,   565,   265,    -1,   562,
      -1,   565,   267,   562,    -1,    -1,   262,    91,   567,     4,
     568,    92,     6,   261,    -1,    -1,   263,    -1,    -1,   569,
     570,    -1,   263,    87,     6,    -1,   263,    88,     6,    -1,
     263,    89,     6,    -1,   263,    90,     6,    -1,    96,    75,
      -1,    96,    93,    -1,   574,   575,   598,    -1,    80,     6,
     261,    -1,    -1,   575,   576,    -1,   577,   579,   261,    -1,
      -1,   262,   578,     4,    -1,    -1,   579,   581,    -1,    -1,
       4,    -1,    -1,   263,    81,   582,     4,   580,    -1,    -1,
     263,    82,   583,   589,    -1,    -1,   263,    83,   584,   593,
      -1,    -1,   263,    84,   585,     4,   580,    -1,    -1,   263,
     177,   586,   588,    -1,    -1,   263,   227,   587,     4,   597,
      -1,   367,    -1,    -1,   264,     4,     4,   265,    -1,   264,
       4,     4,   265,   264,     4,     4,   265,    -1,    -1,   589,
     590,    -1,    -1,     4,   591,   592,    -1,    -1,   264,     4,
       4,   265,    -1,   264,     4,     4,   265,   264,     4,     4,
     265,    -1,   264,     4,     4,   265,   264,     4,     4,   265,
     264,     4,     4,   265,    -1,    -1,   593,   594,    -1,    -1,
       4,   595,   596,    -1,    -1,   264,     4,     4,   265,    -1,
     264,     4,     4,   265,   264,     4,     4,   265,    -1,   264,
       4,     4,   265,   264,     4,     4,   265,   264,     4,     4,
     265,    -1,    -1,   226,     6,    -1,    96,    80,    -1,   600,
     601,   616,    -1,    97,     6,   261,    -1,    -1,   601,   602,
      -1,   603,   605,   261,    -1,    -1,   262,   264,   604,     4,
       4,   265,    -1,    -1,   605,   606,    -1,   263,   615,   100,
       6,     6,    -1,   263,   615,   101,     6,     6,    -1,   263,
     102,     6,    -1,    -1,    -1,   263,   103,   607,     4,   608,
     609,    -1,   367,    -1,    -1,    -1,   612,   105,   610,     4,
     611,   614,    -1,    -1,    -1,   104,   613,     4,    -1,    -1,
     106,     6,    -1,    98,    -1,    99,    -1,    96,    97,    -1,
     618,   619,    96,   147,    -1,   147,     6,   261,    -1,    -1,
     619,   620,    -1,    -1,    -1,   262,   621,     4,   623,   622,
     624,   625,   261,    -1,   148,    -1,   149,    -1,   150,    -1,
     152,     6,    -1,   151,     6,    -1,   153,     6,    -1,    -1,
     625,   626,    -1,    -1,   263,   154,   627,   629,    -1,    -1,
     263,   155,   628,   629,    -1,    -1,   629,   630,    -1,    -1,
     264,   156,   631,     4,   265,    -1,    -1,   264,    31,   632,
       4,   265,    -1,   634,   635,   645,    -1,   107,     6,   261,
      -1,    -1,   635,   636,    -1,    -1,    -1,   262,   104,   637,
       4,     4,   105,   638,     4,     4,   261,    -1,    -1,   262,
     108,   639,     4,     4,   261,    -1,    -1,   262,   109,   640,
       4,   641,   261,    -1,   262,   180,   261,    -1,    -1,    -1,
     104,   642,     4,   105,   643,     4,    -1,    -1,   108,   644,
       4,    -1,    96,   107,    -1,   647,   648,   672,    -1,   110,
       6,   261,    -1,    -1,   648,   649,    -1,   650,   655,   261,
      -1,    -1,   262,   651,     4,   652,    -1,    -1,   111,   653,
     654,    -1,    -1,   118,    -1,   119,    -1,    -1,   120,    -1,
     121,    -1,    -1,   655,   656,    -1,    -1,   263,   112,   657,
       4,     4,   615,   663,    -1,    -1,   263,   113,   658,     4,
       4,   670,    -1,    -1,   263,   114,   659,     4,   668,    -1,
      -1,   263,   115,   660,     4,     4,   615,   663,    -1,    -1,
     263,   116,   661,     4,     4,   670,    -1,    -1,   263,   117,
     662,     4,   668,    -1,   367,    -1,    -1,   665,    19,   664,
     667,    -1,    -1,   665,   666,    -1,   151,     6,     6,    -1,
     152,     6,     6,    -1,    -1,   667,     4,    -1,    -1,   668,
     669,    -1,    87,     6,    -1,    89,     6,    -1,    88,     6,
      -1,    90,     6,    -1,   671,    -1,   670,   671,    -1,    87,
       6,     6,    -1,    89,     6,     6,    -1,    88,     6,     6,
      -1,    90,     6,     6,    -1,    96,   110,    -1,    -1,   673,
     674,    -1,    -1,   264,   675,     4,     4,   676,   265,    -1,
      -1,   263,   126,    -1,    -1,   677,   678,    -1,    -1,   681,
     679,   481,    -1,    -1,    59,   680,     4,    -1,    36,    -1,
      37,    -1,    52,    -1,   183,    -1,   683,   686,   685,    -1,
     175,     6,   684,    -1,    -1,   261,    -1,    96,   175,    -1,
      -1,   686,   687,    -1,    -1,    -1,   262,   688,     4,     4,
     689,   690,   261,    -1,    -1,   690,   691,    -1,    -1,   263,
     166,   692,   693,    -1,    -1,   693,   694,    -1,     4,     6,
      -1,     4,     3,    -1,     4,     4,    -1,   696,   698,   697,
      -1,   210,     6,   261,    -1,    96,   210,    -1,    -1,   698,
     699,    -1,   700,   711,   710,   261,    -1,    -1,    -1,   262,
      28,   701,     4,   702,   704,    -1,    -1,   262,   211,   703,
     708,    -1,    -1,   704,   705,    -1,   263,    58,     6,    -1,
     263,   237,     6,    -1,   706,    -1,    -1,   263,   174,   707,
       4,    -1,   263,   212,    -1,   263,   213,    -1,   263,   214,
      -1,   263,   256,    -1,    -1,    -1,   263,   174,   709,     4,
      -1,   263,   214,    -1,   263,    71,    -1,   263,   259,     6,
      -1,    -1,   710,   711,    -1,    30,   387,   387,    -1,    -1,
     250,   712,   384,   385,   385,   386,    -1,   714,   716,   715,
      -1,   212,     6,   261,    -1,    96,   212,    -1,    -1,   716,
     717,    -1,   718,   721,   261,    -1,    -1,    -1,   262,    28,
     719,     4,   720,   722,    -1,    -1,   721,   722,    -1,    30,
     387,   387,    -1,    -1,   250,   723,   384,   385,   385,   386,
      -1,   725,   727,   726,    -1,   213,     6,   261,    -1,    96,
     213,    -1,    -1,   727,   728,    -1,   731,   734,   261,    -1,
      -1,    -1,   262,   253,   729,     4,   730,   738,   384,   386,
     261,    -1,    -1,    -1,   262,    28,   732,     4,   733,   737,
     735,    -1,    -1,   734,   735,    -1,    30,   387,   387,    -1,
      -1,   250,   736,   384,   385,   385,   386,    -1,    -1,   263,
     258,    -1,    -1,   263,   258,    -1,   740,   743,   742,   741,
      -1,    60,     6,   261,    -1,    96,    60,    -1,    -1,   742,
     743,    -1,    -1,    -1,   262,   744,     4,   745,   746,   261,
      -1,    -1,   746,   747,    -1,   263,   242,    -1,    -1,    -1,
     263,    28,   748,     4,    56,     6,   749,   753,    -1,    -1,
     263,   253,   750,     4,    -1,    -1,   263,   254,   751,     4,
      -1,    -1,   263,   244,   752,     4,     6,    -1,   755,    -1,
      -1,   753,   754,    -1,   238,     6,    -1,    58,     6,    -1,
     255,     6,    -1,    -1,   263,   166,   756,   757,    -1,    -1,
     757,   758,    -1,     4,     6,    -1,     4,     3,    -1,     4,
       4,    -1,   760,   762,   761,    -1,   249,     6,   261,    -1,
      96,   249,    -1,    -1,   762,   763,    -1,    -1,   262,    48,
       6,   764,   384,   385,   386,   261,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   356,   356,   359,   360,   360,   386,   387,   400,   418,
     419,   420,   423,   423,   423,   423,   424,   424,   424,   425,
     425,   425,   426,   426,   426,   427,   427,   427,   428,   428,
     428,   428,   429,   432,   432,   432,   432,   433,   433,   433,
     434,   434,   434,   435,   435,   435,   436,   436,   436,   436,
     436,   439,   439,   446,   495,   495,   501,   501,   507,   507,
     513,   520,   519,   535,   536,   539,   539,   548,   548,   557,
     557,   566,   566,   575,   575,   584,   584,   593,   593,   602,
     602,   612,   611,   622,   621,   642,   644,   644,   649,   649,
     655,   660,   665,   671,   672,   675,   683,   690,   697,   697,
     710,   710,   722,   722,   735,   736,   737,   738,   739,   740,
     741,   742,   745,   744,   762,   765,   777,   778,   781,   792,
     795,   798,   804,   805,   808,   809,   810,   808,   822,   823,
     825,   831,   837,   843,   864,   864,   884,   884,   904,   908,
     929,   930,   929,   948,   949,   948,   989,   989,  1013,  1024,
    1042,  1060,  1078,  1096,  1096,  1114,  1114,  1133,  1151,  1151,
    1169,  1186,  1187,  1210,  1234,  1235,  1258,  1282,  1287,  1292,
    1297,  1303,  1305,  1307,  1309,  1311,  1313,  1315,  1317,  1322,
    1323,  1323,  1326,  1332,  1334,  1332,  1348,  1358,  1396,  1399,
    1407,  1408,  1411,  1411,  1415,  1416,  1419,  1434,  1443,  1454,
    1453,  1487,  1492,  1494,  1497,  1498,  1498,  1501,  1502,  1505,
    1511,  1540,  1546,  1549,  1552,  1558,  1559,  1562,  1563,  1562,
    1574,  1575,  1578,  1578,  1583,  1584,  1583,  1612,  1612,  1622,
    1624,  1622,  1647,  1648,  1654,  1666,  1678,  1690,  1690,  1703,
    1706,  1709,  1710,  1713,  1720,  1726,  1732,  1738,  1744,  1750,
    1756,  1757,  1760,  1761,  1760,  1770,  1773,  1778,  1779,  1782,
    1782,  1785,  1800,  1801,  1804,  1819,  1828,  1838,  1840,  1843,
    1846,  1853,  1854,  1857,  1863,  1876,  1876,  1884,  1885,  1890,
    1896,  1897,  1900,  1900,  1900,  1900,  1901,  1901,  1901,  1901,
    1902,  1902,  1902,  1905,  1912,  1912,  1918,  1918,  1926,  1927,
    1930,  1936,  1938,  1940,  1942,  1947,  1949,  1955,  1966,  1979,
    1978,  2000,  2001,  2021,  2021,  2040,  2040,  2044,  2045,  2048,
    2063,  2072,  2082,  2085,  2085,  2100,  2102,  2105,  2112,  2119,
    2137,  2139,  2141,  2144,  2150,  2156,  2159,  2166,  2167,  2170,
    2181,  2185,  2185,  2188,  2187,  2196,  2196,  2204,  2205,  2208,
    2208,  2222,  2222,  2229,  2229,  2238,  2239,  2246,  2250,  2251,
    2254,  2255,  2254,  2270,  2273,  2290,  2290,  2308,  2308,  2311,
    2314,  2317,  2320,  2323,  2326,  2329,  2329,  2340,  2342,  2342,
    2345,  2346,  2345,  2383,  2389,  2399,  2382,  2413,  2413,  2417,
    2423,  2424,  2427,  2442,  2451,  2461,  2463,  2465,  2467,  2469,
    2473,  2472,  2483,  2483,  2486,  2487,  2487,  2490,  2491,  2494,
    2496,  2498,  2501,  2503,  2505,  2509,  2513,  2516,  2516,  2522,
    2541,  2521,  2550,  2551,  2555,  2564,  2570,  2597,  2606,  2613,
    2619,  2625,  2630,  2637,  2644,  2651,  2658,  2659,  2661,  2662,
    2665,  2667,  2667,  2670,  2688,  2689,  2693,  2696,  2714,  2720,
    2722,  2724,  2726,  2728,  2730,  2732,  2734,  2750,  2752,  2754,
    2756,  2759,  2762,  2763,  2766,  2769,  2770,  2773,  2773,  2774,
    2774,  2777,  2786,  2787,  2786,  2805,  2806,  2805,  2809,  2810,
    2809,  2850,  2850,  2879,  2882,  2885,  2888,  2888,  2891,  2894,
    2897,  2906,  2909,  2912,  2912,  2916,  2920,  2929,  2929,  2971,
    2971,  2980,  2980,  2998,  2999,  2998,  3006,  3007,  3010,  3025,
    3034,  3044,  3045,  3050,  3051,  3054,  3056,  3058,  3060,  3064,
    3079,  3082,  3082,  3100,  3107,  3099,  3114,  3119,  3126,  3133,
    3136,  3142,  3143,  3146,  3152,  3152,  3163,  3164,  3167,  3174,
    3175,  3178,  3180,  3180,  3183,  3183,  3185,  3191,  3201,  3206,
    3207,  3210,  3225,  3234,  3244,  3245,  3248,  3256,  3264,  3273,
    3280,  3284,  3287,  3301,  3315,  3316,  3319,  3320,  3330,  3343,
    3343,  3348,  3348,  3353,  3358,  3364,  3365,  3368,  3368,  3377,
    3378,  3381,  3382,  3385,  3390,  3395,  3400,  3406,  3417,  3428,
    3431,  3437,  3438,  3441,  3447,  3447,  3454,  3455,  3460,  3461,
    3464,  3464,  3468,  3468,  3471,  3470,  3479,  3479,  3483,  3483,
    3485,  3485,  3503,  3510,  3511,  3520,  3534,  3535,  3539,  3538,
    3549,  3550,  3563,  3584,  3615,  3616,  3620,  3619,  3628,  3629,
    3642,  3663,  3695,  3696,  3699,  3708,  3711,  3722,  3723,  3726,
    3732,  3732,  3738,  3739,  3743,  3748,  3753,  3758,  3759,  3758,
    3784,  3791,  3792,  3790,  3798,  3799,  3799,  3805,  3806,  3812,
    3812,  3814,  3820,  3826,  3832,  3833,  3836,  3837,  3836,  3841,
    3843,  3846,  3848,  3850,  3852,  3855,  3856,  3860,  3859,  3863,
    3862,  3867,  3868,  3870,  3870,  3872,  3872,  3875,  3879,  3886,
    3887,  3890,  3891,  3890,  3899,  3899,  3907,  3907,  3915,  3921,
    3922,  3921,  3927,  3927,  3933,  3940,  3943,  3950,  3951,  3954,
    3960,  3960,  3966,  3967,  3974,  3975,  3977,  3981,  3982,  3984,
    3987,  3988,  3991,  3991,  3997,  3997,  4003,  4003,  4009,  4009,
    4015,  4015,  4021,  4021,  4026,  4034,  4033,  4037,  4038,  4041,
    4046,  4052,  4053,  4056,  4057,  4059,  4061,  4063,  4065,  4069,
    4070,  4073,  4076,  4079,  4082,  4086,  4090,  4091,  4094,  4094,
    4103,  4104,  4107,  4108,  4111,  4110,  4123,  4123,  4126,  4128,
    4130,  4132,  4135,  4137,  4143,  4144,  4147,  4151,  4152,  4155,
    4156,  4155,  4165,  4166,  4168,  4168,  4172,  4173,  4176,  4191,
    4200,  4210,  4212,  4216,  4220,  4221,  4224,  4233,  4234,  4233,
    4251,  4250,  4265,  4266,  4269,  4295,  4317,  4321,  4321,  4337,
    4352,  4367,  4382,  4411,  4413,  4413,  4428,  4443,  4471,  4500,
    4501,  4504,  4510,  4509,  4539,  4541,  4545,  4549,  4550,  4553,
    4561,  4562,  4561,  4569,  4570,  4573,  4579,  4578,  4595,  4597,
    4601,  4605,  4606,  4609,  4616,  4617,  4616,  4637,  4638,  4637,
    4645,  4646,  4649,  4655,  4654,  4671,  4672,  4692,  4693,  4713,
    4716,  4734,  4738,  4739,  4742,  4743,  4742,  4753,  4754,  4757,
    4762,  4764,  4762,  4771,  4771,  4777,  4777,  4783,  4783,  4789,
    4792,  4793,  4796,  4802,  4808,  4816,  4816,  4820,  4821,  4824,
    4835,  4844,  4855,  4857,  4874,  4878,  4879,  4883,  4882
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "QSTRING", "T_STRING", "SITE_PATTERN",
  "NUMBER", "K_HISTORY", "K_NAMESCASESENSITIVE", "K_DESIGN", "K_VIAS",
  "K_TECH", "K_UNITS", "K_ARRAY", "K_FLOORPLAN", "K_SITE", "K_CANPLACE",
  "K_CANNOTOCCUPY", "K_DIEAREA", "K_PINS", "K_DEFAULTCAP", "K_MINPINS",
  "K_WIRECAP", "K_TRACKS", "K_GCELLGRID", "K_DO", "K_BY", "K_STEP",
  "K_LAYER", "K_ROW", "K_RECT", "K_COMPS", "K_COMP_GEN", "K_SOURCE",
  "K_WEIGHT", "K_EEQMASTER", "K_FIXED", "K_COVER", "K_UNPLACED",
  "K_PLACED", "K_FOREIGN", "K_REGION", "K_REGIONS", "K_NETS",
  "K_START_NET", "K_MUSTJOIN", "K_ORIGINAL", "K_USE", "K_STYLE",
  "K_PATTERN", "K_PATTERNNAME", "K_ESTCAP", "K_ROUTED", "K_NEW", "K_SNETS",
  "K_SHAPE", "K_WIDTH", "K_VOLTAGE", "K_SPACING", "K_NONDEFAULTRULE",
  "K_NONDEFAULTRULES", "K_N", "K_S", "K_E", "K_W", "K_FN", "K_FE", "K_FS",
  "K_FW", "K_GROUPS", "K_GROUP", "K_SOFT", "K_MAXX", "K_MAXY",
  "K_MAXHALFPERIMETER", "K_CONSTRAINTS", "K_NET", "K_PATH", "K_SUM",
  "K_DIFF", "K_SCANCHAINS", "K_START", "K_FLOATING", "K_ORDERED", "K_STOP",
  "K_IN", "K_OUT", "K_RISEMIN", "K_RISEMAX", "K_FALLMIN", "K_FALLMAX",
  "K_WIREDLOGIC", "K_MAXDIST", "K_ASSERTIONS", "K_DISTANCE", "K_MICRONS",
  "K_END", "K_IOTIMINGS", "K_RISE", "K_FALL", "K_VARIABLE", "K_SLEWRATE",
  "K_CAPACITANCE", "K_DRIVECELL", "K_FROMPIN", "K_TOPIN", "K_PARALLEL",
  "K_TIMINGDISABLES", "K_THRUPIN", "K_MACRO", "K_PARTITIONS", "K_TURNOFF",
  "K_FROMCLOCKPIN", "K_FROMCOMPPIN", "K_FROMIOPIN", "K_TOCLOCKPIN",
  "K_TOCOMPPIN", "K_TOIOPIN", "K_SETUPRISE", "K_SETUPFALL", "K_HOLDRISE",
  "K_HOLDFALL", "K_VPIN", "K_SUBNET", "K_XTALK", "K_PIN", "K_SYNTHESIZED",
  "K_DEFINE", "K_DEFINES", "K_DEFINEB", "K_IF", "K_THEN", "K_ELSE",
  "K_FALSE", "K_TRUE", "K_EQ", "K_NE", "K_LE", "K_LT", "K_GE", "K_GT",
  "K_OR", "K_AND", "K_NOT", "K_SPECIAL", "K_DIRECTION", "K_RANGE", "K_FPC",
  "K_HORIZONTAL", "K_VERTICAL", "K_ALIGN", "K_MIN", "K_MAX", "K_EQUAL",
  "K_BOTTOMLEFT", "K_TOPRIGHT", "K_ROWS", "K_TAPER", "K_TAPERRULE",
  "K_VERSION", "K_DIVIDERCHAR", "K_BUSBITCHARS", "K_PROPERTYDEFINITIONS",
  "K_STRING", "K_REAL", "K_INTEGER", "K_PROPERTY", "K_BEGINEXT",
  "K_ENDEXT", "K_NAMEMAPSTRING", "K_ON", "K_OFF", "K_X", "K_Y",
  "K_COMPONENT", "K_PINPROPERTIES", "K_TEST", "K_COMMONSCANPINS", "K_SNET",
  "K_COMPONENTPIN", "K_REENTRANTPATHS", "K_SHIELD", "K_SHIELDNET",
  "K_NOSHIELD", "K_ANTENNAPINPARTIALMETALAREA",
  "K_ANTENNAPINPARTIALMETALSIDEAREA", "K_ANTENNAPINGATEAREA",
  "K_ANTENNAPINDIFFAREA", "K_ANTENNAPINMAXAREACAR",
  "K_ANTENNAPINMAXSIDEAREACAR", "K_ANTENNAPINPARTIALCUTAREA",
  "K_ANTENNAPINMAXCUTCAR", "K_SIGNAL", "K_POWER", "K_GROUND", "K_CLOCK",
  "K_TIEOFF", "K_ANALOG", "K_SCAN", "K_RESET", "K_RING", "K_STRIPE",
  "K_FOLLOWPIN", "K_IOWIRE", "K_COREWIRE", "K_BLOCKWIRE", "K_FILLWIRE",
  "K_BLOCKAGEWIRE", "K_PADRING", "K_BLOCKRING", "K_BLOCKAGES",
  "K_PLACEMENT", "K_SLOTS", "K_FILLS", "K_PUSHDOWN", "K_NETLIST", "K_DIST",
  "K_USER", "K_TIMING", "K_BALANCED", "K_STEINER", "K_TRUNK",
  "K_FIXEDBUMP", "K_FENCE", "K_FREQUENCY", "K_GUIDE", "K_MAXBITS",
  "K_PARTITION", "K_TYPE", "K_ANTENNAMODEL", "K_DRCFILL", "K_OXIDE1",
  "K_OXIDE2", "K_OXIDE3", "K_OXIDE4", "K_CUTSIZE", "K_CUTSPACING",
  "K_DESIGNRULEWIDTH", "K_DIAGWIDTH", "K_ENCLOSURE", "K_HALO",
  "K_GROUNDSENSITIVITY", "K_HARDSPACING", "K_LAYERS", "K_MINCUTS",
  "K_NETEXPR", "K_OFFSET", "K_ORIGIN", "K_ROWCOL", "K_STYLES", "K_POLYGON",
  "K_PORT", "K_SUPPLYSENSITIVITY", "K_VIA", "K_VIARULE", "K_WIREEXT",
  "K_EXCEPTPGNET", "K_FILLWIREOPC", "K_OPC", "K_PARTIAL", "K_ROUTEHALO",
  "';'", "'-'", "'+'", "'('", "')'", "'*'", "','", "$accept", "def_file",
  "version_stmt", "@1", "case_sens_stmt", "rules", "rule",
  "design_section", "design_name", "@2", "end_design", "tech_name", "@3",
  "array_name", "@4", "floorplan_name", "@5", "history",
  "prop_def_section", "@6", "property_defs", "property_def", "@7", "@8",
  "@9", "@10", "@11", "@12", "@13", "@14", "@15", "@16",
  "property_type_and_val", "@17", "@18", "opt_num_val", "units",
  "divider_char", "bus_bit_chars", "site", "@19", "canplace", "@20",
  "cannotoccupy", "@21", "orient", "die_area", "@22", "pin_cap_rule",
  "start_def_cap", "pin_caps", "pin_cap", "end_def_cap", "pin_rule",
  "start_pins", "pins", "pin", "@23", "@24", "@25", "pin_options",
  "pin_option", "@26", "@27", "@28", "@29", "@30", "@31", "@32", "@33",
  "@34", "@35", "pin_layer_spacing_opt", "pin_poly_spacing_opt",
  "pin_oxide", "use_type", "pin_layer_opt", "@36", "end_pins", "row_rule",
  "@37", "@38", "row_do_option", "row_step_option", "row_options",
  "row_option", "@39", "row_prop_list", "row_prop", "tracks_rule", "@40",
  "track_start", "track_type", "track_layer_statement", "@41",
  "track_layers", "track_layer", "gcellgrid", "extension_section",
  "extension_stmt", "via_section", "via", "via_declarations",
  "via_declaration", "@42", "@43", "layer_stmts", "layer_stmt", "@44",
  "@45", "@46", "@47", "@48", "@49", "layer_viarule_opts", "@50",
  "firstPt", "nextPt", "otherPts", "pt", "via_end", "regions_section",
  "regions_start", "regions_stmts", "regions_stmt", "@51", "@52",
  "rect_list", "region_options", "region_option", "@53",
  "region_prop_list", "region_prop", "region_type", "comps_section",
  "start_comps", "comps_rule", "comp", "comp_start", "comp_id_and_name",
  "@54", "comp_net_list", "comp_options", "comp_option",
  "comp_extension_stmt", "comp_eeq", "@55", "comp_generate", "@56",
  "opt_pattern", "comp_source", "source_type", "comp_region",
  "comp_pnt_list", "comp_halo", "@57", "halo_soft", "comp_routehalo",
  "@58", "comp_property", "@59", "comp_prop_list", "comp_prop",
  "comp_region_start", "comp_foreign", "@60", "opt_paren", "comp_type",
  "placement_status", "weight", "end_comps", "nets_section", "start_nets",
  "net_rules", "one_net", "net_and_connections", "net_start", "@61",
  "net_name", "@62", "@63", "net_connections", "net_connection", "@64",
  "@65", "@66", "conn_opt", "net_options", "net_option", "@67", "@68",
  "@69", "@70", "@71", "@72", "@73", "@74", "@75", "@76", "@77", "@78",
  "net_prop_list", "net_prop", "netsource_type", "vpin_stmt", "@79",
  "vpin_begin", "@80", "vpin_layer_opt", "@81", "vpin_options",
  "vpin_status", "net_type", "paths", "new_path", "@82", "path", "@83",
  "@84", "path_item_list", "path_item", "path_pt", "opt_taper_style_s",
  "opt_taper_style", "opt_taper", "@85", "opt_style", "opt_spaths",
  "opt_shape_style", "end_nets", "shape_type", "snets_section",
  "snet_rules", "snet_rule", "snet_options", "snet_option",
  "snet_other_option", "@86", "@87", "@88", "@89", "@90", "@91", "@92",
  "@93", "@94", "shield_layer", "@95", "snet_width", "@96", "snet_voltage",
  "@97", "snet_spacing", "@98", "@99", "snet_prop_list", "snet_prop",
  "opt_snet_range", "opt_range", "pattern_type", "spaths", "snew_path",
  "@100", "spath", "@101", "@102", "width", "start_snets", "end_snets",
  "groups_section", "groups_start", "group_rules", "group_rule",
  "start_group", "@103", "group_members", "group_member", "group_options",
  "group_option", "@104", "@105", "group_region", "group_prop_list",
  "group_prop", "group_soft_options", "group_soft_option", "groups_end",
  "assertions_section", "constraint_section", "assertions_start",
  "constraints_start", "constraint_rules", "constraint_rule",
  "operand_rule", "operand", "@106", "@107", "operand_list",
  "wiredlogic_rule", "@108", "opt_plus", "delay_specs", "delay_spec",
  "constraints_end", "assertions_end", "scanchains_section",
  "scanchain_start", "scanchain_rules", "scan_rule", "start_scan", "@109",
  "scan_members", "opt_pin", "scan_member", "@110", "@111", "@112", "@113",
  "@114", "@115", "opt_common_pins", "floating_inst_list",
  "one_floating_inst", "@116", "floating_pins", "ordered_inst_list",
  "one_ordered_inst", "@117", "ordered_pins", "partition_maxbits",
  "scanchain_end", "iotiming_section", "iotiming_start", "iotiming_rules",
  "iotiming_rule", "start_iotiming", "@118", "iotiming_members",
  "iotiming_member", "@119", "@120", "iotiming_drivecell_opt", "@121",
  "@122", "iotiming_frompin", "@123", "iotiming_parallel", "risefall",
  "iotiming_end", "floorplan_contraints_section", "fp_start", "fp_stmts",
  "fp_stmt", "@124", "@125", "h_or_v", "constraint_type",
  "constrain_what_list", "constrain_what", "@126", "@127",
  "row_or_comp_list", "row_or_comp", "@128", "@129",
  "timingdisables_section", "timingdisables_start", "timingdisables_rules",
  "timingdisables_rule", "@130", "@131", "@132", "@133", "td_macro_option",
  "@134", "@135", "@136", "timingdisables_end", "partitions_section",
  "partitions_start", "partition_rules", "partition_rule",
  "start_partition", "@137", "turnoff", "turnoff_setup", "turnoff_hold",
  "partition_members", "partition_member", "@138", "@139", "@140", "@141",
  "@142", "@143", "minmaxpins", "@144", "min_or_max_list",
  "min_or_max_member", "pin_list", "risefallminmax1_list",
  "risefallminmax1", "risefallminmax2_list", "risefallminmax2",
  "partitions_end", "comp_names", "comp_name", "@145", "subnet_opt_syn",
  "subnet_options", "subnet_option", "@146", "@147", "subnet_type",
  "pin_props_section", "begin_pin_props", "opt_semi", "end_pin_props",
  "pin_prop_list", "pin_prop_terminal", "@148", "@149", "pin_prop_options",
  "pin_prop", "@150", "pin_prop_name_value_list", "pin_prop_name_value",
  "blockage_section", "blockage_start", "blockage_end", "blockage_defs",
  "blockage_def", "blockage_rule", "@151", "@152", "@153",
  "layer_blockage_rules", "layer_blockage_rule", "comp_blockage_rule",
  "@154", "placement_comp_rule", "@155", "rectPoly_blockage_rules",
  "rectPoly_blockage", "@156", "slot_section", "slot_start", "slot_end",
  "slot_defs", "slot_def", "slot_rule", "@157", "@158", "geom_slot_rules",
  "geom_slot", "@159", "fill_section", "fill_start", "fill_end",
  "fill_defs", "fill_def", "@160", "@161", "fill_rule", "@162", "@163",
  "geom_fill_rules", "geom_fill", "@164", "fill_layer_opc", "fill_via_opc",
  "nondefaultrule_section", "nondefault_start", "nondefault_end",
  "nondefault_defs", "nondefault_def", "@165", "@166",
  "nondefault_options", "nondefault_option", "@167", "@168", "@169",
  "@170", "@171", "nondefault_layer_options", "nondefault_layer_option",
  "nondefault_prop_opt", "@172", "nondefault_prop_list", "nondefault_prop",
  "styles_section", "styles_start", "styles_end", "styles_rules",
  "styles_rule", "@173", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,    59,    45,    43,    40,    41,    42,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   268,   269,   270,   271,   270,   272,   272,   272,   273,
     273,   273,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   277,   276,   278,   280,   279,   282,   281,   284,   283,
     285,   287,   286,   288,   288,   290,   289,   291,   289,   292,
     289,   293,   289,   294,   289,   295,   289,   296,   289,   297,
     289,   298,   289,   299,   289,   289,   301,   300,   302,   300,
     300,   300,   300,   303,   303,   304,   305,   306,   308,   307,
     310,   309,   312,   311,   313,   313,   313,   313,   313,   313,
     313,   313,   315,   314,   316,   317,   318,   318,   319,   320,
     321,   322,   323,   323,   325,   326,   327,   324,   328,   328,
     329,   329,   329,   329,   330,   329,   331,   329,   329,   329,
     332,   333,   329,   334,   335,   329,   336,   329,   329,   329,
     329,   329,   329,   337,   329,   338,   329,   329,   339,   329,
     329,   340,   340,   340,   341,   341,   341,   342,   342,   342,
     342,   343,   343,   343,   343,   343,   343,   343,   343,   344,
     345,   344,   346,   348,   349,   347,   350,   350,   351,   351,
     352,   352,   354,   353,   355,   355,   356,   356,   356,   358,
     357,   359,   360,   360,   361,   362,   361,   363,   363,   364,
     365,   366,   367,   368,   369,   370,   370,   372,   373,   371,
     374,   374,   376,   375,   377,   378,   375,   379,   375,   380,
     381,   375,   375,   375,   382,   382,   382,   383,   382,   384,
     385,   386,   386,   387,   387,   387,   387,   388,   389,   390,
     391,   391,   393,   394,   392,   395,   395,   396,   396,   398,
     397,   397,   399,   399,   400,   400,   400,   401,   401,   402,
     403,   404,   404,   405,   406,   408,   407,   409,   409,   409,
     410,   410,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   412,   414,   413,   416,   415,   417,   417,
     418,   419,   419,   419,   419,   420,   420,   421,   421,   423,
     422,   424,   424,   426,   425,   428,   427,   429,   429,   430,
     430,   430,   431,   433,   432,   434,   434,   435,   435,   435,
     436,   436,   436,   437,   438,   439,   440,   441,   441,   442,
     443,   445,   444,   447,   446,   448,   446,   449,   449,   451,
     450,   452,   450,   453,   450,   454,   454,   454,   455,   455,
     457,   458,   456,   456,   456,   459,   456,   460,   456,   456,
     456,   456,   456,   456,   456,   461,   456,   456,   462,   456,
     463,   464,   456,   465,   466,   467,   456,   468,   456,   456,
     469,   469,   470,   470,   470,   471,   471,   471,   471,   471,
     473,   472,   475,   474,   476,   477,   476,   478,   478,   479,
     479,   479,   480,   480,   480,   481,   481,   483,   482,   485,
     486,   484,   487,   487,   488,   488,   488,   488,   489,   489,
     489,   489,   489,   489,   489,   489,   490,   490,   491,   491,
     492,   493,   492,   494,   495,   495,   496,   496,   497,   498,
     498,   498,   498,   498,   498,   498,   498,   498,   498,   498,
     498,   499,   500,   500,   501,   502,   502,   503,   503,   503,
     503,   504,   505,   506,   504,   507,   508,   504,   509,   510,
     504,   511,   504,   504,   504,   504,   512,   504,   504,   504,
     504,   504,   504,   513,   504,   504,   514,   515,   514,   517,
     516,   519,   518,   521,   522,   520,   523,   523,   524,   524,
     524,   525,   525,   526,   526,   527,   527,   527,   527,   528,
     528,   530,   529,   532,   533,   531,   534,   535,   536,   537,
     538,   539,   539,   540,   542,   541,   543,   543,   544,   545,
     545,   546,   547,   546,   548,   546,   546,   549,   549,   550,
     550,   551,   551,   551,   552,   552,   553,   553,   553,   554,
     555,   556,   557,   558,   559,   559,   560,   560,   561,   563,
     562,   564,   562,   562,   562,   565,   565,   567,   566,   568,
     568,   569,   569,   570,   570,   570,   570,   571,   572,   573,
     574,   575,   575,   576,   578,   577,   579,   579,   580,   580,
     582,   581,   583,   581,   584,   581,   585,   581,   586,   581,
     587,   581,   581,   588,   588,   588,   589,   589,   591,   590,
     592,   592,   592,   592,   593,   593,   595,   594,   596,   596,
     596,   596,   597,   597,   598,   599,   600,   601,   601,   602,
     604,   603,   605,   605,   606,   606,   606,   607,   608,   606,
     606,   610,   611,   609,   612,   613,   612,   614,   614,   615,
     615,   616,   617,   618,   619,   619,   621,   622,   620,   623,
     623,   624,   624,   624,   624,   625,   625,   627,   626,   628,
     626,   629,   629,   631,   630,   632,   630,   633,   634,   635,
     635,   637,   638,   636,   639,   636,   640,   636,   636,   642,
     643,   641,   644,   641,   645,   646,   647,   648,   648,   649,
     651,   650,   652,   652,   653,   653,   653,   654,   654,   654,
     655,   655,   657,   656,   658,   656,   659,   656,   660,   656,
     661,   656,   662,   656,   656,   664,   663,   665,   665,   666,
     666,   667,   667,   668,   668,   669,   669,   669,   669,   670,
     670,   671,   671,   671,   671,   672,   673,   673,   675,   674,
     676,   676,   677,   677,   679,   678,   680,   678,   681,   681,
     681,   681,   682,   683,   684,   684,   685,   686,   686,   688,
     689,   687,   690,   690,   692,   691,   693,   693,   694,   694,
     694,   695,   696,   697,   698,   698,   699,   701,   702,   700,
     703,   700,   704,   704,   705,   705,   705,   707,   706,   706,
     706,   706,   706,   708,   709,   708,   708,   708,   708,   710,
     710,   711,   712,   711,   713,   714,   715,   716,   716,   717,
     719,   720,   718,   721,   721,   722,   723,   722,   724,   725,
     726,   727,   727,   728,   729,   730,   728,   732,   733,   731,
     734,   734,   735,   736,   735,   737,   737,   738,   738,   739,
     740,   741,   742,   742,   744,   745,   743,   746,   746,   747,
     748,   749,   747,   750,   747,   751,   747,   752,   747,   747,
     753,   753,   754,   754,   754,   756,   755,   757,   757,   758,
     758,   758,   759,   760,   761,   762,   762,   764,   763
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     0,     4,     0,     3,     3,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     2,     0,     4,     0,     4,     0,     4,
       1,     0,     5,     0,     2,     0,     5,     0,     5,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
       5,     0,     5,     0,     5,     2,     0,     4,     0,     4,
       1,     2,     2,     0,     1,     5,     3,     3,     0,    14,
       0,    14,     0,    14,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     6,     3,     2,     0,     2,     5,     2,
       3,     3,     0,     2,     0,     0,     0,    10,     0,     2,
       2,     1,     3,     3,     0,     4,     0,     4,     3,     2,
       0,     0,     8,     0,     0,    10,     0,     8,     3,     4,
       4,     4,     4,     0,     6,     0,     6,     4,     0,     6,
       3,     0,     2,     2,     0,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     3,     2,     0,     0,    11,     0,     5,     0,     3,
       0,     2,     0,     4,     0,     2,     2,     2,     2,     0,
       9,     2,     1,     1,     0,     0,     4,     0,     2,     1,
       8,     1,     2,     3,     3,     0,     2,     0,     0,     6,
       0,     2,     0,     6,     0,     0,     9,     0,     4,     0,
       0,    24,     1,     1,     4,     4,     6,     0,     4,     1,
       1,     0,     2,     4,     4,     4,     4,     2,     4,     3,
       0,     2,     0,     0,     7,     2,     3,     0,     2,     0,
       4,     3,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     0,     2,     3,     2,     0,     4,     0,     2,     2,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     0,     5,     0,     1,
       3,     1,     1,     1,     1,     2,     2,     2,     3,     0,
       8,     0,     1,     0,     6,     0,     4,     1,     2,     2,
       2,     2,     2,     0,     6,     1,     2,     3,     2,     4,
       2,     2,     2,     3,     2,     3,     3,     0,     2,     3,
       1,     0,     3,     0,     3,     0,     6,     0,     2,     0,
       6,     0,     6,     0,     6,     0,     1,     2,     0,     2,
       0,     0,     5,     3,     2,     0,     4,     0,     4,     3,
       3,     3,     3,     3,     3,     0,     4,     1,     0,     4,
       0,     0,     5,     0,     0,     0,     8,     0,     4,     1,
       1,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       0,     6,     0,     4,     0,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     3,     0,
       0,     6,     0,     2,     1,     2,     8,     1,     4,     4,
       4,     4,     5,     5,     5,     5,     0,     2,     1,     1,
       1,     0,     3,     2,     0,     2,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     2,     3,     0,     2,     1,     1,     1,
       1,     2,     0,     0,     5,     0,     0,     6,     0,     0,
       9,     0,     6,     3,     2,     3,     0,     4,     3,     3,
       3,     3,     3,     0,     4,     1,     0,     0,     2,     0,
       5,     0,     4,     0,     0,     7,     1,     2,     2,     2,
       2,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       2,     0,     3,     0,     0,     7,     1,     3,     2,     3,
       3,     0,     2,     4,     0,     3,     0,     2,     1,     0,
       2,     3,     0,     4,     0,     4,     1,     2,     1,     0,
       2,     2,     2,     2,     0,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     0,     2,     1,     1,     4,     0,
       3,     0,     6,     4,     4,     1,     3,     0,     8,     0,
       1,     0,     2,     3,     3,     3,     3,     2,     2,     3,
       3,     0,     2,     3,     0,     3,     0,     2,     0,     1,
       0,     5,     0,     4,     0,     4,     0,     5,     0,     4,
       0,     5,     1,     0,     4,     8,     0,     2,     0,     3,
       0,     4,     8,    12,     0,     2,     0,     3,     0,     4,
       8,    12,     0,     2,     2,     3,     3,     0,     2,     3,
       0,     6,     0,     2,     5,     5,     3,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     3,     0,     2,     1,
       1,     2,     4,     3,     0,     2,     0,     0,     8,     1,
       1,     1,     2,     2,     2,     0,     2,     0,     4,     0,
       4,     0,     2,     0,     5,     0,     5,     3,     3,     0,
       2,     0,     0,    10,     0,     6,     0,     6,     3,     0,
       0,     6,     0,     3,     2,     3,     3,     0,     2,     3,
       0,     4,     0,     3,     0,     1,     1,     0,     1,     1,
       0,     2,     0,     7,     0,     6,     0,     5,     0,     7,
       0,     6,     0,     5,     1,     0,     4,     0,     2,     3,
       3,     0,     2,     0,     2,     2,     2,     2,     2,     1,
       2,     3,     3,     3,     3,     2,     0,     2,     0,     6,
       0,     2,     0,     2,     0,     3,     0,     3,     1,     1,
       1,     1,     3,     3,     0,     1,     2,     0,     2,     0,
       0,     7,     0,     2,     0,     4,     0,     2,     2,     2,
       2,     3,     3,     2,     0,     2,     4,     0,     0,     6,
       0,     4,     0,     2,     3,     3,     1,     0,     4,     2,
       2,     2,     2,     0,     0,     4,     2,     2,     3,     0,
       2,     3,     0,     6,     3,     3,     2,     0,     2,     3,
       0,     0,     6,     0,     2,     3,     0,     6,     3,     3,
       2,     0,     2,     3,     0,     0,     9,     0,     0,     7,
       0,     2,     3,     0,     6,     0,     2,     0,     2,     4,
       3,     2,     0,     2,     0,     0,     6,     0,     2,     2,
       0,     0,     8,     0,     4,     0,     4,     0,     5,     1,
       0,     2,     2,     2,     2,     0,     4,     0,     2,     2,
       2,     2,     3,     3,     2,     0,     2,     0,     8
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       3,     4,     0,     6,     0,     1,     0,     0,     0,     0,
       0,    11,     0,     5,     7,     8,    60,    51,     0,    54,
       0,    56,    58,    98,   100,   102,   112,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,   211,
       0,     0,     0,     0,     0,    10,    12,    37,     2,    48,
      33,    40,    42,    45,    50,    39,    34,    47,    35,    36,
      38,    43,   116,    44,   122,    46,    49,     0,    41,    17,
      32,   215,    26,   250,    15,   271,    22,   337,    29,   462,
      20,   531,    13,    16,   564,   564,    27,   591,    21,   637,
      19,   664,    31,   689,    24,   707,    25,   777,    14,   794,
      28,   827,    18,   841,    23,     0,    30,   895,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
     202,   203,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,    63,   774,     0,     0,     0,     0,     0,     0,   199,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   864,   862,     0,
       0,   214,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   121,     0,     0,   270,   249,   336,   527,   860,
     530,   563,   590,   562,   636,   688,   706,   663,    96,    97,
       0,   775,   773,   792,   825,   839,   893,     0,     0,   117,
     114,     0,   124,   123,   120,     0,     0,   217,   216,   213,
       0,   252,   251,     0,   275,   272,   280,   277,   269,     0,
     341,   338,   358,   340,   335,     0,   465,   463,   461,     0,
     534,   532,   536,   529,     0,     0,   565,   566,   567,   560,
       0,   561,     0,   594,   592,   596,   589,     0,     0,   638,
     642,   635,     0,   666,   665,     0,     0,   690,   687,     0,
     710,   708,   720,   705,     0,   779,   772,   778,     0,     0,
     791,   795,     0,     0,     0,   824,   828,   833,     0,     0,
     838,   842,   850,     0,     0,     0,     0,   892,   896,    52,
      55,     0,    57,    59,     0,     0,     0,     0,     0,   241,
     240,     0,     0,     0,    65,    77,    71,    83,    73,    67,
       0,    79,    75,    69,    81,    64,     0,   119,   182,     0,
       0,   247,     0,   248,     0,   334,     0,     0,   274,   448,
       0,     0,   528,     0,   559,     0,   539,   588,   569,   571,
       0,     0,   577,   581,   587,   634,     0,     0,   661,   640,
       0,   662,     0,   704,   691,   694,   696,     0,   755,     0,
       0,   776,     0,   793,   797,   800,     0,   822,   819,   826,
     830,     0,   840,   847,   844,     0,   865,     0,   859,   863,
     894,     0,    95,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   218,
     253,     0,   273,     0,   293,   281,   292,   288,   282,   283,
     287,   289,   290,   291,     0,   286,   284,     0,   285,   279,
     278,   343,     0,   342,   339,     0,   389,   359,   377,   404,
     464,     0,   495,   466,   470,   467,   468,   469,   535,   538,
     537,     0,     0,     0,     0,     0,     0,     0,   595,   593,
       0,   612,   597,     0,   639,     0,   650,   643,     0,     0,
       0,     0,   698,   712,   709,     0,   734,   721,     0,     0,
     813,     0,     0,     0,     0,     0,   836,   829,   834,     0,
       0,     0,   853,   843,   851,   867,   861,   897,   104,   106,
     107,   105,   108,   111,   110,   109,     0,     0,     0,   243,
     245,   244,   246,   242,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     220,     0,   276,   296,     0,     0,   294,   330,   331,   328,
     332,   323,   322,   315,   212,   309,     0,   306,     0,   305,
       0,   347,     0,     0,     0,   412,   413,   367,     0,     0,
       0,     0,   414,   375,   402,   383,     0,   387,   378,   380,
     364,   365,   360,   405,     0,   481,     0,     0,   486,     0,
       0,     0,     0,   499,   501,   503,   493,   475,   484,     0,
     478,   471,   533,     0,   546,   540,   570,     0,   575,     0,
       0,   579,   568,     0,   582,   600,   602,   604,   606,   608,
     610,     0,   659,   660,     0,   647,     0,   669,   670,   667,
       0,     0,     0,   714,   711,   722,   724,   726,   728,   730,
     732,   780,   798,     0,   801,   821,     0,   796,   820,   831,
       0,     0,   848,   845,     0,     0,     0,     0,     0,     0,
       0,     0,   184,    90,    88,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   118,   125,   204,
       0,     0,   257,     0,   301,   302,   303,   304,   300,   333,
       0,     0,     0,     0,   311,   313,   307,     0,   327,   344,
     345,   399,   395,   396,   397,   398,   363,   370,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   373,   374,   518,
     515,   516,   517,   369,   372,     0,     0,     0,   371,     0,
       0,   381,     0,   361,     0,     0,     0,   483,   489,     0,
     491,   492,   488,   490,     0,     0,     0,     0,     0,   485,
       0,   473,   544,   554,   542,     0,   573,     0,   574,   580,
       0,     0,     0,     0,     0,     0,   616,   624,     0,   613,
       0,     0,   646,     0,     0,     0,     0,     0,     0,   699,
     702,     0,   715,   716,   717,     0,     0,     0,     0,     0,
       0,   782,   802,   817,   814,   816,     0,     0,     0,   835,
       0,   855,   857,   852,     0,   866,     0,   868,   879,     0,
       0,     0,     0,   210,   186,    91,   513,   513,    92,    66,
      78,    72,    84,    74,    68,    80,    76,    70,    82,     0,
     205,     0,   219,     0,   233,   221,   232,   255,     0,     0,
     298,   295,   329,     0,     0,   316,   317,   312,     0,     0,
     308,     0,   348,     0,   368,   376,   403,   384,     0,   388,
     390,   379,     0,   366,     0,   406,   400,     0,   487,     0,
     502,     0,     0,   494,   506,   476,   479,     0,     0,   541,
     549,     0,   576,     0,   583,   584,   585,   586,   598,   603,
     605,   598,     0,   609,   632,   641,   648,     0,     0,   671,
       0,     0,     0,   675,   692,   695,     0,     0,   697,   718,
     719,   713,     0,     0,   743,     0,     0,   743,     0,   799,
       0,   818,   241,   832,     0,     0,     0,     0,     0,     0,
     870,   885,   869,   877,   873,   875,   241,     0,     0,     0,
       0,   190,     0,    93,    93,   126,     0,   200,   222,   237,
     227,     0,     0,     0,   224,   229,   256,   254,     0,   258,
     299,   297,     0,   325,     0,   320,   321,   319,   318,     0,
       0,   349,   353,   351,     0,   756,   393,   394,   392,   391,
     419,   382,   415,   362,   407,     0,   500,   504,   509,   510,
     508,   507,   496,     0,   523,   474,   519,   548,     0,   545,
       0,     0,     0,   555,   543,   572,     0,   599,   601,   618,
     617,   626,   625,   607,     0,     0,   611,   654,   644,   645,
     673,   672,   674,     0,     0,     0,   703,     0,     0,   727,
       0,     0,   733,   781,     0,   783,     0,   803,   806,   815,
     823,   241,   856,   849,   858,   241,   241,     0,   887,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      89,    87,   128,   209,   207,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,   326,   324,     0,   314,     0,
       0,     0,   346,   385,   436,   417,   416,   410,   411,   409,
     401,     0,   482,   511,   477,     0,     0,     0,   521,   520,
     547,   556,   557,   558,     0,   550,   578,   620,   628,     0,
     633,   655,   649,     0,   668,     0,   676,     0,   700,   737,
       0,     0,     0,     0,   725,   749,     0,     0,     0,     0,
     744,   737,   731,   784,     0,   807,   809,   810,   811,     0,
     812,   837,     0,   854,     0,   886,     0,   874,   876,   898,
       0,     0,     0,     0,   185,     0,   191,   514,     0,   206,
     207,     0,   238,   228,     0,   235,   234,   225,     0,   262,
     267,   268,   261,     0,   355,   355,   355,   758,   762,   757,
       0,     0,     0,     0,   505,   498,     0,   526,   444,     0,
     552,   553,   551,     0,   619,     0,   627,   614,     0,   651,
     677,   679,     0,     0,   723,     0,     0,     0,     0,     0,
     750,   745,   747,   746,   748,   729,   786,   804,     0,   805,
     846,     0,     0,   888,   878,     0,     0,     0,   188,   192,
     127,     0,   129,   131,     0,   208,     0,     0,     0,     0,
     260,   310,     0,   356,     0,     0,     0,     0,   386,     0,
     440,   441,     0,   420,   437,   439,   438,   418,   408,     0,
     241,     0,   522,     0,     0,     0,   656,     0,   681,   681,
     693,   701,   735,     0,     0,   738,   751,   753,   752,   754,
     785,   808,   871,   890,   891,   889,     0,     0,     0,     0,
     187,   194,   140,     0,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   143,   139,   134,
     146,     0,   223,   236,     0,     0,     0,   263,   357,   350,
     354,   352,     0,   768,   769,   770,   766,   771,   763,   764,
     443,     0,     0,     0,   422,   512,   480,     0,   524,   445,
       0,     0,     0,   652,   678,   680,   741,     0,     0,     0,
     787,   880,    99,   101,   103,     0,   193,     0,   138,   132,
     179,   179,   179,   179,     0,     0,   179,     0,   167,   168,
     169,   170,   160,     0,   133,     0,     0,     0,   148,     0,
       0,   265,   266,   264,   760,     0,     0,   442,     0,     0,
       0,     0,   421,     0,     0,   422,   621,   629,     0,   657,
       0,   682,   736,   739,   740,   789,   790,   788,   872,   189,
       0,   195,   141,   180,   149,   150,   151,   152,   153,   155,
     157,   158,   137,   144,   135,     0,   241,     0,     0,     0,
     767,   765,     0,   428,     0,   430,     0,   429,     0,   431,
     424,   423,   427,   447,   449,   450,   451,   452,   453,   454,
     455,   458,   459,   460,   457,   456,   446,   525,     0,     0,
     615,     0,   653,   685,   683,   742,     0,     0,     0,   881,
     197,   198,   196,   161,     0,     0,     0,     0,   164,     0,
     226,     0,   761,   759,   432,   434,   433,   435,     0,   425,
       0,     0,   658,     0,     0,   883,   882,   884,     0,     0,
       0,   181,   154,   156,   159,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,   162,   163,     0,   165,   166,
       0,     0,     0,     0,   622,   630,   686,   684,   142,     0,
     147,     0,     0,     0,     0,   241,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,   426,   623,   631,     0,
       0,     0,     0,     0,     0,     0,     0,   231
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     2,     3,     4,     7,    12,    55,    56,    57,   118,
      58,    59,   120,    60,   122,    61,   123,    62,    63,   151,
     210,   335,   415,   420,   424,   417,   419,   423,   416,   422,
     425,   418,   677,   827,   826,  1070,    64,    65,    66,    67,
     124,    68,   125,    69,   126,   526,    70,   127,    71,    72,
     157,   219,   220,    73,    74,   158,   223,   339,   839,  1072,
    1168,  1242,  1386,  1383,  1367,  1483,  1385,  1488,  1387,  1485,
    1486,  1487,  1510,  1517,  1382,   727,  1424,  1484,   224,    75,
     134,   824,   951,  1300,  1067,  1166,  1301,  1366,  1421,    76,
     225,    77,   132,   841,   956,  1169,  1170,    78,    79,  1253,
      80,    81,   160,   228,   342,   550,   690,   845,  1075,  1081,
    1248,  1077,  1082,  1532,   846,  1076,   190,   410,   411,   320,
     229,    82,    83,   161,   232,   344,   551,   692,   849,   969,
    1179,  1250,  1327,  1182,    84,    85,   162,   235,   236,   237,
     346,   348,   347,   435,   436,   437,   700,   438,   693,   971,
     439,   698,   440,   569,   441,   704,   858,   442,   859,   443,
     703,   855,   856,   444,   445,   702,   974,   446,   447,   448,
     238,    86,    87,   163,   241,   242,   243,   350,   453,   571,
     863,   709,   862,  1089,  1091,  1090,  1254,   351,   457,   743,
     874,   742,   718,   735,   740,   741,   872,   737,   985,  1188,
     739,   869,   870,   716,   458,   994,   459,   736,   594,   744,
    1100,  1101,   592,   991,  1096,  1191,   992,  1094,  1344,  1402,
    1451,  1452,  1190,  1264,  1265,  1341,  1266,  1271,  1349,   244,
    1466,    88,   164,   247,   353,   463,   464,   761,   887,   758,
    1002,   760,  1003,   746,   749,   757,  1104,  1105,   465,   754,
     466,   755,   467,   756,  1103,   883,   884,  1194,   953,   733,
    1005,  1109,  1199,  1006,  1107,  1405,  1198,    89,   248,    90,
      91,   165,   251,   252,   355,   356,   470,   471,   615,   890,
     888,  1009,  1014,  1115,   889,  1013,   253,    92,    93,    94,
      95,   166,   256,   257,   618,   472,   473,   619,   258,   476,
     770,   477,   624,   261,   259,    96,    97,   168,   264,   265,
     366,   367,  1018,   482,   775,   776,   777,   778,   779,   780,
     903,   899,  1020,  1117,  1204,   900,  1022,  1118,  1206,  1026,
     266,    98,    99,   169,   269,   270,   483,   370,   487,   783,
    1027,  1122,  1277,  1409,  1123,  1208,  1472,   636,   271,   100,
     101,   170,   274,   372,   786,   639,   913,  1033,  1126,  1278,
    1279,  1354,  1411,  1504,  1503,   102,   103,   171,   277,   489,
    1034,   490,   491,   791,   916,  1213,   917,   278,   104,   105,
     172,   281,   282,   379,   644,   794,   921,   380,   497,   795,
     796,   797,   798,   799,   800,  1214,  1356,  1215,  1285,  1412,
    1039,  1140,  1134,  1135,   283,  1093,  1189,  1257,  1439,  1258,
    1338,  1396,  1395,  1339,   106,   107,   212,   286,   173,   287,
     382,   801,   928,  1045,  1226,  1290,  1360,   108,   109,   290,
     174,   291,   292,   499,   802,   500,   929,  1047,  1048,  1228,
     654,   930,   503,   388,   502,   110,   111,   295,   175,   296,
     297,   504,   808,   391,   508,   661,   112,   113,   300,   176,
     301,   510,   812,   302,   509,   811,   395,   514,   665,   936,
     938,   114,   115,   398,   304,   178,   303,   515,   666,   817,
    1057,  1361,  1060,  1061,  1059,  1418,  1479,   818,  1058,  1155,
    1233,   116,   117,   307,   179,   308,   667
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1190
static const short int yypact[] =
{
     -85, -1190,    80,    77,   124, -1190,   -53,    44,  -126,  -118,
    -105, -1190,   319, -1190, -1190, -1190, -1190, -1190,   159, -1190,
      74, -1190, -1190, -1190, -1190, -1190, -1190,   172,   179,   -50,
     -50, -1190,   184,   187,   227,   236,   239,   258,   274,   280,
     282,   219,   292,   301,   306,   312,   257,   369, -1190, -1190,
     385,   390,   403,   412,   425, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190,   428, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,   177, -1190, -1190,   318,   226,
     444,   401,   497,   505,   519,   520,   523,   264,   275, -1190,
   -1190, -1190, -1190,   529,   558,   310,   320,   325,   329,   334,
     351,   353,   355,   356, -1190,   359,   360,   363,   364,   366,
     368, -1190,   371,   372,   373,   374,   375,    48,   -52, -1190,
     -46,   -44,   -31,   -26,   -25,   -24,   -19,   -14,     3,     4,
       6,    33,    38,    42,    49,    52,    53, -1190, -1190,    57,
     376, -1190,   377,   561,   380,   389,   604,   609,   617,     8,
     264, -1190, -1190,   603,   626, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
     185, -1190, -1190, -1190, -1190, -1190, -1190,   639,   627, -1190,
   -1190,   632, -1190, -1190, -1190,   621,   646, -1190, -1190, -1190,
     618, -1190, -1190,   628, -1190, -1190, -1190, -1190, -1190,   619,
   -1190, -1190, -1190, -1190, -1190,   607, -1190, -1190, -1190,   594,
   -1190, -1190, -1190, -1190,   571,   304, -1190, -1190, -1190, -1190,
     591, -1190,   587, -1190, -1190, -1190, -1190,   573,   404, -1190,
   -1190, -1190,   522, -1190, -1190,   564,    54, -1190, -1190,   562,
   -1190, -1190, -1190, -1190,   498, -1190, -1190, -1190,   464,    19,
   -1190, -1190,    -3,   463,   648, -1190, -1190, -1190,   465,     5,
   -1190, -1190, -1190,   673,    59,   430,   633, -1190, -1190, -1190,
   -1190,   419, -1190, -1190,   676,   677,   678,    10,    11,   264,
   -1190,   679,   680,   426, -1190, -1190, -1190, -1190, -1190, -1190,
     526, -1190, -1190, -1190, -1190, -1190,   667, -1190, -1190,   686,
     685, -1190,   688, -1190,   689, -1190,   690,  -158,     9, -1190,
      62,  -151, -1190,  -147, -1190,   691,   692, -1190, -1190, -1190,
     433,   434, -1190, -1190, -1190, -1190,   695,  -111, -1190, -1190,
    -102, -1190,   696, -1190, -1190, -1190, -1190,   440, -1190,   700,
     -63, -1190,   701, -1190, -1190, -1190,   264, -1190, -1190, -1190,
   -1190,   -15, -1190, -1190, -1190,   -10, -1190,   647, -1190, -1190,
   -1190,   702, -1190,   397,   397,   397,   441,   445,   447,   448,
     264,   453,   682,   709, -1190,   712,   714,   715,   716,   718,
     719, -1190,   720,   721,   723,   727,   726,   470,   707, -1190,
   -1190,   731, -1190,    56, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190,    14, -1190, -1190,   264, -1190, -1190,
   -1190, -1190,   472, -1190, -1190,   606, -1190, -1190, -1190,   710,
   -1190,   536, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,   -34,   733,   735,   367,   367,   736,   129, -1190, -1190,
     263, -1190, -1190,   737, -1190,   253, -1190, -1190,    64,   738,
     739,   740, -1190,   634, -1190,   254, -1190, -1190,   744,   745,
     489,   264,   264,    -9,   749,   264, -1190, -1190, -1190,   750,
     751,   264, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190,   732,   734,   741, -1190,
   -1190, -1190, -1190, -1190, -1190,   756,   397,   -38,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,   -38,   495,   687,   758,
   -1190,   264, -1190, -1190,   267,   759, -1190, -1190, -1190,   264,
   -1190, -1190, -1190, -1190, -1190, -1190,   761, -1190,   264,   264,
     397, -1190,   764,   -43,   763, -1190, -1190, -1190,   349,   765,
      51,   768, -1190, -1190, -1190, -1190,   769, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190,   264, -1190,   267,   770, -1190,   349,
     771,    51,   772, -1190, -1190, -1190, -1190, -1190, -1190,   773,
   -1190,   766, -1190,    35, -1190, -1190, -1190,   776, -1190,     2,
     152,   518, -1190,   416, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,   778, -1190, -1190,   777, -1190,   149, -1190, -1190, -1190,
     780,   781,    63,   222, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190,   -35, -1190, -1190,   264, -1190, -1190, -1190,
     264,   264, -1190, -1190,   264,   264,   186,   264,   784,   785,
     787,   533, -1190,   792, -1190, -1190,   783,   535,   537,   538,
     539,   540,   541,   542,   543,   544,   545, -1190, -1190,   779,
     247,   264,   264,   793, -1190, -1190, -1190, -1190, -1190, -1190,
     804,   397,   805,   806,   742, -1190, -1190,   264, -1190,   548,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,   810, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,   811,   812,   813, -1190,   814,
     815, -1190,   816, -1190,   817,   264,   819, -1190, -1190,   820,
   -1190, -1190, -1190, -1190,   821,   822,   825,   827,   828, -1190,
     829, -1190, -1190, -1190, -1190,   830, -1190,   367, -1190, -1190,
     728,   831,   832,   833,   834,   837, -1190, -1190,   838,   579,
     840,   570, -1190,   841,   842,   843,   362,   746,   575, -1190,
   -1190,   585, -1190, -1190,   233,   846,   848,   849,   850,   851,
     852, -1190, -1190, -1190, -1190, -1190,   853,   264,    12, -1190,
     264,   584,   595, -1190,   264, -1190,    55, -1190, -1190,   264,
     835,   836,   839, -1190,   844, -1190,   711,   711, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,   856,
   -1190,   605, -1190,   139, -1190, -1190, -1190, -1190,   264,   259,
     860, -1190, -1190,    28,   321,   806, -1190, -1190,   861,   866,
   -1190,     7, -1190,   867, -1190, -1190, -1190, -1190,   354,   814,
   -1190, -1190,   868, -1190,   868, -1190, -1190,   264, -1190,   869,
   -1190,   871,   394,   827, -1190, -1190, -1190,   870,    15,   135,
   -1190,   874, -1190,   873, -1190, -1190, -1190, -1190,   876,   878,
     879,   876,   880, -1190,   659, -1190, -1190,   881,   882, -1190,
     883,   884,   885, -1190, -1190, -1190,   888,   889, -1190, -1190,
   -1190, -1190,   890,   891, -1190,   892,   893, -1190,   300,   610,
     894, -1190,   264, -1190,   264,   641,    13,   642,   264,   264,
   -1190, -1190, -1190, -1190, -1190, -1190,   264,   895,   896,   897,
     898, -1190,   899,   900,   900, -1190,   903, -1190, -1190, -1190,
   -1190,   902,   904,   905, -1190, -1190, -1190, -1190,  -117, -1190,
   -1190, -1190,   906, -1190,   397, -1190, -1190, -1190, -1190,   907,
     910, -1190, -1190, -1190,   644, -1190, -1190, -1190, -1190, -1190,
   -1190,   862, -1190,   862,   386,   264, -1190, -1190, -1190, -1190,
   -1190, -1190,   912,   264, -1190,   864, -1190, -1190,   264, -1190,
     913,   914,   916, -1190,   919, -1190,   625, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190,   920,   921, -1190,   824, -1190, -1190,
   -1190, -1190, -1190,   316,   922,   826, -1190,   278,   462,   466,
     278,   462,   466, -1190,   767, -1190,   -17, -1190, -1190, -1190,
   -1190,   264, -1190, -1190, -1190,   264,   264,   925, -1190,   926,
     928,   930,   664,   908,   909,   911,   915,   328,   931, -1190,
   -1190, -1190, -1190, -1190,   903,   935,   936,   938,   937,   939,
     940,   943,   944, -1190,   384, -1190, -1190,   945, -1190,   946,
     948,   949, -1190,   693, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,   264, -1190,   798, -1190,   870,   264,   950, -1190, -1190,
   -1190, -1190, -1190, -1190,   432, -1190, -1190,   694,   697,   684,
   -1190, -1190, -1190,   854, -1190,   273, -1190,   951, -1190, -1190,
     954,   956,   957,   958,   462, -1190,   959,   960,   961,   962,
   -1190, -1190,   462, -1190,   963, -1190, -1190, -1190, -1190,   964,
   -1190, -1190,   717, -1190,   917,   967,   966, -1190, -1190, -1190,
     969,   970,   971,   973, -1190,   788, -1190, -1190,   350, -1190,
     903,   264, -1190, -1190,   974, -1190, -1190, -1190,   722, -1190,
   -1190, -1190, -1190,   975,   724,   724,   724, -1190, -1190, -1190,
     -11,   868,   397,   976, -1190,   864,   264, -1190, -1190,   870,
   -1190, -1190, -1190,   979, -1190,   980, -1190,   725,   982, -1190,
   -1190, -1190,   729,   984, -1190,    29,   985,   986,   987,   988,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,   991, -1190,
   -1190,   990,   485, -1190, -1190,   992,   993,   994,   977, -1190,
   -1190,   266, -1190, -1190,   264, -1190,   264,   995,   264,   762,
     998, -1190,   -47, -1190,   747,   748,   752,   999,   140,  1000,
   -1190, -1190,    16, -1190, -1190, -1190, -1190, -1190, -1190,  1001,
     264,   150, -1190,  1005,  1006,  1007, -1190,  1010, -1190, -1190,
   -1190, -1190, -1190,  1009,  1012, -1190, -1190, -1190, -1190, -1190,
    1015, -1190, -1190, -1190, -1190, -1190,   760,   774,   775,  1014,
   -1190, -1190, -1190,   349, -1190,  1018,  1017,  1019,  1020,  1021,
    1022,  1024,  1025,  1026,   326, -1190,  1002, -1190, -1190, -1190,
   -1190,   397, -1190, -1190,   264,  1027,   494, -1190, -1190, -1190,
   -1190, -1190,  1030, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,  1033,    17,    18, -1190, -1190, -1190,    60, -1190, -1190,
     782,   786,  1034, -1190,   789,   789, -1190,  1035,  1036,   572,
   -1190, -1190, -1190, -1190, -1190,  1037,  1041,  1042, -1190, -1190,
     996,   996,   996,   996,  1011,  1028,   996,  1029, -1190, -1190,
   -1190, -1190, -1190,  1044, -1190,  1045,  1048,  1050, -1190,   264,
    1049, -1190, -1190, -1190,   795,  1055,   868, -1190,    22,    24,
      25,    26,    21,  1054,   269, -1190,   797,   799,   800,   934,
      47, -1190,  1058, -1190, -1190, -1190, -1190, -1190,   -23, -1190,
     602, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,   802,   264,   801,   941,   803,
   -1190,   862,   807, -1190,   808, -1190,   809, -1190,   818, -1190,
     340, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190,    21,  1065,  1066,
   -1190,  1069, -1190, -1190, -1190, -1190,  1070,  1071,  1072, -1190,
   -1190, -1190, -1190,   -20,  1067,  1075,  1076,  1077,   -12,  1078,
   -1190,   845, -1190, -1190, -1190, -1190, -1190, -1190,  1079, -1190,
    1082,  1083, -1190,  1085,  1086, -1190, -1190, -1190,  1087,  1088,
     264, -1190, -1190, -1190, -1190,  1089,  1090,   264,  1091, -1190,
    1056,   847,   855,   857,   858, -1190, -1190,   264, -1190, -1190,
     264,   859,  1094,  1093,   863,   865, -1190, -1190, -1190,   264,
   -1190,  1096,  1064,  1097,  1098,   264,  1099,  1100,  1101,  1103,
   -1190,   872,  1102,   875,   877,   901, -1190, -1190, -1190,  1104,
    1105,   886,   887,  1107,  1108,  1109,  1110, -1190
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,    58, -1190, -1190,   138, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,  -400, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,  -592, -1189, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,  1074, -1190, -1190,   -61,   162, -1190, -1190,  -341,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190,  -501,  -188,  -407,  -127,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,   532, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,   270, -1190, -1190, -1190, -1190, -1190,   -37, -1190,
   -1190, -1190, -1190, -1190, -1190,   968, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190,  -693, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,   261, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,   672,  -866, -1190, -1190,   -57, -1190, -1190,  -266,
   -1190, -1150, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190,   260, -1190,   314,   546,
      39, -1190, -1190,   -54, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,  1051, -1190, -1190,  -246, -1190, -1190,   675, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190,   250, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190,  -956, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190,  -131, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,    20, -1190, -1190, -1190, -1190,
     225, -1190,   112, -1033, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190,   651, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190,   347, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190,   220, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190,   918, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190, -1190,
   -1190, -1190, -1190, -1190, -1190, -1190, -1190
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -498
static const short int yytable[] =
{
     191,   656,   319,   533,   527,   528,   434,   750,   993,   363,
     456,   981,   462,   449,   317,   505,   406,   408,   567,  1007,
     511,   386,  1342,  1398,  1400,  1450,   481,   386,  1442,   486,
    1444,  1446,  1448,   393,   972,  1476,   803,  1259,  1508,   496,
    1263,  1144,   505,   511,   221,    11,  1515,   384,  1282,  1083,
     226,    -9,   230,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,   233,   451,    -9,    -9,   217,
     239,   245,   249,    -9,     1,    -9,   762,   254,  1473,  1328,
       5,  1129,   260,   940,  1141,     6,    -9,    -9,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,    -9,   262,
     267,  1220,   272,   432,    -9,   433,   763,   452,  1403,  1220,
     454,  1084,   455,    -9,   460,  1404,   461,     9,    10,    -9,
     564,  1348,   130,   131,    -9,   673,   674,   675,     8,   275,
     614,   676,   982,   711,   279,    13,   672,    -9,   284,   804,
      -9,    -9,   729,    14,   218,   288,  1260,  1261,   293,   298,
     479,    -9,   480,   305,    -9,   397,    15,  1145,   374,   484,
     810,   485,   375,   376,   814,   119,   819,   789,   121,   958,
     708,   790,   712,   713,   714,   715,  1333,  1334,   128,   805,
    1283,  1284,  1425,  1426,  1427,   129,   323,  1430,   959,   960,
     135,    -9,  1335,   136,   324,  1146,  1147,  1148,   494,  1336,
     495,   764,   564,  1474,    -9,    -9,    -9,  1010,  1011,  1012,
     222,    -9,   637,   638,   325,  1477,   227,  1509,   231,    -9,
    1149,   941,   563,   564,   806,  1516,   326,   612,   144,   613,
     385,   234,  1478,   137,   377,   506,   240,   240,   250,  1150,
     512,   387,   138,   255,   327,   139,   507,   387,   255,   784,
     785,   513,   657,  1262,    -9,   328,    -9,    -9,   394,   501,
     149,   329,   506,   512,   140,   263,   268,   766,   273,   767,
     730,   731,   732,   983,   318,   450,   407,   409,   189,   189,
     141,   330,  1343,  1399,  1401,  1262,   142,  1443,   143,  1445,
    1447,  1449,   189,    -9,  1302,   276,   565,   942,   145,   943,
     280,   852,   557,   558,   285,   560,   564,   146,   944,   945,
     331,   289,   147,  1303,   294,   299,   566,   568,   148,   306,
     570,   177,   180,  1337,   975,   976,    16,   977,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     792,   793,    29,    30,   625,   626,   627,   628,    31,   844,
      32,   632,   633,   919,   920,   634,   635,   986,   987,   332,
     988,    33,    34,   333,   334,  1498,   645,   646,   647,   648,
     649,   650,   150,    35,   655,   191,   632,   633,   660,    36,
     358,   359,   360,   361,   664,   961,   962,   963,    37,   964,
     622,   152,   623,   965,    38,   362,   153,   998,   999,    39,
    1000,   518,   519,   520,   521,   522,   523,   524,   525,   154,
    1304,  1305,    40,  1347,  1262,    41,    42,   768,   155,   767,
     564,   564,  1097,  1098,   691,  1099,    43,  1210,  1211,    44,
     564,   156,   701,   564,   159,  1200,  1201,  1055,  1202,   177,
     629,   706,   707,   358,   359,   360,   361,   815,   182,   816,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,   518,   519,
     520,   521,   522,   523,   524,   525,    45,   745,   807,  1454,
    1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,    46,
      47,    48,   694,   695,   696,   697,    49,   181,  1293,  1294,
     630,  1295,  1255,  1256,    50,  1314,   183,  1391,  1392,  1464,
    1393,   184,  1106,   771,   772,   773,   774,  1315,   842,   185,
     843,  1316,   909,   910,   911,   912,  1317,  1318,  1319,  1320,
     967,   892,   968,   186,   187,  1050,  1465,   188,   189,    51,
    1441,    52,    53,   809,   191,   193,   192,   813,   191,  1062,
     191,   719,   720,   721,   722,   723,   724,   725,   726,  1130,
    1131,  1132,  1133,  1136,  1137,  1138,  1139,  1378,  1379,  1380,
    1381,  1043,   194,  1044,   847,   848,   595,   311,    54,   596,
     597,   195,   575,   576,  1086,  1415,  1416,  1124,  1417,  1125,
     860,   196,   598,   599,   600,   601,   197,   602,   582,  1164,
     198,  1165,   603,   604,   605,   199,   678,   679,   680,   681,
     682,   683,   684,   685,   686,  1480,  1481,  1180,  1482,  1181,
     314,  1240,   200,  1241,   201,   315,   202,   203,   876,   932,
     204,   205,   934,   316,   206,   207,   939,   208,   321,   209,
     322,   946,   211,   213,   214,   215,   216,   309,   310,   573,
     574,   312,   575,   576,  1151,   336,   340,   337,  1152,  1153,
     313,   338,   577,   578,   579,   580,   341,   581,   582,   345,
     343,   352,   349,   354,   357,   583,   364,   365,   369,   371,
     368,   373,   378,   381,   383,   389,   390,   396,   392,   400,
     402,   401,   403,   404,   405,   412,   413,   414,   421,   426,
     427,   428,   429,   430,   431,   468,   469,   474,   475,   478,
     488,   492,   606,   564,   493,   498,   529,   516,   517,   535,
     530,  1368,   531,   532,   534,   536,   537,   607,   538,   539,
     540,   966,   541,   542,   543,   544,   973,   545,   584,   585,
     586,   546,   547,   548,   549,   552,   572,   616,   593,   617,
     621,   631,   640,   641,   642,   643,  1051,  1324,   651,   652,
     995,  1056,   653,   659,   662,   663,   687,   668,   608,   669,
     609,  1008,   671,   688,   689,   699,   670,   705,   710,   717,
    -472,   728,   587,   564,   734,   738,   748,   751,   753,   759,
     765,   769,   781,   782,   787,   788,   610,   828,   588,   589,
     820,   821,  1268,   822,   823,   825,   829,   850,   830,   831,
     832,   833,   834,   835,   836,   837,   838,   840,   851,   853,
     854,   191,   861,   857,   864,   865,   866,   867,   868,   871,
     893,   875,   873,   877,   878,   879,   880,  1243,   590,   881,
     591,   882,   885,   886,   891,   905,   915,   894,   895,   896,
     897,   898,   901,   902,   904,   906,   918,   935,   907,   908,
     922,   914,   923,   924,   925,   926,   927,   952,   937,   931,
     955,   947,   948,  1346,   970,   949,   957,   979,  1102,   950,
     980,   984,   990,  1046,  1004,   996,   191,   997,  1015,  1016,
    1017,  1110,  1019,  1021,  1024,  1025,  1116,  1028,  1029,  1030,
    1031,  1032,  1035,  1036,  1037,  1038,  1040,  1041,  1049,  1052,
    1054,  1063,  1064,  1065,  1066,  1068,  1069,  1073,  1078,  1092,
    1079,  1080,  1085,  1087,  1088,  1095,  -497,  1108,  1196,  1111,
    1112,  1388,  1113,  1114,  1119,  1159,  1127,  1120,  1121,  1154,
    1156,  1128,  1157,  1143,  1158,  1160,  1161,  1167,  1162,  1171,
    1172,  1163,  1173,  1174,  1193,  1175,  1176,  1177,  1178,  1207,
    1184,  1183,  1185,  1186,  1239,  1212,  1197,  1187,  1203,  1209,
    1216,  1205,  1217,  1218,  1219,  1221,  1222,  1223,  1224,  1227,
    1229,  1232,  1234,  1231,  1192,  1235,  1236,  1237,  1230,  1238,
    1247,  1251,  1269,  1273,  1274,  1249,  1276,  1252,  1281,  1275,
    1280,  1286,  1287,  1288,  1289,  1291,  1292,  1325,  1296,  1297,
    1298,  1323,  1326,  1332,  1299,  1384,  1340,  1345,  1270,  1350,
    1351,  1352,  1329,  1330,  1353,  1357,  1530,  1331,  1358,  1359,
    1365,  1362,  1369,  1370,  1423,  1371,  1372,  1373,  1374,  1490,
    1375,  1376,  1377,  1390,  1394,  1363,  1364,  1397,  1408,  1428,
    1471,  1413,  1414,  1419,  1246,  1420,  1422,  1406,  1432,  1433,
    1499,  1407,  1434,  1410,  1435,  1437,  1429,  1431,  1438,  1440,
    1453,  1468,  1475,  1469,  1491,  1470,  1489,  1492,  1493,  1500,
    1501,  1511,  1494,  1495,  1496,  1502,  1505,  1506,  1507,  1512,
    1513,  1514,  1533,  1497,  1518,  1520,  1521,  1522,  1519,  1523,
    1524,  1547,  1071,  1525,  1526,  1528,  1529,  1531,  1541,  1542,
    1546,  1548,  1549,  1551,   133,  1553,  1552,  1554,  1556,  1245,
    1560,  1561,  1534,  1564,  1565,  1566,  1567,  1321,  1074,  1322,
    1535,   191,  1536,  1537,  1540,   978,  1563,  1543,   747,  1544,
     989,  1244,   246,   611,  1267,  1555,  1389,  1559,  1550,  1467,
    1557,   954,  1558,  1001,  1195,  1272,   167,   752,  1355,  1562,
     620,  1023,  1042,  1142,   658,   933,  1053,     0,     0,     0,
       0,  1225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1539,     0,     0,     0,     0,     0,     0,     0,
       0,  1545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1527,     0,     0,     0,     0,     0,     0,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1538
};

static const short int yycheck[] =
{
     127,   502,   190,   410,   404,   405,   347,   599,   874,   255,
     351,     4,   353,     4,     6,    30,     6,     6,     4,     4,
      30,    30,     6,     6,     6,     4,   367,    30,     6,   370,
       6,     6,     6,    28,     6,    58,    71,    48,    58,   380,
    1190,    58,    30,    30,    96,     1,    58,    28,    19,   166,
      96,     7,    96,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    96,     4,    23,    24,    21,
      96,    96,    96,    29,   159,    31,    41,    96,    31,   126,
       0,  1037,    96,    28,  1040,     8,    42,    43,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    54,    96,
      96,  1134,    96,   261,    60,   263,    71,    45,    48,  1142,
     261,   228,   263,    69,   261,    55,   263,   170,   171,    75,
     167,  1271,   172,   173,    80,   163,   164,   165,     4,    96,
     471,   169,   125,   176,    96,   261,   536,    93,    96,   174,
      96,    97,    91,   261,    96,    96,   157,   158,    96,    96,
     261,   107,   263,    96,   110,    96,   261,   174,   104,   261,
     661,   263,   108,   109,   665,     6,   667,   104,    94,    30,
     570,   108,   215,   216,   217,   218,    36,    37,     6,   214,
     151,   152,  1371,  1372,  1373,     6,     1,  1376,    49,    50,
       6,   147,    52,     6,     9,   212,   213,   214,   261,    59,
     263,   166,   167,   156,   160,   161,   162,    72,    73,    74,
     262,   167,   148,   149,    29,   238,   262,   237,   262,   175,
     237,   166,   166,   167,   259,   237,    41,   261,     9,   263,
     211,   262,   255,     6,   180,   250,   262,   262,   262,   256,
     250,   250,     6,   262,    59,     6,   261,   250,   262,   100,
     101,   261,   261,   264,   210,    70,   212,   213,   253,   386,
       3,    76,   250,   250,     6,   262,   262,   265,   262,   267,
     219,   220,   221,   266,   266,   266,   266,   266,   264,   264,
       6,    96,   266,   266,   266,   264,     6,   265,     6,   265,
     265,   265,   264,   249,    28,   262,   240,   242,     6,   244,
     262,   701,    36,    37,   262,    39,   167,     6,   253,   254,
     125,   262,     6,    47,   262,   262,   260,   444,     6,   262,
     447,   262,     4,   183,     3,     4,     7,     6,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     118,   119,    23,    24,    81,    82,    83,    84,    29,   690,
      31,    98,    99,   120,   121,   102,   103,     3,     4,   174,
       6,    42,    43,   178,   179,    25,   112,   113,   114,   115,
     116,   117,     3,    54,   501,   502,    98,    99,   505,    60,
      76,    77,    78,    79,   511,   246,   247,   248,    69,   250,
     261,     6,   263,   254,    75,    91,     6,     3,     4,    80,
       6,    61,    62,    63,    64,    65,    66,    67,    68,     6,
     144,   145,    93,   263,   264,    96,    97,   265,     6,   267,
     167,   167,    36,    37,   551,    39,   107,   154,   155,   110,
     167,     6,   559,   167,     6,     3,     4,   938,     6,   262,
     177,   568,   569,    76,    77,    78,    79,   261,     4,   263,
     184,   185,   186,   187,   188,   189,   190,   191,    61,    62,
      63,    64,    65,    66,    67,    68,   147,   594,   656,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   160,
     161,   162,   215,   216,   217,   218,   167,   261,     3,     4,
     227,     6,  1185,  1186,   175,   229,    95,     3,     4,   230,
       6,     4,  1003,    87,    88,    89,    90,   241,   261,     4,
     263,   245,   150,   151,   152,   153,   250,   251,   252,   253,
     261,   767,   263,     4,     4,   932,   257,     4,   264,   210,
    1396,   212,   213,   660,   661,     6,   261,   664,   665,   946,
     667,   192,   193,   194,   195,   196,   197,   198,   199,    87,
      88,    89,    90,    87,    88,    89,    90,   231,   232,   233,
     234,   261,     4,   263,   691,   692,    30,     6,   249,    33,
      34,   261,    36,    37,   974,     3,     4,   261,     6,   263,
     707,   261,    46,    47,    48,    49,   261,    51,    52,   261,
     261,   263,    56,    57,    58,   261,   538,   539,   540,   541,
     542,   543,   544,   545,   546,     3,     4,   223,     6,   225,
       6,   261,   261,   263,   261,     6,   261,   261,   745,   807,
     261,   261,   810,     6,   261,   261,   814,   261,    25,   261,
       4,   819,   261,   261,   261,   261,   261,   261,   261,    33,
      34,   261,    36,    37,  1051,     6,    25,    20,  1055,  1056,
     261,    19,    46,    47,    48,    49,    10,    51,    52,    31,
      42,    54,    43,    69,    93,    59,    75,    80,   264,   147,
      97,   107,   110,   175,   210,   212,    28,     4,   213,   249,
     261,    48,     6,     6,     6,     6,     6,   261,   162,    22,
       4,     6,     4,     4,     4,     4,     4,   264,   264,     4,
       4,   261,   166,   167,     4,     4,   265,    60,     6,    27,
     265,  1303,   265,   265,   261,     6,     4,   181,     4,     4,
       4,   848,     4,     4,     4,     4,   853,     4,   122,   123,
     124,     4,     6,   263,    27,     4,   264,     4,    28,     4,
       4,     4,     4,     4,     4,   111,   934,  1248,     4,     4,
     877,   939,   263,     4,     4,     4,   261,    25,   222,    25,
     224,   888,     6,    76,     6,     6,    25,     6,     4,     6,
       4,     6,   166,   167,     6,     6,     6,     6,     6,     6,
       4,   263,     4,     6,     4,     4,   250,     4,   182,   183,
       6,     6,  1192,     6,   261,     3,   261,     4,   261,   261,
     261,   261,   261,   261,   261,   261,   261,    28,     4,     4,
       4,   938,   264,    71,     4,     4,     4,     4,     4,     4,
      92,     4,     6,     4,     4,     4,     4,  1168,   222,     4,
     224,     4,     4,     4,     4,   265,   261,     6,     6,     6,
       6,     4,     4,   264,     4,     4,   261,   263,     6,     6,
       4,   105,     4,     4,     4,     4,     4,   146,   263,     6,
       4,    26,    26,  1270,     4,    26,   261,     6,   995,    25,
       4,     4,     4,   263,     4,     6,  1003,     6,     4,     6,
       4,  1008,     4,     4,     4,   226,   261,     6,     6,     6,
       6,     6,     4,     4,     4,     4,     4,     4,     4,   258,
     258,     6,     6,     6,     6,     6,     6,     4,     6,   265,
       6,     6,     6,     6,     4,    53,     4,    53,  1106,     6,
       6,  1321,     6,     4,     4,   261,     4,     6,   104,     4,
       4,   105,     4,   166,     4,    27,    27,     6,    27,     4,
       4,    26,     4,     6,   146,     6,     6,     4,     4,   265,
       4,     6,     4,     4,   166,     4,     6,   264,   264,   105,
       6,   264,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     4,     6,    56,  1101,     6,     6,     6,   261,     6,
       6,     6,     6,     4,     4,   263,     4,   263,     4,   264,
     261,     6,     6,     6,     6,     4,     6,   235,     6,     6,
       6,     6,     4,     4,    27,     3,     6,     6,  1196,     4,
       4,     4,   265,   265,     4,     6,  1517,   265,     6,     4,
       6,   261,     4,     6,    28,     6,     6,     6,     6,  1436,
       6,     6,     6,     6,     4,   261,   261,     4,     4,    28,
     106,     6,     6,     6,  1171,     4,     4,   265,     4,     4,
    1450,   265,     4,   264,     4,     6,    28,    28,   263,     4,
       6,   264,     4,   264,   263,   265,   264,   126,   265,     4,
       4,     4,   265,   265,   265,     6,     6,     6,     6,     4,
       4,     4,    26,   265,     6,     6,     4,     4,   243,     4,
       4,    27,   954,     6,     6,     6,     6,     6,     4,     6,
       4,     4,     4,     4,    30,     4,     6,     4,     6,  1170,
       6,     6,   265,     6,     6,     6,     6,  1244,   956,  1246,
     265,  1248,   265,   265,   265,   855,   239,   264,   596,   264,
     869,  1168,   164,   461,  1191,   263,  1324,   236,  1545,  1405,
     265,   827,   265,   883,  1105,  1199,    95,   601,  1279,   263,
     475,   901,   927,  1041,   503,   808,   936,    -1,    -1,    -1,
      -1,  1141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1539,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1510,    -1,    -1,    -1,    -1,    -1,    -1,
    1517,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1527
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,   159,   269,   270,   271,     0,     8,   272,     4,   170,
     171,     1,   273,   261,   261,   261,     7,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    23,
      24,    29,    31,    42,    43,    54,    60,    69,    75,    80,
      93,    96,    97,   107,   110,   147,   160,   161,   162,   167,
     175,   210,   212,   213,   249,   274,   275,   276,   278,   279,
     281,   283,   285,   286,   304,   305,   306,   307,   309,   311,
     314,   316,   317,   321,   322,   347,   357,   359,   365,   366,
     368,   369,   389,   390,   402,   403,   439,   440,   499,   535,
     537,   538,   555,   556,   557,   558,   573,   574,   599,   600,
     617,   618,   633,   634,   646,   647,   682,   683,   695,   696,
     713,   714,   724,   725,   739,   740,   759,   760,   277,     6,
     280,    94,   282,   284,   308,   310,   312,   315,     6,     6,
     172,   173,   360,   360,   348,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     9,     6,     6,     6,     6,     3,
       3,   287,     6,     6,     6,     6,     6,   318,   323,     6,
     370,   391,   404,   441,   500,   539,   559,   559,   575,   601,
     619,   635,   648,   686,   698,   716,   727,   262,   743,   762,
       4,   261,     4,    95,     4,     4,     4,     4,     4,   264,
     384,   387,   261,     6,     4,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     288,   261,   684,   261,   261,   261,   261,    21,    96,   319,
     320,    96,   262,   324,   346,   358,    96,   262,   371,   388,
      96,   262,   392,    96,   262,   405,   406,   407,   438,    96,
     262,   442,   443,   444,   497,    96,   443,   501,   536,    96,
     262,   540,   541,   554,    96,   262,   560,   561,   566,   572,
      96,   571,    96,   262,   576,   577,   598,    96,   262,   602,
     603,   616,    96,   262,   620,    96,   262,   636,   645,    96,
     262,   649,   650,   672,    96,   262,   685,   687,    96,   262,
     697,   699,   700,    96,   262,   715,   717,   718,    96,   262,
     726,   728,   731,   744,   742,    96,   262,   761,   763,   261,
     261,     6,   261,   261,     6,     6,     6,     6,   266,   385,
     387,    25,     4,     1,     9,    29,    41,    59,    70,    76,
      96,   125,   174,   178,   179,   289,     6,    20,    19,   325,
      25,    10,   372,    42,   393,    31,   408,   410,   409,    43,
     445,   455,    54,   502,    69,   542,   543,    93,    76,    77,
      78,    79,    91,   562,    75,    80,   578,   579,    97,   264,
     605,   147,   621,   107,   104,   108,   109,   180,   110,   651,
     655,   175,   688,   210,    28,   211,    30,   250,   711,   212,
      28,   721,   213,    28,   253,   734,     4,    96,   741,   743,
     249,    48,   261,     6,     6,     6,     6,   266,     6,   266,
     385,   386,     6,     6,   261,   290,   296,   293,   299,   294,
     291,   162,   297,   295,   292,   298,    22,     4,     6,     4,
       4,     4,   261,   263,   367,   411,   412,   413,   415,   418,
     420,   422,   425,   427,   431,   432,   435,   436,   437,     4,
     266,     4,    45,   446,   261,   263,   367,   456,   472,   474,
     261,   263,   367,   503,   504,   516,   518,   520,     4,     4,
     544,   545,   563,   564,   264,   264,   567,   569,     4,   261,
     263,   367,   581,   604,   261,   263,   367,   606,     4,   637,
     639,   640,   261,     4,   261,   263,   367,   656,     4,   701,
     703,   387,   712,   710,   719,    30,   250,   261,   722,   732,
     729,    30,   250,   261,   735,   745,    60,     6,    61,    62,
      63,    64,    65,    66,    67,    68,   313,   313,   313,   265,
     265,   265,   265,   386,   261,    27,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     6,   263,    27,
     373,   394,     4,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,   166,   167,   240,   260,     4,   387,   421,
     387,   447,   264,    33,    34,    36,    37,    46,    47,    48,
      49,    51,    52,    59,   122,   123,   124,   166,   182,   183,
     222,   224,   480,    28,   476,    30,    33,    34,    46,    47,
      48,    49,    51,    56,    57,    58,   166,   181,   222,   224,
     250,   480,   261,   263,   367,   546,     4,     4,   562,   565,
     565,     4,   261,   263,   570,    81,    82,    83,    84,   177,
     227,     4,    98,    99,   102,   103,   615,   148,   149,   623,
       4,     4,     4,   111,   652,   112,   113,   114,   115,   116,
     117,     4,     4,   263,   708,   387,   384,   261,   711,     4,
     387,   723,     4,     4,   387,   736,   746,   764,    25,    25,
      25,     6,   313,   163,   164,   165,   169,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   261,    76,     6,
     374,   387,   395,   416,   215,   216,   217,   218,   419,     6,
     414,   387,   433,   428,   423,     6,   387,   387,   313,   449,
       4,   176,   215,   216,   217,   218,   471,     6,   460,   192,
     193,   194,   195,   196,   197,   198,   199,   343,     6,    91,
     219,   220,   221,   527,     6,   461,   475,   465,     6,   468,
     462,   463,   459,   457,   477,   387,   511,   419,     6,   512,
     343,     6,   527,     6,   517,   519,   521,   513,   507,     6,
     509,   505,    41,    71,   166,     4,   265,   267,   265,   263,
     568,    87,    88,    89,    90,   582,   583,   584,   585,   586,
     587,     4,     6,   607,   100,   101,   622,     4,     4,   104,
     108,   641,   118,   119,   653,   657,   658,   659,   660,   661,
     662,   689,   702,    71,   174,   214,   259,   385,   720,   387,
     384,   733,   730,   387,   384,   261,   263,   747,   755,   384,
       6,     6,     6,   261,   349,     3,   302,   301,     4,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   326,
      28,   361,   261,   263,   367,   375,   382,   387,   387,   396,
       4,     4,   313,     4,     4,   429,   430,    71,   424,   426,
     387,   264,   450,   448,     4,     4,     4,     4,     4,   469,
     470,     4,   464,     6,   458,     4,   387,     4,     4,     4,
       4,     4,     4,   523,   524,     4,     4,   506,   548,   552,
     547,     4,   562,    92,     6,     6,     6,     6,     4,   589,
     593,     4,   264,   588,     4,   265,     4,     6,     6,   150,
     151,   152,   153,   624,   105,   261,   642,   644,   261,   120,
     121,   654,     4,     4,     4,     4,     4,     4,   690,   704,
     709,     6,   385,   722,   385,   263,   737,   263,   738,   385,
      28,   166,   242,   244,   253,   254,   385,    26,    26,    26,
      25,   350,   146,   526,   526,     4,   362,   261,    30,    49,
      50,   246,   247,   248,   250,   254,   387,   261,   263,   397,
       4,   417,     6,   387,   434,     3,     4,     6,   430,     6,
       4,     4,   125,   266,     4,   466,     3,     4,     6,   470,
       4,   481,   484,   481,   473,   387,     6,     6,     3,     4,
       6,   524,   508,   510,     4,   528,   531,     4,   387,   549,
      72,    73,    74,   553,   550,     4,     6,     4,   580,     4,
     590,     4,   594,   580,     4,   226,   597,   608,     6,     6,
       6,     6,     6,   625,   638,     4,     4,     4,     4,   668,
       4,     4,   668,   261,   263,   691,   263,   705,   706,     4,
     386,   385,   258,   735,   258,   384,   385,   748,   756,   752,
     750,   751,   386,     6,     6,     6,     6,   352,     6,     6,
     303,   303,   327,     4,   364,   376,   383,   379,     6,     6,
       6,   377,   380,   166,   228,     6,   313,     6,     4,   451,
     453,   452,   265,   673,   485,    53,   482,    36,    37,    39,
     478,   479,   387,   522,   514,   515,   384,   532,    53,   529,
     387,     6,     6,     6,     4,   551,   261,   591,   595,     4,
       6,   104,   609,   612,   261,   263,   626,     4,   105,   615,
      87,    88,    89,    90,   670,   671,    87,    88,    89,    90,
     669,   615,   670,   166,    58,   174,   212,   213,   214,   237,
     256,   386,   386,   386,     4,   757,     4,     4,     4,   261,
      27,    27,    27,    26,   261,   263,   353,     6,   328,   363,
     364,     4,     4,     4,     6,     6,     6,     4,     4,   398,
     223,   225,   401,     6,     4,     4,     4,   264,   467,   674,
     490,   483,   387,   146,   525,   528,   385,     6,   534,   530,
       3,     4,     6,   264,   592,   264,   596,   265,   613,   105,
     154,   155,     4,   643,   663,   665,     6,     6,     6,     6,
     671,     6,     6,     6,     6,   663,   692,     6,   707,     6,
     261,    56,     4,   758,     6,     6,     6,     6,     6,   166,
     261,   263,   329,   367,   436,   363,   387,     6,   378,   263,
     399,     6,   263,   367,   454,   454,   454,   675,   677,    48,
     157,   158,   264,   489,   491,   492,   494,   484,   313,     6,
     385,   495,   531,     4,     4,   264,     4,   610,   627,   628,
     261,     4,    19,   151,   152,   666,     6,     6,     6,     6,
     693,     4,     6,     3,     4,     6,     6,     6,     6,    27,
     351,   354,    28,    47,   144,   145,   184,   185,   186,   187,
     188,   189,   190,   191,   229,   241,   245,   250,   251,   252,
     253,   387,   387,     6,   384,   235,     4,   400,   126,   265,
     265,   265,     4,    36,    37,    52,    59,   183,   678,   681,
       6,   493,     6,   266,   486,     6,   386,   263,   489,   496,
       4,     4,     4,     4,   629,   629,   664,     6,     6,     4,
     694,   749,   261,   261,   261,     6,   355,   332,   343,     4,
       6,     6,     6,     6,     6,     6,     6,     6,   231,   232,
     233,   234,   342,   331,     3,   334,   330,   336,   313,   385,
       6,     3,     4,     6,     4,   680,   679,     4,     6,   266,
       6,   266,   487,    48,    55,   533,   265,   265,     4,   611,
     264,   630,   667,     6,     6,     3,     4,     6,   753,     6,
       4,   356,     4,    28,   344,   344,   344,   344,    28,    28,
     344,    28,     4,     4,     4,     4,   385,     6,   263,   676,
       4,   481,     6,   265,     6,   265,     6,   265,     6,   265,
       4,   488,   489,     6,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   230,   257,   498,   487,   264,   264,
     265,   106,   614,    31,   156,     4,    58,   238,   255,   754,
       3,     4,     6,   333,   345,   337,   338,   339,   335,   264,
     386,   263,   126,   265,   265,   265,   265,   265,    25,   313,
       4,     4,     6,   632,   631,     6,     6,     6,    58,   237,
     340,     4,     4,     4,     4,    58,   237,   341,     6,   243,
       6,     4,     4,     4,     4,     6,     6,   387,     6,     6,
     384,     6,   381,    26,   265,   265,   265,   265,   387,   385,
     265,     4,     6,   264,   264,   385,     4,    27,     4,     4,
     386,     4,     6,     4,     4,   263,     6,   265,   265,   236,
       6,     6,   263,   239,     6,     6,     6,     6
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 360 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 5:
#line 361 "def.y"
    {
        defVersionNum = convert_defname2num((yyvsp[-1].string));
        if (defVersionNum > 5.7) {
          char temp[120];
          sprintf(temp,
          "Def parser 5.7 does not support def file with version %s. Parser stops executions.",
                  (yyvsp[-1].string));
          defError(6503, temp);
          return 1;
        }
        if (defrVersionStrCbk) {
          CALLBACK(defrVersionStrCbk, defrVersionStrCbkType, (yyvsp[-1].string));
        } else if (defrVersionCbk) {
            CALLBACK(defrVersionCbk, defrVersionCbkType, defVersionNum);
        }
        if (defVersionNum > 5.3 && defVersionNum < 5.4)
          defIgnoreVersion = 1;
        if (defVersionNum < 5.6)     // default to false before 5.6
          names_case_sensitive = reader_case_sensitive;
        else
          names_case_sensitive = 1;
        hasVer = 1;
        doneDesign = 0;
    ;}
    break;

  case 7:
#line 388 "def.y"
    {
        if (defVersionNum < 5.6) {
          names_case_sensitive = 1;
          if (defrCaseSensitiveCbk)
            CALLBACK(defrCaseSensitiveCbk, defrCaseSensitiveCbkType,
                     names_case_sensitive); 
          hasNameCase = 1;
        } else
          if (defrCaseSensitiveCbk) /* write error only if cbk is set */
             if (caseSensitiveWarnings++ < defrCaseSensitiveWarnings)
               defWarning(7011, "NAMESCASESENSITIVE is obsoleted in version 5.6 and later.\nThe DEF parser will ignore this statement.");
      ;}
    break;

  case 8:
#line 401 "def.y"
    {
        if (defVersionNum < 5.6) {
          names_case_sensitive = 0;
          if (defrCaseSensitiveCbk)
            CALLBACK(defrCaseSensitiveCbk, defrCaseSensitiveCbkType,
                     names_case_sensitive);
          hasNameCase = 1;
        } else {
          if (defrCaseSensitiveCbk) { /* write error only if cbk is set */
            if (caseSensitiveWarnings++ < defrCaseSensitiveWarnings) {
              defError(6504, "Def parser version 5.7 and later does not support NAMESCASESENSITIVE OFF.\nEither remove this optional construct or set it to ON.");
              CHKERR();
            }
          }
        }
      ;}
    break;

  case 51:
#line 439 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 52:
#line 440 "def.y"
    {
            if (defrDesignCbk)
              CALLBACK(defrDesignCbk, defrDesignStartCbkType, (yyvsp[-1].string));
            hasDes = 1;
          ;}
    break;

  case 53:
#line 447 "def.y"
    {
            doneDesign = 1;
            if (defrDesignEndCbk)
              CALLBACK(defrDesignEndCbk, defrDesignEndCbkType, 0);
            // 11/16/2001 - pcr 408334
            // Return 1 if there is any errors during parsing
            if (errors)
                return 1;

            if (!hasVer)
              defWarning(7012, "VERSION is a required statement on DEF file.\nWithout VERSION defined, the DEF file is technically illegal.\nRefer the LEF/DEF Language Reference manual on how to define this statement.");
            if (!hasNameCase && defVersionNum < 5.6)
              defWarning(7013, "NAMESCASESENSITIVE is a required statement on DEF file with version 5.6 and earlier.\nWithout NAMESCASESENSITIVE defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
            if (!hasBusBit && defVersionNum < 5.6)
              defWarning(7014, "BUSBITCHARS is a required statement on DEF file with version 5.6 and earlier.\nWithout BUSBITCHARS defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
            if (!hasDivChar && defVersionNum < 5.6)
              defWarning(7015, "DIVIDERCHAR is a required statement on DEF file with version 5.6 and earlier.\nWithout DIVIDECHAR defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
            if (!hasDes)
              defWarning(7016, "DESIGN is a required statement on DEF file.");
            hasVer = 0;
            hasNameCase = 0;
            hasBusBit = 0;
            hasDivChar = 0;
            hasDes = 0;

            assertionWarnings = 0;
            blockageWarnings = 0;
            caseSensitiveWarnings = 0;
            componentWarnings = 0;
            constraintWarnings = 0;
            defaultCapWarnings = 0;
            gcellGridWarnings = 0;
            iOTimingWarnings = 0;
            netWarnings = 0;
            nonDefaultWarnings = 0;
            pinExtWarnings = 0;
            pinWarnings = 0;
            regionWarnings = 0;
            rowWarnings = 0;
            scanchainWarnings = 0;
            sNetWarnings = 0;
            stylesWarnings = 0;
            trackWarnings = 0;
            unitsWarnings = 0;
            versionWarnings = 0;
            viaWarnings = 0;
          ;}
    break;

  case 54:
#line 495 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 55:
#line 496 "def.y"
    { 
            if (defrTechnologyCbk)
              CALLBACK(defrTechnologyCbk, defrTechNameCbkType, (yyvsp[-1].string));
          ;}
    break;

  case 56:
#line 501 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 57:
#line 502 "def.y"
    { 
            if (defrArrayNameCbk)
              CALLBACK(defrArrayNameCbk, defrArrayNameCbkType, (yyvsp[-1].string));
          ;}
    break;

  case 58:
#line 507 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 59:
#line 508 "def.y"
    { 
            if (defrFloorPlanNameCbk)
              CALLBACK(defrFloorPlanNameCbk, defrFloorPlanNameCbkType, (yyvsp[-1].string));
          ;}
    break;

  case 60:
#line 514 "def.y"
    { 
            if (defrHistoryCbk)
              CALLBACK(defrHistoryCbk, defrHistoryCbkType, History_text);
          ;}
    break;

  case 61:
#line 520 "def.y"
    {
            parsing_property = 1;
            defInPropDef = 1;     /* set flag as inside propertydefinitions */
            if (defrPropDefStartCbk)
              CALLBACK(defrPropDefStartCbk, defrPropDefStartCbkType, 0);
          ;}
    break;

  case 62:
#line 527 "def.y"
    { 
            if (defrPropDefEndCbk)
              CALLBACK(defrPropDefEndCbk, defrPropDefEndCbkType, 0);
            real_num = 0;     /* just want to make sure it is reset */
            parsing_property = 0;
            defInPropDef = 0;     /* reset flag */
          ;}
    break;

  case 64:
#line 537 "def.y"
    { ;}
    break;

  case 65:
#line 539 "def.y"
    {dumb_mode = 1; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 66:
#line 541 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("design", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrDesignProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 67:
#line 548 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 68:
#line 550 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("net", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrNetProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 69:
#line 557 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 70:
#line 559 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("specialnet", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrSNetProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 71:
#line 566 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 72:
#line 568 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("region", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrRegionProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 73:
#line 575 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 74:
#line 577 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("group", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrGroupProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 75:
#line 584 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 76:
#line 586 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("component", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrCompProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 77:
#line 593 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 78:
#line 595 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("row", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrRowProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 79:
#line 602 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 80:
#line 604 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("pin", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrPinDefProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 81:
#line 612 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 82:
#line 614 "def.y"
    {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("componentpin", (yyvsp[-2].string));
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrCompPinProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
              }
            ;}
    break;

  case 83:
#line 622 "def.y"
    { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); ;}
    break;

  case 84:
#line 624 "def.y"
    {
              if (defVersionNum < 5.6) {
                if (nonDefaultWarnings++ < defrNonDefaultWarnings) {
                  defMsg = (char*)defMalloc(1000); 
                  sprintf (defMsg,
                     "NONDEFAULTRULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6505, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              } else {
                if (defrPropCbk) {
                  defrProp.defiProp::setPropType("nondefaultrule", (yyvsp[-2].string));
		  CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                  defrNDefProp.defiPropType::setPropType((yyvsp[-2].string), defPropDefType);
                }
              }
            ;}
    break;

  case 85:
#line 642 "def.y"
    { yyerrok; yyclearin;;}
    break;

  case 86:
#line 644 "def.y"
    { real_num = 0 ;}
    break;

  case 87:
#line 645 "def.y"
    {
              if (defrPropCbk) defrProp.defiProp::setPropInteger();
              defPropDefType = 'I';
            ;}
    break;

  case 88:
#line 649 "def.y"
    { real_num = 1 ;}
    break;

  case 89:
#line 650 "def.y"
    {
              if (defrPropCbk) defrProp.defiProp::setPropReal();
              defPropDefType = 'R';
              real_num = 0;
            ;}
    break;

  case 90:
#line 656 "def.y"
    {
              if (defrPropCbk) defrProp.defiProp::setPropString();
              defPropDefType = 'S';
            ;}
    break;

  case 91:
#line 661 "def.y"
    {
              if (defrPropCbk) defrProp.defiProp::setPropQString((yyvsp[0].string));
              defPropDefType = 'Q';
            ;}
    break;

  case 92:
#line 666 "def.y"
    {
              if (defrPropCbk) defrProp.defiProp::setPropNameMapString((yyvsp[0].string));
              defPropDefType = 'S';
            ;}
    break;

  case 94:
#line 673 "def.y"
    { if (defrPropCbk) defrProp.defiProp::setNumber((yyvsp[0].dval)); ;}
    break;

  case 95:
#line 676 "def.y"
    {
            if (defrUnitsCbk) {
              if (defValidNum((int)(yyvsp[-1].dval)))
                CALLBACK(defrUnitsCbk,  defrUnitsCbkType, (yyvsp[-1].dval));
            }
          ;}
    break;

  case 96:
#line 684 "def.y"
    {
            if (defrDividerCbk)
              CALLBACK(defrDividerCbk, defrDividerCbkType, (yyvsp[-1].string));
            hasDivChar = 1;
          ;}
    break;

  case 97:
#line 691 "def.y"
    { 
            if (defrBusBitCbk)
              CALLBACK(defrBusBitCbk, defrBusBitCbkType, (yyvsp[-1].string));
            hasBusBit = 1;
          ;}
    break;

  case 98:
#line 697 "def.y"
    { dumb_mode = 1; no_num = 1; defrSite.defiSite::clear(); ;}
    break;

  case 99:
#line 700 "def.y"
    {
            if (defrSiteCbk) {
              defrSite.defiSite::setName((yyvsp[-11].string));
              defrSite.defiSite::setLocation((yyvsp[-10].dval),(yyvsp[-9].dval));
              defrSite.defiSite::setOrient((yyvsp[-8].integer));
              defrSite.defiSite::setDo((yyvsp[-6].dval),(yyvsp[-4].dval),(yyvsp[-2].dval),(yyvsp[-1].dval));
	      CALLBACK(defrSiteCbk, defrSiteCbkType, &(defrSite));
            }
	  ;}
    break;

  case 100:
#line 710 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 101:
#line 712 "def.y"
    {
              if (defrCanplaceCbk) {
                defrCanplace.defiSite::setName((yyvsp[-11].string));
                defrCanplace.defiSite::setLocation((yyvsp[-10].dval),(yyvsp[-9].dval));
                defrCanplace.defiSite::setOrient((yyvsp[-8].integer));
                defrCanplace.defiSite::setDo((yyvsp[-6].dval),(yyvsp[-4].dval),(yyvsp[-2].dval),(yyvsp[-1].dval));
		CALLBACK(defrCanplaceCbk, defrCanplaceCbkType,
		&(defrCanplace));
              }
            ;}
    break;

  case 102:
#line 722 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 103:
#line 724 "def.y"
    {
              if (defrCannotOccupyCbk) {
                defrCannotOccupy.defiSite::setName((yyvsp[-11].string));
                defrCannotOccupy.defiSite::setLocation((yyvsp[-10].dval),(yyvsp[-9].dval));
                defrCannotOccupy.defiSite::setOrient((yyvsp[-8].integer));
                defrCannotOccupy.defiSite::setDo((yyvsp[-6].dval),(yyvsp[-4].dval),(yyvsp[-2].dval),(yyvsp[-1].dval));
		CALLBACK(defrCannotOccupyCbk, defrCannotOccupyCbkType,
                        &(defrCannotOccupy));
              }
            ;}
    break;

  case 104:
#line 735 "def.y"
    {(yyval.integer) = 0;;}
    break;

  case 105:
#line 736 "def.y"
    {(yyval.integer) = 1;;}
    break;

  case 106:
#line 737 "def.y"
    {(yyval.integer) = 2;;}
    break;

  case 107:
#line 738 "def.y"
    {(yyval.integer) = 3;;}
    break;

  case 108:
#line 739 "def.y"
    {(yyval.integer) = 4;;}
    break;

  case 109:
#line 740 "def.y"
    {(yyval.integer) = 5;;}
    break;

  case 110:
#line 741 "def.y"
    {(yyval.integer) = 6;;}
    break;

  case 111:
#line 742 "def.y"
    {(yyval.integer) = 7;;}
    break;

  case 112:
#line 745 "def.y"
    {
            if (!defrGeomPtr) {  // set up for more than 4 points
              defrGeomPtr =
                (defiGeometries*)defMalloc(sizeof(defiGeometries));
              defrGeomPtr->defiGeometries::Init();
            } else    // Just reset the number of points
              defrGeomPtr->defiGeometries::Reset();
	  ;}
    break;

  case 113:
#line 754 "def.y"
    {
	    if (defrDieAreaCbk) {
               defrDieArea.defiBox::addPoint(defrGeomPtr);
               CALLBACK(defrDieAreaCbk, defrDieAreaCbkType, &(defrDieArea));
            }
          ;}
    break;

  case 114:
#line 763 "def.y"
    { ;}
    break;

  case 115:
#line 766 "def.y"
    {
          if (defVersionNum < 5.4) {
             if (defrDefaultCapCbk)
                CALLBACK(defrDefaultCapCbk, defrDefaultCapCbkType, ROUND((yyvsp[0].dval)));
          } else {
             if (defrDefaultCapCbk) /* write error only if cbk is set */
                if (defaultCapWarnings++ < defrDefaultCapWarnings)
                   defWarning(7017, "DEFAULTCAP is obsoleted in version 5.4 and earlier.\nThe DEF parser will ignore this statement.");
          }
        ;}
    break;

  case 118:
#line 782 "def.y"
    {
            if (defVersionNum < 5.4) {
	      if (defrPinCapCbk) {
	        defrPinCap.defiPinCap::setPin(ROUND((yyvsp[-3].dval)));
	        defrPinCap.defiPinCap::setCap((yyvsp[-1].dval));
	        CALLBACK(defrPinCapCbk, defrPinCapCbkType, &(defrPinCap));
	      }
            }
	  ;}
    break;

  case 119:
#line 793 "def.y"
    { ;}
    break;

  case 120:
#line 796 "def.y"
    { ;}
    break;

  case 121:
#line 799 "def.y"
    { 
            if (defrStartPinsCbk)
              CALLBACK(defrStartPinsCbk, defrStartPinsCbkType, ROUND((yyvsp[-1].dval)));
          ;}
    break;

  case 124:
#line 808 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 125:
#line 809 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 126:
#line 810 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk) {
              defrPin.defiPin::Setup((yyvsp[-4].string), (yyvsp[0].string));
            }
            hasPort = 0;
          ;}
    break;

  case 127:
#line 817 "def.y"
    { 
            if (defrPinCbk)
              CALLBACK(defrPinCbk, defrPinCbkType, &defrPin);
          ;}
    break;

  case 130:
#line 826 "def.y"
    {
            if (defrPinCbk)
              defrPin.defiPin::setSpecial();
          ;}
    break;

  case 131:
#line 832 "def.y"
    { 
            if (defrPinExtCbk)
              CALLBACK(defrPinExtCbk, defrPinExtCbkType, History_text);
          ;}
    break;

  case 132:
#line 838 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::setDirection((yyvsp[0].string));
          ;}
    break;

  case 133:
#line 844 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "NETEXPR statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6506, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk)
                defrPin.defiPin::setNetExpr((yyvsp[0].string));

            }
          ;}
    break;

  case 134:
#line 864 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 135:
#line 865 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "SUPPLYSENSITIVITY statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6507, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk)
                defrPin.defiPin::setSupplySens((yyvsp[0].string));
            }
          ;}
    break;

  case 136:
#line 884 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 137:
#line 885 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "GROUNDSENSITIVITY statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6508, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk)
                defrPin.defiPin::setGroundSens((yyvsp[0].string));
            }
          ;}
    break;

  case 138:
#line 905 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk) defrPin.defiPin::setUse((yyvsp[0].string));
          ;}
    break;

  case 139:
#line 909 "def.y"
    {
            if (defVersionNum < 5.7) {
               if (defrPinCbk || defrPinExtCbk) {
                 if ((pinWarnings++ < defrPinWarnings) &&
                     (pinWarnings++ < defrPinExtWarnings)) {
                   defMsg = (char*)defMalloc(10000);
                   sprintf (defMsg,
                     "PORT in PINS is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
                   defError(6555, defMsg);
                   defFree(defMsg);
                   CHKERR();
                 }
               }
            } else {
               if (defrPinCbk || defrPinExtCbk)
                 defrPin.defiPin::addPort();
               hasPort = 1;
            }
          ;}
    break;

  case 140:
#line 929 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 141:
#line 930 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::addPortLayer((yyvsp[0].string));
              else
                 defrPin.defiPin::addLayer((yyvsp[0].string));
            }
          ;}
    break;

  case 142:
#line 939 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::addPortLayerPts((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
              else
                 defrPin.defiPin::addLayerPts((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
            }
          ;}
    break;

  case 143:
#line 948 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 144:
#line 949 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6509, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortPolygon((yyvsp[0].string));
                else
                   defrPin.defiPin::addPolygon((yyvsp[0].string));
              }
            }
            if (!defrGeomPtr) {
              defrGeomPtr =
                (defiGeometries*)defMalloc(sizeof(defiGeometries));
              defrGeomPtr->defiGeometries::Init();
            } else {  // Just reset the number of points
              defrGeomPtr->defiGeometries::Reset();
            }
          ;}
    break;

  case 145:
#line 979 "def.y"
    {
            if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortPolygonPts(defrGeomPtr);
                else
                   defrPin.defiPin::addPolygonPts(defrGeomPtr);
              }
            }
          ;}
    break;

  case 146:
#line 989 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 147:
#line 990 "def.y"
    {
            if (defVersionNum < 5.7) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "PIN VIA statement is a version 5.7 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6556, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortVia((yyvsp[-4].string), (yyvsp[-2].dval), (yyvsp[-1].dval));
                else
                   defrPin.defiPin::addVia((yyvsp[-4].string), (yyvsp[-2].dval), (yyvsp[-1].dval));
              }
            }
          ;}
    break;

  case 148:
#line 1014 "def.y"
    {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::setPortPlacement((yyvsp[-2].integer), (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
              else
                 defrPin.defiPin::setPlacement((yyvsp[-2].integer), (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
            }
          ;}
    break;

  case 149:
#line 1025 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
                  defError(6510, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinPartialMetalArea((int)(yyvsp[-1].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 150:
#line 1043 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6511, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinPartialMetalSideArea((int)(yyvsp[-1].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 151:
#line 1061 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINGATEAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6512, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
              if (defrPinCbk || defrPinExtCbk)
                defrPin.defiPin::addAPinGateArea((int)(yyvsp[-1].dval), (yyvsp[0].string)); 
            ;}
    break;

  case 152:
#line 1079 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINDIFFAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6513, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinDiffArea((int)(yyvsp[-1].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 153:
#line 1096 "def.y"
    {dumb_mode=1;;}
    break;

  case 154:
#line 1097 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINMAXAREACAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6514, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinMaxAreaCar((int)(yyvsp[-3].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 155:
#line 1114 "def.y"
    {dumb_mode=1;;}
    break;

  case 156:
#line 1116 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6515, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinMaxSideAreaCar((int)(yyvsp[-3].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 157:
#line 1134 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6516, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinPartialCutArea((int)(yyvsp[-1].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 158:
#line 1151 "def.y"
    {dumb_mode=1;;}
    break;

  case 159:
#line 1152 "def.y"
    {
            if (defVersionNum <= 5.3) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAPINMAXCUTCAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6517, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAPinMaxCutCar((int)(yyvsp[-3].dval), (yyvsp[0].string)); 
          ;}
    break;

  case 160:
#line 1170 "def.y"
    {  /* 5.5 syntax */
            if (defVersionNum < 5.5) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6518, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
          ;}
    break;

  case 162:
#line 1188 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6519, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortLayerSpacing((int)(yyvsp[0].dval));
                else
                   defrPin.defiPin::addLayerSpacing((int)(yyvsp[0].dval));
              }
            }
          ;}
    break;

  case 163:
#line 1211 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "DESIGNRULEWIDTH statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6520, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortLayerDesignRuleWidth((int)(yyvsp[0].dval));
                else
                   defrPin.defiPin::addLayerDesignRuleWidth((int)(yyvsp[0].dval));
              }
            }
          ;}
    break;

  case 165:
#line 1236 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6521, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortPolySpacing((int)(yyvsp[0].dval));
                else
                   defrPin.defiPin::addPolySpacing((int)(yyvsp[0].dval));
              }
            }
          ;}
    break;

  case 166:
#line 1259 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrPinCbk || defrPinExtCbk) {
                if ((pinWarnings++ < defrPinWarnings) &&
                    (pinWarnings++ < defrPinExtWarnings)) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "DESIGNRULEWIDTH statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6522, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            } else {
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortPolyDesignRuleWidth((int)(yyvsp[0].dval));
                else
                   defrPin.defiPin::addPolyDesignRuleWidth((int)(yyvsp[0].dval));
              }
            }
          ;}
    break;

  case 167:
#line 1283 "def.y"
    { aOxide = 1;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          ;}
    break;

  case 168:
#line 1288 "def.y"
    { aOxide = 2;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          ;}
    break;

  case 169:
#line 1293 "def.y"
    { aOxide = 3;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          ;}
    break;

  case 170:
#line 1298 "def.y"
    { aOxide = 4;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          ;}
    break;

  case 171:
#line 1304 "def.y"
    { (yyval.string) = (char*)"SIGNAL"; ;}
    break;

  case 172:
#line 1306 "def.y"
    { (yyval.string) = (char*)"POWER"; ;}
    break;

  case 173:
#line 1308 "def.y"
    { (yyval.string) = (char*)"GROUND"; ;}
    break;

  case 174:
#line 1310 "def.y"
    { (yyval.string) = (char*)"CLOCK"; ;}
    break;

  case 175:
#line 1312 "def.y"
    { (yyval.string) = (char*)"TIEOFF"; ;}
    break;

  case 176:
#line 1314 "def.y"
    { (yyval.string) = (char*)"ANALOG"; ;}
    break;

  case 177:
#line 1316 "def.y"
    { (yyval.string) = (char*)"SCAN"; ;}
    break;

  case 178:
#line 1318 "def.y"
    { (yyval.string) = (char*)"RESET"; ;}
    break;

  case 179:
#line 1322 "def.y"
    { (yyval.string) = (char*)""; ;}
    break;

  case 180:
#line 1323 "def.y"
    {dumb_mode=1;;}
    break;

  case 181:
#line 1324 "def.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 182:
#line 1327 "def.y"
    { 
          if (defrPinEndCbk)
            CALLBACK(defrPinEndCbk, defrPinEndCbkType, 0);
        ;}
    break;

  case 183:
#line 1332 "def.y"
    {dumb_mode = 2; no_num = 2; ;}
    break;

  case 184:
#line 1334 "def.y"
    {
          if (defrRowCbk) {
            rowName = (yyvsp[-4].string);
            defrRow.defiRow::setup((yyvsp[-4].string), (yyvsp[-3].string), (yyvsp[-2].dval), (yyvsp[-1].dval), (yyvsp[0].integer));
          }
        ;}
    break;

  case 185:
#line 1342 "def.y"
    {
          if (defrRowCbk) 
	    CALLBACK(defrRowCbk, defrRowCbkType, &defrRow);
        ;}
    break;

  case 186:
#line 1348 "def.y"
    {
          if (defVersionNum < 5.6) {
            if (defrRowCbk) {
              if (rowWarnings++ < defrRowWarnings) {
                defError(6523, "The DO statement which is required in ROW statement is missing.");
                CHKERR();
              }
            }
          }
        ;}
    break;

  case 187:
#line 1359 "def.y"
    {
          /* 06/05/2002 - pcr 448455 */
          /* Check for 1 and 0 in the correct position */
          /* 07/26/2002 - Commented out due to pcr 459218 */
          if (hasDoStep) {
            /* 04/29/2004 - pcr 695535 */
            /* changed the testing */
            if ((((yyvsp[-1].dval) == 1) && (yStep == 0)) ||
                (((yyvsp[-3].dval) == 1) && (xStep == 0))) {
              /* do nothing */
            } else 
              if (defVersionNum < 5.6) {
                if (defrRowCbk) {
                  if (rowWarnings++ < defrRowWarnings) {
                    defMsg = (char*)defMalloc(1000);
                    sprintf(defMsg,
                            "ROW %s: Both spaceX and spaceY values were specified. The correct syntax for ROW DO statement is \"DO numX BY 1 STEP spaceX 0 | DO 1 BY numY STEP 0 spaceY\". Only the space direction for the appropriate row orientation was retained.", rowName);
                    defWarning(7018, defMsg);
                    defFree(defMsg);
                    }
                  }
              }
          }
          /* pcr 459218 - Error if at least numX or numY does not equal 1 */
          if (((yyvsp[-3].dval) != 1) && ((yyvsp[-1].dval) != 1)) {
            if (defrRowCbk) {
              if (rowWarnings++ < defrRowWarnings) {
                defError(6524, "Syntax error, correct syntax is either \"DO 1 BY num... or DO numX BY 1...\"");
                CHKERR();
              }
            }
          }
          if (defrRowCbk)
            defrRow.defiRow::setDo(ROUND((yyvsp[-3].dval)), ROUND((yyvsp[-1].dval)), xStep, yStep);
        ;}
    break;

  case 188:
#line 1396 "def.y"
    {
          hasDoStep = 0;
        ;}
    break;

  case 189:
#line 1400 "def.y"
    {
          hasDoStep = 1;
          defrRow.defiRow::setHasDoStep();
          xStep = (yyvsp[-1].dval);
          yStep = (yyvsp[0].dval);
        ;}
    break;

  case 192:
#line 1411 "def.y"
    { dumb_mode = 10000000; parsing_property = 1;;}
    break;

  case 193:
#line 1413 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 196:
#line 1420 "def.y"
    {
          if (defrRowCbk) {
             char propTp;
             char* str = ringCopy("                       ");
             propTp =  defrRowProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "ROW");
             /* For backword compatibility, also set the string value */
             /* We will use a temporary string to store the number.
              * The string space is borrowed from the ring buffer
              * in the lexer. */
             sprintf(str, "%g", (yyvsp[0].dval));
             defrRow.defiRow::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
          }
        ;}
    break;

  case 197:
#line 1435 "def.y"
    {
          if (defrRowCbk) {
             char propTp;
             propTp =  defrRowProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "ROW");
             defrRow.defiRow::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 198:
#line 1444 "def.y"
    {
          if (defrRowCbk) {
             char propTp;
             propTp =  defrRowProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "ROW");
             defrRow.defiRow::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 199:
#line 1454 "def.y"
    {
          if (defrTrackCbk) {
            defrTrack.defiTrack::setup((yyvsp[-1].string));
          }
        ;}
    break;

  case 200:
#line 1460 "def.y"
    {
          if (((yyvsp[-4].dval) <= 0) && (defVersionNum >= 5.4)) {
            if (defrTrackCbk)
              if (trackWarnings++ < defrTrackWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The DO number %g in TRACK is invalid./nThe number value has to be greater than 0", (yyvsp[-4].dval));
                defError(6525, defMsg);
                defFree(defMsg);
              }
          }
          if ((yyvsp[-2].dval) < 0) {
            if (defrTrackCbk)
              if (trackWarnings++ < defrTrackWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The STEP number %g in TRACK is invalid./nThe number value has to be greater than 0", (yyvsp[-2].dval));
                defError(6526, defMsg);
                defFree(defMsg);
              }
          }
          if (defrTrackCbk) {
            defrTrack.defiTrack::setDo(ROUND((yyvsp[-7].dval)), ROUND((yyvsp[-4].dval)), (yyvsp[-2].dval));
	    CALLBACK(defrTrackCbk, defrTrackCbkType, &defrTrack);
          }
        ;}
    break;

  case 201:
#line 1488 "def.y"
    {
          (yyval.string) = (yyvsp[0].string);
        ;}
    break;

  case 202:
#line 1493 "def.y"
    { (yyval.string) = (char*)"X";;}
    break;

  case 203:
#line 1495 "def.y"
    { (yyval.string) = (char*)"Y";;}
    break;

  case 205:
#line 1498 "def.y"
    { dumb_mode = 1000; ;}
    break;

  case 206:
#line 1499 "def.y"
    { dumb_mode = 0; ;}
    break;

  case 209:
#line 1506 "def.y"
    {
          if (defrTrackCbk)
            defrTrack.defiTrack::addLayer((yyvsp[0].string));
        ;}
    break;

  case 210:
#line 1513 "def.y"
    {
          if ((yyvsp[-3].dval) <= 0) {
            if (defrGcellGridCbk)
              if (gcellGridWarnings++ < defrGcellGridWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The DO number %g in GCELLGRID is invalid./nThe number value has to be greater than 0", (yyvsp[-3].dval));
                defError(6527, defMsg);
                defFree(defMsg);
              }
          }
          if ((yyvsp[-1].dval) < 0) {
            if (defrGcellGridCbk)
              if (gcellGridWarnings++ < defrGcellGridWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The STEP number %g in GCELLGRID is invalid./nThe number value has to be greater than 0", (yyvsp[-1].dval));
                defError(6528, defMsg);
                defFree(defMsg);
              }
          }
	  if (defrGcellGridCbk) {
	    defrGcellGrid.defiGcellGrid::setup((yyvsp[-6].string), ROUND((yyvsp[-5].dval)), ROUND((yyvsp[-3].dval)), (yyvsp[-1].dval));
	    CALLBACK(defrGcellGridCbk, defrGcellGridCbkType, &defrGcellGrid);
	  }
	;}
    break;

  case 211:
#line 1541 "def.y"
    {
          if (defrExtensionCbk)
             CALLBACK(defrExtensionCbk, defrExtensionCbkType, History_text);
        ;}
    break;

  case 212:
#line 1547 "def.y"
    { ;}
    break;

  case 214:
#line 1553 "def.y"
    {
          if (defrViaStartCbk)
            CALLBACK(defrViaStartCbk, defrViaStartCbkType, ROUND((yyvsp[-1].dval)));
        ;}
    break;

  case 217:
#line 1562 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 218:
#line 1563 "def.y"
    {
              if (defrViaCbk) defrVia.defiVia::setup((yyvsp[0].string));
              viaRule = 0;
            ;}
    break;

  case 219:
#line 1568 "def.y"
    {
              if (defrViaCbk)
                CALLBACK(defrViaCbk, defrViaCbkType, &defrVia);
              defrVia.defiVia::clear();
            ;}
    break;

  case 222:
#line 1578 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 223:
#line 1579 "def.y"
    {
              if (defrViaCbk)
                defrVia.defiVia::addLayer((yyvsp[-2].string), (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
            ;}
    break;

  case 224:
#line 1583 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 225:
#line 1584 "def.y"
    {
              if (defVersionNum < 5.6) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defMsg = (char*)defMalloc(1000);
                    sprintf (defMsg,
                       "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                    defError(6529, defMsg);
                    defFree(defMsg);
                    CHKERR();
                  }
                }
              }
              if (!defrGeomPtr) {
                defrGeomPtr =
                  (defiGeometries*)defMalloc(sizeof(defiGeometries));
                defrGeomPtr->defiGeometries::Init();
              } else {  // Just reset the number of points
                defrGeomPtr->defiGeometries::Reset();
              }
            ;}
    break;

  case 226:
#line 1606 "def.y"
    {
              if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
                if (defrViaCbk)
                  defrVia.defiVia::addPolygon((yyvsp[-5].string), defrGeomPtr);
              }
            ;}
    break;

  case 227:
#line 1612 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 228:
#line 1613 "def.y"
    {
              if (defVersionNum < 5.6) {
                if (defrViaCbk)
                  defrVia.defiVia::addPattern((yyvsp[0].string));
              } else
                if (defrViaCbk)
                  if (viaWarnings++ < defrViaWarnings)
                    defWarning(7019, "PATTERNNAME is obsoleted in version 5.6 and later.\nDEF parser will ignore this statement."); 
            ;}
    break;

  case 229:
#line 1622 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 230:
#line 1624 "def.y"
    {dumb_mode = 3;no_num = 1; ;}
    break;

  case 231:
#line 1627 "def.y"
    {
               viaRule = 1;
               if (defVersionNum < 5.6) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defMsg = (char*)defMalloc(1000);
                    sprintf (defMsg,
                       "VIARULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                    defError(6529, defMsg);
                    defFree(defMsg);
                    CHKERR();
                  }
                }
              } else {
                if (defrViaCbk)
                   defrVia.defiVia::addViaRule((yyvsp[-20].string), (int)(yyvsp[-17].dval), (int)(yyvsp[-16].dval), (yyvsp[-12].string), (yyvsp[-11].string),
                                             (yyvsp[-10].string), (int)(yyvsp[-7].dval), (int)(yyvsp[-6].dval), (int)(yyvsp[-3].dval),
                                             (int)(yyvsp[-2].dval), (int)(yyvsp[-1].dval), (int)(yyvsp[0].dval)); 
              }
            ;}
    break;

  case 233:
#line 1649 "def.y"
    { 
            if (defrViaExtCbk)
              CALLBACK(defrViaExtCbk, defrViaExtCbkType, History_text);
          ;}
    break;

  case 234:
#line 1655 "def.y"
    {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6530, "The ROWCOL statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addRowCol((int)(yyvsp[-1].dval), (int)(yyvsp[0].dval));
            ;}
    break;

  case 235:
#line 1667 "def.y"
    {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6531, "The ORIGIN statement has to  be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addOrigin((int)(yyvsp[-1].dval), (int)(yyvsp[0].dval));
            ;}
    break;

  case 236:
#line 1679 "def.y"
    {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6532, " The OFFSET statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addOffset((int)(yyvsp[-3].dval), (int)(yyvsp[-2].dval), (int)(yyvsp[-1].dval), (int)(yyvsp[0].dval));
            ;}
    break;

  case 237:
#line 1690 "def.y"
    {dumb_mode = 1;no_num = 1; ;}
    break;

  case 238:
#line 1691 "def.y"
    {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6533, " The PATTERN statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addCutPattern((yyvsp[0].string));
            ;}
    break;

  case 239:
#line 1704 "def.y"
    { defrGeomPtr->defiGeometries::startList((yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 240:
#line 1707 "def.y"
    { defrGeomPtr->defiGeometries::addToList((yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 243:
#line 1714 "def.y"
    {
            save_x = (yyvsp[-2].dval);
            save_y = (yyvsp[-1].dval);
            (yyval.pt).x = ROUND((yyvsp[-2].dval));
            (yyval.pt).y = ROUND((yyvsp[-1].dval));
          ;}
    break;

  case 244:
#line 1721 "def.y"
    {
            save_y = (yyvsp[-1].dval);
            (yyval.pt).x = ROUND(save_x);
            (yyval.pt).y = ROUND((yyvsp[-1].dval));
          ;}
    break;

  case 245:
#line 1727 "def.y"
    {
            save_x = (yyvsp[-2].dval);
            (yyval.pt).x = ROUND((yyvsp[-2].dval));
            (yyval.pt).y = ROUND(save_y);
          ;}
    break;

  case 246:
#line 1733 "def.y"
    {
            (yyval.pt).x = ROUND(save_x);
            (yyval.pt).y = ROUND(save_y);
          ;}
    break;

  case 247:
#line 1739 "def.y"
    { 
          if (defrViaEndCbk)
            CALLBACK(defrViaEndCbk, defrViaEndCbkType, 0);
        ;}
    break;

  case 248:
#line 1745 "def.y"
    {
          if (defrRegionEndCbk)
            CALLBACK(defrRegionEndCbk, defrRegionEndCbkType, 0);
        ;}
    break;

  case 249:
#line 1751 "def.y"
    {
          if (defrRegionStartCbk)
            CALLBACK(defrRegionStartCbk, defrRegionStartCbkType, ROUND((yyvsp[-1].dval)));
        ;}
    break;

  case 251:
#line 1758 "def.y"
    {;}
    break;

  case 252:
#line 1760 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 253:
#line 1761 "def.y"
    {
          if (defrRegionCbk)
             defrRegion.defiRegion::setup((yyvsp[0].string));
          regTypeDef = 0;
        ;}
    break;

  case 254:
#line 1767 "def.y"
    { CALLBACK(defrRegionCbk, defrRegionCbkType, &defrRegion); ;}
    break;

  case 255:
#line 1771 "def.y"
    { if (defrRegionCbk)
	  defrRegion.defiRegion::addRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 256:
#line 1774 "def.y"
    { if (defrRegionCbk)
	  defrRegion.defiRegion::addRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y); ;}
    break;

  case 259:
#line 1782 "def.y"
    { dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 260:
#line 1784 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 261:
#line 1786 "def.y"
    {
           if (regTypeDef) {
              if (defrRegionCbk) {
                if (regionWarnings++ < defrRegionWarnings) {
                  defError(6534, "The TYPE statement has already defined in the REGION statement");
                  CHKERR();
                }
              }
           }
           if (defrRegionCbk) defrRegion.defiRegion::setType((yyvsp[0].string));
           regTypeDef = 1;
         ;}
    break;

  case 264:
#line 1805 "def.y"
    {
          if (defrRegionCbk) {
             char propTp;
             char* str = ringCopy("                       ");
             propTp = defrRegionProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "REGION");
             /* For backword compatibility, also set the string value */
             /* We will use a temporary string to store the number.
              * The string space is borrowed from the ring buffer
              * in the lexer. */
             sprintf(str, "%g", (yyvsp[0].dval));
             defrRegion.defiRegion::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
          }
        ;}
    break;

  case 265:
#line 1820 "def.y"
    {
          if (defrRegionCbk) {
             char propTp;
             propTp = defrRegionProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "REGION");
             defrRegion.defiRegion::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 266:
#line 1829 "def.y"
    {
          if (defrRegionCbk) {
             char propTp;
             propTp = defrRegionProp.defiPropType::propType((yyvsp[-1].string));
             CHKPROPTYPE(propTp, (yyvsp[-1].string), "REGION");
             defrRegion.defiRegion::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 267:
#line 1839 "def.y"
    { (yyval.string) = (char*)"FENCE"; ;}
    break;

  case 268:
#line 1841 "def.y"
    { (yyval.string) = (char*)"GUIDE"; ;}
    break;

  case 270:
#line 1847 "def.y"
    { 
            if (defrComponentStartCbk)
              CALLBACK(defrComponentStartCbk, defrComponentStartCbkType,
                       ROUND((yyvsp[-1].dval)));
         ;}
    break;

  case 273:
#line 1858 "def.y"
    {
            if (defrComponentCbk)
              CALLBACK(defrComponentCbk, defrComponentCbkType, &defrComponent);
         ;}
    break;

  case 274:
#line 1864 "def.y"
    {
            dumb_mode = 0;
            no_num = 0;
            /* The net connections were added to the array in
             * reverse order so fix the order now */
            /* Do not need to reverse the order since the storing is
               in sequence now  08/07/2001
            if (defrComponentCbk)
              defrComponent.defiComponent::reverseNetOrder();
            */
         ;}
    break;

  case 275:
#line 1876 "def.y"
    {dumb_mode = 1000000000; no_num = 10000000; ;}
    break;

  case 276:
#line 1878 "def.y"
    {
            if (defrComponentCbk)
              defrComponent.defiComponent::IdAndName((yyvsp[-1].string), (yyvsp[0].string));
         ;}
    break;

  case 277:
#line 1884 "def.y"
    { ;}
    break;

  case 278:
#line 1886 "def.y"
    {
              if (defrComponentCbk)
                defrComponent.defiComponent::addNet("*");
            ;}
    break;

  case 279:
#line 1891 "def.y"
    {
              if (defrComponentCbk)
                defrComponent.defiComponent::addNet((yyvsp[0].string));
            ;}
    break;

  case 293:
#line 1906 "def.y"
    {
          if (defrComponentCbk)
            CALLBACK(defrComponentExtCbk, defrComponentExtCbkType,
                     History_text);
        ;}
    break;

  case 294:
#line 1912 "def.y"
    {dumb_mode=1; no_num = 1; ;}
    break;

  case 295:
#line 1913 "def.y"
    {
          if (defrComponentCbk)
            defrComponent.defiComponent::setEEQ((yyvsp[0].string));
        ;}
    break;

  case 296:
#line 1918 "def.y"
    { dumb_mode = 2;  no_num = 2; ;}
    break;

  case 297:
#line 1920 "def.y"
    {
          if (defrComponentCbk)
             defrComponent.defiComponent::setGenerate((yyvsp[-1].string), (yyvsp[0].string));
        ;}
    break;

  case 298:
#line 1926 "def.y"
    { (yyval.string) = (char*)""; ;}
    break;

  case 299:
#line 1928 "def.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 300:
#line 1931 "def.y"
    {
          if (defrComponentCbk)
            defrComponent.defiComponent::setSource((yyvsp[0].string));
        ;}
    break;

  case 301:
#line 1937 "def.y"
    { (yyval.string) = (char*)"NETLIST"; ;}
    break;

  case 302:
#line 1939 "def.y"
    { (yyval.string) = (char*)"DIST"; ;}
    break;

  case 303:
#line 1941 "def.y"
    { (yyval.string) = (char*)"USER"; ;}
    break;

  case 304:
#line 1943 "def.y"
    { (yyval.string) = (char*)"TIMING"; ;}
    break;

  case 305:
#line 1948 "def.y"
    { ;}
    break;

  case 306:
#line 1950 "def.y"
    {
	  if (defrComponentCbk)
	    defrComponent.defiComponent::setRegionName((yyvsp[0].string));
	;}
    break;

  case 307:
#line 1956 "def.y"
    { 
          /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
          if (defVersionNum < 5.5) {
            if (defrComponentCbk)
	       defrComponent.defiComponent::setRegionBounds((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, 
                                                            (yyvsp[0].pt).x, (yyvsp[0].pt).y);
          }
          else
            defWarning(7020, "REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
	;}
    break;

  case 308:
#line 1967 "def.y"
    { 
          /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
          if (defVersionNum < 5.5) {
	    if (defrComponentCbk)
	       defrComponent.defiComponent::setRegionBounds((yyvsp[-1].pt).x, (yyvsp[-1].pt).y,
                                                            (yyvsp[0].pt).x, (yyvsp[0].pt).y);
          }
          else
            defWarning(7020, "REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
	;}
    break;

  case 309:
#line 1979 "def.y"
    {
          if (defVersionNum < 5.6) {
             if (defrComponentCbk) {
               if (componentWarnings++ < defrComponentWarnings) {
                 defMsg = (char*)defMalloc(1000);
                 sprintf (defMsg,
                    "HALO statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                 defError(6529, defMsg);
                 defFree(defMsg);
                 CHKERR();
               }
             }
          }
        ;}
    break;

  case 310:
#line 1994 "def.y"
    {
          if (defrComponentCbk)
            defrComponent.defiComponent::setHalo((int)(yyvsp[-3].dval), (int)(yyvsp[-2].dval),
                                                 (int)(yyvsp[-1].dval), (int)(yyvsp[0].dval));
        ;}
    break;

  case 312:
#line 2002 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrComponentCbk) {
             if (componentWarnings++ < defrComponentWarnings) {
                defMsg = (char*)defMalloc(10000);
                sprintf (defMsg,
                  "HALO SOFT is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
                defError(6550, defMsg);
                defFree(defMsg);
                CHKERR();
             }
           }
        } else {
           if (defrComponentCbk)
             defrComponent.defiComponent::setHaloSoft();
        }
      ;}
    break;

  case 313:
#line 2021 "def.y"
    { dumb_mode = 2; no_num = 2; ;}
    break;

  case 314:
#line 2022 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrComponentCbk) {
             if (componentWarnings++ < defrComponentWarnings) {
                defMsg = (char*)defMalloc(10000);
                sprintf (defMsg,
                  "ROUTEHALO is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
                defError(6551, defMsg);
                defFree(defMsg);
                CHKERR();
             }
           }
        } else {
           if (defrComponentCbk)
             defrComponent.defiComponent::setRouteHalo((yyvsp[-3].dval), (yyvsp[-1].string), (yyvsp[0].string));
        }
      ;}
    break;

  case 315:
#line 2040 "def.y"
    { dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 316:
#line 2042 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 319:
#line 2049 "def.y"
    {
          if (defrComponentCbk) {
            char propTp;
            char* str = ringCopy("                       ");
            propTp = defrCompProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "COMPONENT");
            /* For backword compatibility, also set the string value */
            /* We will use a temporary string to store the number.
             * The string space is borrowed from the ring buffer
             * in the lexer. */
            sprintf(str, "%g", (yyvsp[0].dval));
            defrComponent.defiComponent::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
          }
        ;}
    break;

  case 320:
#line 2064 "def.y"
    {
          if (defrComponentCbk) {
            char propTp;
            propTp = defrCompProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "COMPONENT");
            defrComponent.defiComponent::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 321:
#line 2073 "def.y"
    {
          if (defrComponentCbk) {
            char propTp;
            propTp = defrCompProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "COMPONENT");
            defrComponent.defiComponent::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 322:
#line 2083 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 323:
#line 2085 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 324:
#line 2087 "def.y"
    { 
          if (defVersionNum < 5.6) {
            if (defrComponentCbk) {
              defrComponent.defiComponent::setForeignName((yyvsp[-2].string));
              defrComponent.defiComponent::setForeignLocation((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
            }
         } else
            if (defrComponentCbk)
              if (componentWarnings++ < defrComponentWarnings)
                defWarning(7021, "FOREIGN is obsoleted in version 5.6 and later.\nThe DEF parser will ignore this statement.");
         ;}
    break;

  case 325:
#line 2101 "def.y"
    { (yyval.pt) = (yyvsp[0].pt); ;}
    break;

  case 326:
#line 2103 "def.y"
    { (yyval.pt).x = ROUND((yyvsp[-1].dval)); (yyval.pt).y = ROUND((yyvsp[0].dval)); ;}
    break;

  case 327:
#line 2106 "def.y"
    {
          if (defrComponentCbk) {
            defrComponent.defiComponent::setPlacementStatus((yyvsp[-2].integer));
            defrComponent.defiComponent::setPlacementLocation((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
          }
        ;}
    break;

  case 328:
#line 2113 "def.y"
    {
          if (defrComponentCbk)
            defrComponent.defiComponent::setPlacementStatus(
                                         DEFI_COMPONENT_UNPLACED);
            defrComponent.defiComponent::setPlacementLocation(-1, -1, -1);
        ;}
    break;

  case 329:
#line 2120 "def.y"
    {
          if (defVersionNum < 5.4) {   /* PCR 495463 */
            if (defrComponentCbk) {
              defrComponent.defiComponent::setPlacementStatus(
                                          DEFI_COMPONENT_UNPLACED);
              defrComponent.defiComponent::setPlacementLocation((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
            }
          } else {
            if (componentWarnings++ < defrComponentWarnings)
               defWarning(7022, "In the COMPONENT UNPLACED statement, point & orient are invalid in version 5.4 and later.\nDEF parser will ignore this statement.");
            if (defrComponentCbk)
              defrComponent.defiComponent::setPlacementStatus(
                                           DEFI_COMPONENT_UNPLACED);
              defrComponent.defiComponent::setPlacementLocation((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer));
          }
        ;}
    break;

  case 330:
#line 2138 "def.y"
    { (yyval.integer) = DEFI_COMPONENT_FIXED; ;}
    break;

  case 331:
#line 2140 "def.y"
    { (yyval.integer) = DEFI_COMPONENT_COVER; ;}
    break;

  case 332:
#line 2142 "def.y"
    { (yyval.integer) = DEFI_COMPONENT_PLACED; ;}
    break;

  case 333:
#line 2145 "def.y"
    {
          if (defrComponentCbk)
            defrComponent.defiComponent::setWeight(ROUND((yyvsp[0].dval)));
        ;}
    break;

  case 334:
#line 2151 "def.y"
    { 
          if (defrComponentCbk)
            CALLBACK(defrComponentEndCbk, defrComponentEndCbkType, 0);
        ;}
    break;

  case 336:
#line 2160 "def.y"
    { 
          if (defrNetStartCbk)
            CALLBACK(defrNetStartCbk, defrNetStartCbkType, ROUND((yyvsp[-1].dval)));
          netOsnet = 1;
        ;}
    break;

  case 339:
#line 2171 "def.y"
    { 
          if (defrNetCbk)
            CALLBACK(defrNetCbk, defrNetCbkType, &defrNet);
        ;}
    break;

  case 340:
#line 2182 "def.y"
    {dumb_mode = 0; no_num = 0; ;}
    break;

  case 341:
#line 2185 "def.y"
    {dumb_mode = 1000000000; no_num = 10000000; nondef_is_keyword = TRUE; mustjoin_is_keyword = TRUE;;}
    break;

  case 343:
#line 2188 "def.y"
    {
          /* 9/22/1999 */
          /* this is shared by both net and special net */
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::setName((yyvsp[0].string));
          if (defrNetNameCbk)
            CALLBACK(defrNetNameCbk, defrNetNameCbkType, (yyvsp[0].string));
        ;}
    break;

  case 345:
#line 2196 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 346:
#line 2197 "def.y"
    {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addMustPin((yyvsp[-3].string), (yyvsp[-1].string), 0);
          dumb_mode = 3;
          no_num = 3;
        ;}
    break;

  case 349:
#line 2208 "def.y"
    {dumb_mode = 1000000000; no_num = 10000000;;}
    break;

  case 350:
#line 2210 "def.y"
    {
          /* 9/22/1999 */
          /* since the code is shared by both net & special net, */
          /* need to check on both flags */
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin((yyvsp[-4].string), (yyvsp[-2].string), (yyvsp[-1].integer));
          /* 1/14/2000 - pcr 289156 */
          /* reset dumb_mode & no_num to 3 , just in case */
          /* the next statement is another net_connection */
          dumb_mode = 3;
          no_num = 3;
        ;}
    break;

  case 351:
#line 2222 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 352:
#line 2223 "def.y"
    {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin("*", (yyvsp[-2].string), (yyvsp[-1].integer));
          dumb_mode = 3;
          no_num = 3;
        ;}
    break;

  case 353:
#line 2229 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 354:
#line 2230 "def.y"
    {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin("PIN", (yyvsp[-2].string), (yyvsp[-1].integer));
          dumb_mode = 3;
          no_num = 3;
        ;}
    break;

  case 355:
#line 2238 "def.y"
    { (yyval.integer) = 0; ;}
    break;

  case 356:
#line 2240 "def.y"
    {
          if (defrNetConnectionExtCbk)
	    CALLBACK(defrNetConnectionExtCbk, defrNetConnectionExtCbkType,
              History_text);
          (yyval.integer) = 0;
        ;}
    break;

  case 357:
#line 2247 "def.y"
    { (yyval.integer) = 1; ;}
    break;

  case 360:
#line 2254 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 361:
#line 2255 "def.y"
    {  
          if (defrNetCbk) defrNet.defiNet::addWire((yyvsp[-1].string), NULL);
        ;}
    break;

  case 362:
#line 2259 "def.y"
    {
          by_is_keyword = FALSE;
          do_is_keyword = FALSE;
          new_is_keyword = FALSE;
          nondef_is_keyword = FALSE;
          mustjoin_is_keyword = FALSE;
          step_is_keyword = FALSE;
          orient_is_keyword = FALSE;
          needNPCbk = 0;
        ;}
    break;

  case 363:
#line 2271 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setSource((yyvsp[0].string)); ;}
    break;

  case 364:
#line 2274 "def.y"
    {
          if (defVersionNum < 5.5) {
            if (defrNetCbk) {
              if (netWarnings++ < defrNetWarnings) {
                 defMsg = (char*)defMalloc(1000);
                 sprintf (defMsg,
                    "FIXEDBUMP statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                 defError(6530, defMsg);
                 defFree(defMsg);
                 CHKERR();
              }
            }
          }
          if (defrNetCbk) defrNet.defiNet::setFixedbump();
        ;}
    break;

  case 365:
#line 2290 "def.y"
    { real_num = 1; ;}
    break;

  case 366:
#line 2291 "def.y"
    {
          if (defVersionNum < 5.5) {
            if (defrNetCbk) {
              if (netWarnings++ < defrNetWarnings) {
                 defMsg = (char*)defMalloc(1000);
                 sprintf (defMsg,
                    "FREQUENCY statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                 defError(6530, defMsg);
                 defFree(defMsg);
                 CHKERR();
              }
            }
          }
          if (defrNetCbk) defrNet.defiNet::setFrequency((yyvsp[0].dval));
          real_num = 0;
        ;}
    break;

  case 367:
#line 2308 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 368:
#line 2309 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setOriginal((yyvsp[0].string)); ;}
    break;

  case 369:
#line 2312 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setPattern((yyvsp[0].string)); ;}
    break;

  case 370:
#line 2315 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setWeight(ROUND((yyvsp[0].dval))); ;}
    break;

  case 371:
#line 2318 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setXTalk(ROUND((yyvsp[0].dval))); ;}
    break;

  case 372:
#line 2321 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setCap((yyvsp[0].dval)); ;}
    break;

  case 373:
#line 2324 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setUse((yyvsp[0].string)); ;}
    break;

  case 374:
#line 2327 "def.y"
    { if (defrNetCbk) defrNet.defiNet::setStyle((int)(yyvsp[0].dval)); ;}
    break;

  case 375:
#line 2329 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 376:
#line 2330 "def.y"
    { 
          if (defrNetCbk && defrNetNonDefaultRuleCbk) {
             /* User wants a callback on nondefaultrule */
             CALLBACK(defrNetNonDefaultRuleCbk,
                      defrNetNonDefaultRuleCbkType, (yyvsp[0].string));
          }
          /* Still save data in the class */
          if (defrNetCbk) defrNet.defiNet::setNonDefaultRule((yyvsp[0].string));
        ;}
    break;

  case 378:
#line 2342 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 379:
#line 2343 "def.y"
    { if (defrNetCbk) defrNet.defiNet::addShieldNet((yyvsp[0].string)); ;}
    break;

  case 380:
#line 2345 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 381:
#line 2346 "def.y"
    { /* since the parser still support 5.3 and earlier, can't */
          /* move NOSHIELD in net_type */
          if (defVersionNum < 5.4) {   /* PCR 445209 */
            if (defrNetCbk) defrNet.defiNet::addNoShield("");
            by_is_keyword = FALSE;
            do_is_keyword = FALSE;
            new_is_keyword = FALSE;
            step_is_keyword = FALSE;
            orient_is_keyword = FALSE;
            shield = TRUE;    /* save the path info in the shield paths */
          } else
            if (defrNetCbk) defrNet.defiNet::addWire("NOSHIELD", NULL);
        ;}
    break;

  case 382:
#line 2360 "def.y"
    {
          if (defVersionNum < 5.4) {   /* PCR 445209 */
            shield = FALSE;
            by_is_keyword = FALSE;
            do_is_keyword = FALSE;
            new_is_keyword = FALSE;
            step_is_keyword = FALSE;
            nondef_is_keyword = FALSE;
            mustjoin_is_keyword = FALSE;
            orient_is_keyword = FALSE;
          } else {
            by_is_keyword = FALSE;
            do_is_keyword = FALSE;
            new_is_keyword = FALSE;
            step_is_keyword = FALSE;
            nondef_is_keyword = FALSE;
            mustjoin_is_keyword = FALSE;
            orient_is_keyword = FALSE;
          }
          needNPCbk = 0;
        ;}
    break;

  case 383:
#line 2383 "def.y"
    { dumb_mode = 1; no_num = 1;
          if (defrNetCbk) {
            defrSubnet = (defiSubnet*)defMalloc(sizeof(defiSubnet));
            defrSubnet->defiSubnet::Init();
          }
        ;}
    break;

  case 384:
#line 2389 "def.y"
    {
          if (defrNetCbk && defrNetSubnetNameCbk) {
            /* User wants a callback on Net subnetName */
            CALLBACK(defrNetSubnetNameCbk, defrNetSubnetNameCbkType, (yyvsp[0].string));
          }
          /* Still save the subnet name in the class */
          if (defrNetCbk) {
            defrSubnet->defiSubnet::setName((yyvsp[0].string));
          }
        ;}
    break;

  case 385:
#line 2399 "def.y"
    {
          routed_is_keyword = TRUE;
          fixed_is_keyword = TRUE;
          cover_is_keyword = TRUE;
        ;}
    break;

  case 386:
#line 2403 "def.y"
    {
          if (defrNetCbk) {
            defrNet.defiNet::addSubnet(defrSubnet);
            defrSubnet = 0;
            routed_is_keyword = FALSE;
            fixed_is_keyword = FALSE;
            cover_is_keyword = FALSE;
          }
        ;}
    break;

  case 387:
#line 2413 "def.y"
    {dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 388:
#line 2415 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 389:
#line 2418 "def.y"
    { 
          if (defrNetExtCbk)
            CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text);
        ;}
    break;

  case 392:
#line 2428 "def.y"
    {
          if (defrNetCbk) {
            char propTp;
            char* str = ringCopy("                       ");
            propTp = defrNetProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "NET");
            /* For backword compatibility, also set the string value */
            /* We will use a temporary string to store the number.
             * The string space is borrowed from the ring buffer
             * in the lexer. */
            sprintf(str, "%g", (yyvsp[0].dval));
            defrNet.defiNet::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
          }
        ;}
    break;

  case 393:
#line 2443 "def.y"
    {
          if (defrNetCbk) {
            char propTp;
            propTp = defrNetProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "NET");
            defrNet.defiNet::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 394:
#line 2452 "def.y"
    {
          if (defrNetCbk) {
            char propTp;
            propTp = defrNetProp.defiPropType::propType((yyvsp[-1].string));
            CHKPROPTYPE(propTp, (yyvsp[-1].string), "NET");
            defrNet.defiNet::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
          }
        ;}
    break;

  case 395:
#line 2462 "def.y"
    { (yyval.string) = (char*)"NETLIST"; ;}
    break;

  case 396:
#line 2464 "def.y"
    { (yyval.string) = (char*)"DIST"; ;}
    break;

  case 397:
#line 2466 "def.y"
    { (yyval.string) = (char*)"USER"; ;}
    break;

  case 398:
#line 2468 "def.y"
    { (yyval.string) = (char*)"TIMING"; ;}
    break;

  case 399:
#line 2470 "def.y"
    { (yyval.string) = (char*)"TEST"; ;}
    break;

  case 400:
#line 2473 "def.y"
    {
          /* vpin_options may have to deal with orient */
          orient_is_keyword = TRUE;
        ;}
    break;

  case 401:
#line 2478 "def.y"
    { if (defrNetCbk)
            defrNet.defiNet::addVpinBounds((yyvsp[-3].pt).x, (yyvsp[-3].pt).y, (yyvsp[-2].pt).x, (yyvsp[-2].pt).y);
          orient_is_keyword = FALSE;
        ;}
    break;

  case 402:
#line 2483 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 403:
#line 2484 "def.y"
    { if (defrNetCbk) defrNet.defiNet::addVpin((yyvsp[0].string)); ;}
    break;

  case 405:
#line 2487 "def.y"
    {dumb_mode=1;;}
    break;

  case 406:
#line 2488 "def.y"
    { if (defrNetCbk) defrNet.defiNet::addVpinLayer((yyvsp[0].string)); ;}
    break;

  case 408:
#line 2492 "def.y"
    { if (defrNetCbk) defrNet.defiNet::addVpinLoc((yyvsp[-2].string), (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].integer)); ;}
    break;

  case 409:
#line 2495 "def.y"
    { (yyval.string) = (char*)"PLACED"; ;}
    break;

  case 410:
#line 2497 "def.y"
    { (yyval.string) = (char*)"FIXED"; ;}
    break;

  case 411:
#line 2499 "def.y"
    { (yyval.string) = (char*)"COVER"; ;}
    break;

  case 412:
#line 2502 "def.y"
    { (yyval.string) = (char*)"FIXED"; ;}
    break;

  case 413:
#line 2504 "def.y"
    { (yyval.string) = (char*)"COVER"; ;}
    break;

  case 414:
#line 2506 "def.y"
    { (yyval.string) = (char*)"ROUTED"; ;}
    break;

  case 415:
#line 2510 "def.y"
    { if (defrNeedPathData && defrNetCbk)
          pathIsDone(shield, 0, netOsnet, &needNPCbk);
      ;}
    break;

  case 416:
#line 2514 "def.y"
    { ;}
    break;

  case 417:
#line 2516 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 418:
#line 2517 "def.y"
    { if (defrNeedPathData && defrNetCbk)
          pathIsDone(shield, 0, netOsnet, &needNPCbk);
      ;}
    break;

  case 419:
#line 2522 "def.y"
    {
        /* 9/26/2002 - pcr 449514
        Check if $1 is equal to TAPER or TAPERRULE, layername
        is missing */
        if ((strcmp((yyvsp[0].string), "TAPER") == 0) || (strcmp((yyvsp[0].string), "TAPERRULE") == 0)) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defError(6531, "layerName which is required in path is missing");
              CHKERR();
            }
          }
          /* Since there is already error, the next token is insignificant */
          dumb_mode = 1; no_num = 1;
        } else {
          if (defrNeedPathData) defrPath->defiPath::addLayer((yyvsp[0].string));
          dumb_mode = 0; no_num = 0;
        }
      ;}
    break;

  case 420:
#line 2541 "def.y"
    { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
/*
       dumb_mode = 1; by_is_keyword = TRUE; do_is_keyword = TRUE;
*/
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; ;}
    break;

  case 421:
#line 2548 "def.y"
    { dumb_mode = 0; ;}
    break;

  case 424:
#line 2556 "def.y"
    {
        if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
          if (strcmp((yyvsp[0].string), "TAPER") == 0)
            defrPath->defiPath::setTaper();
          else 
            defrPath->defiPath::addVia((yyvsp[0].string));
        }
      ;}
    break;

  case 425:
#line 2565 "def.y"
    { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
            defrPath->defiPath::addVia((yyvsp[-1].string));
            defrPath->defiPath::addViaRotation((yyvsp[0].integer));
        }
      ;}
    break;

  case 426:
#line 2571 "def.y"
    {
        if (defVersionNum < 5.5) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "VIA DO statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6532, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        }
        if (((yyvsp[-5].dval) == 0) || ((yyvsp[-3].dval) == 0)) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defError(6533, "Either the numX or numY in the VIA DO statement has the value 0.\nThis is illegal.");
              CHKERR();
            }
          }
        }
        if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
            defrPath->defiPath::addVia((yyvsp[-7].string));
            defrPath->defiPath::addViaData((int)(yyvsp[-5].dval), (int)(yyvsp[-3].dval), (int)(yyvsp[-1].dval), (int)(yyvsp[0].dval));
        }
      ;}
    break;

  case 427:
#line 2598 "def.y"
    { // reset dumb_mode to 1 just incase the next token is a via of the path
        // 2/5/2004 - pcr 686781
        dumb_mode = 100000; by_is_keyword = TRUE; do_is_keyword = TRUE;
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; ;}
    break;

  case 428:
#line 2607 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND((yyvsp[-2].dval)), ROUND((yyvsp[-1].dval))); 
	save_x = (yyvsp[-2].dval);
	save_y = (yyvsp[-1].dval);
      ;}
    break;

  case 429:
#line 2614 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND(save_x), ROUND((yyvsp[-1].dval))); 
	save_y = (yyvsp[-1].dval);
      ;}
    break;

  case 430:
#line 2620 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND((yyvsp[-2].dval)), ROUND(save_y)); 
	save_x = (yyvsp[-2].dval);
      ;}
    break;

  case 431:
#line 2626 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND(save_x), ROUND(save_y)); 
      ;}
    break;

  case 432:
#line 2631 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND((yyvsp[-3].dval)), ROUND((yyvsp[-2].dval)), ROUND((yyvsp[-1].dval))); 
	save_x = (yyvsp[-3].dval);
	save_y = (yyvsp[-2].dval);
      ;}
    break;

  case 433:
#line 2638 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND(save_x), ROUND((yyvsp[-2].dval)),
	  ROUND((yyvsp[-1].dval))); 
	save_y = (yyvsp[-2].dval);
      ;}
    break;

  case 434:
#line 2645 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND((yyvsp[-3].dval)), ROUND(save_y),
	  ROUND((yyvsp[-1].dval))); 
	save_x = (yyvsp[-3].dval);
      ;}
    break;

  case 435:
#line 2652 "def.y"
    {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND(save_x), ROUND(save_y),
	  ROUND((yyvsp[-1].dval))); 
      ;}
    break;

  case 440:
#line 2666 "def.y"
    { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) defrPath->defiPath::setTaper(); ;}
    break;

  case 441:
#line 2667 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 442:
#line 2668 "def.y"
    { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) defrPath->defiPath::addTaperRule((yyvsp[0].string)); ;}
    break;

  case 443:
#line 2671 "def.y"
    { 
        if (defVersionNum < 5.6) {
           if (defrNeedPathData) {
             if (netWarnings++ < defrNetWarnings) {
               defMsg = (char*)defMalloc(1000);
               sprintf (defMsg,
                  "STYLE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
               defError(6534, defMsg);
               defFree(defMsg);
               CHKERR();
             }
           }
        } else
           if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
             defrPath->defiPath::addStyle((int)(yyvsp[0].dval));
      ;}
    break;

  case 446:
#line 2694 "def.y"
    { if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
          defrPath->defiPath::addShape((yyvsp[0].string)); ;}
    break;

  case 447:
#line 2697 "def.y"
    { if (defVersionNum < 5.6) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "STYLE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6534, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        } else {
          if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
            defrPath->defiPath::addStyle((int)(yyvsp[0].dval));
        }
      ;}
    break;

  case 448:
#line 2715 "def.y"
    { 
            CALLBACK(defrNetEndCbk, defrNetEndCbkType, 0);
            netOsnet = 0;
          ;}
    break;

  case 449:
#line 2721 "def.y"
    { (yyval.string) = (char*)"RING"; ;}
    break;

  case 450:
#line 2723 "def.y"
    { (yyval.string) = (char*)"STRIPE"; ;}
    break;

  case 451:
#line 2725 "def.y"
    { (yyval.string) = (char*)"FOLLOWPIN"; ;}
    break;

  case 452:
#line 2727 "def.y"
    { (yyval.string) = (char*)"IOWIRE"; ;}
    break;

  case 453:
#line 2729 "def.y"
    { (yyval.string) = (char*)"COREWIRE"; ;}
    break;

  case 454:
#line 2731 "def.y"
    { (yyval.string) = (char*)"BLOCKWIRE"; ;}
    break;

  case 455:
#line 2733 "def.y"
    { (yyval.string) = (char*)"FILLWIRE"; ;}
    break;

  case 456:
#line 2735 "def.y"
    {
              if (defVersionNum < 5.7) {
                 if (defrNeedPathData) {
                   if (fillWarnings++ < defrFillWarnings) {
                     defMsg = (char*)defMalloc(10000);
                     sprintf (defMsg,
                       "FILLWIREOPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
                     defError(6552, defMsg);
                     defFree(defMsg);
                     CHKERR();
                  }
                }
              }
              (yyval.string) = (char*)"FILLWIREOPC";
            ;}
    break;

  case 457:
#line 2751 "def.y"
    { (yyval.string) = (char*)"DRCFILL"; ;}
    break;

  case 458:
#line 2753 "def.y"
    { (yyval.string) = (char*)"BLOCKAGEWIRE"; ;}
    break;

  case 459:
#line 2755 "def.y"
    { (yyval.string) = (char*)"PADRING"; ;}
    break;

  case 460:
#line 2757 "def.y"
    { (yyval.string) = (char*)"BLOCKRING"; ;}
    break;

  case 464:
#line 2767 "def.y"
    { CALLBACK(defrSNetCbk, defrSNetCbkType, &defrNet); ;}
    break;

  case 471:
#line 2778 "def.y"
    {
             if (defrSNetCbk) {   /* PCR 902306 */
               defMsg = (char*)defMalloc(1024);
               sprintf(defMsg, "The SPECIAL NET statement, with type %s, does not have any routine statement defined.\nThe DEF parser will ignore this statemnet.", (yyvsp[0].string));
               defWarning(7023, defMsg);
               defFree(defMsg);
             }
            ;}
    break;

  case 472:
#line 2786 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 473:
#line 2787 "def.y"
    {
            if (defrSNetCbk) defrNet.defiNet::addWire((yyvsp[-1].string), NULL);
            ;}
    break;

  case 474:
#line 2791 "def.y"
    {
            // 7/17/2003 - Fix for pcr 604848, add a callback for each wire
            if (defrSNetWireCbk) {
               CALLBACK(defrSNetWireCbk, defrSNetWireCbkType, &defrNet);
               defrNet.defiNet::freeWire();
            }
            by_is_keyword = FALSE;
            do_is_keyword = FALSE;
            new_is_keyword = FALSE;
            step_is_keyword = FALSE;
            orient_is_keyword = FALSE;
            needSNPCbk = 0;
            ;}
    break;

  case 475:
#line 2805 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 476:
#line 2806 "def.y"
    { shieldName = (yyvsp[0].string); ;}
    break;

  case 478:
#line 2809 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 479:
#line 2810 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrSNetCbk) {
                if (sNetWarnings++ < defrSNetWarnings) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6535, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (!defrGeomPtr) {
              defrGeomPtr =
                (defiGeometries*)defMalloc(sizeof(defiGeometries));
              defrGeomPtr->defiGeometries::Init();
            } else {  // Just reset the number of points
              defrGeomPtr->defiGeometries::Reset();
            }
          ;}
    break;

  case 480:
#line 2832 "def.y"
    {
            if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
              if (defrSNetCbk) {
                // needSNPCbk will indicate that it has reach the max
                // memory and if user has set partialPathCBk, def parser
                // will make the callback.
                // This will improve performance
                // This construct is only in specialnet
                defrNet.defiNet::addPolygon((yyvsp[-5].string), defrGeomPtr, &needSNPCbk);
                if (needSNPCbk && defrSNetPartialPathCbk) {
                   CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                            &defrNet);
                   defrNet.defiNet::clearRectPolyNPath();
                }
              }
            }
          ;}
    break;

  case 481:
#line 2850 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 482:
#line 2851 "def.y"
    {
            if (defVersionNum < 5.6) {
              if (defrSNetCbk) {
                if (sNetWarnings++ < defrSNetWarnings) {
                  defMsg = (char*)defMalloc(1000);
                  sprintf (defMsg,
                     "RECT statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
                  defError(6536, defMsg);
                  defFree(defMsg);
                  CHKERR();
                }
              }
            }
            if (defrSNetCbk) {
              // needSNPCbk will indicate that it has reach the max
              // memory and if user has set partialPathCBk, def parser
              // will make the callback.
              // This will improve performance
              // This construct is only in specialnet
              defrNet.defiNet::addRect((yyvsp[-2].string), (yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y, &needSNPCbk);
              if (needSNPCbk && defrSNetPartialPathCbk) {
                 CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                          &defrNet);
                 defrNet.defiNet::clearRectPolyNPath();
              }
            }
          ;}
    break;

  case 483:
#line 2880 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setSource((yyvsp[0].string)); ;}
    break;

  case 484:
#line 2883 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setFixedbump(); ;}
    break;

  case 485:
#line 2886 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setFrequency((yyvsp[0].dval)); ;}
    break;

  case 486:
#line 2888 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 487:
#line 2889 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setOriginal((yyvsp[0].string)); ;}
    break;

  case 488:
#line 2892 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setPattern((yyvsp[0].string)); ;}
    break;

  case 489:
#line 2895 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setWeight(ROUND((yyvsp[0].dval))); ;}
    break;

  case 490:
#line 2898 "def.y"
    { 
              /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
              if (defVersionNum < 5.5)
                 if (defrSNetCbk) defrNet.defiNet::setCap((yyvsp[0].dval));
              else
                 defWarning(7024, "ESTCAP is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
            ;}
    break;

  case 491:
#line 2907 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setUse((yyvsp[0].string)); ;}
    break;

  case 492:
#line 2910 "def.y"
    { if (defrSNetCbk) defrNet.defiNet::setStyle((int)(yyvsp[0].dval)); ;}
    break;

  case 493:
#line 2912 "def.y"
    {dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 494:
#line 2914 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 495:
#line 2917 "def.y"
    { CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text); ;}
    break;

  case 496:
#line 2920 "def.y"
    {
             if (defrSNetCbk) {
               defMsg = (char*)defMalloc(1024);
               sprintf(defMsg, "The SPECIAL NET SHIELD statement, does not have any routine statement defined.\nThe DEF parser will ignore this statemnet.");
               defWarning(7025, defMsg);
               defFree(defMsg);
             }
            ;}
    break;

  case 497:
#line 2929 "def.y"
    { /* since the parser still supports 5.3 and earlier, */
              /* can't just move SHIELD in net_type */
              if (defVersionNum < 5.4) { /* PCR 445209 */
                if (defrSNetCbk) defrNet.defiNet::addShield(shieldName);
                by_is_keyword = FALSE;
                do_is_keyword = FALSE;
                new_is_keyword = FALSE;
                step_is_keyword = FALSE;
                orient_is_keyword = FALSE;
                shield = TRUE;   /* save the path info in the shield paths */
              } else
                if (defrSNetCbk) defrNet.defiNet::addWire("SHIELD", shieldName);
            ;}
    break;

  case 498:
#line 2943 "def.y"
    {
              // 7/17/2003 - Fix for pcr 604848, add a callback for each wire
              if (defrSNetWireCbk) {
                 CALLBACK(defrSNetWireCbk, defrSNetWireCbkType, &defrNet);
                 if (defVersionNum < 5.4)
                   defrNet.defiNet::freeShield();
                 else
                   defrNet.defiNet::freeWire();
              }
              if (defVersionNum < 5.4) {  /* PCR 445209 */
                shield = FALSE;
                by_is_keyword = FALSE;
                do_is_keyword = FALSE;
                new_is_keyword = FALSE;
                step_is_keyword = FALSE;
                nondef_is_keyword = FALSE;
                mustjoin_is_keyword = FALSE;
                orient_is_keyword = FALSE;
              } else {
                by_is_keyword = FALSE;
                do_is_keyword = FALSE;
                new_is_keyword = FALSE;
                step_is_keyword = FALSE;
                orient_is_keyword = FALSE;
              }
              needSNPCbk = 0;
            ;}
    break;

  case 499:
#line 2971 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 500:
#line 2972 "def.y"
    {
              /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
              if (defVersionNum < 5.5)
                 if (defrSNetCbk) defrNet.defiNet::setWidth((yyvsp[-1].string), (yyvsp[0].dval));
              else
                 defWarning(7026, "WIDTH is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
            ;}
    break;

  case 501:
#line 2980 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 502:
#line 2981 "def.y"
    {
              if (numIsInt((yyvsp[0].string))) {
                 if (defrSNetCbk) defrNet.defiNet::setVoltage(atoi((yyvsp[0].string)));
              } else {
                 if (defrSNetCbk) {
                   if (sNetWarnings++ < defrSNetWarnings) {
                     defMsg = (char*)defMalloc(1000);
                     sprintf (defMsg,
                        "The value %s for statement VOLTAGE is not an integer.\nThe syntax requires an integer in units of millivolts", (yyvsp[0].string));
                     defError(6537, defMsg);
                     defFree(defMsg);
                     CHKERR();
                   }
                 }
              }
            ;}
    break;

  case 503:
#line 2998 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 504:
#line 2999 "def.y"
    {
              if (defrSNetCbk) defrNet.defiNet::setSpacing((yyvsp[-1].string),(yyvsp[0].dval));
            ;}
    break;

  case 505:
#line 3003 "def.y"
    {
            ;}
    break;

  case 508:
#line 3011 "def.y"
    {
              if (defrSNetCbk) {
                char propTp;
                char* str = ringCopy("                       ");
                propTp = defrSNetProp.defiPropType::propType((yyvsp[-1].string));
                CHKPROPTYPE(propTp, (yyvsp[-1].string), "SPECIAL NET");
                /* For backword compatibility, also set the string value */
                /* We will use a temporary string to store the number.
                 * The string space is borrowed from the ring buffer
                 * in the lexer. */
                sprintf(str, "%g", (yyvsp[0].dval));
                defrNet.defiNet::addNumProp((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
              }
            ;}
    break;

  case 509:
#line 3026 "def.y"
    {
              if (defrSNetCbk) {
                char propTp;
                propTp = defrSNetProp.defiPropType::propType((yyvsp[-1].string));
                CHKPROPTYPE(propTp, (yyvsp[-1].string), "SPECIAL NET");
                defrNet.defiNet::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
              }
            ;}
    break;

  case 510:
#line 3035 "def.y"
    {
              if (defrSNetCbk) {
                char propTp;
                propTp = defrSNetProp.defiPropType::propType((yyvsp[-1].string));
                CHKPROPTYPE(propTp, (yyvsp[-1].string), "SPECIAL NET");
                defrNet.defiNet::addProp((yyvsp[-1].string), (yyvsp[0].string), propTp);
              }
            ;}
    break;

  case 512:
#line 3046 "def.y"
    {
              if (defrSNetCbk) defrNet.defiNet::setRange((yyvsp[-1].dval),(yyvsp[0].dval));
            ;}
    break;

  case 514:
#line 3052 "def.y"
    { defrProp.defiProp::setRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 515:
#line 3055 "def.y"
    { (yyval.string) = (char*)"BALANCED"; ;}
    break;

  case 516:
#line 3057 "def.y"
    { (yyval.string) = (char*)"STEINER"; ;}
    break;

  case 517:
#line 3059 "def.y"
    { (yyval.string) = (char*)"TRUNK"; ;}
    break;

  case 518:
#line 3061 "def.y"
    { (yyval.string) = (char*)"WIREDLOGIC"; ;}
    break;

  case 519:
#line 3065 "def.y"
    { 
        if (defrNeedPathData && defrSNetCbk) {
           if (needSNPCbk && defrSNetPartialPathCbk) { 
              // require a callback before proceed because needSNPCbk must be
              // set to 1 from the previous pathIsDone and user has registered
              // a callback routine.
              CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                       &defrNet);
              needSNPCbk = 0;   // reset the flag
              pathIsDone(shield, 1, netOsnet, &needSNPCbk);
           } else
              pathIsDone(shield, 0, netOsnet, &needSNPCbk);
        }
      ;}
    break;

  case 520:
#line 3080 "def.y"
    { ;}
    break;

  case 521:
#line 3082 "def.y"
    { dumb_mode = 1; ;}
    break;

  case 522:
#line 3083 "def.y"
    { if (defrNeedPathData && defrSNetCbk) {
           if (needSNPCbk && defrSNetPartialPathCbk) {
              // require a callback before proceed because needSNPCbk must be
              // set to 1 from the previous pathIsDone and user has registered
              // a callback routine.
              CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                       &defrNet);
              needSNPCbk = 0;   // reset the flag
              pathIsDone(shield, 1, netOsnet, &needSNPCbk);
              // reset any poly or rect in special wiring statement
              defrNet.defiNet::clearRectPolyNPath();
           } else
              pathIsDone(shield, 0, netOsnet, &needSNPCbk);
        }
      ;}
    break;

  case 523:
#line 3100 "def.y"
    { if (defrNeedPathData && defrSNetCbk) defrPath->defiPath::addLayer((yyvsp[0].string));
        dumb_mode = 0; no_num = 0;
      ;}
    break;

  case 524:
#line 3107 "def.y"
    { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; ;}
    break;

  case 525:
#line 3112 "def.y"
    { dumb_mode = 0; ;}
    break;

  case 526:
#line 3115 "def.y"
    { if (defrNeedPathData && defrSNetCbk)
          defrPath->defiPath::addWidth(ROUND((yyvsp[0].dval)));
      ;}
    break;

  case 527:
#line 3120 "def.y"
    { 
        if (defrSNetStartCbk)
          CALLBACK(defrSNetStartCbk, defrSNetStartCbkType, ROUND((yyvsp[-1].dval)));
        netOsnet = 2;
      ;}
    break;

  case 528:
#line 3127 "def.y"
    { 
        if (defrSNetEndCbk)
          CALLBACK(defrSNetEndCbk, defrSNetEndCbkType, 0);
        netOsnet = 0;
      ;}
    break;

  case 530:
#line 3137 "def.y"
    {
        if (defrGroupsStartCbk)
           CALLBACK(defrGroupsStartCbk, defrGroupsStartCbkType, ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 533:
#line 3147 "def.y"
    {
        if (defrGroupCbk)
           CALLBACK(defrGroupCbk, defrGroupCbkType, &defrGroup);
      ;}
    break;

  case 534:
#line 3152 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 535:
#line 3153 "def.y"
    {
        dumb_mode = 1000000000;
        no_num = 1000000000;
        /* dumb_mode is automatically turned off at the first
         * + in the options or at the ; at the end of the group */
        if (defrGroupCbk) defrGroup.defiGroup::setup((yyvsp[0].string));
        if (defrGroupNameCbk)
           CALLBACK(defrGroupNameCbk, defrGroupNameCbkType, (yyvsp[0].string));
      ;}
    break;

  case 537:
#line 3165 "def.y"
    {  ;}
    break;

  case 538:
#line 3168 "def.y"
    {
        /* if (defrGroupCbk) defrGroup.defiGroup::addMember($1); */
        if (defrGroupMemberCbk)
          CALLBACK(defrGroupMemberCbk, defrGroupMemberCbkType, (yyvsp[0].string));
      ;}
    break;

  case 541:
#line 3179 "def.y"
    { ;}
    break;

  case 542:
#line 3180 "def.y"
    { dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 543:
#line 3182 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 544:
#line 3183 "def.y"
    { dumb_mode = 1;  no_num = 1; ;}
    break;

  case 545:
#line 3184 "def.y"
    { ;}
    break;

  case 546:
#line 3186 "def.y"
    { 
        if (defrGroupMemberCbk)
          CALLBACK(defrGroupExtCbk, defrGroupExtCbkType, History_text);
      ;}
    break;

  case 547:
#line 3192 "def.y"
    {
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5) {
          if (defrGroupCbk)
            defrGroup.defiGroup::addRegionRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
        }
        else
          defWarning(7027, "GROUP REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      ;}
    break;

  case 548:
#line 3202 "def.y"
    { if (defrGroupCbk)
          defrGroup.defiGroup::setRegionName((yyvsp[0].string));
      ;}
    break;

  case 551:
#line 3211 "def.y"
    {
        if (defrGroupCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrGroupProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "GROUP");
          /* For backword compatibility, also set the string value */
          /* We will use a temporary string to store the number.
           * The string space is borrowed from the ring buffer
           * in the lexer. */
          sprintf(str, "%g", (yyvsp[0].dval));
          defrGroup.defiGroup::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
        }
      ;}
    break;

  case 552:
#line 3226 "def.y"
    {
        if (defrGroupCbk) {
          char propTp;
          propTp = defrGroupProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "GROUP");
          defrGroup.defiGroup::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 553:
#line 3235 "def.y"
    {
        if (defrGroupCbk) {
          char propTp;
          propTp = defrGroupProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "GROUP");
          defrGroup.defiGroup::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 555:
#line 3246 "def.y"
    { ;}
    break;

  case 556:
#line 3249 "def.y"
    {
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setMaxX(ROUND((yyvsp[0].dval)));
        else
          defWarning(7028, "GROUP SOFT MAXX is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      ;}
    break;

  case 557:
#line 3257 "def.y"
    { 
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setMaxY(ROUND((yyvsp[0].dval)));
        else
          defWarning(7029, "GROUP SOFT MAXY is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      ;}
    break;

  case 558:
#line 3265 "def.y"
    { 
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setPerim(ROUND((yyvsp[0].dval)));
        else
          defWarning(7030, "GROUP SOFT MAXHALFPERIMETER is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      ;}
    break;

  case 559:
#line 3274 "def.y"
    { 
        if (defrGroupsEndCbk)
          CALLBACK(defrGroupsEndCbk, defrGroupsEndCbkType, 0);
      ;}
    break;

  case 562:
#line 3288 "def.y"
    {
        if ((defVersionNum < 5.4) && (defrAssertionsStartCbk)) {
          CALLBACK(defrAssertionsStartCbk, defrAssertionsStartCbkType,
	           ROUND((yyvsp[-1].dval)));
        } else {
          if (defrAssertionCbk)
            if (assertionWarnings++ < defrAssertionWarnings)
              defWarning(7031, "ASSERTIONS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
        if (defrAssertionCbk)
          defrAssertion.defiAssertion::setAssertionMode();
      ;}
    break;

  case 563:
#line 3302 "def.y"
    {
        if ((defVersionNum < 5.4) && (defrConstraintsStartCbk)) {
          CALLBACK(defrConstraintsStartCbk, defrConstraintsStartCbkType,
                   ROUND((yyvsp[-1].dval)));
        } else {
          if (defrConstraintCbk)
            if (constraintWarnings++ < defrConstraintWarnings)
              defWarning(7032, "CONSTRAINTS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
        if (defrConstraintCbk)
          defrAssertion.defiAssertion::setConstraintMode();
      ;}
    break;

  case 567:
#line 3321 "def.y"
    {
        if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
          if (defrAssertion.defiAssertion::isConstraint()) 
            CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion);
          if (defrAssertion.defiAssertion::isAssertion()) 
            CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion);
        }
      ;}
    break;

  case 568:
#line 3331 "def.y"
    { 
        if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
          if (defrAssertion.defiAssertion::isConstraint()) 
	    CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion);
          if (defrAssertion.defiAssertion::isAssertion()) 
            CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion);
        }
   
        // reset all the flags and everything
        defrAssertion.defiAssertion::clear();
      ;}
    break;

  case 569:
#line 3343 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 570:
#line 3344 "def.y"
    {
         if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::addNet((yyvsp[0].string));
      ;}
    break;

  case 571:
#line 3348 "def.y"
    {dumb_mode = 4; no_num = 4;;}
    break;

  case 572:
#line 3349 "def.y"
    {
         if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::addPath((yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-1].string), (yyvsp[0].string));
      ;}
    break;

  case 573:
#line 3354 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::setSum();
      ;}
    break;

  case 574:
#line 3359 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::setDiff();
      ;}
    break;

  case 576:
#line 3366 "def.y"
    { ;}
    break;

  case 577:
#line 3368 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 578:
#line 3370 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setWiredlogic((yyvsp[-4].string), (yyvsp[-1].dval));
      ;}
    break;

  case 579:
#line 3377 "def.y"
    { (yyval.string) = (char*)""; ;}
    break;

  case 580:
#line 3379 "def.y"
    { (yyval.string) = (char*)"+"; ;}
    break;

  case 583:
#line 3386 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setRiseMin((yyvsp[0].dval));
      ;}
    break;

  case 584:
#line 3391 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setRiseMax((yyvsp[0].dval));
      ;}
    break;

  case 585:
#line 3396 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setFallMin((yyvsp[0].dval));
      ;}
    break;

  case 586:
#line 3401 "def.y"
    {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setFallMax((yyvsp[0].dval));
      ;}
    break;

  case 587:
#line 3407 "def.y"
    { if ((defVersionNum < 5.4) && defrConstraintsEndCbk) {
          CALLBACK(defrConstraintsEndCbk, defrConstraintsEndCbkType, 0);
        } else {
          if (defrConstraintsEndCbk) {
            if (constraintWarnings++ < defrConstraintWarnings)
              defWarning(7032, "CONSTRAINTS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
          }
        }
      ;}
    break;

  case 588:
#line 3418 "def.y"
    { if ((defVersionNum < 5.4) && defrAssertionsEndCbk) {
          CALLBACK(defrAssertionsEndCbk, defrAssertionsEndCbkType, 0);
        } else {
          if (defrAssertionsEndCbk) {
            if (assertionWarnings++ < defrAssertionWarnings)
              defWarning(7031, "ASSERTIONS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
          }
        }
      ;}
    break;

  case 590:
#line 3432 "def.y"
    { if (defrScanchainsStartCbk)
          CALLBACK(defrScanchainsStartCbk, defrScanchainsStartCbkType,
                   ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 592:
#line 3439 "def.y"
    {;}
    break;

  case 593:
#line 3442 "def.y"
    { 
        if (defrScanchainCbk)
          CALLBACK(defrScanchainCbk, defrScanchainCbkType, &defrScanchain);
      ;}
    break;

  case 594:
#line 3447 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 595:
#line 3448 "def.y"
    {
        if (defrScanchainCbk)
          defrScanchain.defiScanchain::setName((yyvsp[0].string));
        bit_is_keyword = TRUE;
      ;}
    break;

  case 598:
#line 3460 "def.y"
    { (yyval.string) = (char*)""; ;}
    break;

  case 599:
#line 3462 "def.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 600:
#line 3464 "def.y"
    {dumb_mode = 2; no_num = 2;;}
    break;

  case 601:
#line 3465 "def.y"
    { if (defrScanchainCbk)
          defrScanchain.defiScanchain::setStart((yyvsp[-1].string), (yyvsp[0].string));
      ;}
    break;

  case 602:
#line 3468 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 603:
#line 3469 "def.y"
    { dumb_mode = 0; no_num = 0; ;}
    break;

  case 604:
#line 3471 "def.y"
    {
         dumb_mode = 1;
         no_num = 1;
         if (defrScanchainCbk)
           defrScanchain.defiScanchain::addOrderedList();
      ;}
    break;

  case 605:
#line 3478 "def.y"
    { dumb_mode = 0; no_num = 0; ;}
    break;

  case 606:
#line 3479 "def.y"
    {dumb_mode = 2; no_num = 2; ;}
    break;

  case 607:
#line 3480 "def.y"
    { if (defrScanchainCbk)
          defrScanchain.defiScanchain::setStop((yyvsp[-1].string), (yyvsp[0].string));
      ;}
    break;

  case 608:
#line 3483 "def.y"
    { dumb_mode = 10; no_num = 10; ;}
    break;

  case 609:
#line 3484 "def.y"
    { dumb_mode = 0;  no_num = 0; ;}
    break;

  case 610:
#line 3485 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 611:
#line 3487 "def.y"
    {
        if (defVersionNum < 5.5) {
          if (defrScanchainCbk) {
            if (scanchainWarnings++ < defrScanchainWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "PARTITION statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6538, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        }
        if (defrScanchainCbk)
          defrScanchain.defiScanchain::setPartition((yyvsp[-1].string), (yyvsp[0].integer));
      ;}
    break;

  case 612:
#line 3504 "def.y"
    {
        if (defrScanChainExtCbk)
          CALLBACK(defrScanChainExtCbk, defrScanChainExtCbkType, History_text);
      ;}
    break;

  case 613:
#line 3510 "def.y"
    { ;}
    break;

  case 614:
#line 3512 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut((yyvsp[-1].string));
	}
      ;}
    break;

  case 615:
#line 3521 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-6].string), "IN") == 0 || strcmp((yyvsp[-6].string), "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "OUT") == 0 || strcmp((yyvsp[-6].string), "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut((yyvsp[-5].string));
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut((yyvsp[-1].string));
	}
      ;}
    break;

  case 618:
#line 3539 "def.y"
    {
	dumb_mode = 1000;
	no_num = 1000;
	if (defrScanchainCbk)
	  defrScanchain.defiScanchain::addFloatingInst((yyvsp[0].string));
      ;}
    break;

  case 619:
#line 3546 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 620:
#line 3549 "def.y"
    { ;}
    break;

  case 621:
#line 3551 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-1].string));
          else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
            defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      ;}
    break;

  case 622:
#line 3564 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-6].string), "IN") == 0 || strcmp((yyvsp[-6].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "OUT") == 0 || strcmp((yyvsp[-6].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "BITS") == 0 || strcmp((yyvsp[-6].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-5].string));
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      ;}
    break;

  case 623:
#line 3586 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-10].string), "IN") == 0 || strcmp((yyvsp[-10].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-9].string));
	  else if (strcmp((yyvsp[-10].string), "OUT") == 0 || strcmp((yyvsp[-10].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-9].string));
	  else if (strcmp((yyvsp[-10].string), "BITS") == 0 || strcmp((yyvsp[-10].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-9].string));
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp((yyvsp[-6].string), "IN") == 0 || strcmp((yyvsp[-6].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "OUT") == 0 || strcmp((yyvsp[-6].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "BITS") == 0 || strcmp((yyvsp[-6].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-5].string));
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      ;}
    break;

  case 626:
#line 3620 "def.y"
    { dumb_mode = 1000; no_num = 1000; 
	if (defrScanchainCbk)
	  defrScanchain.defiScanchain::addOrderedInst((yyvsp[0].string));
      ;}
    break;

  case 627:
#line 3625 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 628:
#line 3628 "def.y"
    { ;}
    break;

  case 629:
#line 3630 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-1].string));
          else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
         }
	}
      ;}
    break;

  case 630:
#line 3643 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-6].string), "IN") == 0 || strcmp((yyvsp[-6].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "OUT") == 0 || strcmp((yyvsp[-6].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "BITS") == 0 || strcmp((yyvsp[-6].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-5].string));
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	}
      ;}
    break;

  case 631:
#line 3665 "def.y"
    {
	if (defrScanchainCbk) {
	  if (strcmp((yyvsp[-10].string), "IN") == 0 || strcmp((yyvsp[-10].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-9].string));
	  else if (strcmp((yyvsp[-10].string), "OUT") == 0 || strcmp((yyvsp[-10].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-9].string));
	  else if (strcmp((yyvsp[-10].string), "BITS") == 0 || strcmp((yyvsp[-10].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-9].string));
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp((yyvsp[-6].string), "IN") == 0 || strcmp((yyvsp[-6].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "OUT") == 0 || strcmp((yyvsp[-6].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-5].string));
	  else if (strcmp((yyvsp[-6].string), "BITS") == 0 || strcmp((yyvsp[-6].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-5].string));
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp((yyvsp[-2].string), "IN") == 0 || strcmp((yyvsp[-2].string), "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "OUT") == 0 || strcmp((yyvsp[-2].string), "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut((yyvsp[-1].string));
	  else if (strcmp((yyvsp[-2].string), "BITS") == 0 || strcmp((yyvsp[-2].string), "bits") == 0) {
            bitsNum = atoi((yyvsp[-1].string));
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	}
      ;}
    break;

  case 632:
#line 3695 "def.y"
    { (yyval.integer) = -1; ;}
    break;

  case 633:
#line 3697 "def.y"
    { (yyval.integer) = ROUND((yyvsp[0].dval)); ;}
    break;

  case 634:
#line 3700 "def.y"
    { 
        if (defrScanchainsEndCbk)
          CALLBACK(defrScanchainsEndCbk, defrScanchainsEndCbkType, 0);
        bit_is_keyword = FALSE;
        dumb_mode = 0; no_num = 0;
      ;}
    break;

  case 636:
#line 3712 "def.y"
    {
        if (defVersionNum < 5.4 && defrIOTimingsStartCbk) {
          CALLBACK(defrIOTimingsStartCbk, defrIOTimingsStartCbkType, ROUND((yyvsp[-1].dval)));
        } else {
          if (defrIOTimingsStartCbk)
            if (iOTimingWarnings++ < defrIOTimingWarnings)
              defWarning(7035, "IOTIMINGS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
      ;}
    break;

  case 638:
#line 3724 "def.y"
    { ;}
    break;

  case 639:
#line 3727 "def.y"
    { 
        if (defVersionNum < 5.4 && defrIOTimingCbk)
          CALLBACK(defrIOTimingCbk, defrIOTimingCbkType, &defrIOTiming);
      ;}
    break;

  case 640:
#line 3732 "def.y"
    {dumb_mode = 2; no_num = 2; ;}
    break;

  case 641:
#line 3733 "def.y"
    {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setName((yyvsp[-2].string), (yyvsp[-1].string));
      ;}
    break;

  case 644:
#line 3744 "def.y"
    {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setVariable((yyvsp[-3].string), (yyvsp[-1].dval), (yyvsp[0].dval));
      ;}
    break;

  case 645:
#line 3749 "def.y"
    {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setSlewRate((yyvsp[-3].string), (yyvsp[-1].dval), (yyvsp[0].dval));
      ;}
    break;

  case 646:
#line 3754 "def.y"
    {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setCapacitance((yyvsp[0].dval));
      ;}
    break;

  case 647:
#line 3758 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 648:
#line 3759 "def.y"
    {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setDriveCell((yyvsp[0].string));
      ;}
    break;

  case 650:
#line 3785 "def.y"
    {
        if (defVersionNum < 5.4 && defrIoTimingsExtCbk)
          CALLBACK(defrIoTimingsExtCbk, defrIoTimingsExtCbkType, History_text);
      ;}
    break;

  case 651:
#line 3791 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 652:
#line 3792 "def.y"
    {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setTo((yyvsp[0].string));
      ;}
    break;

  case 655:
#line 3799 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 656:
#line 3800 "def.y"
    {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setFrom((yyvsp[0].string));
      ;}
    break;

  case 658:
#line 3807 "def.y"
    {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setParallel((yyvsp[0].dval));
      ;}
    break;

  case 659:
#line 3812 "def.y"
    { (yyval.string) = (char*)"RISE"; ;}
    break;

  case 660:
#line 3812 "def.y"
    { (yyval.string) = (char*)"FALL"; ;}
    break;

  case 661:
#line 3815 "def.y"
    {
        if (defVersionNum < 5.4 && defrIOTimingsEndCbk)
          CALLBACK(defrIOTimingsEndCbk, defrIOTimingsEndCbkType, 0);
      ;}
    break;

  case 662:
#line 3821 "def.y"
    { 
        if (defrFPCEndCbk)
          CALLBACK(defrFPCEndCbk, defrFPCEndCbkType, 0);
      ;}
    break;

  case 663:
#line 3827 "def.y"
    {
        if (defrFPCStartCbk)
          CALLBACK(defrFPCStartCbk, defrFPCStartCbkType, ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 665:
#line 3834 "def.y"
    {;}
    break;

  case 666:
#line 3836 "def.y"
    { dumb_mode = 1; no_num = 1;  ;}
    break;

  case 667:
#line 3837 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setName((yyvsp[-1].string), (yyvsp[0].string)); ;}
    break;

  case 668:
#line 3839 "def.y"
    { if (defrFPCCbk) CALLBACK(defrFPCCbk, defrFPCCbkType, &defrFPC); ;}
    break;

  case 669:
#line 3842 "def.y"
    { (yyval.string) = (char*)"HORIZONTAL"; ;}
    break;

  case 670:
#line 3844 "def.y"
    { (yyval.string) = (char*)"VERTICAL"; ;}
    break;

  case 671:
#line 3847 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setAlign(); ;}
    break;

  case 672:
#line 3849 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setMax((yyvsp[0].dval)); ;}
    break;

  case 673:
#line 3851 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setMin((yyvsp[0].dval)); ;}
    break;

  case 674:
#line 3853 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setEqual((yyvsp[0].dval)); ;}
    break;

  case 677:
#line 3860 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setDoingBottomLeft(); ;}
    break;

  case 679:
#line 3863 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::setDoingTopRight(); ;}
    break;

  case 683:
#line 3870 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 684:
#line 3871 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::addRow((yyvsp[-1].string)); ;}
    break;

  case 685:
#line 3872 "def.y"
    {dumb_mode = 1; no_num = 1; ;}
    break;

  case 686:
#line 3873 "def.y"
    { if (defrFPCCbk) defrFPC.defiFPC::addComps((yyvsp[-1].string)); ;}
    break;

  case 688:
#line 3880 "def.y"
    { 
        if (defrTimingDisablesStartCbk)
          CALLBACK(defrTimingDisablesStartCbk, defrTimingDisablesStartCbkType,
                   ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 690:
#line 3888 "def.y"
    {;}
    break;

  case 691:
#line 3890 "def.y"
    { dumb_mode = 2; no_num = 2;  ;}
    break;

  case 692:
#line 3891 "def.y"
    { dumb_mode = 2; no_num = 2;  ;}
    break;

  case 693:
#line 3892 "def.y"
    {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setFromTo((yyvsp[-6].string), (yyvsp[-5].string), (yyvsp[-2].string), (yyvsp[-1].string));
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                &defrTimingDisable);
        }
      ;}
    break;

  case 694:
#line 3899 "def.y"
    {dumb_mode = 2; no_num = 2; ;}
    break;

  case 695:
#line 3900 "def.y"
    {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setThru((yyvsp[-2].string), (yyvsp[-1].string));
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                   &defrTimingDisable);
        }
      ;}
    break;

  case 696:
#line 3907 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 697:
#line 3908 "def.y"
    {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setMacro((yyvsp[-2].string));
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                &defrTimingDisable);
        }
      ;}
    break;

  case 698:
#line 3916 "def.y"
    { if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setReentrantPathsFlag();
      ;}
    break;

  case 699:
#line 3921 "def.y"
    {dumb_mode = 1; no_num = 1;;}
    break;

  case 700:
#line 3922 "def.y"
    {dumb_mode=1; no_num = 1;;}
    break;

  case 701:
#line 3923 "def.y"
    {
        if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setMacroFromTo((yyvsp[-3].string),(yyvsp[0].string));
      ;}
    break;

  case 702:
#line 3927 "def.y"
    {dumb_mode=1; no_num = 1;;}
    break;

  case 703:
#line 3928 "def.y"
    {
        if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setMacroThru((yyvsp[0].string));
      ;}
    break;

  case 704:
#line 3934 "def.y"
    { 
        if (defrTimingDisablesEndCbk)
          CALLBACK(defrTimingDisablesEndCbk, defrTimingDisablesEndCbkType, 0);
      ;}
    break;

  case 706:
#line 3944 "def.y"
    {
        if (defrPartitionsStartCbk)
          CALLBACK(defrPartitionsStartCbk, defrPartitionsStartCbkType,
                   ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 708:
#line 3952 "def.y"
    { ;}
    break;

  case 709:
#line 3955 "def.y"
    { 
        if (defrPartitionCbk)
          CALLBACK(defrPartitionCbk, defrPartitionCbkType, &defrPartition);
      ;}
    break;

  case 710:
#line 3960 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 711:
#line 3961 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setName((yyvsp[-1].string));
      ;}
    break;

  case 713:
#line 3968 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::addTurnOff((yyvsp[-1].string), (yyvsp[0].string));
      ;}
    break;

  case 714:
#line 3974 "def.y"
    { (yyval.string) = (char*)" "; ;}
    break;

  case 715:
#line 3976 "def.y"
    { (yyval.string) = (char*)"R"; ;}
    break;

  case 716:
#line 3978 "def.y"
    { (yyval.string) = (char*)"F"; ;}
    break;

  case 717:
#line 3981 "def.y"
    { (yyval.string) = (char*)" "; ;}
    break;

  case 718:
#line 3983 "def.y"
    { (yyval.string) = (char*)"R"; ;}
    break;

  case 719:
#line 3985 "def.y"
    { (yyval.string) = (char*)"F"; ;}
    break;

  case 722:
#line 3991 "def.y"
    {dumb_mode=2; no_num = 2;;}
    break;

  case 723:
#line 3993 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromClockPin((yyvsp[-3].string), (yyvsp[-2].string));
      ;}
    break;

  case 724:
#line 3997 "def.y"
    {dumb_mode=2; no_num = 2; ;}
    break;

  case 725:
#line 3999 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromCompPin((yyvsp[-2].string), (yyvsp[-1].string));
      ;}
    break;

  case 726:
#line 4003 "def.y"
    {dumb_mode=1; no_num = 1; ;}
    break;

  case 727:
#line 4005 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromIOPin((yyvsp[-1].string));
      ;}
    break;

  case 728:
#line 4009 "def.y"
    {dumb_mode=2; no_num = 2; ;}
    break;

  case 729:
#line 4011 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToClockPin((yyvsp[-3].string), (yyvsp[-2].string));
      ;}
    break;

  case 730:
#line 4015 "def.y"
    {dumb_mode=2; no_num = 2; ;}
    break;

  case 731:
#line 4017 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToCompPin((yyvsp[-2].string), (yyvsp[-1].string));
      ;}
    break;

  case 732:
#line 4021 "def.y"
    {dumb_mode=1; no_num = 2; ;}
    break;

  case 733:
#line 4022 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToIOPin((yyvsp[-1].string));
      ;}
    break;

  case 734:
#line 4027 "def.y"
    { 
        if (defrPartitionsExtCbk)
          CALLBACK(defrPartitionsExtCbk, defrPartitionsExtCbkType,
                   History_text);
      ;}
    break;

  case 735:
#line 4034 "def.y"
    { dumb_mode = 1000000000; no_num = 10000000; ;}
    break;

  case 736:
#line 4035 "def.y"
    { dumb_mode = 0; no_num = 0; ;}
    break;

  case 738:
#line 4039 "def.y"
    { ;}
    break;

  case 739:
#line 4042 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setMin((yyvsp[-1].dval), (yyvsp[0].dval));
      ;}
    break;

  case 740:
#line 4047 "def.y"
    {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setMax((yyvsp[-1].dval), (yyvsp[0].dval));
      ;}
    break;

  case 742:
#line 4054 "def.y"
    { if (defrPartitionCbk) defrPartition.defiPartition::addPin((yyvsp[0].string)); ;}
    break;

  case 745:
#line 4060 "def.y"
    { if (defrPartitionCbk) defrPartition.defiPartition::addRiseMin((yyvsp[0].dval)); ;}
    break;

  case 746:
#line 4062 "def.y"
    { if (defrPartitionCbk) defrPartition.defiPartition::addFallMin((yyvsp[0].dval)); ;}
    break;

  case 747:
#line 4064 "def.y"
    { if (defrPartitionCbk) defrPartition.defiPartition::addRiseMax((yyvsp[0].dval)); ;}
    break;

  case 748:
#line 4066 "def.y"
    { if (defrPartitionCbk) defrPartition.defiPartition::addFallMax((yyvsp[0].dval)); ;}
    break;

  case 751:
#line 4074 "def.y"
    { if (defrPartitionCbk)
          defrPartition.defiPartition::addRiseMinRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 752:
#line 4077 "def.y"
    { if (defrPartitionCbk)
          defrPartition.defiPartition::addFallMinRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 753:
#line 4080 "def.y"
    { if (defrPartitionCbk)
          defrPartition.defiPartition::addRiseMaxRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 754:
#line 4083 "def.y"
    { if (defrPartitionCbk)
          defrPartition.defiPartition::addFallMaxRange((yyvsp[-1].dval), (yyvsp[0].dval)); ;}
    break;

  case 755:
#line 4087 "def.y"
    { if (defrPartitionsEndCbk)
          CALLBACK(defrPartitionsEndCbk, defrPartitionsEndCbkType, 0); ;}
    break;

  case 757:
#line 4092 "def.y"
    { ;}
    break;

  case 758:
#line 4094 "def.y"
    {dumb_mode=2; no_num = 2; ;}
    break;

  case 759:
#line 4096 "def.y"
    {
        /* note that the first T_STRING could be the keyword VPIN */
        if (defrNetCbk)
          defrSubnet->defiSubnet::addPin((yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-1].integer));
      ;}
    break;

  case 760:
#line 4103 "def.y"
    { (yyval.integer) = 0; ;}
    break;

  case 761:
#line 4105 "def.y"
    { (yyval.integer) = 1; ;}
    break;

  case 764:
#line 4111 "def.y"
    {  
        if (defrNetCbk) defrSubnet->defiSubnet::addWire((yyvsp[0].string));
      ;}
    break;

  case 765:
#line 4115 "def.y"
    {  
        by_is_keyword = FALSE;
        do_is_keyword = FALSE;
        new_is_keyword = FALSE;
        step_is_keyword = FALSE;
        orient_is_keyword = FALSE;
        needNPCbk = 0;
      ;}
    break;

  case 766:
#line 4123 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 767:
#line 4124 "def.y"
    { if (defrNetCbk) defrSubnet->defiSubnet::setNonDefault((yyvsp[0].string)); ;}
    break;

  case 768:
#line 4127 "def.y"
    { (yyval.string) = (char*)"FIXED"; ;}
    break;

  case 769:
#line 4129 "def.y"
    { (yyval.string) = (char*)"COVER"; ;}
    break;

  case 770:
#line 4131 "def.y"
    { (yyval.string) = (char*)"ROUTED"; ;}
    break;

  case 771:
#line 4133 "def.y"
    { (yyval.string) = (char*)"NOSHIELD"; ;}
    break;

  case 773:
#line 4138 "def.y"
    { if (defrPinPropStartCbk)
          CALLBACK(defrPinPropStartCbk, defrPinPropStartCbkType, ROUND((yyvsp[-1].dval))); ;}
    break;

  case 774:
#line 4143 "def.y"
    { ;}
    break;

  case 775:
#line 4145 "def.y"
    { ;}
    break;

  case 776:
#line 4148 "def.y"
    { if (defrPinPropEndCbk)
          CALLBACK(defrPinPropEndCbk, defrPinPropEndCbkType, 0); ;}
    break;

  case 779:
#line 4155 "def.y"
    { dumb_mode = 2; no_num = 2; ;}
    break;

  case 780:
#line 4156 "def.y"
    { if (defrPinPropCbk) defrPinProp.defiPinProp::setName((yyvsp[-1].string), (yyvsp[0].string)); ;}
    break;

  case 781:
#line 4158 "def.y"
    { if (defrPinPropCbk) {
          CALLBACK(defrPinPropCbk, defrPinPropCbkType, &defrPinProp);
         // reset the property number
         defrPinProp.defiPinProp::clear();
        }
      ;}
    break;

  case 784:
#line 4168 "def.y"
    { dumb_mode = 10000000; parsing_property = 1; ;}
    break;

  case 785:
#line 4170 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 788:
#line 4177 "def.y"
    {
        if (defrPinPropCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrCompPinProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "PINPROPERTIES");
          /* For backword compatibility, also set the string value */
          /* We will use a temporary string to store the number.
           * The string space is borrowed from the ring buffer
           * in the lexer. */
          sprintf(str, "%g", (yyvsp[0].dval));
          defrPinProp.defiPinProp::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
        }
      ;}
    break;

  case 789:
#line 4192 "def.y"
    {
        if (defrPinPropCbk) {
          char propTp;
          propTp = defrCompPinProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "PINPROPERTIES");
          defrPinProp.defiPinProp::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 790:
#line 4201 "def.y"
    {
        if (defrPinPropCbk) {
          char propTp;
          propTp = defrCompPinProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "PINPROPERTIES");
          defrPinProp.defiPinProp::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 792:
#line 4213 "def.y"
    { if (defrBlockageStartCbk)
          CALLBACK(defrBlockageStartCbk, defrBlockageStartCbkType, ROUND((yyvsp[-1].dval))); ;}
    break;

  case 793:
#line 4217 "def.y"
    { if (defrBlockageEndCbk)
          CALLBACK(defrBlockageEndCbk, defrBlockageEndCbkType, 0); ;}
    break;

  case 796:
#line 4226 "def.y"
    {
        if (defrBlockageCbk) {
          CALLBACK(defrBlockageCbk, defrBlockageCbkType, &defrBlockage);
          defrBlockage.defiBlockage::clear();
        }
      ;}
    break;

  case 797:
#line 4233 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 798:
#line 4234 "def.y"
    {
        if (defrBlockageCbk) {
          if (defrBlockage.defiBlockage::hasPlacement() != 0) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6539, "The BLOCKAGE statement has both the LAYER & the PLACEMENT statements defined.\nThis is illegal.");
              CHKERR();
            }
          }
          defrBlockage.defiBlockage::setLayer((yyvsp[0].string));
          defrBlockage.defiBlockage::clearPoly(); // free poly, if any
        }
        hasBlkLayerComp = 0;
        hasBlkLayerSpac = 0;
      ;}
    break;

  case 800:
#line 4251 "def.y"
    {
        if (defrBlockageCbk) {
          if (defrBlockage.defiBlockage::hasLayer() != 0) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6539, "The BLOCKAGE statement has both the LAYER & the PLACEMENT statements defined.");
              CHKERR();
            }
          }
          defrBlockage.defiBlockage::setPlacement();
        }
        hasBlkPlaceComp = 0;
      ;}
    break;

  case 804:
#line 4270 "def.y"
    {
        if (defVersionNum < 5.6) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6540, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        } else if (hasBlkLayerSpac) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH  statement has defined in the layer.\nThis is illegal");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setSpacing((int)(yyvsp[0].dval));
          hasBlkLayerSpac = 1;
        }
      ;}
    break;

  case 805:
#line 4296 "def.y"
    {
        if (defVersionNum < 5.6) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH  statement has defined in the layer.\nThis is illegal");
              CHKERR();
            }
          }
        } else if (hasBlkLayerSpac) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH  statement has defined in the layer.\nThis is illegal");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setDesignRuleWidth((int)(yyvsp[0].dval));
          hasBlkLayerSpac = 1;
        }
      ;}
    break;

  case 807:
#line 4321 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 808:
#line 4322 "def.y"
    {
        if (hasBlkLayerComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setComponent((yyvsp[0].string));
          hasBlkLayerComp = 1;
        }
      ;}
    break;

  case 809:
#line 4338 "def.y"
    {
        if (hasBlkLayerComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setSlots();
          hasBlkLayerComp = 1;
        }
      ;}
    break;

  case 810:
#line 4353 "def.y"
    {
        if (hasBlkLayerComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setFills();
          hasBlkLayerComp = 1;
        }
      ;}
    break;

  case 811:
#line 4368 "def.y"
    {
        if (hasBlkLayerComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setPushdown();
          hasBlkLayerComp = 1;
        }
      ;}
    break;

  case 812:
#line 4383 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrBlockageCbk) {
             if (blockageWarnings++ < defrBlockageWarnings) {
               defMsg = (char*)defMalloc(10000);
               sprintf (defMsg,
                 "EXCEPTPGNET is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
               defError(6549, defMsg);
               defFree(defMsg);
               CHKERR();
              }
           }
        } else {
           if (hasBlkLayerComp) {
             if (defrBlockageCbk) {
               if (blockageWarnings++ < defrBlockageWarnings) {
                 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
                 CHKERR();
               }
             }
           } else {
             if (defrBlockageCbk)
               defrBlockage.defiBlockage::setExceptpgnet();
             hasBlkLayerComp = 1;
           }
        }
      ;}
    break;

  case 814:
#line 4413 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 815:
#line 4414 "def.y"
    {
        if (hasBlkPlaceComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setComponent((yyvsp[0].string));
          hasBlkPlaceComp = 1;
        }
      ;}
    break;

  case 816:
#line 4429 "def.y"
    {
        if (hasBlkPlaceComp) {
          if (defrBlockageCbk) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
              CHKERR();
            }
          }
        } else {
          if (defrBlockageCbk)
            defrBlockage.defiBlockage::setPushdown();
          hasBlkPlaceComp = 1;
        }
      ;}
    break;

  case 817:
#line 4444 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrBlockageCbk) {
             if (blockageWarnings++ < defrBlockageWarnings) {
               defMsg = (char*)defMalloc(10000);
               sprintf (defMsg,
                 "PLACEMENT SOFT is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
               defError(6547, defMsg);
               defFree(defMsg);
               CHKERR();
             }
           }
        } else {
           if (hasBlkPlaceComp) {
             if (defrBlockageCbk) {
               if (blockageWarnings++ < defrBlockageWarnings) {
                 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
                 CHKERR();
               }
             }
           } else {
             if (defrBlockageCbk)
               defrBlockage.defiBlockage::setSoft();
             hasBlkPlaceComp = 1;
           }
        }
      ;}
    break;

  case 818:
#line 4472 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrBlockageCbk) {
             if (blockageWarnings++ < defrBlockageWarnings) {
                defMsg = (char*)defMalloc(10000);
                sprintf (defMsg,
                  "PARTIAL is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
                defError(6548, defMsg);
                defFree(defMsg);
                CHKERR();
             }
           }
        } else {
           if (hasBlkPlaceComp) {
             if (defrBlockageCbk) {
               if (blockageWarnings++ < defrBlockageWarnings) {
                 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
                 CHKERR();
               }
             }
           } else {
             if (defrBlockageCbk)
               defrBlockage.defiBlockage::setPartial((yyvsp[0].dval));
             hasBlkPlaceComp = 1;
           }
         }
      ;}
    break;

  case 821:
#line 4505 "def.y"
    {
        if (defrBlockageCbk)
          defrBlockage.defiBlockage::addRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
      ;}
    break;

  case 822:
#line 4510 "def.y"
    {
        if (defrBlockageCbk) {
          if (!defrGeomPtr) {
            defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
            defrGeomPtr->defiGeometries::Init();
          } else   // Just reset the number of points
            defrGeomPtr->defiGeometries::Reset();
        }
      ;}
    break;

  case 823:
#line 4520 "def.y"
    {
        if (defrBlockageCbk) {
          if (defVersionNum >= 5.6) {  // only 5.6 and beyond
            if (defrBlockage.defiBlockage::hasLayer()) {  // only in layer
              if (defrBlockageCbk)
                defrBlockage.defiBlockage::addPolygon(defrGeomPtr);
            } else {
              if (defrBlockageCbk) {
                if (blockageWarnings++ < defrBlockageWarnings) {
                  defError(6544, "A POLYGON statement is defined in the BLOCKAGE statement,\nbut it is not defined in the BLOCKAGE LAYER statement.\nThis is illegal");
                  CHKERR();
                }
              }
            }
          }
        }
      ;}
    break;

  case 825:
#line 4542 "def.y"
    { if (defrSlotStartCbk)
          CALLBACK(defrSlotStartCbk, defrSlotStartCbkType, ROUND((yyvsp[-1].dval))); ;}
    break;

  case 826:
#line 4546 "def.y"
    { if (defrSlotEndCbk)
          CALLBACK(defrSlotEndCbk, defrSlotEndCbkType, 0); ;}
    break;

  case 829:
#line 4554 "def.y"
    {
        if (defrSlotCbk) {
          CALLBACK(defrSlotCbk, defrSlotCbkType, &defrSlot);
          defrSlot.defiSlot::clear();
        }
      ;}
    break;

  case 830:
#line 4561 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 831:
#line 4562 "def.y"
    {
        if (defrSlotCbk) {
          defrSlot.defiSlot::setLayer((yyvsp[0].string));
          defrSlot.defiSlot::clearPoly();     // free poly, if any
        }
      ;}
    break;

  case 835:
#line 4574 "def.y"
    {
        if (defrSlotCbk)
          defrSlot.defiSlot::addRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
      ;}
    break;

  case 836:
#line 4579 "def.y"
    {
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      ;}
    break;

  case 837:
#line 4587 "def.y"
    {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond
          if (defrSlotCbk)
            defrSlot.defiSlot::addPolygon(defrGeomPtr);
        }
      ;}
    break;

  case 839:
#line 4598 "def.y"
    { if (defrFillStartCbk)
          CALLBACK(defrFillStartCbk, defrFillStartCbkType, ROUND((yyvsp[-1].dval))); ;}
    break;

  case 840:
#line 4602 "def.y"
    { if (defrFillEndCbk)
          CALLBACK(defrFillEndCbk, defrFillEndCbkType, 0); ;}
    break;

  case 843:
#line 4610 "def.y"
    {
        if (defrFillCbk) {
          CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
          defrFill.defiFill::clear();
        }
      ;}
    break;

  case 844:
#line 4616 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 845:
#line 4617 "def.y"
    {
        if (defrFillCbk) {
          defrFill.defiFill::setVia((yyvsp[0].string));
          defrFill.defiFill::clearPts();
          if (!defrGeomPtr) {
            defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
            defrGeomPtr->defiGeometries::Init();
          } else   // Just reset the number of points
            defrGeomPtr->defiGeometries::Reset();
        }
      ;}
    break;

  case 846:
#line 4629 "def.y"
    {
        if (defrFillCbk) {
          defrFill.defiFill::addPts(defrGeomPtr);
          CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
          defrFill.defiFill::clear();
        }
      ;}
    break;

  case 847:
#line 4637 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 848:
#line 4638 "def.y"
    {
        if (defrFillCbk) {
          defrFill.defiFill::setLayer((yyvsp[0].string));
          defrFill.defiFill::clearPoly();    // free poly, if any
        }
      ;}
    break;

  case 852:
#line 4650 "def.y"
    {
        if (defrFillCbk)
          defrFill.defiFill::addRect((yyvsp[-1].pt).x, (yyvsp[-1].pt).y, (yyvsp[0].pt).x, (yyvsp[0].pt).y);
      ;}
    break;

  case 853:
#line 4655 "def.y"
    {
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      ;}
    break;

  case 854:
#line 4663 "def.y"
    {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond
          if (defrFillCbk)
            defrFill.defiFill::addPolygon(defrGeomPtr);
        }
      ;}
    break;

  case 855:
#line 4671 "def.y"
    {;}
    break;

  case 856:
#line 4673 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrFillCbk) {
             if (fillWarnings++ < defrFillWarnings) {
               defMsg = (char*)defMalloc(10000);
               sprintf (defMsg,
                 "LAYER OPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
               defError(6553, defMsg);
               defFree(defMsg);
               CHKERR();
             }
           }
        } else {
           if (defrFillCbk)
             defrFill.defiFill::setLayerOpc();
        }
      ;}
    break;

  case 857:
#line 4692 "def.y"
    {;}
    break;

  case 858:
#line 4694 "def.y"
    {
        if (defVersionNum < 5.7) {
           if (defrFillCbk) {
             if (fillWarnings++ < defrFillWarnings) {
               defMsg = (char*)defMalloc(10000);
               sprintf (defMsg,
                 "VIA OPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
               defError(6554, defMsg);
               defFree(defMsg);
               CHKERR();
             }
           }
        } else {
           if (defrFillCbk)
             defrFill.defiFill::setViaOpc();
        }
      ;}
    break;

  case 860:
#line 4717 "def.y"
    { 
        if (defVersionNum < 5.6) {
          if (defrNonDefaultStartCbk) {
            if (nonDefaultWarnings++ < defrNonDefaultWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "NONDEFAULTRULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6545, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        } else if (defrNonDefaultStartCbk)
          CALLBACK(defrNonDefaultStartCbk, defrNonDefaultStartCbkType,
                   ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 861:
#line 4735 "def.y"
    { if (defrNonDefaultEndCbk)
          CALLBACK(defrNonDefaultEndCbk, defrNonDefaultEndCbkType, 0); ;}
    break;

  case 864:
#line 4742 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 865:
#line 4743 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::clear(); 
          defrNonDefault.defiNonDefault::setName((yyvsp[0].string));
        }
      ;}
    break;

  case 866:
#line 4750 "def.y"
    { if (defrNonDefaultCbk)
          CALLBACK(defrNonDefaultCbk, defrNonDefaultCbkType, &defrNonDefault); ;}
    break;

  case 869:
#line 4758 "def.y"
    {
        if (defrNonDefaultCbk)
          defrNonDefault.defiNonDefault::setHardspacing();
      ;}
    break;

  case 870:
#line 4762 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 871:
#line 4764 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addLayer((yyvsp[-2].string));
          defrNonDefault.defiNonDefault::addWidth((yyvsp[0].dval));
        }
      ;}
    break;

  case 873:
#line 4771 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 874:
#line 4772 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addVia((yyvsp[0].string));
        }
      ;}
    break;

  case 875:
#line 4777 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 876:
#line 4778 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addViaRule((yyvsp[0].string));
        }
      ;}
    break;

  case 877:
#line 4783 "def.y"
    { dumb_mode = 1; no_num = 1; ;}
    break;

  case 878:
#line 4784 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addMinCuts((yyvsp[-1].string), (int)(yyvsp[0].dval));
        }
      ;}
    break;

  case 882:
#line 4797 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addDiagWidth((yyvsp[0].dval));
        }
      ;}
    break;

  case 883:
#line 4803 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addSpacing((yyvsp[0].dval));
        }
      ;}
    break;

  case 884:
#line 4809 "def.y"
    {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addWireExt((yyvsp[0].dval));
        }
      ;}
    break;

  case 885:
#line 4816 "def.y"
    { dumb_mode = 10000; parsing_property = 1; ;}
    break;

  case 886:
#line 4818 "def.y"
    { dumb_mode = 0; parsing_property = 0; ;}
    break;

  case 889:
#line 4825 "def.y"
    {
        if (defrNonDefaultCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrNDefProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "NONDEFAULTRULE");
          sprintf(str, "%g", (yyvsp[0].dval));
          defrNonDefault.defiNonDefault::addNumProperty((yyvsp[-1].string), (yyvsp[0].dval), str, propTp);
        }
      ;}
    break;

  case 890:
#line 4836 "def.y"
    {
        if (defrNonDefaultCbk) {
          char propTp;
          propTp = defrNDefProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "NONDEFAULTRULE");
          defrNonDefault.defiNonDefault::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 891:
#line 4845 "def.y"
    {
        if (defrNonDefaultCbk) {
          char propTp;
          propTp = defrNDefProp.defiPropType::propType((yyvsp[-1].string));
          CHKPROPTYPE(propTp, (yyvsp[-1].string), "NONDEFAULTRULE");
          defrNonDefault.defiNonDefault::addProperty((yyvsp[-1].string), (yyvsp[0].string), propTp);
        }
      ;}
    break;

  case 893:
#line 4858 "def.y"
    {
        if (defVersionNum < 5.6) {
          if (defrStylesStartCbk) {
            if (stylesWarnings++ < defrStylesWarnings) {
              defMsg = (char*)defMalloc(1000);
              sprintf (defMsg,
                 "STYLES statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
              defError(6546, defMsg);
              defFree(defMsg);
              CHKERR();
            }
          }
        } else if (defrStylesStartCbk)
          CALLBACK(defrStylesStartCbk, defrStylesStartCbkType, ROUND((yyvsp[-1].dval)));
      ;}
    break;

  case 894:
#line 4875 "def.y"
    { if (defrStylesEndCbk)
          CALLBACK(defrStylesEndCbk, defrStylesEndCbkType, 0); ;}
    break;

  case 897:
#line 4883 "def.y"
    {
        if (defrStylesCbk) defrStyles.defiStyles::setStyle((int)(yyvsp[0].dval));
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      ;}
    break;

  case 898:
#line 4892 "def.y"
    {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond will call the callback
          if (defrStylesCbk) {
            defrStyles.defiStyles::setPolygon(defrGeomPtr);
            CALLBACK(defrStylesCbk, defrStylesCbkType, &defrStyles);
          }
        }
      ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 9576 "def.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 4901 "def.y"


