#!/usr/bon/env python
"""
image converter
"""

from PIL import Image


def toRGB565(color):
    red = int(color[0] * 0x1f / 255) & 0x1f
    green = int(color[1] * 0x3f / 255) & 0x3f
    blue = int(color[2] * 0x1f / 255) & 0x1f
    return (red << 11) | (green << 5) | blue


def main():
    file = Image.open("TitleImage.bmp")
    pix = file.load()
    print()
    print()
    print(file.size)
    print(pix[0, 0])

    output = open("title.odb", "wb")
    output.write(int(file.size[0]).to_bytes(2, 'little'))
    output.write(int(file.size[1]).to_bytes(2, 'little'))
    for i in range(file.size[0]):
        for j in range(file.size[1]):
            output.write(toRGB565(pix[i, j]).to_bytes(2, 'little'))
    output.close()


if __name__ == "__main__":
    main()
