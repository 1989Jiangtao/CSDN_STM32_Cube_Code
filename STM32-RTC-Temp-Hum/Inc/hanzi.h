#ifndef __HANZI_H
#define __HANZI_H

//宽x高:16*16
const unsigned char ZH16x16[][16]={	  
	
{0x10,0x60,0x02,0x0C,0xC0,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,0x00},
{0x04,0x04,0x7C,0x03,0x20,0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x20,0x00},/*"江",0*/

{0x10,0x60,0x02,0x8C,0x00,0x44,0x54,0x54,0xF4,0x5F,0x54,0xD4,0x54,0x44,0x40,0x00},
{0x04,0x04,0x7E,0x01,0x20,0x10,0x0C,0x03,0x0A,0x52,0x82,0x7F,0x02,0x02,0x02,0x00},/*"涛",1*/

{0x00,0xF8,0x0C,0x0B,0x08,0x08,0xF8,0x40,0x30,0x8F,0x08,0x08,0x08,0xF8,0x00,0x00},
{0x00,0x7F,0x21,0x21,0x21,0x21,0x7F,0x00,0x00,0x00,0x43,0x80,0x40,0x3F,0x00,0x00},/*"的",2*/

{0x20,0x20,0xFF,0x20,0x24,0xF4,0x54,0x54,0x54,0xFF,0x54,0x55,0x56,0xF4,0x04,0x00},
{0x00,0x00,0xFF,0x00,0x08,0x0B,0x19,0x69,0x09,0x0B,0x49,0x89,0x7D,0x0B,0x08,0x00},/*"博",3*/

{0x10,0x0C,0x84,0x44,0xB4,0xA4,0x25,0x26,0x24,0xA4,0x64,0x24,0x04,0x14,0x0C,0x00},
{0x04,0x04,0x04,0xFA,0x4A,0x4A,0x49,0x49,0x49,0x4A,0x4A,0xFA,0x04,0x04,0x04,0x00},/*"客",4*/

{0x10,0x92,0x92,0xFE,0x92,0x92,0xFE,0x10,0x09,0x8A,0x8C,0xF8,0x8C,0x8A,0x09,0x00},
{0x10,0x0C,0xFF,0x44,0x44,0x44,0xFC,0x00,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x00},/*"群",5*/

};

//宽x高:32*32
const unsigned char ZH32x32[][32]={	  


{0x00,0x00,0x00,0x00,0x08,0x30,0xF0,0xE0,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xC0,0xC0,0x40,0x40,0x40,0x40,0x40,0x40,0x60,0x60,0x40,0x00,0x00,0x00},
{0x00,0x00,0x04,0x0C,0x38,0x30,0x00,0x80,0x70,0x0E,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x20,0x20,0xE0,0xF0,0x3C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x07,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x06,0x06,0x04,0x00,0x00},/*"江",0*/

{0x00,0x00,0x00,0x00,0x00,0x08,0x30,0xF0,0xE0,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFC,0xF8,0x88,0x80,0x80,0x80,0x80,0x80,0xC0,0xE0,0x80,0x00,0x00,0x00},
{0x00,0x00,0x08,0x18,0x30,0x70,0x00,0x00,0xE0,0x9C,0x83,0x80,0x80,0x88,0x88,0x88,0xE8,0xFC,0x9F,0x89,0x88,0x88,0x88,0x88,0x88,0x8C,0xCC,0xC0,0xC0,0x80,0x00,0x00},
{0x00,0x00,0x40,0x40,0xC0,0xE0,0xF8,0x0F,0x00,0x00,0x00,0xC0,0x60,0x30,0x1C,0x17,0xD3,0x90,0x90,0x10,0x10,0x10,0x10,0x11,0xFE,0x16,0x10,0x10,0x18,0x10,0x00,0x00},
{0x00,0x00,0x00,0x00,0x1C,0x3F,0x3F,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x12,0x10,0x10,0x30,0x70,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"涛",1*/

{0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x80,0x7C,0x18,0x08,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x80,0xF0,0x78,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xFF,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0x81,0x40,0x30,0x1C,0x87,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0xFF,0x06,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xFF,0xFF,0x02,0x02,0x02,0x02,0x02,0x02,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0x0F,0x3E,0x18,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x3F,0x3F,0x08,0x08,0x08,0x08,0x08,0x08,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x10,0x30,0x70,0x38,0x3F,0x0F,0x00,0x00,0x00,0x00},/*"的",2*/

{0x00,0x00,0x00,0x00,0x00,0xFC,0xF8,0x08,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFC,0x48,0x40,0x44,0x48,0x58,0x78,0x40,0x60,0x60,0x40,0x00,0x00},
{0x00,0x00,0x08,0x08,0x08,0xFF,0xFF,0x08,0x08,0x0C,0x08,0x00,0xFE,0xFE,0x22,0x22,0x22,0x22,0x22,0xFF,0x22,0x22,0x22,0x22,0x22,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x40,0x40,0x40,0xDF,0x4F,0x42,0x42,0x42,0x42,0x42,0x4F,0x42,0x42,0x42,0x4A,0xF2,0x5F,0x47,0x60,0x60,0x40,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0E,0x00,0x00,0x00,0x00,0x10,0x20,0x20,0x70,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"博",3*/

{0x00,0x00,0x00,0x00,0xC0,0xC0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x44,0x4C,0x58,0x78,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xC0,0xE0,0xC0,0x80,0x00,0x00},
{0x00,0x00,0x00,0x07,0x03,0x00,0x00,0x80,0x60,0x30,0x18,0x1E,0x2F,0x4A,0x88,0x08,0x08,0x88,0xC8,0x68,0x38,0x1C,0x18,0x00,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x80,0x80,0x80,0x40,0x42,0x41,0x20,0x20,0xE0,0x70,0x50,0x58,0x4C,0x44,0x47,0x43,0x43,0x44,0x4C,0x48,0x58,0xF8,0xF0,0x30,0x30,0x70,0x70,0x30,0x20,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"客",4*/

};


#endif
