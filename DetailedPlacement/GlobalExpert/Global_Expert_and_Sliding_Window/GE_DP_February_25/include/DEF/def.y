/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.7, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 *
 *  Error message number:
 *  5000 - def reader, defrReader.cpp
 *  5500 - lex.cpph, yyerror
 *  6000 - def parser, error, lex.cpph, def.y (CALLBACK & CHKERR)
 *    6000 - out of memory
 *    6005 - defiAssertion.cpp
 *    6010 - defiBlockage.cpp
 *    6020 - defiComponent.cpp
 *    6030 - defiFPC.cpp
 *    6040 - defiFill.cpp
 *    6050 - defiGroup.cpp
 *    6060 - defiIOTiming.cpp
 *    6070 - defiMisc.cpp
 *    6080 - defiNet.cpp
 *    6090 - defiNonDefault.cpp
 *    6100 - defiPartition.cpp
 *    6110 - defiPinCap.cpp
 *    6120 - defiPinProp.cpp
 *    6130 - defiRegion.cpp
 *    6140 - defiRowTrack.cpp
 *    6150 - defiScanchain.cpp
 *    6160 - defiSlot.cpp
 *    6170 - defiTimingDisable.cpp
 *    6180 - defiVia.cpp
 *  6500 - def parser, error, def.y
 *  7000 - def parser, warning, lex.cpph
 *  7500 - def parser, warning, lef.y
 *  8000 - def parser, info, lex.cpph
 *  9000 - def writer
 */

%{
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "defrReader.hpp"
#include "lex.h"
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
%}

%union {
        double dval ;
        int    integer ;
        char * string ;
        int    keyword ;  /* really just a nop */
        struct defpoint pt;
        defTOKEN *tk;
}

%token <string>  QSTRING
%token <string>  T_STRING SITE_PATTERN
%token <dval>    NUMBER
%token <keyword> K_HISTORY K_NAMESCASESENSITIVE
%token <keyword> K_DESIGN K_VIAS K_TECH K_UNITS K_ARRAY K_FLOORPLAN
%token <keyword> K_SITE K_CANPLACE K_CANNOTOCCUPY K_DIEAREA
%token <keyword> K_PINS
%token <keyword> K_DEFAULTCAP K_MINPINS K_WIRECAP
%token <keyword> K_TRACKS K_GCELLGRID
%token <keyword> K_DO K_BY K_STEP K_LAYER K_ROW K_RECT
%token <keyword> K_COMPS K_COMP_GEN K_SOURCE K_WEIGHT K_EEQMASTER
%token <keyword> K_FIXED K_COVER K_UNPLACED K_PLACED K_FOREIGN K_REGION 
%token <keyword> K_REGIONS
%token <keyword> K_NETS K_START_NET K_MUSTJOIN K_ORIGINAL K_USE K_STYLE
%token <keyword> K_PATTERN K_PATTERNNAME K_ESTCAP K_ROUTED K_NEW 
%token <keyword> K_SNETS K_SHAPE K_WIDTH K_VOLTAGE K_SPACING K_NONDEFAULTRULE
%token <keyword> K_NONDEFAULTRULES
%token <keyword> K_N K_S K_E K_W K_FN K_FE K_FS K_FW
%token <keyword> K_GROUPS K_GROUP K_SOFT K_MAXX K_MAXY K_MAXHALFPERIMETER
%token <keyword> K_CONSTRAINTS K_NET K_PATH K_SUM K_DIFF 
%token <keyword> K_SCANCHAINS K_START K_FLOATING K_ORDERED K_STOP K_IN K_OUT
%token <keyword> K_RISEMIN K_RISEMAX K_FALLMIN K_FALLMAX K_WIREDLOGIC
%token <keyword> K_MAXDIST
%token <keyword> K_ASSERTIONS
%token <keyword> K_DISTANCE K_MICRONS
%token <keyword> K_END
%token <keyword> K_IOTIMINGS K_RISE K_FALL K_VARIABLE K_SLEWRATE K_CAPACITANCE
%token <keyword> K_DRIVECELL K_FROMPIN K_TOPIN K_PARALLEL
%token <keyword> K_TIMINGDISABLES K_THRUPIN K_MACRO
%token <keyword> K_PARTITIONS K_TURNOFF
%token <keyword> K_FROMCLOCKPIN K_FROMCOMPPIN K_FROMIOPIN
%token <keyword> K_TOCLOCKPIN K_TOCOMPPIN K_TOIOPIN
%token <keyword> K_SETUPRISE K_SETUPFALL K_HOLDRISE K_HOLDFALL
%token <keyword> K_VPIN K_SUBNET K_XTALK K_PIN K_SYNTHESIZED
%token <keyword> K_DEFINE K_DEFINES K_DEFINEB K_IF K_THEN K_ELSE K_FALSE K_TRUE 
%token <keyword> K_EQ K_NE K_LE K_LT K_GE K_GT K_OR K_AND K_NOT
%token <keyword> K_SPECIAL K_DIRECTION K_RANGE
%token <keyword> K_FPC K_HORIZONTAL K_VERTICAL K_ALIGN K_MIN K_MAX K_EQUAL
%token <keyword> K_BOTTOMLEFT K_TOPRIGHT K_ROWS K_TAPER K_TAPERRULE
%token <keyword> K_VERSION K_DIVIDERCHAR K_BUSBITCHARS
%token <keyword> K_PROPERTYDEFINITIONS K_STRING K_REAL K_INTEGER K_PROPERTY
%token <keyword> K_BEGINEXT K_ENDEXT K_NAMEMAPSTRING K_ON K_OFF K_X K_Y
%token <keyword> K_COMPONENT
%token <keyword> K_PINPROPERTIES K_TEST
%token <keyword> K_COMMONSCANPINS K_SNET K_COMPONENTPIN K_REENTRANTPATHS
%token <keyword> K_SHIELD K_SHIELDNET K_NOSHIELD
%token <keyword> K_ANTENNAPINPARTIALMETALAREA K_ANTENNAPINPARTIALMETALSIDEAREA
%token <keyword> K_ANTENNAPINGATEAREA K_ANTENNAPINDIFFAREA
%token <keyword> K_ANTENNAPINMAXAREACAR K_ANTENNAPINMAXSIDEAREACAR
%token <keyword> K_ANTENNAPINPARTIALCUTAREA K_ANTENNAPINMAXCUTCAR
%token <keyword> K_SIGNAL K_POWER K_GROUND K_CLOCK K_TIEOFF K_ANALOG K_SCAN
%token <keyword> K_RESET K_RING K_STRIPE K_FOLLOWPIN K_IOWIRE K_COREWIRE
%token <keyword> K_BLOCKWIRE K_FILLWIRE K_BLOCKAGEWIRE K_PADRING K_BLOCKRING
%token <keyword> K_BLOCKAGES K_PLACEMENT K_SLOTS K_FILLS K_PUSHDOWN
%token <keyword> K_NETLIST K_DIST K_USER K_TIMING K_BALANCED K_STEINER K_TRUNK
%token <keyword> K_FIXEDBUMP K_FENCE K_FREQUENCY K_GUIDE K_MAXBITS
%token <keyword> K_PARTITION K_TYPE K_ANTENNAMODEL K_DRCFILL
%token <keyword> K_OXIDE1 K_OXIDE2 K_OXIDE3 K_OXIDE4
%token <keyword> K_CUTSIZE K_CUTSPACING K_DESIGNRULEWIDTH K_DIAGWIDTH
%token <keyword> K_ENCLOSURE K_HALO K_GROUNDSENSITIVITY
%token <keyword> K_HARDSPACING K_LAYERS K_MINCUTS K_NETEXPR 
%token <keyword> K_OFFSET K_ORIGIN K_ROWCOL K_STYLES
%token <keyword> K_POLYGON K_PORT K_SUPPLYSENSITIVITY K_VIA K_VIARULE K_WIREEXT
%token <keyword> K_EXCEPTPGNET K_FILLWIREOPC K_OPC K_PARTIAL K_ROUTEHALO
%type <pt>      pt opt_paren
%type <integer> comp_net_list subnet_opt_syn
%type <integer> orient
%type <integer> placement_status
%type <string>  net_type subnet_type track_start use_type shape_type source_type
%type <string>  pattern_type netsource_type
%type <tk>      path paths new_path
%type <string>  risefall opt_pin opt_pattern pin_layer_opt
%type <string>  vpin_status opt_plus track_type region_type
%type <string>  h_or_v turnoff_setup turnoff_hold
%type <integer> conn_opt partition_maxbits

%%

def_file: version_stmt case_sens_stmt rules end_design
            ;

version_stmt:  /* empty */
    | K_VERSION { dumb_mode = 1; no_num = 1; } T_STRING ';'
      {
        defVersionNum = convert_defname2num($3);
        if (defVersionNum > 5.7) {
          char temp[120];
          sprintf(temp,
          "Def parser 5.7 does not support def file with version %s. Parser stops executions.",
                  $3);
          defError(6503, temp);
          return 1;
        }
        if (defrVersionStrCbk) {
          CALLBACK(defrVersionStrCbk, defrVersionStrCbkType, $3);
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
    }

case_sens_stmt: /* empty */
    | K_NAMESCASESENSITIVE K_ON ';'
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
      }
    | K_NAMESCASESENSITIVE K_OFF ';'
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
      }

rules: /* empty */
        | rules rule
        | error 
            ;

rule: design_section | assertions_section | blockage_section | comps_section
      | constraint_section | extension_section | fill_section
      | floorplan_contraints_section | groups_section | iotiming_section
      | nets_section |  nondefaultrule_section | partitions_section
      | pin_props_section | regions_section | scanchains_section
      | slot_section | snets_section | styles_section | timingdisables_section
      | via_section
            ;

design_section: array_name | bus_bit_chars | canplace | cannotoccupy |
              design_name | die_area | divider_char | 
              floorplan_name | gcellgrid | history |
              pin_cap_rule | pin_rule | prop_def_section |
              row_rule | site | tech_name | tracks_rule | units
            ;

design_name: K_DESIGN {dumb_mode = 1; no_num = 1; } T_STRING ';' 
	  {
            if (defrDesignCbk)
              CALLBACK(defrDesignCbk, defrDesignStartCbkType, $3);
            hasDes = 1;
          }

end_design: K_END K_DESIGN
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
          }

tech_name: K_TECH { dumb_mode = 1; no_num = 1; } T_STRING ';'
          { 
            if (defrTechnologyCbk)
              CALLBACK(defrTechnologyCbk, defrTechNameCbkType, $3);
          }

array_name: K_ARRAY {dumb_mode = 1; no_num = 1;} T_STRING ';'
	  { 
            if (defrArrayNameCbk)
              CALLBACK(defrArrayNameCbk, defrArrayNameCbkType, $3);
          }

floorplan_name: K_FLOORPLAN { dumb_mode = 1; no_num = 1; } T_STRING ';'
	  { 
            if (defrFloorPlanNameCbk)
              CALLBACK(defrFloorPlanNameCbk, defrFloorPlanNameCbkType, $3);
          }

history: K_HISTORY
          { 
            if (defrHistoryCbk)
              CALLBACK(defrHistoryCbk, defrHistoryCbkType, History_text);
          }

prop_def_section: K_PROPERTYDEFINITIONS
	  {
            parsing_property = 1;
            defInPropDef = 1;     /* set flag as inside propertydefinitions */
            if (defrPropDefStartCbk)
              CALLBACK(defrPropDefStartCbk, defrPropDefStartCbkType, 0);
          }
    property_defs K_END K_PROPERTYDEFINITIONS
	  { 
            if (defrPropDefEndCbk)
              CALLBACK(defrPropDefEndCbk, defrPropDefEndCbkType, 0);
            real_num = 0;     /* just want to make sure it is reset */
            parsing_property = 0;
            defInPropDef = 0;     /* reset flag */
          }

property_defs: /* empty */
        | property_defs property_def
            { }

