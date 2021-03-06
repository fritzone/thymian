#include <sstream>
#include <fstream>

#define URS 5

int main()
{
    std::stringstream ss;

    ss << "#include \"../../tests/tests.h\"" << std::endl;
    ss << "#include \"gtest/gtest.h\"" << std::endl;
    ss << "#include \"nap_runtime.h\"" << std::endl << std::endl;

    ss << "TEST(PushPeekGen, Ints)" << std::endl
        << "{" << std::endl << "SCRIPT_START" << std::endl
        << "\"  \\" << std::endl;

    for(int i=0; i<=URS; i++)
    {
        ss << "int gv_add_" << i << " = " << i << ";  \\" << std::endl ;
    }

    ss << "asm   \\" << std::endl << "{  \\" << std::endl;

    for(int i=0; i<=URS; i++)
    {
        ss << "  mov reg int " << i << " 1;  \\" << std::endl;
    }
    ss << "} \\" << std::endl;
    ss << "\"" << std::endl << "SCRIPT_END" << std::endl
        << "SCRIPT_SHUTDOWN"<<std::endl << "(void)found_indicator;}" << std::endl;


    std::ofstream outFile;
    outFile.open("gen_test.cpp");

    outFile << ss.rdbuf();
}
