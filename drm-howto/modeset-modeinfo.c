/*
 * modeset - DRM Modesetting Example
 *
 * Written 2012 by David Rheinsberg <david.rheinsberg@gmail.com>
 * Dedicated to the Public Domain.
 */

/*
 * DRM Modesetting Howto
 * This document describes the DRM modesetting API. Before we can use the DRM
 * API, we have to include xf86drm.h and xf86drmMode.h. Both are provided by
 * libdrm which every major distribution ships by default. It has no other
 * dependencies and is pretty small.
 *
 * Please ignore all forward-declarations of functions which are used later. I
 * reordered the functions so you can read this document from top to bottom. If
 * you reimplement it, you would probably reorder the functions to avoid all the
 * nasty forward declarations.
 *
 * For easier reading, we ignore all memory-allocation errors of malloc() and
 * friends here. However, we try to correctly handle all other kinds of errors
 * that may occur.
 *
 * All functions and global variables are prefixed with "modeset_*" in this
 * file. So it should be clear whether a function is a local helper or if it is
 * provided by some external library.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main(int argc, char **argv)
{
	int ret = 0;
	const char *card;

	/* check which DRM device to open */
	if (argc > 1)
		card = argv[1];
	else
		card = "/dev/dri/card0";

	fprintf(stderr, "using card '%s'\n", card);

	/* open the DRM device */
    int card_fd = open(card, O_RDWR | O_CLOEXEC);
    drmModeResPtr res = drmModeGetResources(card_fd);
    //uint32_t crtc_fd = res->crtcs[0];
    for(int i=0;i<res->count_connectors;i++){
        uint32_t conn_fd = res->connectors[i];
        printf("conn_fd:%d.\n",conn_fd);
        drmModeConnectorPtr conn = drmModeGetConnector(card_fd,conn_fd);
        printf("conn modes counter:%d.\n",conn->count_modes);
        for(int i=0;i<conn->count_modes;i++){
            drmModeModeInfo modeInfo = conn->modes[i];
            int w = modeInfo.hdisplay;
            int h = modeInfo.vdisplay;
            printf("w=%d,h=%d.\n",w,h);
        }
        drmModeFreeConnector(conn);
    }

   
    //printf("crtc_fd:%d.\n",crtc_fd);
    
        
    drmModeFreeResources(res);

    close(card_fd);

	return ret; 
}