property_def: K_DESIGN {dumb_mode = 1; no_num = 1; defrProp.defiProp::clear(); }
              T_STRING property_type_and_val ';' 
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("design", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrDesignProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_NET { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("net", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrNetProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_SNET { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("specialnet", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrSNetProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_REGION { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("region", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrRegionProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_GROUP { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("group", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrGroupProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_COMPONENT { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("component", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrCompProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_ROW { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("row", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrRowProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_PIN { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("pin", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrPinDefProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_COMPONENTPIN
          { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
            {
              if (defrPropCbk) {
                defrProp.defiProp::setPropType("componentpin", $3);
		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                defrCompPinProp.defiPropType::setPropType($3, defPropDefType);
              }
            }
        | K_NONDEFAULTRULE
          { dumb_mode = 1 ; no_num = 1; defrProp.defiProp::clear(); }
          T_STRING property_type_and_val ';'
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
                  defrProp.defiProp::setPropType("nondefaultrule", $3);
		  CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
                  defrNDefProp.defiPropType::setPropType($3, defPropDefType);
                }
              }
            }
        | error ';' { yyerrok; yyclearin;}

property_type_and_val: K_INTEGER { real_num = 0 } opt_range opt_num_val
            {
              if (defrPropCbk) defrProp.defiProp::setPropInteger();
              defPropDefType = 'I';
            }
        | K_REAL { real_num = 1 } opt_range opt_num_val
            {
              if (defrPropCbk) defrProp.defiProp::setPropReal();
              defPropDefType = 'R';
              real_num = 0;
            }
        | K_STRING
            {
              if (defrPropCbk) defrProp.defiProp::setPropString();
              defPropDefType = 'S';
            }
        | K_STRING QSTRING
            {
              if (defrPropCbk) defrProp.defiProp::setPropQString($2);
              defPropDefType = 'Q';
            }
        | K_NAMEMAPSTRING T_STRING
            {
              if (defrPropCbk) defrProp.defiProp::setPropNameMapString($2);
              defPropDefType = 'S';
            }

opt_num_val: /* empty */
        | NUMBER
            { if (defrPropCbk) defrProp.defiProp::setNumber($1); }

units: K_UNITS K_DISTANCE K_MICRONS NUMBER ';'
	  {
            if (defrUnitsCbk) {
              if (defValidNum((int)$4))
                CALLBACK(defrUnitsCbk,  defrUnitsCbkType, $4);
            }
          }

divider_char: K_DIVIDERCHAR QSTRING ';'
	  {
            if (defrDividerCbk)
              CALLBACK(defrDividerCbk, defrDividerCbkType, $2);
            hasDivChar = 1;
          }

bus_bit_chars: K_BUSBITCHARS QSTRING ';'
	  { 
            if (defrBusBitCbk)
              CALLBACK(defrBusBitCbk, defrBusBitCbkType, $2);
            hasBusBit = 1;
          }

site: K_SITE { dumb_mode = 1; no_num = 1; defrSite.defiSite::clear(); }
        T_STRING NUMBER NUMBER orient
        K_DO NUMBER  K_BY NUMBER  K_STEP NUMBER NUMBER ';' 
	  {
            if (defrSiteCbk) {
              defrSite.defiSite::setName($3);
              defrSite.defiSite::setLocation($4,$5);
              defrSite.defiSite::setOrient($6);
              defrSite.defiSite::setDo($8,$10,$12,$13);
	      CALLBACK(defrSiteCbk, defrSiteCbkType, &(defrSite));
            }
	  }

canplace: K_CANPLACE {dumb_mode = 1;no_num = 1; } T_STRING NUMBER NUMBER
            orient K_DO NUMBER  K_BY NUMBER  K_STEP NUMBER NUMBER ';' 
            {
              if (defrCanplaceCbk) {
                defrCanplace.defiSite::setName($3);
                defrCanplace.defiSite::setLocation($4,$5);
                defrCanplace.defiSite::setOrient($6);
                defrCanplace.defiSite::setDo($8,$10,$12,$13);
		CALLBACK(defrCanplaceCbk, defrCanplaceCbkType,
		&(defrCanplace));
              }
            }
cannotoccupy: K_CANNOTOCCUPY {dumb_mode = 1;no_num = 1; } T_STRING NUMBER NUMBER
            orient K_DO NUMBER  K_BY NUMBER  K_STEP NUMBER NUMBER ';' 
            {
              if (defrCannotOccupyCbk) {
                defrCannotOccupy.defiSite::setName($3);
                defrCannotOccupy.defiSite::setLocation($4,$5);
                defrCannotOccupy.defiSite::setOrient($6);
                defrCannotOccupy.defiSite::setDo($8,$10,$12,$13);
		CALLBACK(defrCannotOccupyCbk, defrCannotOccupyCbkType,
                        &(defrCannotOccupy));
              }
            }

orient: K_N    {$$ = 0;}
        | K_W  {$$ = 1;}
        | K_S  {$$ = 2;}
        | K_E  {$$ = 3;}
        | K_FN {$$ = 4;}
        | K_FW {$$ = 5;}
        | K_FS {$$ = 6;}
        | K_FE {$$ = 7;}

die_area: K_DIEAREA
	  {
            if (!defrGeomPtr) {  // set up for more than 4 points
              defrGeomPtr =
                (defiGeometries*)defMalloc(sizeof(defiGeometries));
              defrGeomPtr->defiGeometries::Init();
            } else    // Just reset the number of points
              defrGeomPtr->defiGeometries::Reset();
	  }
          firstPt nextPt otherPts ';'
          {
	    if (defrDieAreaCbk) {
               defrDieArea.defiBox::addPoint(defrGeomPtr);
               CALLBACK(defrDieAreaCbk, defrDieAreaCbkType, &(defrDieArea));
            }
          }

/* 8/31/2001 - This is obsolete in 5.4 */
pin_cap_rule: start_def_cap pin_caps end_def_cap
            { }

start_def_cap: K_DEFAULTCAP NUMBER 
	{
          if (defVersionNum < 5.4) {
             if (defrDefaultCapCbk)
                CALLBACK(defrDefaultCapCbk, defrDefaultCapCbkType, ROUND($2));
          } else {
             if (defrDefaultCapCbk) /* write error only if cbk is set */
                if (defaultCapWarnings++ < defrDefaultCapWarnings)
                   defWarning(7017, "DEFAULTCAP is obsoleted in version 5.4 and earlier.\nThe DEF parser will ignore this statement.");
          }
        }

pin_caps: /* empty */
        | pin_caps pin_cap
            ;

pin_cap: K_MINPINS NUMBER K_WIRECAP NUMBER ';'
	  {
            if (defVersionNum < 5.4) {
	      if (defrPinCapCbk) {
	        defrPinCap.defiPinCap::setPin(ROUND($2));
	        defrPinCap.defiPinCap::setCap($4);
	        CALLBACK(defrPinCapCbk, defrPinCapCbkType, &(defrPinCap));
	      }
            }
	  }

end_def_cap: K_END K_DEFAULTCAP 
            { }

pin_rule: start_pins pins end_pins
            { }

start_pins: K_PINS NUMBER ';'
	  { 
            if (defrStartPinsCbk)
              CALLBACK(defrStartPinsCbk, defrStartPinsCbkType, ROUND($2));
          }

pins: /* empty */
        | pins pin
            ;

pin: '-' {dumb_mode = 1; no_num = 1; } T_STRING '+' K_NET
	 {dumb_mode = 1; no_num = 1; } T_STRING
          {
            if (defrPinCbk || defrPinExtCbk) {
              defrPin.defiPin::Setup($3, $7);
            }
            hasPort = 0;
          }
        pin_options ';'
          { 
            if (defrPinCbk)
              CALLBACK(defrPinCbk, defrPinCbkType, &defrPin);
          }

pin_options: /* empty */
        | pin_options pin_option

pin_option: '+' K_SPECIAL
          {
            if (defrPinCbk)
              defrPin.defiPin::setSpecial();
          }

        | extension_stmt
          { 
            if (defrPinExtCbk)
              CALLBACK(defrPinExtCbk, defrPinExtCbkType, History_text);
          }

        | '+' K_DIRECTION T_STRING
          {
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::setDirection($3);
          }

        | '+' K_NETEXPR QSTRING
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
                defrPin.defiPin::setNetExpr($3);

            }
          }

        | '+' K_SUPPLYSENSITIVITY { dumb_mode = 1; } T_STRING
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
                defrPin.defiPin::setSupplySens($4);
            }
          }

        | '+' K_GROUNDSENSITIVITY { dumb_mode = 1; } T_STRING
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
                defrPin.defiPin::setGroundSens($4);
            }
          }

        | '+' K_USE use_type
          {
            if (defrPinCbk || defrPinExtCbk) defrPin.defiPin::setUse($3);
          }
        | '+' K_PORT          /* 5.7 */
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
          }

        | '+' K_LAYER { dumb_mode = 1; } T_STRING
          {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::addPortLayer($4);
              else
                 defrPin.defiPin::addLayer($4);
            }
          }
          pin_layer_spacing_opt pt pt
          {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::addPortLayerPts($7.x, $7.y, $8.x, $8.y);
              else
                 defrPin.defiPin::addLayerPts($7.x, $7.y, $8.x, $8.y);
            }
          }

        | '+' K_POLYGON { dumb_mode = 1; } T_STRING
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
                   defrPin.defiPin::addPortPolygon($4);
                else
                   defrPin.defiPin::addPolygon($4);
              }
            }
            if (!defrGeomPtr) {
              defrGeomPtr =
                (defiGeometries*)defMalloc(sizeof(defiGeometries));
              defrGeomPtr->defiGeometries::Init();
            } else {  // Just reset the number of points
              defrGeomPtr->defiGeometries::Reset();
            }
          }
          pin_poly_spacing_opt firstPt nextPt nextPt otherPts
          {
            if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
              if (defrPinCbk || defrPinExtCbk) {
                if (hasPort)
                   defrPin.defiPin::addPortPolygonPts(defrGeomPtr);
                else
                   defrPin.defiPin::addPolygonPts(defrGeomPtr);
              }
            }
          }
        | '+' K_VIA { dumb_mode = 1; } T_STRING '(' NUMBER NUMBER ')'   // 5.7
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
                   defrPin.defiPin::addPortVia($4, $6, $7);
                else
                   defrPin.defiPin::addVia($4, $6, $7);
              }
            }
          }
  
        | placement_status pt orient
          {
            if (defrPinCbk || defrPinExtCbk) {
              if (hasPort)
                 defrPin.defiPin::setPortPlacement($1, $2.x, $2.y, $3);
              else
                 defrPin.defiPin::setPlacement($1, $2.x, $2.y, $3);
            }
          }

        /* The following is 5.4 syntax */
        | '+' K_ANTENNAPINPARTIALMETALAREA NUMBER pin_layer_opt
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
              defrPin.defiPin::addAPinPartialMetalArea((int)$3, $4); 
          }
        | '+' K_ANTENNAPINPARTIALMETALSIDEAREA NUMBER pin_layer_opt
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
              defrPin.defiPin::addAPinPartialMetalSideArea((int)$3, $4); 
          }
        | '+' K_ANTENNAPINGATEAREA NUMBER pin_layer_opt
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
                defrPin.defiPin::addAPinGateArea((int)$3, $4); 
            }
        | '+' K_ANTENNAPINDIFFAREA NUMBER pin_layer_opt
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
              defrPin.defiPin::addAPinDiffArea((int)$3, $4); 
          }
        | '+' K_ANTENNAPINMAXAREACAR NUMBER K_LAYER {dumb_mode=1;} T_STRING
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
              defrPin.defiPin::addAPinMaxAreaCar((int)$3, $6); 
          }
        | '+' K_ANTENNAPINMAXSIDEAREACAR NUMBER K_LAYER {dumb_mode=1;}
           T_STRING
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
              defrPin.defiPin::addAPinMaxSideAreaCar((int)$3, $6); 
          }
        | '+' K_ANTENNAPINPARTIALCUTAREA NUMBER pin_layer_opt
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
              defrPin.defiPin::addAPinPartialCutArea((int)$3, $4); 
          }
        | '+' K_ANTENNAPINMAXCUTCAR NUMBER K_LAYER {dumb_mode=1;} T_STRING
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
              defrPin.defiPin::addAPinMaxCutCar((int)$3, $6); 
          }
        | '+' K_ANTENNAMODEL pin_oxide
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
          }

pin_layer_spacing_opt: /* empty */
        | K_SPACING NUMBER
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
                   defrPin.defiPin::addPortLayerSpacing((int)$2);
                else
                   defrPin.defiPin::addLayerSpacing((int)$2);
              }
            }
          }
        | K_DESIGNRULEWIDTH NUMBER
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
                   defrPin.defiPin::addPortLayerDesignRuleWidth((int)$2);
                else
                   defrPin.defiPin::addLayerDesignRuleWidth((int)$2);
              }
            }
          }

pin_poly_spacing_opt: /* empty */
        | K_SPACING NUMBER
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
                   defrPin.defiPin::addPortPolySpacing((int)$2);
                else
                   defrPin.defiPin::addPolySpacing((int)$2);
              }
            }
          }
        | K_DESIGNRULEWIDTH NUMBER
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
                   defrPin.defiPin::addPortPolyDesignRuleWidth((int)$2);
                else
                   defrPin.defiPin::addPolyDesignRuleWidth((int)$2);
              }
            }
          }

pin_oxide: K_OXIDE1
          { aOxide = 1;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          }
        | K_OXIDE2
          { aOxide = 2;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          }
        | K_OXIDE3
          { aOxide = 3;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          }
        | K_OXIDE4
          { aOxide = 4;
            if (defrPinCbk || defrPinExtCbk)
              defrPin.defiPin::addAntennaModel(aOxide);
          }

use_type: K_SIGNAL
          { $$ = (char*)"SIGNAL"; }
        | K_POWER
          { $$ = (char*)"POWER"; }
        | K_GROUND
          { $$ = (char*)"GROUND"; }
        | K_CLOCK
          { $$ = (char*)"CLOCK"; }
        | K_TIEOFF
          { $$ = (char*)"TIEOFF"; }
        | K_ANALOG
          { $$ = (char*)"ANALOG"; }
        | K_SCAN
          { $$ = (char*)"SCAN"; }
        | K_RESET
          { $$ = (char*)"RESET"; }

pin_layer_opt:
        /* empty */
          { $$ = (char*)""; }
        | K_LAYER {dumb_mode=1;} T_STRING
          { $$ = $3; }

end_pins: K_END K_PINS
	{ 
          if (defrPinEndCbk)
            CALLBACK(defrPinEndCbk, defrPinEndCbkType, 0);
        }

row_rule: K_ROW {dumb_mode = 2; no_num = 2; } T_STRING T_STRING NUMBER NUMBER
      orient
        {
          if (defrRowCbk) {
            rowName = $3;
            defrRow.defiRow::setup($3, $4, $5, $6, $7);
          }
        }
      row_do_option
      row_options ';'
        {
          if (defrRowCbk) 
	    CALLBACK(defrRowCbk, defrRowCbkType, &defrRow);
        }

row_do_option: /* empty */
        {
          if (defVersionNum < 5.6) {
            if (defrRowCbk) {
              if (rowWarnings++ < defrRowWarnings) {
                defError(6523, "The DO statement which is required in ROW statement is missing.");
                CHKERR();
              }
            }
          }
        }
      | K_DO NUMBER K_BY NUMBER row_step_option
        {
          /* 06/05/2002 - pcr 448455 */
          /* Check for 1 and 0 in the correct position */
          /* 07/26/2002 - Commented out due to pcr 459218 */
          if (hasDoStep) {
            /* 04/29/2004 - pcr 695535 */
            /* changed the testing */
            if ((($4 == 1) && (yStep == 0)) ||
                (($2 == 1) && (xStep == 0))) {
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
          if (($2 != 1) && ($4 != 1)) {
            if (defrRowCbk) {
              if (rowWarnings++ < defrRowWarnings) {
                defError(6524, "Syntax error, correct syntax is either \"DO 1 BY num... or DO numX BY 1...\"");
                CHKERR();
              }
            }
          }
          if (defrRowCbk)
            defrRow.defiRow::setDo(ROUND($2), ROUND($4), xStep, yStep);
        }

row_step_option: /* empty */
        {
          hasDoStep = 0;
        }
      | K_STEP NUMBER NUMBER
        {
          hasDoStep = 1;
          defrRow.defiRow::setHasDoStep();
          xStep = $2;
          yStep = $3;
        }

row_options: /* empty */
      | row_options row_option
      ;

row_option : '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1;}
             row_prop_list
         { dumb_mode = 0; parsing_property = 0; }

row_prop_list : /* empty */
       | row_prop_list row_prop
       ;
       
row_prop : T_STRING NUMBER
        {
          if (defrRowCbk) {
             char propTp;
             char* str = ringCopy("                       ");
             propTp =  defrRowProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "ROW");
             /* For backword compatibility, also set the string value */
             /* We will use a temporary string to store the number.
              * The string space is borrowed from the ring buffer
              * in the lexer. */
             sprintf(str, "%g", $2);
             defrRow.defiRow::addNumProperty($1, $2, str, propTp);
          }
        }
      | T_STRING QSTRING
        {
          if (defrRowCbk) {
             char propTp;
             propTp =  defrRowProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "ROW");
             defrRow.defiRow::addProperty($1, $2, propTp);
          }
        }
      | T_STRING T_STRING
        {
          if (defrRowCbk) {
             char propTp;
             propTp =  defrRowProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "ROW");
             defrRow.defiRow::addProperty($1, $2, propTp);
          }
        }

