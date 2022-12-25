#include "EspNowWrapper.hpp"

namespace {

EspNow *esp_now_ptr = nullptr;

static void send_cb(const uint8_t *mac, esp_now_send_status_t status) {
  if (esp_now_ptr == nullptr) {
    return;
  }

  auto &callbacks = esp_now_ptr->getSendCallbacks();
  for (size_t i = 0; i < callbacks.size(); ++i) {
    callbacks[i](mac, status);
  }
}

static void recv_cb(const esp_now_recv_info_t *receive_info, const uint8_t *data, int data_len) {
  if (esp_now_ptr == nullptr) {
    return;
  }

  auto &callbacks = esp_now_ptr->getReceiveCallbacks();
  for (size_t i = 0; i < callbacks.size(); ++i) {
    callbacks[i](*receive_info, data, data_len);
  }
}

} // namespace

EspNow::EspNow() { esp_now_ptr = this; }

EspNow::~EspNow() {
  void(this->deinit());
  esp_now_ptr = nullptr;
}

esp_err_t EspNow::init() {
  auto err = esp_now_init();
  if (err != ESP_OK) {
    return err;
  }

  err = esp_now_register_send_cb(send_cb);
  if (err != ESP_OK) {
    return err;
  }

  err = esp_now_register_recv_cb(recv_cb);
  return err;
}

esp_err_t EspNow::deinit() {
  esp_now_unregister_recv_cb();
  esp_now_unregister_send_cb();
  return esp_now_deinit();
}

esp_err_t EspNow::addPeer(const EspNowPeerInfo &peer_info) {
  esp_now_peer_info_t p{};

  memcpy(&p.peer_addr, peer_info.mac, ESP_NOW_ETH_ALEN);
  memcpy(&p.lmk, peer_info.peer_local_master_key, ESP_NOW_KEY_LEN);

  p.encrypt = peer_info.encrypt;
  p.channel = peer_info.wifi_channel;
  p.ifidx = peer_info.peer_wifi_interface_id;
  p.priv = peer_info.peer_private_data;

  return esp_now_add_peer(&p);
}

esp_err_t EspNow::send(const uint8_t *peer_addr, const uint8_t *data,
                       size_t data_len) {
  return esp_now_send(peer_addr, data, data_len);
}
