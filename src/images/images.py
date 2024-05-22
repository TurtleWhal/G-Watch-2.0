# type: ignore

import subprocess
import os

# Clear generated folder
folder = os.path.abspath("generated")
for filename in os.listdir(folder):
    file_path = os.path.join(folder, filename)
    try:
        if os.path.isfile(file_path) or os.path.islink(file_path):
            os.unlink(file_path)
        elif os.path.isdir(file_path):
            shutil.rmtree(file_path)
    except Exception as e:
        print('Failed to delete %s. Reason: %s' % (file_path, e))


hfile = open("images.hpp", "w")
hfile.truncate(0)
hfile.write("#include \"lvgl.h\"\n\n")

for file in os.listdir("files"):
    print("Converting Image: " + file)
    hfile.write("LV_IMAGE_DECLARE(" + file.split(".")[0] + ");\n")
    subprocess.call(["python3", "lv_img_conv.py", "--ofmt", "C", "--cf", "RGB565A8", "-o", "generated/", "--compress", "NONE", os.path.abspath("files/" + file)])