tracks_rule: track_start NUMBER
        {
          if (defrTrackCbk) {
            defrTrack.defiTrack::setup($1);
          }
        }
        K_DO NUMBER K_STEP NUMBER track_layer_statement ';'
        {
          if (($5 <= 0) && (defVersionNum >= 5.4)) {
            if (defrTrackCbk)
              if (trackWarnings++ < defrTrackWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The DO number %g in TRACK is invalid./nThe number value has to be greater than 0", $5);
                defError(6525, defMsg);
                defFree(defMsg);
              }
          }
          if ($7 < 0) {
            if (defrTrackCbk)
              if (trackWarnings++ < defrTrackWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The STEP number %g in TRACK is invalid./nThe number value has to be greater than 0", $7);
                defError(6526, defMsg);
                defFree(defMsg);
              }
          }
          if (defrTrackCbk) {
            defrTrack.defiTrack::setDo(ROUND($2), ROUND($5), $7);
	    CALLBACK(defrTrackCbk, defrTrackCbkType, &defrTrack);
          }
        }

track_start: K_TRACKS track_type
        {
          $$ = $2;
        }

track_type: K_X
            { $$ = (char*)"X";}
        | K_Y
            { $$ = (char*)"Y";}

track_layer_statement: /* empty */
        | K_LAYER { dumb_mode = 1000; } track_layer track_layers
            { dumb_mode = 0; }

track_layers: /* empty */
        | track_layer track_layers 
            ;

track_layer: T_STRING
        {
          if (defrTrackCbk)
            defrTrack.defiTrack::addLayer($1);
        }

gcellgrid: K_GCELLGRID track_type NUMBER 
     K_DO NUMBER K_STEP NUMBER ';'
	{
          if ($5 <= 0) {
            if (defrGcellGridCbk)
              if (gcellGridWarnings++ < defrGcellGridWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The DO number %g in GCELLGRID is invalid./nThe number value has to be greater than 0", $5);
                defError(6527, defMsg);
                defFree(defMsg);
              }
          }
          if ($7 < 0) {
            if (defrGcellGridCbk)
              if (gcellGridWarnings++ < defrGcellGridWarnings) {
                defMsg = (char*)defMalloc(1000);
                sprintf (defMsg,
                   "The STEP number %g in GCELLGRID is invalid./nThe number value has to be greater than 0", $7);
                defError(6528, defMsg);
                defFree(defMsg);
              }
          }
	  if (defrGcellGridCbk) {
	    defrGcellGrid.defiGcellGrid::setup($2, ROUND($3), ROUND($5), $7);
	    CALLBACK(defrGcellGridCbk, defrGcellGridCbkType, &defrGcellGrid);
	  }
	}

extension_section: K_BEGINEXT
        {
          if (defrExtensionCbk)
             CALLBACK(defrExtensionCbk, defrExtensionCbkType, History_text);
        }

extension_stmt: '+' K_BEGINEXT
        { }

via_section: via via_declarations via_end
            ;
        
via: K_VIAS NUMBER ';' 
        {
          if (defrViaStartCbk)
            CALLBACK(defrViaStartCbk, defrViaStartCbkType, ROUND($2));
        }

via_declarations: /* empty */
        | via_declarations via_declaration
            ;

via_declaration: '-' {dumb_mode = 1;no_num = 1; } T_STRING
            {
              if (defrViaCbk) defrVia.defiVia::setup($3);
              viaRule = 0;
            }
        layer_stmts ';'
            {
              if (defrViaCbk)
                CALLBACK(defrViaCbk, defrViaCbkType, &defrVia);
              defrVia.defiVia::clear();
            }

layer_stmts: /* empty */
        | layer_stmts layer_stmt
            ;

layer_stmt: '+' K_RECT {dumb_mode = 1;no_num = 1; } T_STRING pt pt 
            {
              if (defrViaCbk)
                defrVia.defiVia::addLayer($4, $5.x, $5.y, $6.x, $6.y);
            }
        | '+' K_POLYGON { dumb_mode = 1; } T_STRING
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
            }
            firstPt nextPt nextPt otherPts
            {
              if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
                if (defrViaCbk)
                  defrVia.defiVia::addPolygon($4, defrGeomPtr);
              }
            }
        | '+' K_PATTERNNAME {dumb_mode = 1;no_num = 1; } T_STRING
            {
              if (defVersionNum < 5.6) {
                if (defrViaCbk)
                  defrVia.defiVia::addPattern($4);
              } else
                if (defrViaCbk)
                  if (viaWarnings++ < defrViaWarnings)
                    defWarning(7019, "PATTERNNAME is obsoleted in version 5.6 and later.\nDEF parser will ignore this statement."); 
            }
        | '+' K_VIARULE {dumb_mode = 1;no_num = 1; } T_STRING
          '+' K_CUTSIZE NUMBER NUMBER
          '+' K_LAYERS {dumb_mode = 3;no_num = 1; } T_STRING T_STRING T_STRING
          '+' K_CUTSPACING NUMBER NUMBER
          '+' K_ENCLOSURE NUMBER NUMBER NUMBER NUMBER
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
                   defrVia.defiVia::addViaRule($4, (int)$7, (int)$8, $12, $13,
                                             $14, (int)$17, (int)$18, (int)$21,
                                             (int)$22, (int)$23, (int)$24); 
              }
            }
        | layer_viarule_opts
        | extension_stmt
          { 
            if (defrViaExtCbk)
              CALLBACK(defrViaExtCbk, defrViaExtCbkType, History_text);
          }

layer_viarule_opts: '+' K_ROWCOL NUMBER NUMBER
            {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6530, "The ROWCOL statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addRowCol((int)$3, (int)$4);
            }
        | '+' K_ORIGIN NUMBER NUMBER
            {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6531, "The ORIGIN statement has to  be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addOrigin((int)$3, (int)$4);
            }
        | '+' K_OFFSET NUMBER NUMBER NUMBER NUMBER
            {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6532, " The OFFSET statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addOffset((int)$3, (int)$4, (int)$5, (int)$6);
            }
        | '+' K_PATTERN {dumb_mode = 1;no_num = 1; } T_STRING 
            {
              if (!viaRule) {
                if (defrViaCbk) {
                  if (viaWarnings++ < defrViaWarnings) {
                    defError (6533, " The PATTERN statement has to be located in a VIARULE statement");
                    CHKERR();
                  }
                }
              } else if (defrViaCbk)
                 defrVia.defiVia::addCutPattern($4);
            }

firstPt: pt
          { defrGeomPtr->defiGeometries::startList($1.x, $1.y); }

nextPt: pt
          { defrGeomPtr->defiGeometries::addToList($1.x, $1.y); }

otherPts: /* empty */
        | nextPt otherPts
        ;

pt: '(' NUMBER NUMBER ')'
          {
            save_x = $2;
            save_y = $3;
            $$.x = ROUND($2);
            $$.y = ROUND($3);
          }
        | '(' '*' NUMBER ')'
          {
            save_y = $3;
            $$.x = ROUND(save_x);
            $$.y = ROUND($3);
          }
        | '(' NUMBER '*' ')'
          {
            save_x = $2;
            $$.x = ROUND($2);
            $$.y = ROUND(save_y);
          }
        | '(' '*' '*' ')'
          {
            $$.x = ROUND(save_x);
            $$.y = ROUND(save_y);
          }

via_end: K_END K_VIAS
	{ 
          if (defrViaEndCbk)
            CALLBACK(defrViaEndCbk, defrViaEndCbkType, 0);
        }

regions_section: regions_start regions_stmts K_END K_REGIONS
	{
          if (defrRegionEndCbk)
            CALLBACK(defrRegionEndCbk, defrRegionEndCbkType, 0);
        }

regions_start: K_REGIONS NUMBER ';'
	{
          if (defrRegionStartCbk)
            CALLBACK(defrRegionStartCbk, defrRegionStartCbkType, ROUND($2));
        }

regions_stmts: /* empty */
        | regions_stmts regions_stmt
            {}

regions_stmt: '-' { dumb_mode = 1; no_num = 1; } T_STRING
        {
          if (defrRegionCbk)
             defrRegion.defiRegion::setup($3);
          regTypeDef = 0;
        }
     rect_list region_options ';'
        { CALLBACK(defrRegionCbk, defrRegionCbkType, &defrRegion); }

rect_list :
      pt pt
        { if (defrRegionCbk)
	  defrRegion.defiRegion::addRect($1.x, $1.y, $2.x, $2.y); }
      | rect_list pt pt
        { if (defrRegionCbk)
	  defrRegion.defiRegion::addRect($2.x, $2.y, $3.x, $3.y); }
      ;

region_options: /* empty */
      | region_options region_option
      ;

region_option : '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
                region_prop_list
         { dumb_mode = 0; parsing_property = 0; }
      | '+' K_TYPE region_type      /* 5.4.1 */
         {
           if (regTypeDef) {
              if (defrRegionCbk) {
                if (regionWarnings++ < defrRegionWarnings) {
                  defError(6534, "The TYPE statement has already defined in the REGION statement");
                  CHKERR();
                }
              }
           }
           if (defrRegionCbk) defrRegion.defiRegion::setType($3);
           regTypeDef = 1;
         }
      ;

region_prop_list : /* empty */
       | region_prop_list region_prop
       ;
       
region_prop : T_STRING NUMBER
        {
          if (defrRegionCbk) {
             char propTp;
             char* str = ringCopy("                       ");
             propTp = defrRegionProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "REGION");
             /* For backword compatibility, also set the string value */
             /* We will use a temporary string to store the number.
              * The string space is borrowed from the ring buffer
              * in the lexer. */
             sprintf(str, "%g", $2);
             defrRegion.defiRegion::addNumProperty($1, $2, str, propTp);
          }
        }
      | T_STRING QSTRING
        {
          if (defrRegionCbk) {
             char propTp;
             propTp = defrRegionProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "REGION");
             defrRegion.defiRegion::addProperty($1, $2, propTp);
          }
        }
      | T_STRING T_STRING
        {
          if (defrRegionCbk) {
             char propTp;
             propTp = defrRegionProp.defiPropType::propType($1);
             CHKPROPTYPE(propTp, $1, "REGION");
             defrRegion.defiRegion::addProperty($1, $2, propTp);
          }
        }

region_type: K_FENCE
            { $$ = (char*)"FENCE"; }
      | K_GUIDE
            { $$ = (char*)"GUIDE"; }

comps_section: start_comps comps_rule end_comps
            ;

start_comps: K_COMPS NUMBER ';'
         { 
            if (defrComponentStartCbk)
              CALLBACK(defrComponentStartCbk, defrComponentStartCbkType,
                       ROUND($2));
         }

comps_rule: /* empty */
        | comps_rule comp
            ;

comp: comp_start comp_options ';'
	 {
            if (defrComponentCbk)
              CALLBACK(defrComponentCbk, defrComponentCbkType, &defrComponent);
         }

comp_start: comp_id_and_name comp_net_list
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
         }

comp_id_and_name: '-' {dumb_mode = 1000000000; no_num = 10000000; }
       T_STRING T_STRING
         {
            if (defrComponentCbk)
              defrComponent.defiComponent::IdAndName($3, $4);
         }

comp_net_list: /* empty */
        { }
        | comp_net_list '*'
            {
              if (defrComponentCbk)
                defrComponent.defiComponent::addNet("*");
            }
        | comp_net_list T_STRING
            {
              if (defrComponentCbk)
                defrComponent.defiComponent::addNet($2);
            }

comp_options: /* empty */
        |     comp_options comp_option
            ;

comp_option:  comp_generate | comp_source | comp_type | weight |
              comp_foreign | comp_region | comp_eeq | comp_halo |
              comp_routehalo | comp_property | comp_extension_stmt
            ;

comp_extension_stmt: extension_stmt
        {
          if (defrComponentCbk)
            CALLBACK(defrComponentExtCbk, defrComponentExtCbkType,
                     History_text);
        }

comp_eeq: '+' K_EEQMASTER {dumb_mode=1; no_num = 1; } T_STRING
        {
          if (defrComponentCbk)
            defrComponent.defiComponent::setEEQ($4);
        }

comp_generate: '+' K_COMP_GEN { dumb_mode = 2;  no_num = 2; } T_STRING
    opt_pattern
        {
          if (defrComponentCbk)
             defrComponent.defiComponent::setGenerate($4, $5);
        }
opt_pattern :
    /* empty */
      { $$ = (char*)""; }
    | T_STRING
      { $$ = $1; }

comp_source: '+' K_SOURCE source_type 
        {
          if (defrComponentCbk)
            defrComponent.defiComponent::setSource($3);
        }

source_type: K_NETLIST
            { $$ = (char*)"NETLIST"; }
        | K_DIST
            { $$ = (char*)"DIST"; }
        | K_USER
            { $$ = (char*)"USER"; }
        | K_TIMING
            { $$ = (char*)"TIMING"; }


