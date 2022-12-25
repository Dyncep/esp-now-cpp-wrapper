### ESP-IDF ESPNOW C++ Wrapper

A simple C++ wrapper for the ESP-NOW functions.
This project is not being actively maintained and has not been thoroughly tested.

## Examples

Initialization (wifi is not handled by this wrapper and only one instance of it should be constructed)

```cpp
EspNow esp_now{};
esp_now.init();
```

Adding a peer

```cpp
/** info struct where parameters such as (peer mac, peer lmk, wifi channel, peer wifi interface, encryption are configured) **/
EspNowPeerInfo i{};
esp_err_t e = esp_now.addPeer(i);
```

Sending data

```cpp
uint8_t peer_addr[ESP_NOW_ETH_ALEN]{};
uint8_t data[3]{0x0, 0x0, 0x0};
esp_err_t = esp_now.send(peer_addr, data, 3);
```

Send Callbacks

```cpp
/** mac is the peer mac, status is the status **/
esp_now.addSendCallback([](const uint8_t *mac, esp_now_send_status_t status) {
  /** do something **/
  });
```

Receive Callbacks

```cpp
/** receive info contains data about src/dest.., data is the received data, data_len is the length of the received data **/
esp_now.addReceiveCallback([](const esp_now_recv_info_t &receive_info, const uint8_t *data, data_len) {
  /** do something **/
  });
```
