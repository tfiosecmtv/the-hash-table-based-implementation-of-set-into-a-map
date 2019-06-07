/*
 * set.cpp
 *
 *  Created on: Mar 18, 2019
 *      Author: benja
 */

#include "map.h"

// From previous task:


map::listofpairs::const_iterator 
map::find( const listofpairs& lst, const std::string& key )
{
    auto p = lst. begin( );
    while( p != lst. end( ) && !equal( p -> first, key ))
        ++ p;
    return p;
}

map::listofpairs::iterator
map::find( listofpairs& lst, const std::string& key )
{
    auto p = lst. begin( );
    while( p != lst. end( ) && !equal( p -> first, key ))
        ++ p;
    return p;
}

bool equal( const std::string& s1, const std::string& s2 )
{
    size_t i=0,eq=0;
    if (s1.size()!=s2.size())
    {
        return false;
    } else
    {
        while (s1[i]!='\0')
        {
            char c1 = _tolower(s1[i]);
            char c2 = _tolower(s2[i]);
            if (c1==c2)
            {
                eq++;
            }
            i++;
        }
        
        if (eq==i)
        {
            return true;
        }
    }
    return false;
}

size_t hash( const std::string& st )
{
    size_t i = 0;
    size_t hash_number = 0;
    while (st[i]!='\0')
    {
        char c = tolower(st[i]);
        hash_number = hash_number*349+c;
        i ++;
    }
    return hash_number;
}


bool map::contains_key( const std::string& key ) const
{
    if(find(getbucket(key), key) != getbucket(key).end())
    {
        return true;
    } else
        return false;
}
bool map::insert( const std::string& key, unsigned int val )
{
    if(find(getbucket(key), key) != getbucket(key).end())
    {
        return false;
    } else
    {
        getbucket(key).push_back({key, val});
        map_size ++;
        check_rehash();
        return true;
    }
}
unsigned int& map::operator[] ( const std::string& key )
{
    if ( find(getbucket(key), key) == getbucket(key).end() ) {
        getbucket(key).push_back({key, 0});
        map_size ++;
        check_rehash();
    }
    return find(getbucket(key), key)->second;
}

unsigned int& map::at( const std::string& key )
{
    if ( find(getbucket(key), key) == getbucket(key).end() ) {
        throw std::out_of_range( "at( ): string not found" );
    }
    return find(getbucket(key), key)->second;
}
unsigned int map::at( const std::string& key ) const
{
    if ( find(getbucket(key), key) == getbucket(key).end() ) {
        throw std::out_of_range( "at( ): string not found" );
    }
    return find(getbucket(key), key)->second;
}

void map::rehash( size_t newbucketsize )
{
    if( newbucketsize < 4 )
        newbucketsize = 4;
    newbucketsize *= 2;
    std::vector< listofpairs > newbuckets{ newbucketsize };
    for (size_t i = 0; i < buckets.size(); ++ i)
    {
        for(auto &str: buckets[i])
        {
            newbuckets[hash(str.first)%newbucketsize].push_back(str);
        }
    }
    buckets = newbuckets;
}

void map::check_rehash( )
{
    if ( max_load_factor < loadfactor() ) {
        rehash( buckets.size() );
    }
}

bool map::remove( const std::string& key )
{
    if(find(getbucket(key), key) == getbucket(key).end())
    {
        return false;
    } else
    {
        getbucket(key).erase(find(getbucket(key), key));
        map_size --;
        return true;
    }
}

double map::standarddev( ) const  
{
    double sum = 0.0;
    double lf = loadfactor();
    
    for ( const auto& l : buckets )
    {
        double dif = l.size() - lf;
        sum += dif*dif;
    }
    
    return sqrt( sum / buckets. size( ));
}

void map::clear( ) 
{
    for( auto& l : buckets )
        l. clear( );
    
    map_size = 0;
}


std::ostream& map::print( std::ostream& out ) const
{
    for (size_t i = 0; i < buckets.size(); ++ i)
    {
        std::cout <<"bucket #"<<i <<" { ";
        for (auto it = buckets[i].cbegin(); it != buckets[i].cend(); ++ it)
        {
            std::cout <<"{" <<it->first << "," << it->second <<"}";
            if (it != (--buckets[i].cend())) {
                std::cout << ", ";
            }
        }
        std::cout<< " }" <<std::endl;
    }
    return out;
}


std::ostream& map::printstatistics( std::ostream& out ) const
{
    out << "set size =            " << size( ) << "\n";
    out << "load factor =         " << loadfactor( ) << " ( max = " <<
    max_load_factor << " )\n";
    out << "standard deviation =  " << standarddev( ) << "\n";
    return out;
}


