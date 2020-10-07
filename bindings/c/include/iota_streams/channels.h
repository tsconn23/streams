#ifndef IOTA_STREAMS_CHANNELS_H
#define IOTA_STREAMS_CHANNELS_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Address address_t;
typedef struct ChannelAddress channel_address_t;
typedef struct MsgId msgid_t;
typedef struct PublicKey public_key_t;
typedef struct PskIds psk_ids_t;
typedef struct KePks ke_pks_t;
typedef struct NextMsgIds next_msg_ids_t;
typedef struct UnwrappedMessage unwrapped_message_t;
typedef struct UnwrappedMessages unwrapped_messages_t;

typedef struct MessageLinks {
  address_t const *msg_link;
  address_t const *seq_link;
} message_links_t;

extern void drop_links(message_links_t);

typedef struct PacketPayloads {
  uint8_t const *public_payload_ptr;
  size_t public_payload_size;
  size_t public_payload_cap;
  uint8_t const *masked_payload_ptr;
  size_t masked_payload_size;
  size_t masked_payload_cap;
} packet_payloads_t;

extern void drop_payloads(packet_payloads_t);

////////////
/// Transport
////////////
typedef struct Transport transport_t;
extern transport_t *tsp_new();
extern void tsp_drop(transport_t *);

////////////
/// Author
////////////
typedef struct Author author_t;

extern author_t *auth_new(char const *seed, char const *encoding, size_t payload_length, uint8_t multi_branching, transport_t *tsp);
extern void auth_drop(author_t *);

extern channel_address_t const *auth_channel_address(author_t const *user);
extern uint8_t auth_is_multi_branching(author_t const *user);
extern public_key_t const *auth_get_public_key(author_t const *user);

// Announce
extern address_t const *auth_send_announce(author_t *author);
// Subscribe
extern void *auth_receive_subscribe(author_t *author, address_t const *address);
// Keyload
extern message_links_t auth_send_keyload(author_t *author, address_t const *link_to, psk_ids_t *psk_ids, ke_pks_t ke_pks);

extern message_links_t auth_send_keyload_for_everyone(author_t *author, address_t const *link_to);
// Tagged Packets
extern message_links_t auth_send_tagged_packet(author_t *author, message_links_t link_to, uint8_t const *public_payload_ptr, size_t public_payload_size, uint8_t const *masked_payload_ptr, size_t masked_payload_size);
extern packet_payloads_t auth_receive_tagged_packet(author_t *author, address_t const *address);
// Signed Packets
extern message_links_t auth_send_signed_packet(author_t *author, message_links_t link_to, uint8_t const *public_payload_ptr, size_t public_payload_size, uint8_t const *masked_payload_ptr, size_t masked_payload_size);
extern packet_payloads_t auth_receive_tagged_packet(author_t *author, address_t const *address) ;
// Sequence Message (for multi branch use)
extern address_t const *auth_receive_sequence(author_t *author, address_t const *address);
// MsgId generation
extern next_msg_ids_t *auth_gen_next_msg_ids(author_t *author);
// Generic Processing
extern unwrapped_message_t *auth_receive_msg(author_t *author, address_t const *address);
// Fetching/Syncing
extern unwrapped_messages_t *auth_fetch_next_msgs(author_t *author);
extern unwrapped_messages_t *auth_sync_state(author_t *author);

/////////////
// Subscriber
/////////////
typedef struct Subscriber subscriber_t;
extern subscriber_t *sub_new(char const *seed, char const *encoding, size_t payload_length, transport_t *tsp);
extern void sub_drop(subscriber_t *);

extern channel_address_t const *sub_channel_address(subscriber_t const *user);
extern uint8_t sub_is_multi_branching(subscriber_t const *user);
extern public_key_t const *sub_get_public_key(subscriber_t const *user);

// Registration state
extern uint8_t sub_is_registered(subscriber_t const *subscriber);
extern void sub_unregister(subscriber_t *subscriber);

// Announce
extern void sub_receive_announce(subscriber_t *subscriber, address_t const *address);
// Subscribe
extern address_t const *sub_send_subscribe(subscriber_t *subscriber, address_t const *announcement_link);
// Keyload
extern void sub_receive_keyload(subscriber_t *subscriber, address_t const *address);
// Tagged Packets
extern message_links_t sub_send_tagged_packet(subscriber_t *subscriber, message_links_t link_to, uint8_t const *public_payload_ptr, size_t public_payload_size, uint8_t const *masked_payload_ptr, size_t masked_payload_size);
extern packet_payloads_t sub_receive_tagged_packet(subscriber_t *subscriber, address_t const *address);
// Signed Packets
//extern message_links_t *sub_send_signed_packet(subscriber_t *subscriber, message_links_t *link_to, char *public_payload, char *private_payload);
extern packet_payloads_t sub_receive_signed_packet(subscriber_t *subscriber, address_t const *address);
// Sequence Message (for multi branch use)
extern address_t const *sub_receive_sequence(subscriber_t *subscriber, address_t const *address);
// MsgId Generation
extern next_msg_ids_t *sub_gen_next_msg_ids(subscriber_t *subscriber);
// Generic Message Processing
extern unwrapped_message_t *sub_receive_msg(subscriber_t *subscriber, address_t const *address);
// Fetching/Syncing
extern unwrapped_messages_t *sub_fetch_next_msgs(subscriber_t *subscriber);
extern unwrapped_messages_t *sub_sync_state(subscriber_t *subscriber);

/////////////
/// Utility
/////////////
#ifdef IOTA_STREAMS_CHANNELS_CLIENT
extern void client_add_node(char const *url);
#endif
extern void drop_str(char const *str);

extern char const *get_channel_address_str(channel_address_t const *appinst);
extern char const *get_msgid_str(msgid_t const *msgid);

extern char const *get_address_inst_str(address_t const *address);
extern char const *get_address_id_str(address_t const *address);

extern packet_payloads_t get_payload(unwrapped_message_t const *message);
extern packet_payloads_t get_indexed_payload(unwrapped_messages_t const *messages, size_t index);

#endif //IOTA_STREAMS_CHANNELS_H