comp_region:
	comp_region_start comp_pnt_list
        { }
        | comp_region_start T_STRING 
        {
	  if (defrComponentCbk)
	    defrComponent.defiComponent::setRegionName($2);
	}

comp_pnt_list: pt pt
	{ 
          /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
          if (defVersionNum < 5.5) {
            if (defrComponentCbk)
	       defrComponent.defiComponent::setRegionBounds($1.x, $1.y, 
                                                            $2.x, $2.y);
          }
          else
            defWarning(7020, "REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
	} 
    | comp_pnt_list pt pt
	{ 
          /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
          if (defVersionNum < 5.5) {
	    if (defrComponentCbk)
	       defrComponent.defiComponent::setRegionBounds($2.x, $2.y,
                                                            $3.x, $3.y);
          }
          else
            defWarning(7020, "REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
	} 

comp_halo: '+' K_HALO                    /* 5.7 */
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
        }
        halo_soft NUMBER NUMBER NUMBER NUMBER 
        {
          if (defrComponentCbk)
            defrComponent.defiComponent::setHalo((int)$5, (int)$6,
                                                 (int)$7, (int)$8);
        }

halo_soft: /* empty */                 /* 5.7 */
    | K_SOFT
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
      }

/* 5.7 */
comp_routehalo: '+' K_ROUTEHALO NUMBER { dumb_mode = 2; no_num = 2; } T_STRING T_STRING
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
             defrComponent.defiComponent::setRouteHalo($3, $5, $6);
        }
      }

comp_property: '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
      comp_prop_list
      { dumb_mode = 0; parsing_property = 0; }

comp_prop_list: comp_prop
    | comp_prop_list comp_prop
          ;

comp_prop: T_STRING NUMBER
        {
          if (defrComponentCbk) {
            char propTp;
            char* str = ringCopy("                       ");
            propTp = defrCompProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "COMPONENT");
            /* For backword compatibility, also set the string value */
            /* We will use a temporary string to store the number.
             * The string space is borrowed from the ring buffer
             * in the lexer. */
            sprintf(str, "%g", $2);
            defrComponent.defiComponent::addNumProperty($1, $2, str, propTp);
          }
        }
     | T_STRING QSTRING
        {
          if (defrComponentCbk) {
            char propTp;
            propTp = defrCompProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "COMPONENT");
            defrComponent.defiComponent::addProperty($1, $2, propTp);
          }
        }
     | T_STRING T_STRING
        {
          if (defrComponentCbk) {
            char propTp;
            propTp = defrCompProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "COMPONENT");
            defrComponent.defiComponent::addProperty($1, $2, propTp);
          }
        }

comp_region_start: '+' K_REGION
        { dumb_mode = 1; no_num = 1; }

comp_foreign: '+' K_FOREIGN { dumb_mode = 1; no_num = 1; } T_STRING
        opt_paren orient
        { 
          if (defVersionNum < 5.6) {
            if (defrComponentCbk) {
              defrComponent.defiComponent::setForeignName($4);
              defrComponent.defiComponent::setForeignLocation($5.x, $5.y, $6);
            }
         } else
            if (defrComponentCbk)
              if (componentWarnings++ < defrComponentWarnings)
                defWarning(7021, "FOREIGN is obsoleted in version 5.6 and later.\nThe DEF parser will ignore this statement.");
         }

opt_paren:
       pt
	 { $$ = $1; }
       | NUMBER NUMBER
	 { $$.x = ROUND($1); $$.y = ROUND($2); }

comp_type: placement_status pt orient
        {
          if (defrComponentCbk) {
            defrComponent.defiComponent::setPlacementStatus($1);
            defrComponent.defiComponent::setPlacementLocation($2.x, $2.y, $3);
          }
        }
        | '+' K_UNPLACED
        {
          if (defrComponentCbk)
            defrComponent.defiComponent::setPlacementStatus(
                                         DEFI_COMPONENT_UNPLACED);
            defrComponent.defiComponent::setPlacementLocation(-1, -1, -1);
        }
        | '+' K_UNPLACED pt orient
        {
          if (defVersionNum < 5.4) {   /* PCR 495463 */
            if (defrComponentCbk) {
              defrComponent.defiComponent::setPlacementStatus(
                                          DEFI_COMPONENT_UNPLACED);
              defrComponent.defiComponent::setPlacementLocation($3.x, $3.y, $4);
            }
          } else {
            if (componentWarnings++ < defrComponentWarnings)
               defWarning(7022, "In the COMPONENT UNPLACED statement, point & orient are invalid in version 5.4 and later.\nDEF parser will ignore this statement.");
            if (defrComponentCbk)
              defrComponent.defiComponent::setPlacementStatus(
                                           DEFI_COMPONENT_UNPLACED);
              defrComponent.defiComponent::setPlacementLocation($3.x, $3.y, $4);
          }
        }

placement_status: '+' K_FIXED 
        { $$ = DEFI_COMPONENT_FIXED; }
        | '+' K_COVER 
        { $$ = DEFI_COMPONENT_COVER; }
        | '+' K_PLACED
        { $$ = DEFI_COMPONENT_PLACED; }

weight: '+' K_WEIGHT NUMBER 
        {
          if (defrComponentCbk)
            defrComponent.defiComponent::setWeight(ROUND($3));
        }

end_comps: K_END K_COMPS
        { 
          if (defrComponentCbk)
            CALLBACK(defrComponentEndCbk, defrComponentEndCbkType, 0);
        }

nets_section:  start_nets net_rules end_nets
        ;

start_nets: K_NETS NUMBER ';'
	{ 
          if (defrNetStartCbk)
            CALLBACK(defrNetStartCbk, defrNetStartCbkType, ROUND($2));
          netOsnet = 1;
        }

net_rules: /* empty */
        | net_rules one_net
            ;

one_net: net_and_connections net_options ';'
	{ 
          if (defrNetCbk)
            CALLBACK(defrNetCbk, defrNetCbkType, &defrNet);
        }
/*
** net_and_connections: net_start {dumb_mode = 1000000000; no_num = 10000000;}
**                      net_connections
** wmd -- this can be used to replace
**        | '(' K_PIN {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')' (???)
*/
net_and_connections: net_start
        {dumb_mode = 0; no_num = 0; }

/* pcr 235555 & 236210 */
net_start: '-' {dumb_mode = 1000000000; no_num = 10000000; nondef_is_keyword = TRUE; mustjoin_is_keyword = TRUE;} net_name 

net_name: T_STRING
        {
          /* 9/22/1999 */
          /* this is shared by both net and special net */
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::setName($1);
          if (defrNetNameCbk)
            CALLBACK(defrNetNameCbk, defrNetNameCbkType, $1);
        } net_connections
        | K_MUSTJOIN '(' T_STRING {dumb_mode = 1; no_num = 1;} T_STRING ')'
        {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addMustPin($3, $5, 0);
          dumb_mode = 3;
          no_num = 3;
        }

net_connections: /* empty */
        | net_connections net_connection 
        ;

net_connection: '(' T_STRING {dumb_mode = 1000000000; no_num = 10000000;}
                T_STRING conn_opt ')'
        {
          /* 9/22/1999 */
          /* since the code is shared by both net & special net, */
          /* need to check on both flags */
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin($2, $4, $5);
          /* 1/14/2000 - pcr 289156 */
          /* reset dumb_mode & no_num to 3 , just in case */
          /* the next statement is another net_connection */
          dumb_mode = 3;
          no_num = 3;
        }
        | '(' '*' {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')'
        {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin("*", $4, $5);
          dumb_mode = 3;
          no_num = 3;
        }
        | '(' K_PIN {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')'
        {
          if (defrNetCbk || defrSNetCbk)
            defrNet.defiNet::addPin("PIN", $4, $5);
          dumb_mode = 3;
          no_num = 3;
        }

conn_opt: /* empty */
          { $$ = 0; }
        | extension_stmt
        {
          if (defrNetConnectionExtCbk)
	    CALLBACK(defrNetConnectionExtCbk, defrNetConnectionExtCbkType,
              History_text);
          $$ = 0;
        }
        | '+' K_SYNTHESIZED
        { $$ = 1; }

/* These are all the optional fields for a net that go after the '+' */
net_options: /* empty */
        | net_options net_option
        ;

net_option: '+' net_type { dumb_mode = 1; }
        {  
          if (defrNetCbk) defrNet.defiNet::addWire($2, NULL);
        }
        paths
        {
          by_is_keyword = FALSE;
          do_is_keyword = FALSE;
          new_is_keyword = FALSE;
          nondef_is_keyword = FALSE;
          mustjoin_is_keyword = FALSE;
          step_is_keyword = FALSE;
          orient_is_keyword = FALSE;
          needNPCbk = 0;
        }

        | '+' K_SOURCE netsource_type
        { if (defrNetCbk) defrNet.defiNet::setSource($3); }

        | '+' K_FIXEDBUMP
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
        } 

        | '+' K_FREQUENCY { real_num = 1; } NUMBER
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
          if (defrNetCbk) defrNet.defiNet::setFrequency($4);
          real_num = 0;
        }

        | '+' K_ORIGINAL {dumb_mode = 1; no_num = 1;} T_STRING
        { if (defrNetCbk) defrNet.defiNet::setOriginal($4); }

        | '+' K_PATTERN pattern_type
        { if (defrNetCbk) defrNet.defiNet::setPattern($3); }

        | '+' K_WEIGHT NUMBER
        { if (defrNetCbk) defrNet.defiNet::setWeight(ROUND($3)); }

        | '+' K_XTALK NUMBER
        { if (defrNetCbk) defrNet.defiNet::setXTalk(ROUND($3)); }

        | '+' K_ESTCAP NUMBER
        { if (defrNetCbk) defrNet.defiNet::setCap($3); }

        | '+' K_USE use_type 
        { if (defrNetCbk) defrNet.defiNet::setUse($3); }

        | '+' K_STYLE NUMBER
        { if (defrNetCbk) defrNet.defiNet::setStyle((int)$3); }

        | '+' K_NONDEFAULTRULE { dumb_mode = 1; no_num = 1; } T_STRING
        { 
          if (defrNetCbk && defrNetNonDefaultRuleCbk) {
             /* User wants a callback on nondefaultrule */
             CALLBACK(defrNetNonDefaultRuleCbk,
                      defrNetNonDefaultRuleCbkType, $4);
          }
          /* Still save data in the class */
          if (defrNetCbk) defrNet.defiNet::setNonDefaultRule($4);
        }

        | vpin_stmt

        | '+' K_SHIELDNET { dumb_mode = 1; no_num = 1; } T_STRING
        { if (defrNetCbk) defrNet.defiNet::addShieldNet($4); }

        | '+' K_NOSHIELD { dumb_mode = 1; no_num = 1; }
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
        }
        paths
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
        }

        | '+' K_SUBNET
        { dumb_mode = 1; no_num = 1;
          if (defrNetCbk) {
            defrSubnet = (defiSubnet*)defMalloc(sizeof(defiSubnet));
            defrSubnet->defiSubnet::Init();
          }
        }
        T_STRING {
          if (defrNetCbk && defrNetSubnetNameCbk) {
            /* User wants a callback on Net subnetName */
            CALLBACK(defrNetSubnetNameCbk, defrNetSubnetNameCbkType, $4);
          }
          /* Still save the subnet name in the class */
          if (defrNetCbk) {
            defrSubnet->defiSubnet::setName($4);
          }
        } 
        comp_names {
          routed_is_keyword = TRUE;
          fixed_is_keyword = TRUE;
          cover_is_keyword = TRUE;
        } subnet_options {
          if (defrNetCbk) {
            defrNet.defiNet::addSubnet(defrSubnet);
            defrSubnet = 0;
            routed_is_keyword = FALSE;
            fixed_is_keyword = FALSE;
            cover_is_keyword = FALSE;
          }
        }

        | '+' K_PROPERTY {dumb_mode = 10000000; parsing_property = 1; }
          net_prop_list
        { dumb_mode = 0; parsing_property = 0; }

        | extension_stmt
        { 
          if (defrNetExtCbk)
            CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text);
        }

net_prop_list: net_prop
      | net_prop_list net_prop
      ;

net_prop: T_STRING NUMBER
        {
          if (defrNetCbk) {
            char propTp;
            char* str = ringCopy("                       ");
            propTp = defrNetProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "NET");
            /* For backword compatibility, also set the string value */
            /* We will use a temporary string to store the number.
             * The string space is borrowed from the ring buffer
             * in the lexer. */
            sprintf(str, "%g", $2);
            defrNet.defiNet::addNumProp($1, $2, str, propTp);
          }
        }
        | T_STRING QSTRING
        {
          if (defrNetCbk) {
            char propTp;
            propTp = defrNetProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "NET");
            defrNet.defiNet::addProp($1, $2, propTp);
          }
        }
        | T_STRING T_STRING
        {
          if (defrNetCbk) {
            char propTp;
            propTp = defrNetProp.defiPropType::propType($1);
            CHKPROPTYPE(propTp, $1, "NET");
            defrNet.defiNet::addProp($1, $2, propTp);
          }
        }

netsource_type: K_NETLIST
        { $$ = (char*)"NETLIST"; }
        | K_DIST
        { $$ = (char*)"DIST"; }
        | K_USER
        { $$ = (char*)"USER"; }
        | K_TIMING
        { $$ = (char*)"TIMING"; }
        | K_TEST
        { $$ = (char*)"TEST"; }

vpin_stmt: vpin_begin vpin_layer_opt pt pt 
        {
          /* vpin_options may have to deal with orient */
          orient_is_keyword = TRUE;
        }
        vpin_options
        { if (defrNetCbk)
            defrNet.defiNet::addVpinBounds($3.x, $3.y, $4.x, $4.y);
          orient_is_keyword = FALSE;
        }

vpin_begin: '+' K_VPIN {dumb_mode = 1; no_num = 1;} T_STRING
        { if (defrNetCbk) defrNet.defiNet::addVpin($4); }

vpin_layer_opt: /* empty */
        | K_LAYER {dumb_mode=1;} T_STRING
        { if (defrNetCbk) defrNet.defiNet::addVpinLayer($3); }

