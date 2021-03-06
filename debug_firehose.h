#ifndef DEGUB_FIREHOSE_INCLUDE
#define DEGUB_FIREHOSE_INCLUDE

#include "infoheader.h"

#define DEBUG_LEVEL_FUNDAMENTAL (0)
#define DEBUG_LEVEL_BASIC_FLOW (1)
#define DEBUG_LEVEL_FUNC_TRACE (2)
#define DEBUG_LEVEL_FUNC_FLOW (3)
#define DEBUG_LEVEL_FUNC_LINES (4)
#define DEBUG_LEVEL_ALL (5)

extern int currentDebugLevel;

#define DEBUG_MAX_LEVEL (currentDebugLevel)

#define debugFH(file,level,fmt,...) \
            do { \
            	if (level <= DEBUG_MAX_LEVEL){\
            		int paddingSpaces = MAX_FILENAME_WIDTH-strlen(file); \
            		char* padding = malloc(sizeof(char)*((paddingSpaces)+1)); \
            		padding[paddingSpaces] = '\0'; memset(padding, ' ', paddingSpaces); \
            		char* indent = malloc(sizeof(char)*(level+1)); \
            		indent[level] = '\0'; memset(indent, '\t', level); \
            		fprintf(stderr, "%s %s(L%d): %s"fmt"\n", file, padding, level, indent, ## __VA_ARGS__); \
            		free(indent); \
            		free(padding);\
            	} \
            } while (0)

#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__


#define FUNDAMENTAL(fmt,...) DEFER(debugFH(__FILE__,DEBUG_LEVEL_FUNDAMENTAL, fmt, ## __VA_ARGS__))
#define BASIC_FLOW(fmt,...) DEFER(debugFH(__FILE__,DEBUG_LEVEL_BASIC_FLOW, fmt, ## __VA_ARGS__))
#define FUNC_TRACE DEFER(debugFH(__FILE__,DEBUG_LEVEL_FUNC_TRACE, "%s", __func__))
#define FUNC_FLOW(fmt,...) DEFER(debugFH(__FILE__,DEBUG_LEVEL_FUNC_FLOW, "in '%s': " fmt, __func__, ## __VA_ARGS__))
#define FUNC_LINES(fmt,...) DEFER(debugFH(__FILE__,DEBUG_LEVEL_FUNC_LINES,  "in '%s': " fmt, __func__, ## __VA_ARGS__))
#define DEBUG_OTHER(fmt,...) DEFER(debugFH(__FILE__,DEBUG_LEVEL_ALL, fmt, ## __VA_ARGS__))

#endif //DEGUB_FIREHOSE_INCLUDE
