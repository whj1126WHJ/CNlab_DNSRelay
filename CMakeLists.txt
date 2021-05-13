# CMakeList.txt: CNlab_DNSRelay 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("CNlab_DNSRelay")

# 将源代码添加到此项目的可执行文件。
add_executable (CNlab_DNSRelay "CNlab_DNSRelay.c" "CNlab_DNSRelay.h" "QueryParser.c" "QueryParser.h" "Utils.c" "Utils.h" "DNSHeader.c" "DNSHeader.h" "DNSQuestion.c" "DNSQuestion.h" "DNSRR.c" "DNSRR.h")

# TODO: 如有需要，请添加测试并安装目标。
