# comment conventions

## File Header

```c
/*
 * disk.c - disk block management
 *
 * Copyright (C) 2024 Your Name
 * Licensed under the GPL v2.
 */
#include "disk.h"
```

## Struct

`````c
/*
 * struct block - a disk block
 *
 * @data:   raw block content
 * @offset: byte position on disk
 * @dirty:  set when modified, cleared on writeback
 */
struct block {
    uint8_t  data[1024];
    uint32_t offset;
    int      dirty;
};
`````

## Function

`````c
/*
 * alloc_block - allocate a block from the free list
 *
 * @disk:   disk to allocate from
 * @offset: block-aligned offset
 *
 * Returns the new block, or NULL if the disk is full.
 *
 * Caller must hold disk->mutex.
 */
struct block *alloc_block(struct disk *disk, uint32_t offset);
`````

## Inline

`````c
spin_lock(&disk->lock);   /* prevent race with writeback thread */
i = -1;                   /* sentinel: "no free block found" */
`````

## Markers

`````c
TODO: handle unaligned offsets
FIXME: leaks on error path
HACK: kernel 5.x API workaround
NOTE: O(n) by design, see RFC-42
`````

## Doxygen (if you ever switch)

`````c
/**
 * @brief Allocate a block.
 * @param disk   Disk to allocate from.
 * @param offset Block-aligned offset.
 * @return New block, or NULL.
 * @pre  Caller holds disk->mutex.
 */
struct block *alloc_block(struct disk *disk, uint32_t offset);
`````

## What to avoid

`````c
i++;                       /* increment i */          ← says what, not why
/* this is a block struct */                         ← obvious, skip
/* old version used a fixed array */                 ← stale, delete
`````````
