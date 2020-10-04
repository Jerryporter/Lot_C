#include "nlohmann/json.hpp"
#include "frequentuse.h"
// for convenience
using json = nlohmann::json;
json genKeepAliveMess()
{
    json mess;
    mess = R"({"protocol " : " keepalive "})"_json;
    return mess;
}
int main(int argc, char const *argv[])
{
    cout << genKeepAliveMess() << endl;
    system("Pause");
    return 0;
}
