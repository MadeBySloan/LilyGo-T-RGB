from pathlib import Path
from PIL import Image
def rgb(r,g,b):
    r = r*32//256
    g = g*32//256
    b = b*64//256
    return (r) + (b << 5) + (g << 11)
imageFolder="logoSpinSmall/Logo-spin_"
with open("animationDump.txt","w") as dump:
    haveFound = False
    fileCount = 0
    for i in range(99999):
        filePath = Path(f"{imageFolder}{i:05d}.png")
        if not filePath.exists():
            if haveFound == True:
                break
            else:
                continue
        haveFound = True
        fileCount += 1
    haveFound = False
    print (f"int numberOfFrames = {fileCount};",file=dump)
    print(f"uint16_t const frames[{fileCount}][{480*480}] = {{",file=dump)
    for i in range(99999):
        filePath = Path(f"{imageFolder}{i:05d}.png")
        if not filePath.exists():
            if haveFound == True:
                break
            else:
                continue
        haveFound = True
        img = Image.open(filePath)
        px = img.load()
        w,h = img.size
        assert w == 480,"wrong image size, it must be 480x480px"
        assert h == 480,"wrong image size, it must be 480x480px"
        print("{",end="",file=dump)
        for y in range(480):
            for x in range(480):
                r,g,b=px[x,y]
                value = rgb(r,g,b)
                print(f"0x{value:x},",end="",file=dump)
        print("},",file=dump)
    print("};",file=dump)