#ifndef __FLATTERNABLE_H__
#define __FLATTERNABLE_H__

// DO NOT USE: please use parcelable instead
// This code is deprecated and will not be supported via AIDL code gen. For data
// to be sent over binder, please use parcelables.

#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <utils/errors.h>

#include <type_traits>

namespace cdroid {

// DO NOT USE: please use parcelable instead
// This code is deprecated and will not be supported via AIDL code gen. For data
// to be sent over binder, please use parcelables.
class FlattenableUtils {
public:
    template<size_t N>
    static size_t align(size_t size) {
        static_assert(!(N & (N - 1)), "Can only align to a power of 2.");
        return (size + (N-1)) & ~(N-1);
    }

    template<size_t N>
    static size_t align(void const*& buffer) {
        static_assert(!(N & (N - 1)), "Can only align to a power of 2.");
        uintptr_t b = uintptr_t(buffer);
        buffer = reinterpret_cast<void*>((uintptr_t(buffer) + (N-1)) & ~(N-1));
        return size_t(uintptr_t(buffer) - b);
    }

    template<size_t N>
    static size_t align(void*& buffer) {
        static_assert(!(N & (N - 1)), "Can only align to a power of 2.");
        void* b = buffer;
        buffer = reinterpret_cast<void*>((uintptr_t(buffer) + (N-1)) & ~(N-1));
        size_t delta = size_t(uintptr_t(buffer) - uintptr_t(b));
        memset(b, 0, delta);
        return delta;
    }

    static void advance(void*& buffer, size_t& size, size_t offset) {
        buffer = reinterpret_cast<void*>( uintptr_t(buffer) + offset );
        size -= offset;
    }

    static void advance(void const*& buffer, size_t& size, size_t offset) {
        buffer = reinterpret_cast<void const*>( uintptr_t(buffer) + offset );
        size -= offset;
    }

    // write a POD structure
    template<typename T>
    static void write(void*& buffer, size_t& size, const T& value) {
        static_assert(std::is_trivially_copyable<T>::value,
                      "Cannot flatten a non-trivially-copyable type");
        memcpy(buffer, &value, sizeof(T));
        advance(buffer, size, sizeof(T));
    }

    // read a POD structure
    template<typename T>
    static void read(void const*& buffer, size_t& size, T& value) {
        static_assert(std::is_trivially_copyable<T>::value,
                      "Cannot unflatten a non-trivially-copyable type");
        memcpy(&value, buffer, sizeof(T));
        advance(buffer, size, sizeof(T));
    }
};

// DO NOT USE: please use parcelable instead
// This code is deprecated and will not be supported via AIDL code gen. For data
// to be sent over binder, please use parcelables.
/*
 * The Flattenable protocol allows an object to serialize itself out
 * to a byte-buffer and an array of file descriptors.
 * Flattenable objects must implement this protocol.
 */

template <typename T>
class Flattenable {
public:
    // size in bytes of the flattened object
    inline size_t getFlattenedSize() const;

    // number of file descriptors to flatten
    inline size_t getFdCount() const;

    // flattens the object into buffer.
    // size should be at least of getFlattenedSize()
    // file descriptors are written in the fds[] array but ownership is
    // not transfered (ie: they must be dupped by the caller of
    // flatten() if needed).
    inline status_t flatten(void*& buffer, size_t& size, int*& fds, size_t& count) const;

    // unflattens the object from buffer.
    // size should be equal to the value of getFlattenedSize() when the
    // object was flattened.
    // unflattened file descriptors are found in the fds[] array and
    // don't need to be dupped(). ie: the caller of unflatten doesn't
    // keep ownership. If a fd is not retained by unflatten() it must be
    // explicitly closed.
    inline status_t unflatten(void const*& buffer, size_t& size, int const*& fds, size_t& count);
};

template<typename T>
inline size_t Flattenable<T>::getFlattenedSize() const {
    return static_cast<T const*>(this)->T::getFlattenedSize();
}
template<typename T>
inline size_t Flattenable<T>::getFdCount() const {
    return static_cast<T const*>(this)->T::getFdCount();
}
template<typename T>
inline status_t Flattenable<T>::flatten(
        void*& buffer, size_t& size, int*& fds, size_t& count) const {
    return static_cast<T const*>(this)->T::flatten(buffer, size, fds, count);
}
template<typename T>
inline status_t Flattenable<T>::unflatten(
        void const*& buffer, size_t& size, int const*& fds, size_t& count) {
    return static_cast<T*>(this)->T::unflatten(buffer, size, fds, count);
}

// DO NOT USE: please use parcelable instead
// This code is deprecated and will not be supported via AIDL code gen. For data
// to be sent over binder, please use parcelables.
/*
 * LightFlattenable is a protocol allowing object to serialize themselves out
 * to a byte-buffer. Because it doesn't handle file-descriptors,
 * LightFlattenable is usually more size efficient than Flattenable.
 * LightFlattenable objects must implement this protocol.
 */
template <typename T>
class LightFlattenable {
public:
    // returns whether this object always flatten into the same size.
    // for efficiency, this should always be inline.
    inline bool isFixedSize() const;

    // returns size in bytes of the flattened object. must be a constant.
    inline size_t getFlattenedSize() const;

    // flattens the object into buffer.
    inline status_t flatten(void* buffer, size_t size) const;

    // unflattens the object from buffer of given size.
    inline status_t unflatten(void const* buffer, size_t size);
};

template <typename T>
inline bool LightFlattenable<T>::isFixedSize() const {
    return static_cast<T const*>(this)->T::isFixedSize();
}
template <typename T>
inline size_t LightFlattenable<T>::getFlattenedSize() const {
    return static_cast<T const*>(this)->T::getFlattenedSize();
}
template <typename T>
inline status_t LightFlattenable<T>::flatten(void* buffer, size_t size) const {
    return static_cast<T const*>(this)->T::flatten(buffer, size);
}
template <typename T>
inline status_t LightFlattenable<T>::unflatten(void const* buffer, size_t size) {
    return static_cast<T*>(this)->T::unflatten(buffer, size);
}

// DO NOT USE: please use parcelable instead
// This code is deprecated and will not be supported via AIDL code gen. For data
// to be sent over binder, please use parcelables.
/*
 * LightFlattenablePod is an implementation of the LightFlattenable protocol
 * for POD (plain-old-data) objects.
 * Simply derive from LightFlattenablePod<Foo> to make Foo flattenable; no
 * need to implement any methods; obviously Foo must be a POD structure.
 */
template <typename T>
class LightFlattenablePod : public LightFlattenable<T> {
public:
    inline bool isFixedSize() const {
        return true;
    }

    inline size_t getFlattenedSize() const {
        return sizeof(T);
    }
    inline status_t flatten(void* buffer, size_t size) const {
        if (size < sizeof(T)) return NO_MEMORY;
        memcpy(buffer, static_cast<T const*>(this), sizeof(T));
        return OK;
    }
    inline status_t unflatten(void const* buffer, size_t) {
        memcpy(static_cast<T*>(this), buffer, sizeof(T));
        return OK;
    }
};

}  // namespace cdroid
#endif/*__FLATTERNABLE_H__*/ 
