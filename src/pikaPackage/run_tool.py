import subprocess

# 指定要运行的exe文件路径
exe_path = r"E:\work_space\LVGL\SimulatorV8.3\lv_simulator_show_xml\src\pikaPackage\rust-msc-latest-win10.exe"

# 调用exe文件
try:
    result = subprocess.run(exe_path, check=True)
    print("程序执行成功")
except subprocess.CalledProcessError as e:
    print(f"程序执行失败: {e}")