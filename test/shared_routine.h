#ifndef TEST_SHARED_ROUTINE_H
#define TEST_SHARED_ROUTINE_H

#include <list>

#include "s_max.h"


int shared_main()
{
    std::cout << "Begin Test using input file \"" << TEST_INPUT_FILE << "\" and output file \"" << TEST_OUTPUT_FILE << "\"" << std::endl;

    auto *s_max = new S_max();
    s_max->parse_input(TEST_INPUT_FILE);
    auto result = s_max->find_max();
    delete s_max;

    auto *fileParser = new FileParser(TEST_OUTPUT_FILE);
    std::list<std::string> line = fileParser->nextLine();
    auto expected_result = STRING_TO_DATA_TYPE(line.front());
    delete fileParser;

    if (result == expected_result)
    {
        std::cout << "Test succeeded with result: " << result << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Test failed. Expected result: " << expected_result << ", got result: " << result << std::endl;
        return -1;
    }
}

#endif //TEST_SHARED_ROUTINE_H