vpin_options: /* empty */
        | vpin_status pt orient
        { if (defrNetCbk) defrNet.defiNet::addVpinLoc($1, $2.x, $2.y, $3); }

vpin_status: K_PLACED 
        { $$ = (char*)"PLACED"; }
        | K_FIXED 
        { $$ = (char*)"FIXED"; }
        | K_COVER
        { $$ = (char*)"COVER"; }

net_type: K_FIXED
        { $$ = (char*)"FIXED"; }
        | K_COVER
        { $$ = (char*)"COVER"; }
        | K_ROUTED
        { $$ = (char*)"ROUTED"; }

paths:
    path   // not necessary to do partial callback for net yet
      { if (defrNeedPathData && defrNetCbk)
          pathIsDone(shield, 0, netOsnet, &needNPCbk);
      }
    | paths new_path
      { }

new_path: K_NEW { dumb_mode = 1; } path
      { if (defrNeedPathData && defrNetCbk)
          pathIsDone(shield, 0, netOsnet, &needNPCbk);
      }

path:  T_STRING
      {
        /* 9/26/2002 - pcr 449514
        Check if $1 is equal to TAPER or TAPERRULE, layername
        is missing */
        if ((strcmp($1, "TAPER") == 0) || (strcmp($1, "TAPERRULE") == 0)) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defError(6531, "layerName which is required in path is missing");
              CHKERR();
            }
          }
          /* Since there is already error, the next token is insignificant */
          dumb_mode = 1; no_num = 1;
        } else {
          if (defrNeedPathData) defrPath->defiPath::addLayer($1);
          dumb_mode = 0; no_num = 0;
        }
      }
    opt_taper_style_s path_pt
      { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
/*
       dumb_mode = 1; by_is_keyword = TRUE; do_is_keyword = TRUE;
*/
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; }
    path_item_list
      { dumb_mode = 0; }

path_item_list: /* empty */
    | path_item_list path_item
    ;

path_item:
    T_STRING
      {
        if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
          if (strcmp($1, "TAPER") == 0)
            defrPath->defiPath::setTaper();
          else 
            defrPath->defiPath::addVia($1);
        }
      }
    | T_STRING orient
      { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
            defrPath->defiPath::addVia($1);
            defrPath->defiPath::addViaRotation($2);
        }
      }
    | T_STRING K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
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
        if (($3 == 0) || ($5 == 0)) {
          if (defrNeedPathData) {
            if (netWarnings++ < defrNetWarnings) {
              defError(6533, "Either the numX or numY in the VIA DO statement has the value 0.\nThis is illegal.");
              CHKERR();
            }
          }
        }
        if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
            defrPath->defiPath::addVia($1);
            defrPath->defiPath::addViaData((int)$3, (int)$5, (int)$7, (int)$8);
        }
      }
    | path_pt
      { // reset dumb_mode to 1 just incase the next token is a via of the path
        // 2/5/2004 - pcr 686781
        dumb_mode = 100000; by_is_keyword = TRUE; do_is_keyword = TRUE;
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; }


path_pt :
    '(' NUMBER NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND($2), ROUND($3)); 
	save_x = $2;
	save_y = $3;
      }
    | '(' '*' NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND(save_x), ROUND($3)); 
	save_y = $3;
      }
    | '(' NUMBER '*' ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND($2), ROUND(save_y)); 
	save_x = $2;
      }
    | '(' '*' '*' ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addPoint(ROUND(save_x), ROUND(save_y)); 
      }
    | '(' NUMBER NUMBER NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND($2), ROUND($3), ROUND($4)); 
	save_x = $2;
	save_y = $3;
      }
    | '(' '*' NUMBER NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND(save_x), ROUND($3),
	  ROUND($4)); 
	save_y = $3;
      }
    | '(' NUMBER '*' NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND($2), ROUND(save_y),
	  ROUND($4)); 
	save_x = $2;
      }
    | '(' '*' '*' NUMBER ')'
      {
	if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
	  defrPath->defiPath::addFlushPoint(ROUND(save_x), ROUND(save_y),
	  ROUND($4)); 
      }

opt_taper_style_s: /* empty */
    | opt_taper_style_s opt_taper_style
    ;
opt_taper_style: opt_style
    | opt_taper
    ;

opt_taper: K_TAPER
      { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) defrPath->defiPath::setTaper(); }
    | K_TAPERRULE { dumb_mode = 1; } T_STRING
      { if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) defrPath->defiPath::addTaperRule($3); }

opt_style: K_STYLE NUMBER
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
             defrPath->defiPath::addStyle((int)$2);
      }

opt_spaths: /* empty */
    | opt_spaths opt_shape_style
    ;

opt_shape_style:
    '+' K_SHAPE shape_type
      { if (defrNeedPathData && (defrNetCbk || defrSNetCbk))
          defrPath->defiPath::addShape($3); }
    | '+' K_STYLE NUMBER
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
            defrPath->defiPath::addStyle((int)$3);
        }
      }

end_nets: K_END K_NETS 
	  { 
            CALLBACK(defrNetEndCbk, defrNetEndCbkType, 0);
            netOsnet = 0;
          }

shape_type: K_RING
            { $$ = (char*)"RING"; }
        | K_STRIPE
            { $$ = (char*)"STRIPE"; }
        | K_FOLLOWPIN
            { $$ = (char*)"FOLLOWPIN"; }
        | K_IOWIRE
            { $$ = (char*)"IOWIRE"; }
        | K_COREWIRE
            { $$ = (char*)"COREWIRE"; }
        | K_BLOCKWIRE
            { $$ = (char*)"BLOCKWIRE"; }
        | K_FILLWIRE
            { $$ = (char*)"FILLWIRE"; }
        | K_FILLWIREOPC                         /* 5.7 */
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
              $$ = (char*)"FILLWIREOPC";
            }
        | K_DRCFILL
            { $$ = (char*)"DRCFILL"; }
        | K_BLOCKAGEWIRE
            { $$ = (char*)"BLOCKAGEWIRE"; }
        | K_PADRING
            { $$ = (char*)"PADRING"; }
        | K_BLOCKRING
            { $$ = (char*)"BLOCKRING"; }

snets_section :  start_snets snet_rules end_snets
            ;

snet_rules: /* empty */
        | snet_rules snet_rule
            ;

snet_rule: net_and_connections snet_options ';'
	{ CALLBACK(defrSNetCbk, defrSNetCbkType, &defrNet); }

snet_options: /* empty */
        | snet_options snet_option
            ;

snet_option: snet_width | snet_voltage | 
             snet_spacing | snet_other_option
             ;

snet_other_option: '+' net_type
            {
             if (defrSNetCbk) {   /* PCR 902306 */
               defMsg = (char*)defMalloc(1024);
               sprintf(defMsg, "The SPECIAL NET statement, with type %s, does not have any routine statement defined.\nThe DEF parser will ignore this statemnet.", $2);
               defWarning(7023, defMsg);
               defFree(defMsg);
             }
            }
        |  '+' net_type { dumb_mode = 1; }
            {
            if (defrSNetCbk) defrNet.defiNet::addWire($2, NULL);
            }
            spaths
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
            }
 
        | '+' K_SHIELD { dumb_mode = 1; no_num = 1; } T_STRING
            { shieldName = $4; }
            shield_layer

        | '+' K_POLYGON { dumb_mode = 1; } T_STRING
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
          }
          firstPt nextPt nextPt otherPts
          {
            if (defVersionNum >= 5.6) {  // only add if 5.6 or beyond
              if (defrSNetCbk) {
                // needSNPCbk will indicate that it has reach the max
                // memory and if user has set partialPathCBk, def parser
                // will make the callback.
                // This will improve performance
                // This construct is only in specialnet
                defrNet.defiNet::addPolygon($4, defrGeomPtr, &needSNPCbk);
                if (needSNPCbk && defrSNetPartialPathCbk) {
                   CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                            &defrNet);
                   defrNet.defiNet::clearRectPolyNPath();
                }
              }
            }
          }

        | '+' K_RECT { dumb_mode = 1; } T_STRING pt pt
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
              defrNet.defiNet::addRect($4, $5.x, $5.y, $6.x, $6.y, &needSNPCbk);
              if (needSNPCbk && defrSNetPartialPathCbk) {
                 CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
                          &defrNet);
                 defrNet.defiNet::clearRectPolyNPath();
              }
            }
          }
 
        | '+' K_SOURCE source_type
            { if (defrSNetCbk) defrNet.defiNet::setSource($3); }

        | '+' K_FIXEDBUMP
            { if (defrSNetCbk) defrNet.defiNet::setFixedbump(); }
 
        | '+' K_FREQUENCY NUMBER
            { if (defrSNetCbk) defrNet.defiNet::setFrequency($3); }

        | '+' K_ORIGINAL {dumb_mode = 1; no_num = 1;} T_STRING
            { if (defrSNetCbk) defrNet.defiNet::setOriginal($4); }
 
        | '+' K_PATTERN pattern_type
            { if (defrSNetCbk) defrNet.defiNet::setPattern($3); }
 
        | '+' K_WEIGHT NUMBER
            { if (defrSNetCbk) defrNet.defiNet::setWeight(ROUND($3)); }
 
        | '+' K_ESTCAP NUMBER
            { 
              /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
              if (defVersionNum < 5.5)
                 if (defrSNetCbk) defrNet.defiNet::setCap($3);
              else
                 defWarning(7024, "ESTCAP is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
            }
 
        | '+' K_USE use_type
            { if (defrSNetCbk) defrNet.defiNet::setUse($3); }
 
        | '+' K_STYLE NUMBER
            { if (defrSNetCbk) defrNet.defiNet::setStyle((int)$3); }
 
        | '+' K_PROPERTY {dumb_mode = 10000000; parsing_property = 1; }
          snet_prop_list
            { dumb_mode = 0; parsing_property = 0; }
 
        | extension_stmt
          { CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text); }

shield_layer: /* empty */  /* PCR 902306 */
            {
             if (defrSNetCbk) {
               defMsg = (char*)defMalloc(1024);
               sprintf(defMsg, "The SPECIAL NET SHIELD statement, does not have any routine statement defined.\nThe DEF parser will ignore this statemnet.");
               defWarning(7025, defMsg);
               defFree(defMsg);
             }
            }
        |
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
            }
            spaths
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
            }

snet_width: '+' K_WIDTH { dumb_mode = 1; } T_STRING NUMBER
            {
              /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
              if (defVersionNum < 5.5)
                 if (defrSNetCbk) defrNet.defiNet::setWidth($4, $5);
              else
                 defWarning(7026, "WIDTH is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
            }

snet_voltage: '+' K_VOLTAGE  { dumb_mode = 1; no_num = 1; } T_STRING
            {
              if (numIsInt($4)) {
                 if (defrSNetCbk) defrNet.defiNet::setVoltage(atoi($4));
              } else {
                 if (defrSNetCbk) {
                   if (sNetWarnings++ < defrSNetWarnings) {
                     defMsg = (char*)defMalloc(1000);
                     sprintf (defMsg,
                        "The value %s for statement VOLTAGE is not an integer.\nThe syntax requires an integer in units of millivolts", $4);
                     defError(6537, defMsg);
                     defFree(defMsg);
                     CHKERR();
                   }
                 }
              }
            }

snet_spacing: '+' K_SPACING { dumb_mode = 1; } T_STRING NUMBER
            {
              if (defrSNetCbk) defrNet.defiNet::setSpacing($4,$5);
            }
        opt_snet_range
            {
            }

snet_prop_list: snet_prop
      | snet_prop_list snet_prop
      ;

snet_prop: T_STRING NUMBER
            {
              if (defrSNetCbk) {
                char propTp;
                char* str = ringCopy("                       ");
                propTp = defrSNetProp.defiPropType::propType($1);
                CHKPROPTYPE(propTp, $1, "SPECIAL NET");
                /* For backword compatibility, also set the string value */
                /* We will use a temporary string to store the number.
                 * The string space is borrowed from the ring buffer
                 * in the lexer. */
                sprintf(str, "%g", $2);
                defrNet.defiNet::addNumProp($1, $2, str, propTp);
              }
            }
         | T_STRING QSTRING
            {
              if (defrSNetCbk) {
                char propTp;
                propTp = defrSNetProp.defiPropType::propType($1);
                CHKPROPTYPE(propTp, $1, "SPECIAL NET");
                defrNet.defiNet::addProp($1, $2, propTp);
              }
            }
         | T_STRING T_STRING
            {
              if (defrSNetCbk) {
                char propTp;
                propTp = defrSNetProp.defiPropType::propType($1);
                CHKPROPTYPE(propTp, $1, "SPECIAL NET");
                defrNet.defiNet::addProp($1, $2, propTp);
              }
            }

opt_snet_range: /* nothing */
        | K_RANGE NUMBER NUMBER
            {
              if (defrSNetCbk) defrNet.defiNet::setRange($2,$3);
            }

opt_range: /* nothing */
        | K_RANGE NUMBER NUMBER
            { defrProp.defiProp::setRange($2, $3); }

pattern_type: K_BALANCED
            { $$ = (char*)"BALANCED"; }
        | K_STEINER
            { $$ = (char*)"STEINER"; }
        | K_TRUNK
            { $$ = (char*)"TRUNK"; }
        | K_WIREDLOGIC
            { $$ = (char*)"WIREDLOGIC"; }

spaths:
    spath
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
      }
    | spaths snew_path
      { }

snew_path: K_NEW { dumb_mode = 1; } spath
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
      }

spath:  T_STRING
      { if (defrNeedPathData && defrSNetCbk) defrPath->defiPath::addLayer($1);
        dumb_mode = 0; no_num = 0;
      }
    width opt_spaths path_pt
/*
      { dumb_mode = 1; new_is_keyword = TRUE; }
*/
      { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
        new_is_keyword = TRUE; step_is_keyword = TRUE;
        orient_is_keyword = TRUE; }

    path_item_list
      { dumb_mode = 0; }

width: NUMBER
      { if (defrNeedPathData && defrSNetCbk)
          defrPath->defiPath::addWidth(ROUND($1));
      }

