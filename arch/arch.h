#include "../kconfig.h"


#if defined (TOOLCHAIN_HARMONY)
#include "../toolchain/harmony.h"

#define TOOLCHAIN_PRESENT
#endif

// ============================================================================
// PIC32MX470F512H Support
// ============================================================================
#if defined(__32MX470F512H__) && defined(TOOLCHAIN_PRESENT)
#define __32MX470F512H_HARMONY__
  #include "mips/pic32mx470f512h.h"
#endif

#if defined(__32MX470F512L__)
  #include "mips/pic32mx470f512h.h"
#endif

// ============================================================================
// PIC32MZ2048EFM144 Support
// ============================================================================
// Harmony toolchain defines for PIC32MZ
// Typical defines: __PIC32MZ2048EFM144__ or __32MZ2048EFM144__
#if defined(__PIC32MZ2048EFM144__) && defined(TOOLCHAIN_PRESENT)
#define __PIC32MZ2048EFM144_HARMONY__
  #include "mips/pic32mz2048efm144.h"
#endif

// For non-Harmony toolchains (e.g., plain XC32 without Harmony framework)
// or when Harmony is not used, we still need to include the header
#if defined(__PIC32MZ2048EFM144__) && !defined(TOOLCHAIN_PRESENT)
  #include "mips/pic32mz2048efm144.h"
#endif

// Alternative define that some toolchains might use (without 'PIC32' prefix)
#if defined(__32MZ2048EFM144__) && defined(TOOLCHAIN_PRESENT)
#define __32MZ2048EFM144_HARMONY__
  #include "mips/pic32mz2048efm144.h"
#endif

#if defined(__32MZ2048EFM144__) && !defined(TOOLCHAIN_PRESENT)
  #include "mips/pic32mz2048efm144.h"
#endif




#define arch_has_spi()        HAS_CAPS(CAPS_SPI)
#define arch_has_i2c()        HAS_CAPS(CAPS_I2C)
#define arch_has_core_timer() HAS_CAPS(CAPS_CORE_TIMER)
#define arch_has_uart()        HAS_CAPS(CAPS_UART)

