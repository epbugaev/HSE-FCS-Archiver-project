#include <string>
#include <iostream>
#include "Huffman.h"

int main(int argc, const char *argv[]) {
    if (std::string(argv[1]) == "-c") {
        if (argc < 4) {
            std::cerr << "Incorrect arguments, use -h for help" << std::endl;
            return 1;
        }

        std::string archive_name(argv[2]);
        std::ofstream out(archive_name, std::ios::binary);

        Huffman::Encoder coder = Huffman::Encoder(out);

        for (std::size_t i = 3; i < argc; i++) {
            try {
                std::string file_name(argv[i]);
                bool is_last_file = (i + 1 == argc);
                coder.EncodeFile(file_name, is_last_file);
                coder.ClearFileInfo();
            } catch(std::exception& e) {
                std::cerr << "Error occurect, cannot encode file " << argv[i] << std::endl;
                std::cerr << e.what() << std::endl;
                std::remove(archive_name.c_str());
                return 1;
            }

        }
    } else if (std::string(argv[1]) == "-d") {
        if (argc < 3) {
            std::cerr << "Incorrect arguments, use -h for help" << std::endl;
            return 1;
        }

        std::ifstream in(std::string(argv[2]), std::ios::binary);

        try {
            if (!in.good()) {
                throw std::invalid_argument("File " + std::string(argv[2]) + " not found");
            }

            Huffman::Decoder decoder(in);
            decoder.DecodeArchive();
        } catch (std::exception& e) {
            std::cerr << "Error occured, cannot decode file " << argv[2] << std::endl;
            std::cerr << e.what() << std::endl;
            return 1;
        }
    } else if (std::string(argv[1]) == "-h") {
        std::cout << "This archiver has 3 commands:" << std::endl;
        std::cout
                << "    -c archive_name file1 [file2 ...] - archives files file1, file2, ... and saves result into file archive_name"
                << std::endl;
        std::cout
                << "    -d archive_name - extracts files from archive_name archive and saves them in current directory"
                << std::endl;
        std::cout << "    -h - prints all available commands in the command window" << std::endl;
    }
    return 0;
}