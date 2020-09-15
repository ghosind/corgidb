#ifndef _CDB_UTILS_H_
#define _CDB_UTILS_H_

#define get_aligned_size(size) \
    (size - size % sizeof(size_t) + sizeof(size_t))

#endif // _CDB_UTILS_H_
