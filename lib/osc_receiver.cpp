#include "osc_receiver.h"

#include "string_builder.h"

#include <string>

using namespace std;

OscReceiver::OscReceiver(ThreadedQueue<Input> &tq)
        : tq(tq) {
}

void OscReceiver::ProcessMessage(const osc::ReceivedMessage &m,
                                 const IpEndpointName &ip) {
    try {
        if (m.AddressPattern() == string("/time_server")) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            const char *str;
            args >> str >> osc::EndMessage;

            if (str == string("tick")) {
                tq.push(Input(Input::Type::TICK));
            }
        } else if (m.AddressPattern() == string("/key_server")) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            osc::int32 key;
            args >> key >> osc::EndMessage;

            tq.push(Input(Input::Type::KEY, key));
        }

    } catch (const osc::Exception &e) {
        throw runtime_error(build_string(
            "error parsing message: ", m.AddressPattern(), ": ", e.what()));
    }
}
