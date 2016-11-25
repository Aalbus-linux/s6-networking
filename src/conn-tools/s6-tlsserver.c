/* ISC license. */

#include <sys/types.h>
#include <limits.h>
#include <skalibs/uint64.h>
#include <skalibs/uint.h>
#include <skalibs/gidstuff.h>
#include <skalibs/sgetopt.h>
#include <skalibs/strerr2.h>
#include <skalibs/djbunix.h>
#include <s6-networking/config.h>

#define USAGE "s6-tlsserver [ options ] ip port prog...\n" \
"s6-tcpserver options: [ -q | -Q | -v ] [ -4 | -6 ] [ -1 ] [ -c maxconn ] [ -C localmaxconn ] [ -b backlog ] [ -G gidlist ] [ -g gid ] [ -u uid ] [ -U ]\n" \
"s6-tcpserver-access options: [ -W | -w ] [ -D | -d ] [ -H | -h ] [ -R | -r ] [ -P | -p ] [ -l localname ] [ -B banner ] [ -t timeout ] [ -i rulesdir | -x rulesfile ]\n" \
"s6-tlsd options: [ -S | -s ] [ -Y | -y ] [ -K timeout ]"

#define dieusage() strerr_dieusage(100, USAGE)

typedef struct options_s options_t, *options_t_ref ;
struct options_s
{
  uint64 uid ;
  char const *localname ;
  char const *banner ;
  char const *rules ;
  gid_t gids[NGROUPS_MAX] ;
  gid_t gid ;
  unsigned int maxconn ;
  unsigned int localmaxconn ;
  unsigned int backlog ;
  unsigned int gidn ;
  unsigned int timeout ;
  unsigned int kimeout ;
  unsigned int verbosity : 2 ;
  unsigned int flag46 : 2 ;
  unsigned int flag1 : 1 ;
  unsigned int flagU : 1 ;
  unsigned int flagw : 1 ;
  unsigned int flagD : 1 ;
  unsigned int flagH : 1 ;
  unsigned int flagr : 1 ;
  unsigned int flagp : 1 ;
  unsigned int ruleswhat : 2 ;
  unsigned int flagS : 1 ;
  unsigned int flagy : 1 ;
  unsigned int doaccess : 1 ;
} ;

#define OPTIONS_ZERO \
{ \
  .uid = 0, \
  .localname = 0, \
  .banner = 0, \
  .rules = 0, \
  .maxconn = 0, \
  .localmaxconn = 0, \
  .backlog = (unsigned int)-1, \
  .gidn = (unsigned int)-1, \
  .gid = 0, \
  .timeout = 0, \
  .kimeout = 0, \
  .verbosity = 1, \
  .flag46 = 0, \
  .flag1 = 0, \
  .flagU = 0, \
  .flagw = 0, \
  .flagD = 0, \
  .flagH = 0, \
  .flagr = 0, \
  .flagp = 0, \
  .ruleswhat = 0, \
  .flagS = 0, \
  .flagy = 0, \
  .doaccess = 0 \
}

