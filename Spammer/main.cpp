#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <thread>
#include <atomic>
#include <mutex>

std::atomic<bool> keep_running(true);
std::mutex input_mutex;

void run_applescript(const std::string& script_path, const std::string& phone_number, const std::string& message) {
    std::string escaped_phone_number = "\"" + phone_number + "\"";
    std::string escaped_message = "\"" + message + "\"";

    std::string command = "osascript " + script_path + " " + escaped_phone_number + " " + escaped_message;
    
    system(command.c_str());
}

bool is_valid_phone_number(const std::string& phone_number) {
    std::regex phone_regex(R"(\+\d{10,15})");
    return std::regex_match(phone_number, phone_regex);
}

void monitor_user_input() {
    std::string input;
    while (keep_running) {
        std::getline(std::cin, input);
        if (input == "stop") {
            keep_running = false;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <phone_number> <message>" << std::endl;
        return 1;
    }

    std::string phone_number = argv[1];
    std::string message = argv[2];

    if (!is_valid_phone_number(phone_number)) {
        std::cerr << "Error: Invalid phone number format. Expected format is +123456789." << std::endl;
        return 1;
    }

    std::thread input_thread(monitor_user_input);

    std::cout << "type stop to stop: " << std::endl;
    while (keep_running) {
        run_applescript("executables/message.scpt", phone_number, message);
    }

    if (input_thread.joinable()) {
        input_thread.join();
    }

    std::cout << "Stopping the script." << std::endl;
    return 0;
}
