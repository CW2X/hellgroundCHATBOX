#include <thread>
#include <vector>
#include <chrono>

#include "../source/CHBMain.h"
#include <string>
#include <iostream>

int main()
{
    std::vector< std::thread > threads;
    for ( auto idx = 1; idx < 50; ++idx )
    {
        threads.emplace_back( [idx]
        {
            CHBMain session;

            bool initialized = false;

            std::string retstr, commstr;
            while ( true )
            {
                if ( !session.Update( &retstr, &commstr ) )
                    return;

                if ( !initialized && commstr.find("Ln") == 0 )
                {
                    initialized = true;
                    session.Initialize( std::string( "TEST" ) + std::to_string( idx ), std::string( "TEST" ) + std::to_string( idx ) );
                }

                if ( retstr.empty() && commstr.empty() )
                    std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

                if ( !retstr.empty() )
                    std::cout << retstr << "\n";

                std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
            }
        } );
    }

    for ( std::thread & thread : threads )
        thread.join();

    return 0;
}
