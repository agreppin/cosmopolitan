/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ This program is free software; you can redistribute it and/or modify         │
│ it under the terms of the GNU General Public License as published by         │
│ the Free Software Foundation; version 2 of the License.                      │
│                                                                              │
│ This program is distributed in the hope that it will be useful, but          │
│ WITHOUT ANY WARRANTY; without even the implied warranty of                   │
│ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU             │
│ General Public License for more details.                                     │
│                                                                              │
│ You should have received a copy of the GNU General Public License            │
│ along with this program; if not, write to the Free Software                  │
│ Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA                │
│ 02110-1301 USA                                                               │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/nexgen32e/hascharacter.internal.h"
#include "libc/str/str.h"

/**
 * Returns pointer to first byte matching any in accept, or NULL.
 * @asyncsignalsafe
 */
char16_t *strpbrk16(const char16_t *s, const char16_t *accept) {
  size_t i;
  if (accept[0]) {
    if (!accept[1]) {
      return strchr16(s, accept[0]);
    } else {
      for (i = 0; s[i]; ++i) {
        if (HasCharacter16(s[i], accept)) {
          return (/*unconst*/ char16_t *)&s[i];
        }
      }
    }
  }
  return NULL;
}
