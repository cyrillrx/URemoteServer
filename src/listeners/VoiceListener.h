#pragma once

#include <memory>
#include "Listener.h"
#include "../AI.h"

class VoiceListener : public Listener
{
public:
    VoiceListener(std::shared_ptr<AI> ai);

    virtual ~VoiceListener();

private:
    virtual void doStart();

    std::shared_ptr<AI> ai_;

    void listen(const std::string& keyword);
};
