#ifdef DEBUG_H
#define DEBUG_H

#define DEBUG_OUTPUT alt_printf

#define DEBUG(fmt,...)                             \
  DEBUG_OUTPUT(fmt,__VA_ARGS__)


#endif
