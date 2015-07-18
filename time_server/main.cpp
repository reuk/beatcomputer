#include "timer.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include <vector>
#include <array>
#include <iostream>
#include <string>

using namespace std;

class Ticker {
public:
    Ticker()
        : socket(IpEndpointName(address.c_str(), port)) {
    }

    void tick() {
        using namespace osc;

        array<char, 1024> buffer;
        OutboundPacketStream packet_stream(buffer.data(), buffer.size());
        packet_stream << BeginBundleImmediate << BeginMessage("/time_server")
                      << "tick" << EndMessage << EndBundle;

        socket.Send(packet_stream.Data(), packet_stream.Size());
    }

private:
    static const string address;
    static const int port;
    UdpTransmitSocket socket;
};

const string Ticker::address = "127.0.0.1";
const int Ticker::port = 7000;

class BroadcastTimer : public Timer, public Ticker {
public:
    void callback() override {
        tick();
    }
};

int main() {
    Ticker ticker;

    for (;;) {
        ticker.tick();
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    return 0;
}
