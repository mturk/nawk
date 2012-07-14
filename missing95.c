/* popen and pclose are not part of win 95 and nt,
   but it appears that _popen and _pclose "work".
   if this won't load, use the return NULL statements. */

#if defined(_MSC_VER)
#pragma warning(push, 3)
/*
 * disable or reduce the frequency of...
 *   C4057: indirection to slightly different base types
 *   C4075: slight indirection changes (unsigned short* vs short[])
 *   C4100: unreferenced formal parameter
 *   C4127: conditional expression is constant
 *   C4163: '_rotl64' : not available as an intrinsic function
 *   C4201: nonstandard extension nameless struct/unions
 *   C4244: int to char/short - precision loss
 *   C4514: unreferenced inline function removed
 */
#pragma warning(disable: 4100 4127 4163 4201 4514; once: 4057 4075 4244)

/*
 * Ignore Microsoft's interpretation of secure development
 * and the POSIX string handling API
 */
#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif
#endif
#if !defined(UNICODE)
#define UNICODE
#endif

#define DEBUG
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pclose(FILE *f)
{
    return _pclose(f);  /* return NULL; */
}

size_t strlcpy(char *dst, const char *src, size_t siz)
{
    char *d = dst;
    const char *s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0) {
        if (siz != 0)
            *d = '\0';      /* NUL-terminate dst */
        while (*s++)
            ;
    }

    return s - src - 1;   /* count does not include NUL */
}


size_t strlcat(char *dst, const char *src, size_t siz)
{
    char *d = dst;
    const char *s = src;
    size_t n = siz;
    size_t dlen;

    /* Find the end of dst and adjust bytes left but don't go past end */
    while (n-- != 0 && *d != '\0')
        d++;
    dlen = d - dst;
    n = siz - dlen;

    if (n == 0)
        return dlen + strlen(s);
    while (*s != '\0') {
        if (n != 1) {
            *d++ = *s;
            n--;
        }
        s++;
    }
    *d = '\0';
    return dlen + (s - src);  /* count does not include NUL */
}

/**
 * Maloc that causes process exit in case of ENOMEM
 */
void *xmalloc(size_t size)
{
    void *p = calloc(size, 1);
    if (p == 0) {
        _wperror(L"malloc");
        exit(1);
    }
    return p;
}

void xfree(void *m)
{
    if (m != 0)
        free(m);
}

char *xucsdup(const wchar_t *wcs)
{
    char *mbs;
    int cch = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, 0, 0, 0, 0);
    if (cch == 0)
        return NULL;
    mbs = (char *)xmalloc(cch);
    WideCharToMultiByte(CP_UTF8, 0, wcs, -1, mbs, cch, 0, 0);
    return mbs;
}

FILE *ufopen(const char *f, const char *m)
{
    wchar_t wf[8192];
    wchar_t wm[64];
    if (MultiByteToWideChar(CP_UTF8, 0, f, -1, wf, 8192) == 0)
        return NULL;
    if (MultiByteToWideChar(CP_UTF8, 0, m, -1, wm, 64) == 0)
        return NULL;
    return _wfopen(wf, wm);
}

FILE *upopen(const char *f, const char *m)
{
    wchar_t wf[8192];
    wchar_t wm[64];
    if (MultiByteToWideChar(CP_UTF8, 0, f, -1, wf, 8192) == 0)
        return NULL;
    if (MultiByteToWideChar(CP_UTF8, 0, m, -1, wm, 64) == 0)
        return NULL;
    return _wpopen(wf, wm);
}
