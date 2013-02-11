#ifndef _DNSMDEFINES_HPP_
#define _DNSMDEFINES_HPP_

//A temporary measure, until StateLoader/StateMachine are weaned from xml++
namespace xmlpp{
    class DomParser;
}

namespace ElementalEngine
{

struct STATEPARAMS
{
    IHashString* fileName;
    xmlpp::DomParser* parser;
};

} //namespace

#endif