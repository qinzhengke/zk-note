VMware：目前最好的虚拟机软件{#vmware}
=================================

# Linux虚拟机浏览器如何连接主机proxy服务？

Step 1: 虚拟机的网络配置中选择“NAT模式”。

Step 2: 在host中启动proxy服务，例如基于v2ray的Netch，设置好socks5和HTTP端口，例如分别是2801和2802，并且要设置“允许外部设备接入”，（有的时候又叫做“允许局域网设备接入”，一个意思）。

Step 3: 查看host的IP地址，Windows用ipconfig，Linux用ifconfig，尝试在虚拟机中ping主机的ip地址，确保能够ping通。

Step 4: 下载ProxySwitchOmega插件，在github上可以下载，以crx文件保存。74以后的chrome和chrominium都不能直接安装crx文件，需要特殊的安装手法。将.crx后缀重命名为.rar，然后解压成文件夹，在chrome://extensions地址上，打开开发者模式，然后选择“Load unpacked”，选择刚才解压的文件夹即可。

Step 5: 在SwitchOmega插件界面设置proxy账户，填入主机的ip地址和HTTP端口号，即2802，点击“应用”。上外网的时候选对应proxy账户即可。