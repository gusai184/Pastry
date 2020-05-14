# Pastry
A distributed hash table (DHT) is a decentralized distributed system that provides a lookup same as hash table, and any node inside network can efficiently retrieve the value associated with a given key.

## Commnads<br>
  1) join \<id> \<ip> \<port> - Sends join request to Node having given id,ip and port in command already part of pastry network
  2) set \<key> <value> - Sets the key and value pair inside pastry network
  3) get \<key> - Fatches value of given key from pastry network
  4) lt - Prints own leaf table
  5) rt - Prints own Routing table 
  6) nt - Prints own neighbour table
  7) ht - prints own hash table
  8) exit - Graceful exit from pastry network 
 
 ## Command Execution sequence <br>
   1) make 
   2) sudo ./pastry 
   3) port : \<port>
   5) join \<id> \<ip> \<port> 
   6) set \<key> \<value> 
   7) get \<key> 
   8) exit

## References<br>
[http://rowstron.azurewebsites.net/PAST/pastry.pdf](http://rowstron.azurewebsites.net/PAST/pastry.pdf)<br>
[https://docs.google.com/document/d/1RTiLzbn8el-D5N875sDR2InlOhb4J_5mHeBUTWJqB6I/edit?usp=sharing](https://docs.google.com/document/d/1RTiLzbn8el-D5N875sDR2InlOhb4J_5mHeBUTWJqB6I/edit?usp=sharing)


