/*
    Capabilities

    Copyright (c) 2008 by Alexey Ignatiev <twosev@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef CAPABILITIES_H_
#define CAPABILITIES_H_

const char ICQ_CAPABILITY_SRVxRELAY[]   = { 0x09, 0x46, 0x13, 0x49, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_SHORTCAPS[]   = { 0x09, 0x46, 0x00, 0x00, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMVOICE[]    = { 0x09, 0x46, 0x13, 0x41, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMSENDFILE[] = { 0x09, 0x46, 0x13, 0x43, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_DIRECT[]      = { 0x09, 0x46, 0x13, 0x44, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMIMAGE[]    = { 0x09, 0x46, 0x13, 0x45, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMICON[]     = { 0x09, 0x46, 0x13, 0x46, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIM_STOCKS[]  = { 0x09, 0x46, 0x13, 0x47, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMGETFILE[]  = { 0x09, 0x46, 0x13, 0x48, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIM_GAMES[]   = { 0x09, 0x46, 0x13, 0x4A, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_BUDDY_LIST[]  = { 0x09, 0x46, 0x13, 0x4B, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AVATAR[]      = { 0x09, 0x46, 0x13, 0x4C, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIM_SUPPORT[] = { 0x09, 0x46, 0x13, 0x4D, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_UTF8[]        = { 0x09, 0x46, 0x13, 0x4E, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_RTFxMSGS[]    = { 0x97, 0xB1, 0x27, 0x51, 0x24, 0x3C,
                                            0x43, 0x34, 0xAD, 0x22, 0xD6, 0xAB,
                                            0xF7, 0x3F, 0x14, 0x92 };

const char ICQ_CAPABILITY_TYPING[]      = { 0x56, 0x3F, 0xC8, 0x09, 0x0B, 0x6F,
                                            0x41, 0xBD, 0x9F, 0x79, 0x42, 0x26,
                                            0x09, 0xDF, 0xA2, 0xF3 };

const char ICQ_CAPABILITY_AIMxINTER[]   = { 0x09, 0x46, 0x13, 0x4D, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICHAT[]       = { 0x09, 0x46, 0x00, 0x00, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_XTRAZ[]       = { 0x1A, 0x09, 0x3C, 0x6C, 0xD7, 0xFD,
                                            0x4E, 0xC5, 0x9D, 0x51, 0xA6, 0x47,
                                            0x4E, 0x34, 0xF5, 0xA0 };

const char ICQ_CAPABILITY_BART[]        = { 0x09, 0x46, 0x13, 0x46, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_LICQxVER[]    = { 'L',  'i',  'c',  'q',  ' ',  'c',
                                            'l',  'i',  'e',  'n',  't',  ' ',
                                            0x00, 0x00, 0x00, 0x00 };
 
const char ICQ_CAPABILITY_SIMxVER[]     = { 'S',  'I',  'M',  ' ',  'c',  'l', 
                                            'i',  'e',  'n',  't',  ' ',  ' ',
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_QUTIMxVER[]   = { 'q',  'u',  't',  'i',  'm',  0x00, 
                                            0x00,  0x00,  0x00,  0x00,  0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_K8QUTIMxVER[] = { 'k', '8', 'q', 'u', 't', 'I', 'M',
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_SIMOLDxVER[]  = { 0x97, 0xb1, 0x27, 0x51, 0x24, 0x3c,
                                            0x43, 0x34, 0xad, 0x22, 0xd6, 0xab,
                                            0xf7, 0x3f, 0x14, 0x00 };

const char ICQ_CAPABILITY_KOPETExVER[]  = { 'K',  'o',  'p',  'e',  't',  'e',
                                            ' ',  'I',  'C',  'Q',  ' ',  ' ',
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_MICQxVER[]    = { 'm',  'I',  'C',  'Q',  ' ', 0xA9,
                                            ' ',  'R',  '.',  'K',  '.',  ' ', 
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_MIRANDAxVER[] = { 'M', 'i', 'r', 'a', 'n', 'd', 'a',
                                            'M', 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_MIRMOBxVER[]  = { 'M', 'i', 'r', 'a', 'n', 'd', 'a',
                                            'M', 'o', 'b', 'i', 'l', 'e', 0x00,
                                            0x00, 0x00 };
                                              
const char ICQ_CAPABILITY_MIMPACKxVER[] = { 'M', 'I', 'M', '/', 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJS7xVER[]  = { 'i', 'c', 'q', 'j', 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJPxVER[]   = { 'i', 'c', 'q', 'p', 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJS7SxVER[] = { 'i', 'c', 'q', 'j', 0x00, 'S', 'e',
                                            'c', 'u', 'r', 'e', 0x00, 'I', 'M',
                                            0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJS7OxVER[] = { 0x69, 0x63, 0x71, 0x6a, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJSINxVER[] = { 's', 'i', 'n', 'j',  0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ICQJENxVER[]  = { 'e', 'n', 'q', 'j', 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMOSCARxVER[]= { 'M', 'i', 'r', 'a', 'n', 'd', 'a',
                                            'A', 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_TRILLIANxVER[]= { 0x97, 0xb1, 0x27, 0x51, 0x24, 0x3c,
                                            0x43, 0x34, 0xad, 0x22, 0xd6, 0xab,
                                            0xf7, 0x3f, 0x14, 0x09 };

const char ICQ_CAPABILITY_TRILCRPTxVER[]= { 0xf2, 0xe7, 0xc7, 0xf4, 0xfe, 0xad,
                                            0x4d, 0xfb, 0xb2, 0x35, 0x36, 0x79,
                                            0x8b, 0xdf, 0x00, 0x00 };

const char ICQ_CAPABILITY_CLIMMxVER[]   = { 'c', 'l', 'i', 'm', 'm', 0xA9, ' ',
                                            'R', '.', 'K', '.', ' ', 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ANDRQxVER[]   = { '&', 'R', 'Q', 'i', 'n', 's', 'i',
                                            'd', 'e', 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_RANDQxVER[]   = { 'R', '&', 'Q', 'i', 'n', 's', 'i',
                                            'd', 'e', 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_MCHATxVER[]   = { 'm', 'C', 'h', 'a', 't', ' ', 'i',
                                            'c', 'q', ' ', 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_JIMMxVER[]    = { 'J', 'i', 'm', 'm', ' ', 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_COREPGRxVER[] = { 'C', 'O', 'R', 'E', ' ', 'P', 'a',
                                            'g', 'e', 'r', 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_DICHATxVER[]  = { 'D', '[', 'i', ']', 'C', 'h', 'a',
                                            't', ' ', 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_NAIMxVER[]    = { 0xFF, 0xFF, 0xFF, 0xFF, 'n', 'a',
                                            'i', 'm', 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_ANSTxVER[]    = { 0x44, 0xE5, 0xBF, 0xCE, 0xB0, 0x96,
                                            0xE5, 0x47, 0xBD, 0x65, 0xEF, 0xD6,
                                            0xA3, 0x7E, 0x36, 0x02 };

const char ICQ_CAPABILITY_QIPxVER[]     = { 0x56, 0x3F, 0xC8, 0x09, 0x0B, 0x6F,
                                            0x41, 'Q', 'I', 'P', ' ', '2', '0',
                                            '0', '5', 'a' };

const char ICQ_CAPABILITY_QIPPDAxVER[]  = { 0x56, 0x3F, 0xC8, 0x09, 0x0B, 0x6F,
                                            0x41, 'Q', 'I', 'P', ' ', ' ', ' ',
                                            ' ', ' ', '!' };

const char ICQ_CAPABILITY_QIPMOBxVER[]  = { 0x56, 0x3F, 0xC8, 0x09, 0x0B, 0x6F,
                                            0x41, 'Q', 'I', 'P', ' ', ' ', ' ',
                                            ' ', ' ', '"' };

const char ICQ_CAPABILITY_QIPINFxVER[]  = { 0x7C, 0x73, 0x75, 0x02, 0xC3, 0xBE,
                                            0x4F, 0x3E, 0xA6, 0x9F, 0x01, 0x53,
                                            0x13, 0x43, 0x1E, 0x1A };

const char ICQ_CAPABILITY_QIPPLUGINS[]  = { 0x7c, 0x53, 0x3f, 0xfa, 0x68, 0x00,
                                            0x4f, 0x21, 0xbc, 0xfb, 0xc7, 0xd2,
                                            0x43, 0x9a, 0xad, 0x31 };

const char ICQ_CAPABILITY_QIP1[]        = { 0xd3, 0xd4, 0x53, 0x19, 0x8b, 0x32,
                                            0x40, 0x3b, 0xac, 0xc7, 0xd1, 0xa9,
                                            0xe2, 0xb5, 0x81, 0x3e };

const char ICQ_CAPABILITY_QIPSYMBxVER[] = { 0x51, 0xad, 0xd1, 0x90, 0x72, 0x04,
                                            0x47, 0x3d, 0xa1, 0xa1, 0x49, 0xf4,
                                            0xa3, 0x97, 0xa4, 0x1f };

const char ICQ_CAPABILITY_VMICQxVER[]   = { 0x56, 0x6d, 0x49, 0x43, 0x51, 0x20,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_SMAPERxVER[]  = { 'S', 'm', 'a', 'p', 'e', 'r', ' ',
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_IMPLUXxVER[]  = { 0x8e, 0xcd, 0x90, 0xe7, 0x4f, 0x18,
                                            0x28, 0xf8, 0x02, 0xec, 0xd6, 0x18,
                                            0xa4, 0xe9, 0xde, 0x68 };

const char ICQ_CAPABILITY_YAPPxVER[]    = { 0x59, 0x61, 0x70, 0x70, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_IM2xVER[]     = { 0x74, 0xED, 0xC3, 0x36, 0x44, 0xDF,
                                            0x48, 0x5B, 0x8B, 0x1C, 0x67, 0x1A,
                                            0x1F, 0x86, 0x09, 0x9F };

const char ICQ_CAPABILITY_MACICQxVER[]  = { 0xdd, 0x16, 0xf2, 0x02, 0x84, 0xe6,
                                            0x11, 0xd4, 0x90, 0xdb, 0x00, 0x10,
                                            0x4b, 0x9b, 0x4b, 0x7d };

const char ICQ_CAPABILITY_IS2001[]      = { 0x2e, 0x7a, 0x64, 0x75, 0xfa, 0xdf,
                                            0x4d, 0xc8, 0x88, 0x6f, 0xea, 0x35,
                                            0x95, 0xfd, 0xb6, 0xdf };
const char ICQ_CAPABILITY_IS2002[]      = { 0x10, 0xcf, 0x40, 0xd1, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };
const char ICQ_CAPABILITY_COMM20012[]   = { 0xa0, 0xe9, 0x3f, 0x37, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_STRICQxVER[]  = { 0xa0, 0xe9, 0x3f, 0x37, 0x4f, 0xe9,
                                            0xd3, 0x11, 0xbc, 0xd2, 0x00, 0x04,
                                            0xac, 0x96, 0xdd, 0x96 };

const char ICQ_CAPABILITY_ICQLITExVER[] = { 0x17, 0x8C, 0x2D, 0x9B, 0xDA, 0xA5,
                                            0x45, 0xBB, 0x8D, 0xDB, 0xF3, 0xBD,
                                            0xBD, 0x53, 0xA1, 0x0A };

const char ICQ_CAPABILITY_AIMCHAT[]     = { 0x74, 0x8F, 0x24, 0x20, 0x62, 0x87,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_PIGEONxVER[]  = { 'P', 'I', 'G', 'E', 'O', 'N', '!',
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00};

const char ICQ_CAPABILITY_RAMBLER[]     = { 0x7E, 0x11, 0xB7, 0x78, 0xA3, 0x53,
                                            0x49, 0x26, 0xA8, 0x02, 0x44, 0x73,
                                            0x52, 0x08, 0xC4, 0x2A };

const char ICQ_CAPABILITY_ABV[]         = { 0x00, 0xE7, 0xE0, 0xDF, 0xA9, 0xD0,
                                            0x4F, 0xe1, 0x91, 0x62, 0xC8, 0x90,
                                            0x9A, 0x13, 0x2A, 0x1B };

const char ICQ_CAPABILITY_NETVIGATOR[]  = { 0x4C, 0x6B, 0x90, 0xA3, 0x3D, 0x2D,
                                            0x48, 0x0E, 0x89, 0xD6, 0x2E, 0x4B,
                                            0x2C, 0x10, 0xD9, 0x9F };

const char ICQ_CAPABILITY_TZERS[]       = { 0xb2, 0xec, 0x8f, 0x16, 0x7c, 0x6f,
                                            0x45, 0x1b, 0xbd, 0x79, 0xdc, 0x58,
                                            0x49, 0x78, 0x88, 0xb9 };

const char ICQ_CAPABILITY_HTMLMSGS[]    = { 0x01, 0x38, 0xca, 0x7b, 0x76, 0x9a,
                                            0x49, 0x15, 0x88, 0xf2, 0x13, 0xfc,
                                            0x00, 0x97, 0x9e, 0xa8 };

const char ICQ_CAPABILITY_LIVEVIDEO[]   = { 0x09, 0x46, 0x01, 0x01, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_SIMPLITE[]    = { 0x53, 0x49, 0x4D, 0x50, 0x53, 0x49,
                                            0x4D, 0x50, 0x53, 0x49, 0x4D, 0x50,
                                            0x53, 0x49, 0x4D, 0x50 };

const char ICQ_CAPABILITY_SIMPPRO[]     = { 0x53, 0x49, 0x4D, 0x50, 0x5F, 0x50,
                                            0x52, 0x4F, 0x53, 0x49, 0x4D, 0x50,
                                            0x5F, 0x50, 0x52, 0x4F };

const char ICQ_CAPABILITY_IMSECURE[]    = { 'I', 'M', 's', 'e', 'c', 'u', 'r',
                                            'e', 'C', 'p', 'h', 'r', 0x00,
                                            0x00, 0x06, 0x01 };

const char ICQ_CAPABILITY_MSGTYPE2[]    = { 0x09, 0x49, 0x13, 0x49, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMICQ[]      = { 0x09, 0x46, 0x13, 0x4D, 0x4C, 0x7F,
                                            0x11, 0xD1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMAUDIO[]    = { 0x09, 0x46, 0x01, 0x04, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_PALMJICQ[]    = { 'J', 'I', 'C', 'Q', 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_INLUXMSGR[]   = { 0xA7, 0xE4, 0x0A, 0x96, 0xB3, 0xA0,
                                            0x47, 0x9A, 0xB8, 0x45, 0xC9, 0xE4,
                                            0x67, 0xC5, 0x6B, 0x1F };

const char ICQ_CAPABILITY_MIPCLIENT[]   = { 0x4D, 0x49, 0x50, 0x20, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_IMADERING[]   = { 'I', 'M', 'a', 'd', 'e', 'r', 'i',
                                            'n', 'g', ' ', 'C', 'l', 'i', 'e',
                                            'n', 't' };

const char ICQ_CAPABILITY_NATICQxVER[]  = { 'N', 'a', 't', 'I', 'C', 'Q',
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };
                                            
const char ICQ_CAPABILITY_WEBICQPRO[]   = { 'W', 'e', 'b', 'I', 'c', 'q', 'P',
                                            'r', 'o', 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_BAYANICQxVER[]= { 'b', 'a', 'y', 'a', 'n', 'I', 'C',
                                            'Q', 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00 };

//AIM Client version 5.9 capabilities
const char ICQ_CAPABILITY_AIMADDINGS[]  = { 0x09, 0x46, 0x13, 0x47, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMCONTSEND[] = { 0x09, 0x46, 0x13, 0x4b, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMUNK2[]     = { 0x09, 0x46, 0x01, 0x02, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x00, 0x00 };

const char ICQ_CAPABILITY_AIMSNDBDDLST[]= { 0x09, 0x46, 0x00, 0x00, 0x4c, 0x7f,
                                            0x11, 0xd1, 0x82, 0x22, 0x44, 0x45,
                                            0x53, 0x54, 0x13, 0x4B };

const char ICQ_CAPABILITY_IMSECKEY1[]   = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

const char ICQ_CAPABILITY_IMSECKEY2[]   = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00 };

// Short capabilities

const unsigned short ICQ_SHORTCAP_SHORTCAPS    = 0x0000;

const unsigned short ICQ_SHORTCAP_AIMVOICE     = 0x1341;

const unsigned short ICQ_SHORTCAP_SENDFILE     = 0x1343;

const unsigned short ICQ_SHORTCAP_DIRECT       = 0x1344;

const unsigned short ICQ_SHORTCAP_AIMIMAGE     = 0x1345;

const unsigned short ICQ_SHORTCAP_BUDDYCON     = 0x1346;

const unsigned short ICQ_SHORTCAP_AIMSTOCKS    = 0x1347;

const unsigned short ICQ_SHORTCAP_GETFILE      = 0x1348;

const unsigned short ICQ_SHORTCAP_RELAY        = 0x1349;

const unsigned short ICQ_SHORTCAP_GAMES        = 0x134a;

const unsigned short ICQ_SHORTCAP_AIMBUDDYLIST = 0x134b;

const unsigned short ICQ_SHORTCAP_AVATAR       = 0x134c;

const unsigned short ICQ_SHORTCAP_AIMSUPPORT   = 0x134d;

const unsigned short ICQ_SHORTCAP_UTF          = 0x134e;
                                            
#endif /*CAPABILITIES_H_*/
