# coding: utf-8
import numpy as np
from PIL import Image, ImageDraw
from math import sin,cos
from numpngw import write_apng

W,H = 1024,1024


COLOR_BLACK   = (0x00, 0x00, 0x00, 0x00)
COLOR_WHITE   = (0xF0, 0xF0, 0xE0)
COLOR_BLUE    = (0x0D, 0x36, 0xFF)
COLOR_BLYNK   = (0x2E, 0xFF, 0xB9)
COLOR_RED     = (0xFF, 0x10, 0x08)
COLOR_MAGENTA = (0xA7, 0x00, 0xFF)

def fr(color):
    im = Image.new('RGBA', (W,H))
    surface = ImageDraw.Draw(im)
    surface.ellipse((10,10,W-20,H-20), fill=color)
    del surface
    return np.array(im.resize((16,16), Image.ANTIALIAS))

def fade(color):
    frames = []
    for i in range(25):
        frames.append(fr(color + tuple([int(i*(255.0/25))])))
    for i in range(25):
        frames.append(fr(color + tuple([255-int(i*(255.0/25))])))
    return frames

path = "./docs/images/states/"
write_apng(path + "0.png", map(fr,[COLOR_BLUE, COLOR_BLACK]), delay=[50, 500])
write_apng(path + "1.png", map(fr,[COLOR_BLUE, COLOR_BLACK]), delay=[200, 200])
write_apng(path + "2.png", map(fr,[COLOR_BLYNK, COLOR_BLACK]), delay=[50, 500])
write_apng(path + "3.png", map(fr,[COLOR_BLYNK, COLOR_BLACK]), delay=[100, 100])
write_apng(path + "4.png", fade(COLOR_BLYNK), delay=100)
write_apng(path + "5.png", map(fr,[COLOR_MAGENTA, COLOR_BLACK]), delay=[50, 50])
write_apng(path + "6.png", map(fr,[COLOR_RED, COLOR_BLACK, COLOR_RED, COLOR_BLACK]), delay=[80, 100, 80, 1000])
write_apng(path + "7.png", fade(COLOR_WHITE), delay=50)
write_apng(path + "8.png", map(fr,[COLOR_WHITE, COLOR_BLACK]), delay=[100, 100])