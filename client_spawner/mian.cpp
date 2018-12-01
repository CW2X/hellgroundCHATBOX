#include <thread>
#include <vector>
#include <chrono>

#include "../source/CHBMain.h"
#include <string>
#include <iostream>
#include <stdlib.h>

int main()
{
    while( true )
    {
        bool shutdown = false;

        std::vector< std::thread > threads;
        threads.reserve( 1000 );

        for ( auto batch = 0; batch < 5; ++batch )
        {
            for ( auto idx = 0; idx < 200; ++idx )
            {
                std::string account = std::string( "TEST" ) + std::to_string( batch * 200 + idx );
                threads.emplace_back( [&shutdown, batch, account]
                {
                    CHBMain session;

                    bool initialized = false;

                    std::string retstr, commstr;
                    while ( !shutdown )
                    {
                        if ( !session.Update( &retstr, &commstr ) )
                            return;

                        if ( !initialized && commstr.find( "Ln" ) == 0 )
                        {
                            std::this_thread::sleep_for( std::chrono::milliseconds( ( rand() % 1000 ) + 100 ) );

                            initialized = true;
                            std::cout << "Initializing client: " << account << "\n";
                            session.Initialize( account, account );
                        }

                        if ( retstr.empty() && commstr.empty() )
                            std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

                        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
                    }
                } );
            }

            std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
        }

        //std::this_thread::sleep_for( std::chrono::seconds( (rand() % 120) + 60 ) );
        //shutdown = true;

        for ( auto & t : threads )
            t.join();
    }

    return 0;
}
