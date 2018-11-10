#include "Module.h"
#include "../CHBMain.h"

Module::Module( CHBMain* chbMain )
    : m_chbMain( chbMain )
{

}

void Module::send_out_pack()
{
    m_chbMain->send_out_pack( &OuPack );
}

void Module::print( std::string s )
{
    m_chbMain->print( s );
}

void Module::i_comm( std::string s )
{
    m_chbMain->i_comm( s );
}
