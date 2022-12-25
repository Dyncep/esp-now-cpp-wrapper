#ifndef ESP_NOW_CPP_WRAPPER_HPP
#define ESP_NOW_CPP_WRAPPER_HPP

#include "esp_now.h"
#include <functional>
#include <string.h>
#include <vector>

/**
 * struct EspNowPeerInfo - used when adding a peer without encrypted
 * communication, also used as a nested struct when adding a peer with encrypted
 * communication
 */
struct EspNowPeerInfo {
  /**
   * @mac mac address of the peer
   */
  uint8_t mac[ESP_NOW_ETH_ALEN];

  /**
   * @peer_local_master_key peer local master keep that is used to encrypt data
   */
  uint8_t peer_local_master_key[ESP_NOW_KEY_LEN];

  /**
   * @wifi_channel channel that the peer uses (wifi)
   */
  uint8_t wifi_channel{};

  /**
   * @peer_wifi_interface_id the wifi interface that the peer uses
   */
  wifi_interface_t peer_wifi_interface_id{};

  /**
   * @encrypt whether or not this peer sends their data encrypted
   */
  bool encrypt = {false};

  /**
   * @peer_private_data the peer private data
   */
  void *peer_private_data{nullptr};
};

class EspNow {
public:
  /** the send data call back type **/
  using SendCallback =
      std::function<void(const uint8_t *mac, esp_now_send_status_t status)>;

  /** the receive data call back type **/
  using ReceiveCallback =
      std::function<void(const esp_now_recv_info_t &receive_info,
                         const uint8_t *data, int data_len)>;

  /**
   * @brief constructor
   */
  EspNow();

  /**
   * @brief destructor
   */
  ~EspNow();

  /**
   * @brief initialize the instance
   */
  esp_err_t init();

  /**
   * @brief deinitialize the instance (also called in the destructor)
   */
  esp_err_t deinit();

  /**
   * @brief add a peer
   *
   * @param peer_info - the peer info
   */
  esp_err_t addPeer(const EspNowPeerInfo &peer_info);

  /**
   * @brief send data to the peer
   *
   * @param peer_addr - the address of the peer
   * @param data - the data to send
   * @param data_len - the length of the data to send
   *
   * @return esp_err_t
   */
  esp_err_t send(const uint8_t *peer_addr, const uint8_t *data,
                 size_t data_len);

  /**
   * @brief getter method for the receive callbacks
   */
  const std::vector<ReceiveCallback> &getReceiveCallbacks() const {
    return receive_callbacks;
  }

  /**
   * @brief getter method for the send callbacks
   */
  const std::vector<SendCallback> &getSendCallbacks() const {
    return send_callbacks;
  }

  /**
   * @brief add a receive callback to the list of receive callbacks
   *
   * @param receive_callback - the receive callback to add
   */
  void addReceiveCallback(const ReceiveCallback &receive_callback) {
    receive_callbacks.push_back(receive_callback);
  }

  /**
   * @brief add a send callback to the list of send callbacks
   *
   * @param send_callback - the send callback to add
   */
  void addSendCallback(const SendCallback &send_callback) {
    send_callbacks.push_back(send_callback);
  }

private:
  /**
   * @receive_callbacks the receive callbacks
   */
  std::vector<ReceiveCallback> receive_callbacks{};

  /**
   * @send_callbacks the send callbacks
   */
  std::vector<SendCallback> send_callbacks{};
};

#endif
