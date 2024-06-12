# type: ignore

import subprocess
import os
import shutil

def copy_and_rename(src_path, dest_path, new_name):
    shutil.copy(src_path, f"{dest_path}/{new_name}")

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


if os.name == "nt":
    windows = ["cmd.exe", "/c"]
else:
    windows = []


hfile = open("images.hpp", "w")
hfile.truncate(0)
hfile.write("#include \"lvgl.h\"\n\n")
hfile.write("#define SET_IMG(obj, img) lv_image_set_src(obj, &img);\n\n")

for file in os.listdir("files"):
    print("Converting Image: " + file)

    copy_and_rename("files/" + file, "generated", "IMG_" + file.split(".")[0].upper() + "." + file.split(".")[1])

    hfile.write("LV_IMAGE_DECLARE(IMG_" + file.split(".")[0].upper() + ");\n")
    subprocess.call(windows + ["python3", "lv_img_conv.py", "--ofmt", "C", "--cf", "RGB565A8", "-o", "generated/", "--compress", "NONE", os.path.abspath("generated/IMG_" + file.split(".")[0].upper() + "." + file.split(".")[1])])
    
    os.remove("generated/IMG_" + file.split(".")[0].upper() + "." + file.split(".")[1])