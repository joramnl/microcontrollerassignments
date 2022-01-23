#include "MCP23S08.hpp"
#include <cstdint>
#include <cstdio>
#include <utility>
#include "miscutil.hpp"

#define MCP_DEVICE_ADDRESS_READ  0b01000111
#define MCP_DEVICE_ADDRESS_WRITE 0b01000110

#define CONVO_LOG_LENGTH 50
// debug logging
char recvlog[CONVO_LOG_LENGTH];
char sendlog[CONVO_LOG_LENGTH];
bool reads  [CONVO_LOG_LENGTH];
unsigned char used;
namespace mcp{
    namespace pins{
        DigitalOut   SCK   (D13, HIGH ); // serial clock
        DigitalOut   SI    (D11, LOW ); // serial input
        DigitalIn    SO    (D12      ); // serial output
        //           A1    (POW, HIGH);// hardware address input
        //           A0    (POW, HIGH);// hardware address input
        //           RESET (POW, HIGH);// reset (biased)
        DigitalOut   CS    (D10, HIGH); // chip select
        InterruptIn  INTR  (D6       );
        // rhs is not directly controlled
    }
    // waits one SPI clock cycle
    inline void tick() { wait_ns(100); }
    /// starts a conversation
    inline void start() {
      tick(); // prevent accidental collisions at the cost of 100ns per command
      sys::status.write(HIGH);
      pins::CS.write(LOW);
    }
    /// exchanges one bit with the peripheral
    inline bool exchangebit(bool b) {
      tick();
      pins::SI.write(b);
      pins::SCK.write(LOW);
      tick();
      bool out = pins::SO.read();
      pins::SCK.write(HIGH);
      return out;
    }
    /// stops the conversation
    inline void stop() {
      tick();
      pins::CS.write(HIGH);
      sys::status.write(LOW);
    }
    /// exchanges one byte with the peripheral
    char exchange(char value) {
      char out = exchangebit(value & 0x80);
      for (char select = 0x40; select; select >>= 1) {
        out = out << 1 | exchangebit(value & select);
      }

      sendlog[used] = value;
      recvlog[used] = out;
      used++;
      if (used > CONVO_LOG_LENGTH) {
        used = 0; // prevents overwriting memory at the cost of invalid data
      }
      fflush(stdout);

      return out;
    }
    /// receives one byte from the peripheral
    static inline char receive() {
      reads[used] = true;
      return exchange(0);
    }
    /// sends one byte to the peripheral
    static inline void send(char value) {
      reads[used] = false;
      exchange(value);
    }

    bool sequential = false;
    /// ensures the data is exchanged using peripheral mode. there is no need to
    /// switch back because it does not remove any utilities
    void ensureSequential() {
      if (!sequential) {
        mcp::update(MCPAddress::IOCON, MCP_IOCON_SEQUENTIAL_OPERATION,
                    ~MCP_IOCON_SEQUENTIAL_OPERATION);
        sequential = true;
      }
    }
    /// applies a given set of bits onto a byte, only overwriting the bits that
    /// are masked using carry
    inline void apply(char *destination, char carry, char resource) {
      *destination = (~carry & *destination) | (carry & resource);
    }
    /// updates a given register
    void update(MCPAddress address, char value, char carry) {
      if (carry) {
        apply(&value, carry, get(address));
      }
      set(address, value);
    }
    /// sets the value of a register
    void set(MCPAddress addr, const char value) {
      start();
      send(MCP_DEVICE_ADDRESS_WRITE);
      send(addr);
      send(value);
      stop();
    }
    /// gets the value of a register
    char get(MCPAddress address) {
      start();
      send(MCP_DEVICE_ADDRESS_READ);
      send(address);
      char out = receive();
      stop();
      return out;
    }

    void setSeq(MCPAddress address, const char *value, size_t len) {
      ensureSequential();
      start();
      send(MCP_DEVICE_ADDRESS_WRITE);
      send(address);
      while (len--) {
        send(*value);
        ++value;
      }
      stop();
    }

    void getSeq(MCPAddress startAddress, char *value, size_t len) {
      ensureSequential();
      start();
      send(MCP_DEVICE_ADDRESS_READ);
      send(startAddress);
      while (len--) {
        *value = receive();
        ++value;
      }
      stop();
    }

    void updateSeq(MCPAddress startAddress, char *value, const char *carry,
                   size_t len) {
      ensureSequential();
      start();
      send(MCP_DEVICE_ADDRESS_READ);
      send(startAddress);
      for (size_t i = 0; i < len; ++i) {
        apply(&value[i], carry[i], receive());
      }
      stop();
      setSeq(startAddress, value, len);
    }

    void toggleBit(MCPAddress address, char bit) {
      setBit(address, bit, !getBit(address, bit));
    }

    void setBit(MCPAddress address, char bit, bool value) {
      update(address, value ? bit : 0, ~bit);
    }

    bool getBit(MCPAddress address, char bit) { return get(address) & bit; }
}