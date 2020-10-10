#include "nlohmann/json.hpp"
#include "frequentuse.h"
using json = nlohmann::json;

int main(int argc, char const *argv[])
{
    vector<json> mess;
    json keepLiveMess = R"({"protocol": "keepalive"})"_json;
    mess.push_back(keepLiveMess);
    cout<<keepLiveMess.dump()<<endl;
    cout<<keepLiveMess<<endl;
    system("pause");
    return 0;
}
