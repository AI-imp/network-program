# muticast
## 1.Preliminary knowledge
- A group corresponds to a multicast address
- 01:00:5E is the fixed prefix of the multicast MAC address
- IP multicast is implemented based on UDP, and UDP frames are encapsulated in Ethernet frames. The Ethernet frame format is as follows
###### ![a](https://github.com/AI-imp/network-program/blob/main/multicasts/pictures/1.png?raw=true)
## 2.Run code
### 2.1 Create a multicast group
run the multicast receiver program and sender program
```bash
gcc recevier.c -o recevier
./recevier [your multicast address] [your port]

```
### 2.2 Wireshark packet capture filter settings
```
host [your multicast address] and port [your port]
```
### 2.3 Test
###### ![image name](https://github.com/AI-imp/network-program/blob/main/multicasts/pictures/2.png?raw=true)





