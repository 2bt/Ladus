#pragma once

#include "stdwasm.hpp"


template <class T>
class Vector {
public:
    // XXX: destructors of T are not called lol

    ~Vector() {
        free(m_begin);
    }
    int len() const { return m_end - m_begin; }
    void append(T const& t) {
        if (m_end == m_cap) {
            int l = len();
            int n = max(l * 2, 4);
            T*  b = (T*) malloc(sizeof(T) * n);
            for (T* p = b, *i = m_begin; i < m_end; ) *p++ = *i++;
            free(m_begin);
            m_begin = b;
            m_end   = b + l;
            m_cap   = b + n;
        }
        *m_end++ = t;
    }
    void remove(int i = -1) {
        T* p = i < 0 ? m_end + i : m_begin + i;
        --m_end;
        for (; p < m_end; ++p) *p = p[1];
    }
    void clear() {
        m_end = m_begin;
    }
    T& operator[](int i) { return m_begin[i]; }
    T const& operator[](int i) const { return m_begin[i]; }
    T* begin() { return m_begin; }
    T const* begin() const { return m_begin; }
    T* end() { return m_end; }
    T const* end() const { return m_end; }

   Vector() {}
   Vector(Vector<T> const&) = delete;
   Vector<T>& operator=(Vector<T> const&) = delete;
private:
    T* m_begin = nullptr;
    T* m_end   = nullptr;
    T* m_cap   = nullptr;
};
