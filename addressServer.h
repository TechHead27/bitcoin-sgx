#pragma once

#include <string>

const int port = 49529; // randomly selected from unreserved range
const std::string serverAddress = "tcp://localhost:" + std::to_string(port);
const uint64_t amountDue = 3; // ransom cost, could also be set by server
