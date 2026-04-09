#define TOOLCHAIN_HARMONY
//#define USE_HYPERVISOR_OS

#if defined(USE_HYPERVISOR_OS)
#define TASK_SEGMENT_BLOCK_SIZE 64
#define MAX_CACHE_SEGMENTS 4

#endif


