#include "../includes/Server.hpp"

int main(int ac, char **av)
{
    try
    {
        if(ac != 3)
            throw(std::runtime_error("Give only 3 arguments\n"));
        Server server(av[1], av[2]);
        std::cout << "SIZE AFTER SELECTTTTTTTTTTTTTTT" << std::endl;
        server.main_loop();
        //parsing of arguments
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}