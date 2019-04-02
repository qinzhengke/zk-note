Android make{#android_make}
===========================

### LOCAL_SRC_FILES宏的初始路径
给LOCAL_SRC_FILES添加源文件的时候，添加的路径一定要以LOCAL_PATH为起点，一定不要使用其他起点，包括绝对路径。
