#include "../kconfig.h"


#if defined (TOOLCHAIN_HARMONY)
#include "../toolchain/harmony.h"

#define TOOLCHAIN_PRESENT
#endif

#if defined(__32MX470F512H__) && defined(TOOLCHAIN_PRESENT)
#define __32MX470F512H_HARMONY__
  #include "mips/pic32mx470f512h.h"
#endif


#if defined(__32MX470F512L__)
  #include "mips/pic32mx470f512h.h"
#endif




#define arch_has_spi()        HAS_CAPS(CAPS_SPI)
#define arch_has_i2c()        HAS_CAPS(CAPS_I2C)
#define arch_has_core_timer() HAS_CAPS(CAPS_CORE_TIMER)
#define arch_has_uart()        HAS_CAPS(CAPS_UART)

