/*
*				fitscleanup.c
*
* Signal-catching routines to clean-up temporary files.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	AstrOmatic FITS/LDAC library
*
*	Copyright:		(C) 1995-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
*
*	License:		GNU General Public License
*
*	AstrOmatic software is free software: you can redistribute it and/or
*	modify it under the terms of the GNU General Public License as
*	published by the Free Software Foundation, either version 3 of the
*	License, or (at your option) any later version.
*	AstrOmatic software is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with AstrOmatic software.
*	If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		09/10/2010
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fitscat_defs.h"
#include "fitscat.h"

#define	CLEANUP_NFILES	64

void	(*exit_func)(void);
char	**cleanup_filename;
int	cleanup_nfiles;

/****** cleanup_files ********************************************************
PROTO	void cleanup_files(void)
PURPOSE	Remove temporary files on exit.
INPUT	-.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	25/04/2002
 ***/
void	cleanup_files(void)
  {
   char		**filename;
   int		i;

  filename = cleanup_filename;
  for (i=cleanup_nfiles; i--;)
    {
    remove(*filename);
    free(*(filename++));
    }
  if (cleanup_nfiles)
    {
    free(cleanup_filename);
    cleanup_nfiles = 0;
    }

  return;
  }


/****** add_cleanupfilename **************************************************
PROTO	void add_cleanupfilename(char *filename)
PURPOSE	Add a file name to the list of files to be cleaned up at exit.
INPUT	pointer to filename char string.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	10/01/2003
 ***/
void	add_cleanupfilename(char *filename)
  {

  if (!cleanup_nfiles)
    {
    QMALLOC(cleanup_filename, char *, CLEANUP_NFILES);
    }
  else if (!(cleanup_nfiles%CLEANUP_NFILES))
    {
    QREALLOC(cleanup_filename, char *, cleanup_nfiles+CLEANUP_NFILES);
    }
  QMALLOC(cleanup_filename[cleanup_nfiles], char, MAXCHARS);
  strcpy(cleanup_filename[cleanup_nfiles++], filename);

  return;
  }


/****** remove_cleanupfilename ***********************************************
PROTO	void remove_cleanupfilename(char *filename)
PURPOSE	remove a file name from the list of files to be cleaned up at exit.
INPUT	pointer to filename char string.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	16/07/2007
 ***/
void	remove_cleanupfilename(char *filename)
  {
   char		**filename2, **filename3;
   int		i, j;

  if (!cleanup_nfiles)
    return;

/* Search the cleanup filename list for a match */
  filename2 = cleanup_filename;
  for (i=cleanup_nfiles; i--;)
    if (!strcmp(filename, *(filename2++)))
      {
/* Match found: update the list and free memory is necessary*/
      filename3 = filename2 - 1;
      free(*filename3);
      for (j=i; j--;)
        *(filename3++) = *(filename2++);
      if (!((--cleanup_nfiles)%CLEANUP_NFILES))
        {
        if (cleanup_nfiles)
          {
          QREALLOC(cleanup_filename, char *, cleanup_nfiles);
          }
        else
          free(cleanup_filename);
        }
      break;
      }

  return;
  }


/****** install_cleanup ******************************************************
PROTO	void install_cleanup(void (*func)(void))
PURPOSE	Install the signal-catching and exit routines to start cleanup_files().
INPUT	A pointer to a function to be executed on exit.
OUTPUT	-.
NOTES	Catches everything except STOP and KILL signals.
AUTHOR	E. Bertin (IAP)
VERSION	25/04/2002
 ***/
void	install_cleanup(void (*func)(void))
  {
   void	signal_function(int signum);

  exit_func = func;

  atexit(cleanup_files);
/* Catch CTRL-Cs */
  signal(SIGINT, signal_function);
/* Catch bus errors */
#ifndef WIN32
  signal(SIGBUS, signal_function);
#endif
/* Catch segmentation faults */
  signal(SIGSEGV, signal_function);
/* Catch floating exceptions */
  signal(SIGFPE, signal_function);

  return;
  }


/****** signal_function ******************************************************
PROTO	void signal_function(void)
PURPOSE	The routine called when a signal is catched. Clean up temporary files
	and execute a user-provided function.
INPUT	signal number.
OUTPUT	-.
NOTES   .
AUTHOR	E. Bertin (IAP)
VERSION	25/04/2002
 ***/
void	signal_function(int signum)
  {
  cleanup_files();
  if (exit_func)
    exit_func();

  switch(signum)
    {
    case SIGINT:
      fprintf(stderr, "^C\n");
      exit(-1);
#ifndef WIN32
    case SIGBUS:
      fprintf(stderr, "bus error\n");
      exit(-1);
#endif
    case SIGSEGV:
      fprintf(stderr, "segmentation fault\n");
      exit(-1);
    case SIGFPE:
      fprintf(stderr, "floating exception\n");
      exit(-1);
    default:
      exit(-1);
    }

  return;
  }