start_snets: K_SNETS NUMBER ';'
      { 
        if (defrSNetStartCbk)
          CALLBACK(defrSNetStartCbk, defrSNetStartCbkType, ROUND($2));
        netOsnet = 2;
      }

end_snets: K_END K_SNETS 
      { 
        if (defrSNetEndCbk)
          CALLBACK(defrSNetEndCbk, defrSNetEndCbkType, 0);
        netOsnet = 0;
      }

groups_section: groups_start group_rules groups_end
      ;

groups_start: K_GROUPS NUMBER ';'
      {
        if (defrGroupsStartCbk)
           CALLBACK(defrGroupsStartCbk, defrGroupsStartCbkType, ROUND($2));
      }

group_rules: /* empty */
      | group_rules group_rule
      ;

group_rule: start_group group_members group_options ';'
      {
        if (defrGroupCbk)
           CALLBACK(defrGroupCbk, defrGroupCbkType, &defrGroup);
      }

start_group: '-' { dumb_mode = 1; no_num = 1; } T_STRING 
      {
        dumb_mode = 1000000000;
        no_num = 1000000000;
        /* dumb_mode is automatically turned off at the first
         * + in the options or at the ; at the end of the group */
        if (defrGroupCbk) defrGroup.defiGroup::setup($3);
        if (defrGroupNameCbk)
           CALLBACK(defrGroupNameCbk, defrGroupNameCbkType, $3);
      }

group_members: 
      | group_members group_member
      {  }

group_member: T_STRING
      {
        /* if (defrGroupCbk) defrGroup.defiGroup::addMember($1); */
        if (defrGroupMemberCbk)
          CALLBACK(defrGroupMemberCbk, defrGroupMemberCbkType, $1);
      }

group_options: /* empty */
      | group_options group_option 
      ;

group_option: '+' K_SOFT group_soft_options
      { }
      |     '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
            group_prop_list
      { dumb_mode = 0; parsing_property = 0; }
      |     '+' K_REGION { dumb_mode = 1;  no_num = 1; } group_region
      { }
      | extension_stmt
      { 
        if (defrGroupMemberCbk)
          CALLBACK(defrGroupExtCbk, defrGroupExtCbkType, History_text);
      }

group_region: pt pt
      {
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5) {
          if (defrGroupCbk)
            defrGroup.defiGroup::addRegionRect($1.x, $1.y, $2.x, $2.y);
        }
        else
          defWarning(7027, "GROUP REGION pt pt is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      }
      | T_STRING
      { if (defrGroupCbk)
          defrGroup.defiGroup::setRegionName($1);
      }

group_prop_list : /* empty */
      | group_prop_list group_prop
      ;

group_prop : T_STRING NUMBER
      {
        if (defrGroupCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrGroupProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "GROUP");
          /* For backword compatibility, also set the string value */
          /* We will use a temporary string to store the number.
           * The string space is borrowed from the ring buffer
           * in the lexer. */
          sprintf(str, "%g", $2);
          defrGroup.defiGroup::addNumProperty($1, $2, str, propTp);
        }
      }
      | T_STRING QSTRING
      {
        if (defrGroupCbk) {
          char propTp;
          propTp = defrGroupProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "GROUP");
          defrGroup.defiGroup::addProperty($1, $2, propTp);
        }
      }
      | T_STRING T_STRING
      {
        if (defrGroupCbk) {
          char propTp;
          propTp = defrGroupProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "GROUP");
          defrGroup.defiGroup::addProperty($1, $2, propTp);
        }
      }

group_soft_options: /* empty */
      | group_soft_options group_soft_option 
      { }

group_soft_option: K_MAXX NUMBER
      {
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setMaxX(ROUND($2));
        else
          defWarning(7028, "GROUP SOFT MAXX is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      }
      | K_MAXY NUMBER
      { 
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setMaxY(ROUND($2));
        else
          defWarning(7029, "GROUP SOFT MAXY is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      }
      | K_MAXHALFPERIMETER NUMBER
      { 
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        if (defVersionNum < 5.5)
          if (defrGroupCbk) defrGroup.defiGroup::setPerim(ROUND($2));
        else
          defWarning(7030, "GROUP SOFT MAXHALFPERIMETER is obsoleted in version 5.5 and later.\nThe DEF parser will ignore this statement.");
      }

groups_end: K_END K_GROUPS 
      { 
        if (defrGroupsEndCbk)
          CALLBACK(defrGroupsEndCbk, defrGroupsEndCbkType, 0);
      }

/* 8/31/2001 - This is obsolete in 5.4 */
assertions_section: assertions_start constraint_rules assertions_end
      ;

/* 8/31/2001 - This is obsolete in 5.4 */
constraint_section: constraints_start constraint_rules constraints_end
      ;

assertions_start: K_ASSERTIONS NUMBER ';'
      {
        if ((defVersionNum < 5.4) && (defrAssertionsStartCbk)) {
          CALLBACK(defrAssertionsStartCbk, defrAssertionsStartCbkType,
	           ROUND($2));
        } else {
          if (defrAssertionCbk)
            if (assertionWarnings++ < defrAssertionWarnings)
              defWarning(7031, "ASSERTIONS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
        if (defrAssertionCbk)
          defrAssertion.defiAssertion::setAssertionMode();
      }

constraints_start: K_CONSTRAINTS NUMBER ';'
      {
        if ((defVersionNum < 5.4) && (defrConstraintsStartCbk)) {
          CALLBACK(defrConstraintsStartCbk, defrConstraintsStartCbkType,
                   ROUND($2));
        } else {
          if (defrConstraintCbk)
            if (constraintWarnings++ < defrConstraintWarnings)
              defWarning(7032, "CONSTRAINTS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
        if (defrConstraintCbk)
          defrAssertion.defiAssertion::setConstraintMode();
      }

constraint_rules: /* empty */
      | constraint_rules constraint_rule 
      ;

constraint_rule:   operand_rule 
      | wiredlogic_rule 
      {
        if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
          if (defrAssertion.defiAssertion::isConstraint()) 
            CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion);
          if (defrAssertion.defiAssertion::isAssertion()) 
            CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion);
        }
      }

operand_rule: '-' operand delay_specs ';'
      { 
        if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
          if (defrAssertion.defiAssertion::isConstraint()) 
	    CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion);
          if (defrAssertion.defiAssertion::isAssertion()) 
            CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion);
        }
   
        // reset all the flags and everything
        defrAssertion.defiAssertion::clear();
      }

operand: K_NET { dumb_mode = 1; no_num = 1; } T_STRING 
      {
         if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::addNet($3);
      }
      | K_PATH {dumb_mode = 4; no_num = 4;} T_STRING T_STRING T_STRING T_STRING
      {
         if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::addPath($3, $4, $5, $6);
      }
      | K_SUM  '(' operand_list ')'
      {
        if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::setSum();
      }
      | K_DIFF '(' operand_list ')'
      {
        if (defrConstraintCbk || defrAssertionCbk)
           defrAssertion.defiAssertion::setDiff();
      }

operand_list: operand 
      | operand_list ',' operand
      { }

wiredlogic_rule: '-' K_WIREDLOGIC { dumb_mode = 1; no_num = 1; } T_STRING
      opt_plus K_MAXDIST NUMBER ';'
      {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setWiredlogic($4, $7);
      }

opt_plus:
      /* empty */
      { $$ = (char*)""; }
      | '+'
      { $$ = (char*)"+"; }

delay_specs: /* empty */
      | delay_specs delay_spec
      ;

delay_spec: '+' K_RISEMIN NUMBER 
      {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setRiseMin($3);
      }
      | '+' K_RISEMAX NUMBER 
      {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setRiseMax($3);
      }
      | '+' K_FALLMIN NUMBER 
      {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setFallMin($3);
      }
      | '+' K_FALLMAX NUMBER 
      {
        if (defrConstraintCbk || defrAssertionCbk)
          defrAssertion.defiAssertion::setFallMax($3);
      }

constraints_end: K_END K_CONSTRAINTS
      { if ((defVersionNum < 5.4) && defrConstraintsEndCbk) {
          CALLBACK(defrConstraintsEndCbk, defrConstraintsEndCbkType, 0);
        } else {
          if (defrConstraintsEndCbk) {
            if (constraintWarnings++ < defrConstraintWarnings)
              defWarning(7032, "CONSTRAINTS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
          }
        }
      }

assertions_end: K_END K_ASSERTIONS
      { if ((defVersionNum < 5.4) && defrAssertionsEndCbk) {
          CALLBACK(defrAssertionsEndCbk, defrAssertionsEndCbkType, 0);
        } else {
          if (defrAssertionsEndCbk) {
            if (assertionWarnings++ < defrAssertionWarnings)
              defWarning(7031, "ASSERTIONS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
          }
        }
      }

scanchains_section: scanchain_start scanchain_rules scanchain_end
      ;

scanchain_start: K_SCANCHAINS NUMBER ';'
      { if (defrScanchainsStartCbk)
          CALLBACK(defrScanchainsStartCbk, defrScanchainsStartCbkType,
                   ROUND($2));
      }

scanchain_rules: /* empty */
      | scanchain_rules scan_rule
      {}

scan_rule: start_scan scan_members ';' 
      { 
        if (defrScanchainCbk)
          CALLBACK(defrScanchainCbk, defrScanchainCbkType, &defrScanchain);
      }

start_scan: '-' {dumb_mode = 1; no_num = 1;} T_STRING 
      {
        if (defrScanchainCbk)
          defrScanchain.defiScanchain::setName($3);
        bit_is_keyword = TRUE;
      }

scan_members: 
      | scan_members scan_member
      ;

opt_pin :
      /* empty */
      { $$ = (char*)""; }
      | T_STRING
      { $$ = $1; }

scan_member: '+' K_START {dumb_mode = 2; no_num = 2;} T_STRING opt_pin
      { if (defrScanchainCbk)
          defrScanchain.defiScanchain::setStart($4, $5);
      }
      | '+' K_FLOATING { dumb_mode = 1; no_num = 1; } floating_inst_list
      { dumb_mode = 0; no_num = 0; }
      | '+' K_ORDERED
      {
         dumb_mode = 1;
         no_num = 1;
         if (defrScanchainCbk)
           defrScanchain.defiScanchain::addOrderedList();
      }
      ordered_inst_list
      { dumb_mode = 0; no_num = 0; }
      | '+' K_STOP {dumb_mode = 2; no_num = 2; } T_STRING opt_pin
      { if (defrScanchainCbk)
          defrScanchain.defiScanchain::setStop($4, $5);
      }
      | '+' K_COMMONSCANPINS { dumb_mode = 10; no_num = 10; } opt_common_pins
      { dumb_mode = 0;  no_num = 0; }
      | '+' K_PARTITION { dumb_mode = 1; no_num = 1; } T_STRING  /* 5.5 */
      partition_maxbits
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
          defrScanchain.defiScanchain::setPartition($4, $5);
      }
      | extension_stmt
      {
        if (defrScanChainExtCbk)
          CALLBACK(defrScanChainExtCbk, defrScanChainExtCbkType, History_text);
      }

opt_common_pins: /* empty */
      { }
      | '(' T_STRING T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut($3);
	}
      }
      | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut($3);
	  if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
	    defrScanchain.defiScanchain::setCommonIn($7);
	  else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
	    defrScanchain.defiScanchain::setCommonOut($7);
	}
      }

floating_inst_list: /* empty */
      | floating_inst_list one_floating_inst
      ;

one_floating_inst: T_STRING
      {
	dumb_mode = 1000;
	no_num = 1000;
	if (defrScanchainCbk)
	  defrScanchain.defiScanchain::addFloatingInst($1);
      }
      floating_pins
      { dumb_mode = 1; no_num = 1; }

floating_pins: /* empty */ 
      { }
      | '(' T_STRING  T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($3);
          else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
            defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      }
      | '(' T_STRING  T_STRING ')' '(' T_STRING  T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($3);
	  else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($7);
	  else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($7);
	  else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
            bitsNum = atoi($7);
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      }
      | '(' T_STRING  T_STRING ')' '(' T_STRING  T_STRING ')' '(' T_STRING
      T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($3);
	  else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($7);
	  else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($7);
	  else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
            bitsNum = atoi($7);
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	  if (strcmp($10, "IN") == 0 || strcmp($10, "in") == 0)
	    defrScanchain.defiScanchain::addFloatingIn($11);
	  else if (strcmp($10, "OUT") == 0 || strcmp($10, "out") == 0)
	    defrScanchain.defiScanchain::addFloatingOut($11);
	  else if (strcmp($10, "BITS") == 0 || strcmp($10, "bits") == 0) {
            bitsNum = atoi($11);
	    defrScanchain.defiScanchain::setFloatingBits(bitsNum);
          }
	}
      }
    
ordered_inst_list: /* empty */
      | ordered_inst_list one_ordered_inst
      ;

one_ordered_inst: T_STRING
      { dumb_mode = 1000; no_num = 1000; 
	if (defrScanchainCbk)
	  defrScanchain.defiScanchain::addOrderedInst($1);
      }
      ordered_pins
      { dumb_mode = 1; no_num = 1; }

ordered_pins: /* empty */ 
      { }
      | '(' T_STRING  T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($3);
          else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
         }
	}
      }
      | '(' T_STRING  T_STRING ')' '(' T_STRING  T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($3);
	  else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($7);
	  else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($7);
	  else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
            bitsNum = atoi($7);
            defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	}
      }
      | '(' T_STRING  T_STRING ')' '(' T_STRING  T_STRING ')' '(' T_STRING
      T_STRING ')'
      {
	if (defrScanchainCbk) {
	  if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($3);
	  else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($3);
	  else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
            bitsNum = atoi($3);
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($7);
	  else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($7);
	  else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
            bitsNum = atoi($7);
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	  if (strcmp($10, "IN") == 0 || strcmp($10, "in") == 0)
	    defrScanchain.defiScanchain::addOrderedIn($11);
	  else if (strcmp($10, "OUT") == 0 || strcmp($10, "out") == 0)
	    defrScanchain.defiScanchain::addOrderedOut($11);
	  else if (strcmp($10, "BITS") == 0 || strcmp($10, "bits") == 0) {
            bitsNum = atoi($11);
	    defrScanchain.defiScanchain::setOrderedBits(bitsNum);
          }
	}
      }
    
