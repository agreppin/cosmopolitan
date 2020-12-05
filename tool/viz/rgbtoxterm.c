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
#include "dsp/tty/quant.h"
#include "libc/calls/calls.h"
#include "libc/log/check.h"
#include "libc/runtime/runtime.h"
#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/ex.h"
#include "libc/sysv/consts/exit.h"
#include "libc/sysv/consts/fileno.h"
#include "third_party/getopt/getopt.h"

#define kUsage \
  "Usage:\n\
\n\
  %s [-cber] [-o FILE] [ARGS...]\n\
\n\
Flags:\n\
\n\
  -e         enable emphasis\n\
  -b         emit as background color\n\
  -r         print raw codes\n\
  -c         emit cleanup (reset) codes when done\n\
  -o FILE    redirects output [default: /dev/stdout]\n\
\n\
Arguments:\n\
\n\
  - may be passed via ARGS or STDIN (one per line)\n\
  - may be #RRGGBB or RRGGBB\n\
  - may be #RGB or RGB (#123 → #112233)\n\
  - anything else is printed verbatim\n\
\n"

static FILE *fout_;
static size_t linecap_;
static char *outpath_, *line_;
static bool rawmode_, background_, emphasis_, cleanup_;

wontreturn void usage(int rc, FILE *f) {
  fprintf(f, kUsage, program_invocation_name);
  exit(rc);
}

void getopts(int *argc, char *argv[]) {
  int opt;
  outpath_ = "-";
  while ((opt = getopt(*argc, argv, "?hrecbo:")) != -1) {
    switch (opt) {
      case 'r':
        rawmode_ = true;
        break;
      case 'b':
        background_ = true;
        break;
      case 'c':
        cleanup_ = true;
        break;
      case 'e':
        emphasis_ = true;
        break;
      case 'o':
        outpath_ = optarg;
        break;
      case '?':
      case 'h':
        usage(EXIT_SUCCESS, stdout);
      default:
        usage(EX_USAGE, stderr);
    }
  }
}

void processarg(const char *arg) {
  const char *p;
  size_t r, g, b;
  if (*(p = arg) == '#') p++;
  if (strlen(p) == 3 && (isxdigit(p[0]) && isxdigit(p[1]) && isxdigit(p[2]))) {
    r = hextoint(p[0]) << 4 | hextoint(p[0]);
    g = hextoint(p[1]) << 4 | hextoint(p[1]);
    b = hextoint(p[2]) << 4 | hextoint(p[2]);
  } else if (strlen(p) == 6 &&
             (isxdigit(p[0]) && isxdigit(p[1]) && isxdigit(p[2]) &&
              isxdigit(p[3]) && isxdigit(p[4]) && isxdigit(p[5]))) {
    r = hextoint(p[0]) << 4 | hextoint(p[1]);
    g = hextoint(p[2]) << 4 | hextoint(p[3]);
    b = hextoint(p[4]) << 4 | hextoint(p[5]);
  } else {
    fputs(arg, fout_);
    return;
  }
  fprintf(fout_, "%s[%s%d;5;%hhum%s", rawmode_ ? "\e" : "\\e",
          emphasis_ ? "1;" : "", background_ ? 48 : 38, rgb2tty(r, g, b),
          &"\n"[rawmode_]);
}

int main(int argc, char *argv[]) {
  size_t i;
  getopts(&argc, argv);
  CHECK_NOTNULL((fout_ = fopen(outpath_, "w")));
  if (optind < argc) {
    for (i = optind; i < argc; ++i) {
      processarg(argv[i]);
    }
  } else {
    while ((getline(&line_, &linecap_, stdin)) != -1) {
      processarg(chomp(line_));
    }
    free_s(&line_);
  }
  if (cleanup_) {
    fprintf(fout_, "%s[0m\n", rawmode_ ? "\e" : "\\e");
  }
  CHECK_NE(-1, fclose_s(&fout_));
  return 0;
}
