/**
 * @file ramfs.h
 * @author Santiago Marino
 * @year 2026
 * @brief RAM filesystem (first version) ? external declaration.
 * 
 * This filesystem uses a simple contiguous block allocation scheme.
 * It is provided for compatibility and testing.
 */

#ifndef RAMFS_H
#define RAMFS_H

#include "kernel/fs/fs.h"

extern fs_t ramfs;   /**< Global ramfs instance */

#endif /* RAMFS_H */