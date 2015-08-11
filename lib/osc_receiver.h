#pragma once

#include "input.h"
#include "threaded_queue.h"

#include "osc/OscPacketListener.h"

class OscReceiver : public osc::OscPacketListener {
public:
    OscReceiver(ThreadedQueue<Input> &tq);

protected:
    void ProcessMessage(const osc::ReceivedMessage &m,
                        const IpEndpointName &ip) override;

private:
    ThreadedQueue<Input> &tq;
};