partition_maxbits: /* empty */
      { $$ = -1; }
      | K_MAXBITS NUMBER
      { $$ = ROUND($2); }
    
scanchain_end: K_END K_SCANCHAINS
      { 
        if (defrScanchainsEndCbk)
          CALLBACK(defrScanchainsEndCbk, defrScanchainsEndCbkType, 0);
        bit_is_keyword = FALSE;
        dumb_mode = 0; no_num = 0;
      }

/* 8/31/2001 - This is obsolete in 5.4 */
iotiming_section: iotiming_start iotiming_rules iotiming_end
      ;

iotiming_start: K_IOTIMINGS NUMBER ';'
      {
        if (defVersionNum < 5.4 && defrIOTimingsStartCbk) {
          CALLBACK(defrIOTimingsStartCbk, defrIOTimingsStartCbkType, ROUND($2));
        } else {
          if (defrIOTimingsStartCbk)
            if (iOTimingWarnings++ < defrIOTimingWarnings)
              defWarning(7035, "IOTIMINGS is obsoleted in version 5.4 and later.\nThe DEF parser will ignore this statement.");
        }
      }

iotiming_rules: /* empty */
      | iotiming_rules iotiming_rule
      { }

iotiming_rule: start_iotiming iotiming_members ';' 
      { 
        if (defVersionNum < 5.4 && defrIOTimingCbk)
          CALLBACK(defrIOTimingCbk, defrIOTimingCbkType, &defrIOTiming);
      } 

start_iotiming: '-' '(' {dumb_mode = 2; no_num = 2; } T_STRING T_STRING ')'
      {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setName($4, $5);
      }

iotiming_members: 
      | iotiming_members iotiming_member
      ;

iotiming_member:
      '+' risefall K_VARIABLE NUMBER NUMBER
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setVariable($2, $4, $5);
      }
      | '+' risefall K_SLEWRATE NUMBER NUMBER
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setSlewRate($2, $4, $5);
      }
      | '+' K_CAPACITANCE NUMBER
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setCapacitance($3);
      }
      | '+' K_DRIVECELL {dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setDriveCell($4);
      } iotiming_drivecell_opt
      /*| '+' K_FROMPIN   {dumb_mode = 1; no_num = 1; } T_STRING*/
/*
      | K_FROMPIN {dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setFrom($3);
      }
      K_TOPIN {dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setTo($7);
      }
*/
      /*| '+' K_PARALLEL NUMBER*/
/*
      |  K_PARALLEL NUMBER
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setParallel($2);
      }
*/
      | extension_stmt
      {
        if (defVersionNum < 5.4 && defrIoTimingsExtCbk)
          CALLBACK(defrIoTimingsExtCbk, defrIoTimingsExtCbkType, History_text);
      }

iotiming_drivecell_opt: iotiming_frompin
      K_TOPIN {dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrIOTimingCbk) 
          defrIOTiming.defiIOTiming::setTo($4);
      }
      iotiming_parallel

iotiming_frompin: /* empty */
      | K_FROMPIN {dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setFrom($3);
      }

iotiming_parallel: /* empty */
      | K_PARALLEL NUMBER
      {
        if (defrIOTimingCbk)
          defrIOTiming.defiIOTiming::setParallel($2);
      }

risefall: K_RISE { $$ = (char*)"RISE"; } | K_FALL { $$ = (char*)"FALL"; }

iotiming_end: K_END K_IOTIMINGS
      {
        if (defVersionNum < 5.4 && defrIOTimingsEndCbk)
          CALLBACK(defrIOTimingsEndCbk, defrIOTimingsEndCbkType, 0);
      }

floorplan_contraints_section: fp_start fp_stmts K_END K_FPC
      { 
        if (defrFPCEndCbk)
          CALLBACK(defrFPCEndCbk, defrFPCEndCbkType, 0);
      }

fp_start: K_FPC NUMBER ';'
      {
        if (defrFPCStartCbk)
          CALLBACK(defrFPCStartCbk, defrFPCStartCbkType, ROUND($2));
      }

fp_stmts: /* empty */
      | fp_stmts fp_stmt
      {}

fp_stmt: '-' { dumb_mode = 1; no_num = 1;  } T_STRING h_or_v
      { if (defrFPCCbk) defrFPC.defiFPC::setName($3, $4); }
      constraint_type constrain_what_list ';'
      { if (defrFPCCbk) CALLBACK(defrFPCCbk, defrFPCCbkType, &defrFPC); }

h_or_v: K_HORIZONTAL 
      { $$ = (char*)"HORIZONTAL"; }
      | K_VERTICAL
      { $$ = (char*)"VERTICAL"; }

constraint_type: K_ALIGN
      { if (defrFPCCbk) defrFPC.defiFPC::setAlign(); }
      | K_MAX NUMBER
      { if (defrFPCCbk) defrFPC.defiFPC::setMax($2); }
      | K_MIN NUMBER
      { if (defrFPCCbk) defrFPC.defiFPC::setMin($2); }
      | K_EQUAL NUMBER
      { if (defrFPCCbk) defrFPC.defiFPC::setEqual($2); }

constrain_what_list: /* empty */
      | constrain_what_list constrain_what
      ;

constrain_what: '+' K_BOTTOMLEFT
      { if (defrFPCCbk) defrFPC.defiFPC::setDoingBottomLeft(); }
      row_or_comp_list 
      |       '+' K_TOPRIGHT
      { if (defrFPCCbk) defrFPC.defiFPC::setDoingTopRight(); }
      row_or_comp_list 
      ;

row_or_comp_list: /* empty */
      | row_or_comp_list row_or_comp

row_or_comp: '(' K_ROWS  {dumb_mode = 1; no_num = 1; } T_STRING ')'
      { if (defrFPCCbk) defrFPC.defiFPC::addRow($4); }
      |    '(' K_COMPS {dumb_mode = 1; no_num = 1; } T_STRING ')'
      { if (defrFPCCbk) defrFPC.defiFPC::addComps($4); }

timingdisables_section: timingdisables_start timingdisables_rules
      timingdisables_end
      ;

timingdisables_start: K_TIMINGDISABLES NUMBER ';'
      { 
        if (defrTimingDisablesStartCbk)
          CALLBACK(defrTimingDisablesStartCbk, defrTimingDisablesStartCbkType,
                   ROUND($2));
      }

timingdisables_rules: /* empty */
      | timingdisables_rules timingdisables_rule
      {}

timingdisables_rule: '-' K_FROMPIN { dumb_mode = 2; no_num = 2;  } T_STRING
      T_STRING K_TOPIN { dumb_mode = 2; no_num = 2;  } T_STRING T_STRING ';'
      {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setFromTo($4, $5, $8, $9);
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                &defrTimingDisable);
        }
      }
      | '-' K_THRUPIN {dumb_mode = 2; no_num = 2; } T_STRING T_STRING ';'
      {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setThru($4, $5);
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                   &defrTimingDisable);
        }
      }
      | '-' K_MACRO {dumb_mode = 1; no_num = 1;} T_STRING td_macro_option ';'
      {
        if (defrTimingDisableCbk) {
          defrTimingDisable.defiTimingDisable::setMacro($4);
          CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
                &defrTimingDisable);
        }
      }
      | '-' K_REENTRANTPATHS ';'
      { if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setReentrantPathsFlag();
      }


td_macro_option: K_FROMPIN {dumb_mode = 1; no_num = 1;} T_STRING K_TOPIN
      {dumb_mode=1; no_num = 1;} T_STRING
      {
        if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setMacroFromTo($3,$6);
      }
      |        K_THRUPIN {dumb_mode=1; no_num = 1;} T_STRING
      {
        if (defrTimingDisableCbk)
          defrTimingDisable.defiTimingDisable::setMacroThru($3);
      }

timingdisables_end: K_END K_TIMINGDISABLES
      { 
        if (defrTimingDisablesEndCbk)
          CALLBACK(defrTimingDisablesEndCbk, defrTimingDisablesEndCbkType, 0);
      }


partitions_section: partitions_start partition_rules partitions_end
      ;

partitions_start: K_PARTITIONS NUMBER ';'
      {
        if (defrPartitionsStartCbk)
          CALLBACK(defrPartitionsStartCbk, defrPartitionsStartCbkType,
                   ROUND($2));
      }

partition_rules: /* empty */
      | partition_rules partition_rule
      { }

partition_rule: start_partition partition_members ';' 
      { 
        if (defrPartitionCbk)
          CALLBACK(defrPartitionCbk, defrPartitionCbkType, &defrPartition);
      }

start_partition: '-' { dumb_mode = 1; no_num = 1; } T_STRING turnoff
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setName($3);
      }

turnoff: /* empty */
      | K_TURNOFF turnoff_setup turnoff_hold
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::addTurnOff($2, $3);
      }

turnoff_setup: /* empty */
      { $$ = (char*)" "; }
      | K_SETUPRISE
      { $$ = (char*)"R"; }
      | K_SETUPFALL
      { $$ = (char*)"F"; }

turnoff_hold: /* empty */
      { $$ = (char*)" "; }
      | K_HOLDRISE
      { $$ = (char*)"R"; }
      | K_HOLDFALL
      { $$ = (char*)"F"; }

partition_members: /* empty */
      | partition_members partition_member
      ;

partition_member: '+' K_FROMCLOCKPIN {dumb_mode=2; no_num = 2;}
      T_STRING T_STRING risefall minmaxpins
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromClockPin($4, $5);
      }
      | '+' K_FROMCOMPPIN {dumb_mode=2; no_num = 2; }
      T_STRING T_STRING risefallminmax2_list
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromCompPin($4, $5);
      }
      | '+' K_FROMIOPIN {dumb_mode=1; no_num = 1; } T_STRING
      risefallminmax1_list
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setFromIOPin($4);
      }
      | '+' K_TOCLOCKPIN {dumb_mode=2; no_num = 2; }
      T_STRING T_STRING risefall minmaxpins
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToClockPin($4, $5);
      }
      | '+' K_TOCOMPPIN {dumb_mode=2; no_num = 2; }
      T_STRING T_STRING risefallminmax2_list
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToCompPin($4, $5);
      }
      | '+' K_TOIOPIN {dumb_mode=1; no_num = 2; } T_STRING risefallminmax1_list
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setToIOPin($4);
      }
      | extension_stmt
      { 
        if (defrPartitionsExtCbk)
          CALLBACK(defrPartitionsExtCbk, defrPartitionsExtCbkType,
                   History_text);
      }

minmaxpins: min_or_max_list K_PINS
      { dumb_mode = 1000000000; no_num = 10000000; } pin_list
      { dumb_mode = 0; no_num = 0; }

min_or_max_list: /* empty */
      | min_or_max_list min_or_max_member
      { }

min_or_max_member: K_MIN NUMBER NUMBER
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setMin($2, $3);
      }
      | K_MAX NUMBER NUMBER
      {
        if (defrPartitionCbk)
          defrPartition.defiPartition::setMax($2, $3);
      }

pin_list: /* empty */
      | pin_list T_STRING
      { if (defrPartitionCbk) defrPartition.defiPartition::addPin($2); }

risefallminmax1_list: /* empty */
      | risefallminmax1_list risefallminmax1

risefallminmax1: K_RISEMIN NUMBER
      { if (defrPartitionCbk) defrPartition.defiPartition::addRiseMin($2); }
      | K_FALLMIN NUMBER
      { if (defrPartitionCbk) defrPartition.defiPartition::addFallMin($2); }
      | K_RISEMAX NUMBER
      { if (defrPartitionCbk) defrPartition.defiPartition::addRiseMax($2); }
      | K_FALLMAX NUMBER
      { if (defrPartitionCbk) defrPartition.defiPartition::addFallMax($2); }

risefallminmax2_list:
      risefallminmax2
      | risefallminmax2_list risefallminmax2
      ;

risefallminmax2: K_RISEMIN NUMBER NUMBER
      { if (defrPartitionCbk)
          defrPartition.defiPartition::addRiseMinRange($2, $3); }
      | K_FALLMIN NUMBER NUMBER
      { if (defrPartitionCbk)
          defrPartition.defiPartition::addFallMinRange($2, $3); }
      | K_RISEMAX NUMBER NUMBER
      { if (defrPartitionCbk)
          defrPartition.defiPartition::addRiseMaxRange($2, $3); }
      | K_FALLMAX NUMBER NUMBER
      { if (defrPartitionCbk)
          defrPartition.defiPartition::addFallMaxRange($2, $3); }

partitions_end: K_END K_PARTITIONS
      { if (defrPartitionsEndCbk)
          CALLBACK(defrPartitionsEndCbk, defrPartitionsEndCbkType, 0); }

comp_names: /* empty */
      | comp_names comp_name
      { }

comp_name: '(' {dumb_mode=2; no_num = 2; } T_STRING
      T_STRING subnet_opt_syn ')'
      {
        /* note that the first T_STRING could be the keyword VPIN */
        if (defrNetCbk)
          defrSubnet->defiSubnet::addPin($3, $4, $5);
      }

subnet_opt_syn: /* empty */
      { $$ = 0; }
      | '+' K_SYNTHESIZED
      { $$ = 1; }

subnet_options: /* empty */
      | subnet_options subnet_option

subnet_option: subnet_type
      {  
        if (defrNetCbk) defrSubnet->defiSubnet::addWire($1);
      }
      paths
      {  
        by_is_keyword = FALSE;
        do_is_keyword = FALSE;
        new_is_keyword = FALSE;
        step_is_keyword = FALSE;
        orient_is_keyword = FALSE;
        needNPCbk = 0;
      }
      | K_NONDEFAULTRULE { dumb_mode = 1; no_num = 1; } T_STRING
      { if (defrNetCbk) defrSubnet->defiSubnet::setNonDefault($3); }

subnet_type: K_FIXED
      { $$ = (char*)"FIXED"; }
      | K_COVER
      { $$ = (char*)"COVER"; }
      | K_ROUTED
      { $$ = (char*)"ROUTED"; }
      | K_NOSHIELD
      { $$ = (char*)"NOSHIELD"; }

pin_props_section: begin_pin_props pin_prop_list end_pin_props ;

