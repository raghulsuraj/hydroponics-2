/* This file is a part of 'hydroponics' project.
 * Copyright (c) 2015, Artyom Lebedev <artyom.lebedev@gmail.com>
 * All rights reserved.
 * See LICENSE file for copyright details.
 */

/** @file bitmap.cpp
 * TODO insert description here.
 */

#include "cpu.h"

using namespace adk;

const Bitmaps bitmaps PROGMEM;

BitmapWriter bitmapWriter;

void
BitmapWriter::Poll()
{
    AtomicSection as;
    if (!reqInProgress) {
        StartRequest();
    }
}

void
BitmapWriter::Write(u8 column, u8 page, const Bitmap *bitmap, bool inversed,
                    DoneHandler handler, bool isPgm, bool clear)
{
    AtomicSection as;
    /* Find queue free slot. */
    u8 idx = curReq;
    while (true) {
        if (!reqQueue[idx].bmp) {
            /* Free slot found. */
            break;
        }
        idx++;
        if (idx >= SIZEOF_ARRAY(reqQueue)) {
            idx = 0;
        }
        if (idx == curReq) {
            /* No free slot. */
            return;
        }
    }
    Request &req = reqQueue[idx];
    req.col = column;
    req.page = page;
    req.bmp = bitmap;
    req.inversed = inversed;
    req.handler = handler;
    req.isPgm = isPgm;
    req.clear = clear;
    if (idx == curReq) {
        /* Start output. */
        StartRequest();
    }
}

bool
BitmapWriter::StartRequest()
{
    Request &req = reqQueue[curReq];
    if (!req.bmp) {
        return false;
    }

    reqInProgress = true;

    if (req.isPgm) {
        curBmpWidth = pgm_read_byte(&req.bmp->numColumns);
        curBmpHeight = pgm_read_byte(&req.bmp->numPages);
        if (!req.clear) {
            curBmpData = reinterpret_cast<const u8 *>(pgm_read_word(&req.bmp->data));
            curBmpData += reinterpret_cast<uintptr_t>(&bitmaps);
        }
    } else {
        curBmpWidth = req.bmp->numColumns;
        curBmpHeight = req.bmp->numPages;
        curBmpData = req.bmp->data;
    }

    if (curBmpWidth > DISPLAY_COLUMNS - req.col) {
        curBmpCroppedWidth = DISPLAY_COLUMNS - req.col;
    } else {
        curBmpCroppedWidth = curBmpWidth;
    }

    if (curBmpHeight > DISPLAY_PAGES - req.page) {
        curBmpCroppedHeight = DISPLAY_PAGES - req.page;
    } else {
        curBmpCroppedHeight = curBmpHeight;
    }

    display.Output(Display::Viewport {
        req.col, static_cast<u8>(req.col + curBmpCroppedWidth - 1),
        req.page, static_cast<u8>(req.page + curBmpCroppedHeight - 1)},
        _OutputHandler);

    return true;
}

void
BitmapWriter::NextRequest()
{
    if (reqQueue[curReq].handler) {
        reqQueue[curReq].handler();
    }
    reqQueue[curReq].bmp = 0;
    if (curReq == SIZEOF_ARRAY(reqQueue)) {
        curReq = 0;
    } else {
        curReq++;
    }
    reqInProgress = false;
}

bool
BitmapWriter::_OutputHandler(u8 column, u8 page, u8 *data)
{
    return bitmapWriter.OutputHandler(column, page, data);
}

bool
BitmapWriter::OutputHandler(u8 column, u8 page, u8 *data)
{
    Request &req = reqQueue[curReq];
    u8 _data;
    if (req.clear) {
        _data = 0;
    } else {
        if (req.isPgm) {
            _data = pgm_read_byte(curBmpData);
        } else {
            _data = *curBmpData;
        }
        curBmpData++;
    }

    if (req.inversed) {
        *data = ~_data;
    } else {
        *data = _data;
    }

    if (curBmpWidth != curBmpCroppedWidth &&
        column == req.col + curBmpCroppedWidth -1) {

        curBmpData += curBmpWidth - curBmpCroppedWidth;
    }

    if (column == req.col + curBmpCroppedWidth - 1 &&
        page == req.page + curBmpCroppedHeight - 1) {
        /* Fully drawn. */
        NextRequest();
    }
    return true;
}