int main (int argc, char const *const *argv, char const *const *envp)
{
  options_t o = OPTIONS_ZERO ;
  PROG = "s6-tlsserver" ;
  {
    subgetopt_t l = SUBGETOPT_ZERO ;
    for (;;)
    {
      register int opt = subgetopt_r(argc, argv, "qQv461c:C:b:G:g:u:UWwDdHhRrPpl:B:t:i:x:SsYyK:", &l) ;
      if (opt == -1) break ;
      switch (opt)
      {
        case 'q' : o.verbosity = 0 ; break ;
        case 'Q' : o.verbosity = 1 ; break ;
        case 'v' : o.verbosity = 2 ; break ;
        case '4' : o.flag46 = 1 ; break ;
        case '6' : o.flag46 = 2 ; break ;
        case '1' : o.flag1 = 1 ; break ;
        case 'c' : if (!uint0_scan(l.arg, &o.maxconn)) dieusage() ; if (!o.maxconn) o.maxconn = 1 ; break ;
        case 'C' : if (!uint0_scan(l.arg, &o.localmaxconn)) dieusage() ; if (!o.localmaxconn) o.localmaxconn = 1 ; break ;
        case 'b' : if (!uint0_scan(l.arg, &o.backlog)) dieusage() ; break ;
        case 'G' : if (!gid_scanlist(o.gids, NGROUPS_MAX, l.arg, &o.gidn) && *l.arg) dieusage() ; break ;
        case 'g' : if (!uint0_scan(l.arg, &o.gid)) dieusage() ; break ;
        case 'u' : if (!uint0_scan(l.arg, &o.uid)) dieusage() ; break ;
        case 'U' : o.flagU = 1 ; o.uid = 0 ; o.gid = 0 ; o.gidn = (unsigned int)-1 ; break ;
        case 'W' : o.flagw = 0 ; break ;
        case 'w' : o.flagw = 1 ; break ;
        case 'D' : o.flagD = 1 ; o.doaccess = 1 ; break ;
        case 'd' : o.flagD = 0 ; break ;
        case 'H' : o.flagH = 1 ; o.doaccess = 1 ; break ;
        case 'h' : o.flagH = 0 ; break ;
        case 'R' : o.flagr = 0 ; break ;
        case 'r' : o.flagr = 1 ; o.doaccess = 1 ; break ;
        case 'P' : o.flagp = 0 ; break ;
        case 'p' : o.flagp = 1 ; o.doaccess = 1 ; break ;
        case 'l' : o.localname = l.arg ; o.doaccess = 1 ; break ;
        case 'B' : o.banner = l.arg ; o.doaccess = 1 ; break ;
        case 't' : if (!uint0_scan(l.arg, &o.timeout)) dieusage() ; break ;
        case 'i' : o.rules = l.arg ; o.ruleswhat = 1 ; o.doaccess = 1 ; break ;
        case 'x' : o.rules = l.arg ; o.ruleswhat = 2 ; o.doaccess = 1 ; break ;
        case 'S' : o.flagS = 1 ; break ;
        case 's' : o.flagS = 0 ; break ;
        case 'Y' : o.flagy = 0 ; break ;
        case 'y' : o.flagy = 1 ; break ;
        case 'K' : if (!uint0_scan(l.arg, &o.kimeout)) dieusage() ; break ;
        default : dieusage() ;
      }
    }
    argc -= l.ind ; argv += l.ind ;
    if (argc < 3) dieusage() ;
  }

  {
    unsigned int m = 0 ;
    unsigned int pos = 0 ;
    char fmt[UINT_FMT * 5 + GID_FMT * (NGROUPS_MAX + 1) + UINT64_FMT] ;
    char const *newargv[44 + argc] ;
    newargv[m++] = S6_NETWORKING_BINPREFIX "s6-tcpserver" ;
    if (o.verbosity != 1) newargv[m++] = o.verbosity ? "-v" : "-q" ;
    if (o.flag46) newargv[m++] = o.flag46 == 1 ? "-4" : "-6" ;
    if (o.flag1) newargv[m++] = "-1" ;
    if (o.maxconn)
    {
      newargv[m++] = "-c" ;
      newargv[m++] = fmt + pos ;
      pos += uint_fmt(fmt + pos, o.maxconn) ;
      fmt[pos++] = 0 ;
    }
    if (o.localmaxconn)
    {
      newargv[m++] = "-C" ;
      newargv[m++] = fmt + pos ;
      pos += uint_fmt(fmt + pos, o.localmaxconn) ;
      fmt[pos++] = 0 ;
    }
    if (o.backlog != (unsigned int)-1)
    {
      newargv[m++] = "-b" ;
      newargv[m++] = fmt + pos ;
      pos += uint_fmt(fmt + pos, o.backlog) ;
      fmt[pos++] = 0 ;
    }
    if (o.gidn != (unsigned int)-1)
    {
      newargv[m++] = "-G" ;
      newargv[m++] = fmt + pos ;
      pos += gid_fmtlist(fmt + pos, o.gids, o.gidn) ;
      fmt[pos++] = 0 ;
    }
    if (o.gid)
    {
      newargv[m++] = "-g" ;
      newargv[m++] = fmt + pos ;
      pos += gid_fmt(fmt + pos, o.gid) ;
      fmt[pos++] = 0 ;
    }
    if (o.uid)
    {
      newargv[m++] = "-u" ;
      newargv[m++] = fmt + pos ;
      pos += uint64_fmt(fmt + pos, o.uid) ;
      fmt[pos++] = 0 ;
    }
    if (o.flagU) newargv[m++] = "-U" ;
    newargv[m++] = "--" ;
    if (o.doaccess)
    {
      newargv[m++] = S6_NETWORKING_BINPREFIX "s6-tcpserver-access" ;
      if (o.verbosity != 1)
      {
        newargv[m++] = "-v" ;
        newargv[m++] = o.verbosity ? "2" : "0" ;
      }
      if (o.flagw) newargv[m++] = "-w" ;
      if (o.flagD) newargv[m++] = "-D" ;
      if (o.flagH) newargv[m++] = "-H" ;
      if (o.flagr) newargv[m++] = "-r" ;
      if (o.flagp) newargv[m++] = "-p" ;
      if (o.localname)
      {
        newargv[m++] = "-l" ;
        newargv[m++] = o.localname ;
      }
      if (o.banner)
      {
        newargv[m++] = "-B" ;
        newargv[m++] = o.banner ;
      }
      if (o.timeout)
      {
        newargv[m++] = "-t" ;
        newargv[m++] = fmt + pos ;
        pos += uint_fmt(fmt + pos, o.timeout) ;
        fmt[pos++] = 0 ;
      }
      if (o.ruleswhat)
      {
        newargv[m++] = o.ruleswhat == 1 ? "-i" : "-x" ;
        newargv[m++] = o.rules ;
      }
      newargv[m++] = "--" ;
    }
    newargv[m++] = S6_NETWORKING_BINPREFIX "s6-tlsd" ;
    if (o.verbosity != 1)
    {
      newargv[m++] = "-v" ;
      newargv[m++] = o.verbosity ? "2" : "0" ;
    }
    if (o.flagS) newargv[m++] = "-S" ;
    if (o.flagy) newargv[m++] = "-y" ;
    if (o.kimeout)
    {
      newargv[m++] = "-K" ;
      newargv[m++] = fmt + pos ;
      pos += uint_fmt(fmt + pos, o.kimeout) ;
      fmt[pos++] = 0 ;
    }
    newargv[m++] = "--" ;
    while (*argv) newargv[m++] = *argv++ ;
    newargv[m++] = 0 ;
    pathexec_run(newargv[0], newargv, envp) ;
    strerr_dieexec(111, newargv[0]) ;
  }
}