/**
  popen and pclose are not part of win 95 and nt,
  but it appears that _popen and _pclose "work".
  if this won't load, use the return NULL statements.
*/

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
        perror("malloc");
        exit(1);
    }
    return p;
}

void *xcalloc(size_t number, size_t size)
{
    void *p = calloc(number, size);
    if (p == 0) {
        perror("calloc");
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

FILE *ufopen(const char *f, const wchar_t *m)
{
    wchar_t wf[8192];
    if (MultiByteToWideChar(CP_UTF8, 0, f, -1, wf, 8192) == 0)
        return NULL;
    return _wfopen(wf, m);
}

FILE *upopen(const char *f, const wchar_t *m)
{
    wchar_t wf[8192];
    if (MultiByteToWideChar(CP_UTF8, 0, f, -1, wf, 8192) == 0)
        return NULL;
    return _wpopen(wf, m);
}
