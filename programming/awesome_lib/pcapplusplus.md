# PcapPlusplus：还不错的pcap文件读写库

## 一个小坑

pcpp::PcappFileReaderDevice这个类的拷贝构造函数是private的，这使得std::vector在push_back的时候报错。
并且没有public的空构造函数，
很神奇，不知道为何这样做，要push到vector，只能通过new的方式新建指针,然后把指针push到vector里面。