begin_pin_props: K_PINPROPERTIES NUMBER opt_semi
      { if (defrPinPropStartCbk)
          CALLBACK(defrPinPropStartCbk, defrPinPropStartCbkType, ROUND($2)); }

opt_semi:
      /* empty */
      { }
      | ';'
      { }

end_pin_props: K_END K_PINPROPERTIES
      { if (defrPinPropEndCbk)
          CALLBACK(defrPinPropEndCbk, defrPinPropEndCbkType, 0); }

pin_prop_list: /* empty */
      | pin_prop_list pin_prop_terminal
      ;

pin_prop_terminal: '-' { dumb_mode = 2; no_num = 2; } T_STRING T_STRING
      { if (defrPinPropCbk) defrPinProp.defiPinProp::setName($3, $4); }
      pin_prop_options ';'
      { if (defrPinPropCbk) {
          CALLBACK(defrPinPropCbk, defrPinPropCbkType, &defrPinProp);
         // reset the property number
         defrPinProp.defiPinProp::clear();
        }
      }

pin_prop_options : /* empty */
      | pin_prop_options pin_prop ;

pin_prop: '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
      pin_prop_name_value_list 
      { dumb_mode = 0; parsing_property = 0; }

pin_prop_name_value_list : /* empty */
      | pin_prop_name_value_list pin_prop_name_value
      ;

pin_prop_name_value : T_STRING NUMBER
      {
        if (defrPinPropCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrCompPinProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
          /* For backword compatibility, also set the string value */
          /* We will use a temporary string to store the number.
           * The string space is borrowed from the ring buffer
           * in the lexer. */
          sprintf(str, "%g", $2);
          defrPinProp.defiPinProp::addNumProperty($1, $2, str, propTp);
        }
      }
 | T_STRING QSTRING
      {
        if (defrPinPropCbk) {
          char propTp;
          propTp = defrCompPinProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
          defrPinProp.defiPinProp::addProperty($1, $2, propTp);
        }
      }
 | T_STRING T_STRING
      {
        if (defrPinPropCbk) {
          char propTp;
          propTp = defrCompPinProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
          defrPinProp.defiPinProp::addProperty($1, $2, propTp);
        }
      }

blockage_section: blockage_start blockage_defs blockage_end ;

blockage_start: K_BLOCKAGES NUMBER ';'
      { if (defrBlockageStartCbk)
          CALLBACK(defrBlockageStartCbk, defrBlockageStartCbkType, ROUND($2)); }

blockage_end: K_END K_BLOCKAGES
      { if (defrBlockageEndCbk)
          CALLBACK(defrBlockageEndCbk, defrBlockageEndCbkType, 0); }

blockage_defs: /* empty */
      | blockage_defs blockage_def
      ;

blockage_def: blockage_rule rectPoly_blockage rectPoly_blockage_rules
      ';'
      {
        if (defrBlockageCbk) {
          CALLBACK(defrBlockageCbk, defrBlockageCbkType, &defrBlockage);
          defrBlockage.defiBlockage::clear();
        }
      }

blockage_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING 
      {
        if (defrBlockageCbk) {
          if (defrBlockage.defiBlockage::hasPlacement() != 0) {
            if (blockageWarnings++ < defrBlockageWarnings) {
              defError(6539, "The BLOCKAGE statement has both the LAYER & the PLACEMENT statements defined.\nThis is illegal.");
              CHKERR();
            }
          }
          defrBlockage.defiBlockage::setLayer($4);
          defrBlockage.defiBlockage::clearPoly(); // free poly, if any
        }
        hasBlkLayerComp = 0;
        hasBlkLayerSpac = 0;
      }
      layer_blockage_rules
      /* 10/29/2001 - enhancement */
      | '-' K_PLACEMENT
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
      }
      placement_comp_rule

layer_blockage_rules: /* empty */
      | layer_blockage_rules layer_blockage_rule
      ;

layer_blockage_rule: '+' K_SPACING NUMBER
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
            defrBlockage.defiBlockage::setSpacing((int)$3);
          hasBlkLayerSpac = 1;
        }
      }
      | '+' K_DESIGNRULEWIDTH NUMBER
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
            defrBlockage.defiBlockage::setDesignRuleWidth((int)$3);
          hasBlkLayerSpac = 1;
        }
      }
      | comp_blockage_rule

comp_blockage_rule:
      /* 06/20/2001 - pcr 383204 = pcr 378102 */
      '+' K_COMPONENT { dumb_mode = 1; no_num = 1; } T_STRING
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
            defrBlockage.defiBlockage::setComponent($4);
          hasBlkLayerComp = 1;
        }
      }
      /* 8/30/2001 - pcr 394394 */
      | '+' K_SLOTS
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
      }
      | '+' K_FILLS
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
      }
      | '+' K_PUSHDOWN
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
      }
      | '+' K_EXCEPTPGNET              /* 5.7 */
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
      }

placement_comp_rule: /* empty */
      /* 10/29/2001 - enhancement */
      | '+' K_COMPONENT { dumb_mode = 1; no_num = 1; } T_STRING
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
            defrBlockage.defiBlockage::setComponent($4);
          hasBlkPlaceComp = 1;
        }
      }
      | '+' K_PUSHDOWN
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
      }
      | '+' K_SOFT                   // 5.7
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
      }
      | '+' K_PARTIAL NUMBER         // 5.7
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
               defrBlockage.defiBlockage::setPartial($3);
             hasBlkPlaceComp = 1;
           }
         }
      }

rectPoly_blockage_rules: /* empty */
      | rectPoly_blockage_rules rectPoly_blockage
      ;
  
rectPoly_blockage: K_RECT pt pt
      {
        if (defrBlockageCbk)
          defrBlockage.defiBlockage::addRect($2.x, $2.y, $3.x, $3.y);
      }
      | K_POLYGON
      {
        if (defrBlockageCbk) {
          if (!defrGeomPtr) {
            defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
            defrGeomPtr->defiGeometries::Init();
          } else   // Just reset the number of points
            defrGeomPtr->defiGeometries::Reset();
        }
      }
      firstPt nextPt nextPt otherPts
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
      }

/* 8/31/2001 - 5.4 enhancement */
slot_section: slot_start slot_defs slot_end ;

slot_start: K_SLOTS NUMBER ';'
      { if (defrSlotStartCbk)
          CALLBACK(defrSlotStartCbk, defrSlotStartCbkType, ROUND($2)); }

slot_end: K_END K_SLOTS
      { if (defrSlotEndCbk)
          CALLBACK(defrSlotEndCbk, defrSlotEndCbkType, 0); }

slot_defs: /* empty */
      | slot_defs slot_def
      ;

slot_def: slot_rule geom_slot_rules ';'
      {
        if (defrSlotCbk) {
          CALLBACK(defrSlotCbk, defrSlotCbkType, &defrSlot);
          defrSlot.defiSlot::clear();
        }
      }

slot_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING 
      {
        if (defrSlotCbk) {
          defrSlot.defiSlot::setLayer($4);
          defrSlot.defiSlot::clearPoly();     // free poly, if any
        }
      } geom_slot

geom_slot_rules: /* empty */
      | geom_slot_rules geom_slot
      ;

geom_slot: K_RECT pt pt
      {
        if (defrSlotCbk)
          defrSlot.defiSlot::addRect($2.x, $2.y, $3.x, $3.y);
      }
      | K_POLYGON
      {
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      }
      firstPt nextPt nextPt otherPts
      {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond
          if (defrSlotCbk)
            defrSlot.defiSlot::addPolygon(defrGeomPtr);
        }
      }

/* 8/31/2001 -  5.4 enhancement */
fill_section: fill_start fill_defs fill_end ;

fill_start: K_FILLS NUMBER ';'
      { if (defrFillStartCbk)
          CALLBACK(defrFillStartCbk, defrFillStartCbkType, ROUND($2)); }

fill_end: K_END K_FILLS
      { if (defrFillEndCbk)
          CALLBACK(defrFillEndCbk, defrFillEndCbkType, 0); }

fill_defs: /* empty */
      | fill_defs fill_def
      ;

fill_def: fill_rule geom_fill_rules ';'
      {
        if (defrFillCbk) {
          CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
          defrFill.defiFill::clear();
        }
      }
      | '-' K_VIA { dumb_mode = 1; no_num = 1; } T_STRING   // 5.7
      {
        if (defrFillCbk) {
          defrFill.defiFill::setVia($4);
          defrFill.defiFill::clearPts();
          if (!defrGeomPtr) {
            defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
            defrGeomPtr->defiGeometries::Init();
          } else   // Just reset the number of points
            defrGeomPtr->defiGeometries::Reset();
        }
      }
      fill_via_opc firstPt otherPts ';'
      {
        if (defrFillCbk) {
          defrFill.defiFill::addPts(defrGeomPtr);
          CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
          defrFill.defiFill::clear();
        }
      }

fill_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING 
      {
        if (defrFillCbk) {
          defrFill.defiFill::setLayer($4);
          defrFill.defiFill::clearPoly();    // free poly, if any
        }
      } fill_layer_opc geom_fill             // 5.7

geom_fill_rules: /* empty */
      | geom_fill_rules geom_fill
      ;

geom_fill: K_RECT pt pt
      {
        if (defrFillCbk)
          defrFill.defiFill::addRect($2.x, $2.y, $3.x, $3.y);
      }
      | K_POLYGON
      {
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      }
      firstPt nextPt nextPt otherPts
      {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond
          if (defrFillCbk)
            defrFill.defiFill::addPolygon(defrGeomPtr);
        }
      }

// 5.7
fill_layer_opc: /* empty */ {}
      | '+' K_OPC
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
      }

// 5.7
fill_via_opc: /* empty */ {}
      | '+' K_OPC
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
      }

/* 11/17/2003 - 5.6 enhancement */
nondefaultrule_section: nondefault_start nondefault_def nondefault_defs
    nondefault_end ;

nondefault_start: K_NONDEFAULTRULES NUMBER ';'
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
                   ROUND($2));
      }

nondefault_end: K_END K_NONDEFAULTRULES
      { if (defrNonDefaultEndCbk)
          CALLBACK(defrNonDefaultEndCbk, defrNonDefaultEndCbkType, 0); }

nondefault_defs: /* empty */
      | nondefault_defs nondefault_def
      ;

nondefault_def: '-' { dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::clear(); 
          defrNonDefault.defiNonDefault::setName($3);
        }
      }
      nondefault_options ';'
      { if (defrNonDefaultCbk)
          CALLBACK(defrNonDefaultCbk, defrNonDefaultCbkType, &defrNonDefault); }

nondefault_options: /* empty */ 
      | nondefault_options nondefault_option
      ;

nondefault_option: '+' K_HARDSPACING
      {
        if (defrNonDefaultCbk)
          defrNonDefault.defiNonDefault::setHardspacing();
      }
      | '+' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING
        K_WIDTH NUMBER
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addLayer($4);
          defrNonDefault.defiNonDefault::addWidth($6);
        }
      }
      nondefault_layer_options
      | '+' K_VIA { dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addVia($4);
        }
      }
      | '+' K_VIARULE { dumb_mode = 1; no_num = 1; } T_STRING
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addViaRule($4);
        }
      }
      | '+' K_MINCUTS { dumb_mode = 1; no_num = 1; } T_STRING NUMBER
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addMinCuts($4, (int)$5);
        }
      }
      | nondefault_prop_opt
      ;

nondefault_layer_options: /* empty */
      | nondefault_layer_options nondefault_layer_option

nondefault_layer_option:
      K_DIAGWIDTH NUMBER
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addDiagWidth($2);
        }
      }
      | K_SPACING NUMBER
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addSpacing($2);
        }
      }
      | K_WIREEXT NUMBER
      {
        if (defrNonDefaultCbk) {
          defrNonDefault.defiNonDefault::addWireExt($2);
        }
      }
      ;

nondefault_prop_opt: '+' K_PROPERTY { dumb_mode = 10000; parsing_property = 1; }
                     nondefault_prop_list
      { dumb_mode = 0; parsing_property = 0; }

nondefault_prop_list: /* empty */
      | nondefault_prop_list nondefault_prop
      ;

nondefault_prop: T_STRING NUMBER
      {
        if (defrNonDefaultCbk) {
          char propTp;
          char* str = ringCopy("                       ");
          propTp = defrNDefProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
          sprintf(str, "%g", $2);
          defrNonDefault.defiNonDefault::addNumProperty($1, $2, str, propTp);
        }
      }
      | T_STRING QSTRING
      {
        if (defrNonDefaultCbk) {
          char propTp;
          propTp = defrNDefProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
          defrNonDefault.defiNonDefault::addProperty($1, $2, propTp);
        }
      }
      | T_STRING T_STRING
      {
        if (defrNonDefaultCbk) {
          char propTp;
          propTp = defrNDefProp.defiPropType::propType($1);
          CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
          defrNonDefault.defiNonDefault::addProperty($1, $2, propTp);
        }
      }

/* 12/2/2003 - 5.6 enhancement */
styles_section: styles_start styles_rules styles_end ;

styles_start: K_STYLES NUMBER ';'
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
          CALLBACK(defrStylesStartCbk, defrStylesStartCbkType, ROUND($2));
      }

styles_end: K_END K_STYLES
      { if (defrStylesEndCbk)
          CALLBACK(defrStylesEndCbk, defrStylesEndCbkType, 0); }

styles_rules: /* empty */
      | styles_rules styles_rule
      ;

styles_rule: '-' K_STYLE NUMBER
      {
        if (defrStylesCbk) defrStyles.defiStyles::setStyle((int)$3);
        if (!defrGeomPtr) {
          defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
          defrGeomPtr->defiGeometries::Init();
        } else   // Just reset the number of points
          defrGeomPtr->defiGeometries::Reset();
      }
      firstPt nextPt otherPts ';'
      {
        if (defVersionNum >= 5.6) {  // only 5.6 and beyond will call the callback
          if (defrStylesCbk) {
            defrStyles.defiStyles::setPolygon(defrGeomPtr);
            CALLBACK(defrStylesCbk, defrStylesCbkType, &defrStyles);
          }
        }
      }

%%
