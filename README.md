# HSE-FCS-Archiver-project
Archiver program (completed as a part of HSE FCS programming course) with following features:

- Wrote in C++ language
- Runs via CMD interface
- Includes UNIT tests for each major part of the codebase (completed with catch.hpp library)
- Works best with text files because of their predictable nature (some symbols occure more often than others: they are encoded with less bits, thus reducing required space)
- Personal implementation of Heap data structure is used (only for educational purposes)

# Encoding algorithm
Detailed description of the algorithm can be found on [Wikipedia](https://en.wikipedia.org/wiki/Huffman_coding)

Bit by bit reading/writing was implemented in Bit_io class to allow saving encoded files

# Guide
Use following commands after running: 

- **-h** - prints all available commands in the command window
- **-c archive_name file1 [file2 ...]** - archives files file1, file2, ... and saves result into file **archive_name**
- **-d archive_name** - extracts files from archive_name archive and saves them in current directory"
