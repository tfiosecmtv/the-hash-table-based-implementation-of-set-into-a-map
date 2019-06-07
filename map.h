
#ifndef MAP_INCLUDED
#define MAP_INCLUDED   1

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>

bool equal( const std::string& s1, const std::string& s2 );
size_t hash( const std::string& s );

class map
{
    size_t map_size;
    double max_load_factor;
    
    using listofpairs = std::list< std::pair< std::string, unsigned int >> ;
    // So that we don't have to type and read it all the time.
    
    std::vector< listofpairs > buckets;
    
public:  // You can make it private later.
    
    // `static' because both methods don't use the fields of
    // the class. (map_size and max_load_factor).
    
    static listofpairs :: iterator
    find( listofpairs& lst, const std::string& key );
    
    static listofpairs :: const_iterator
    find( const listofpairs & lst, const std::string& key );
    
    
    listofpairs& getbucket( const std::string& key )
    { return buckets[ hash( key ) % buckets. size( ) ]; }
    const listofpairs& getbucket( const std::string& key ) const
    { return buckets[ hash( key ) % buckets. size( ) ]; }
    
public:
    // Number of buckets cannot be zero.
    
    map( size_t num_buckets = 4, double max_load_factor = 3.0 )
    : map_size{ 0 },
    max_load_factor{ max_load_factor },
    buckets{ std::vector< listofpairs > ( num_buckets ) }
    {
        if( num_buckets == 0 )
            throw std::runtime_error( "number of buckets cannot be zero" );
            }
    
    map( const map& m ) = default;
    map& operator = ( const map& m ) = default;
    ~map() = default;
    
    map( std::initializer_list<std::pair< std::string, unsigned int >> init )
    : map( )
    {
        for( const auto& p : init )
            insert( p. first, p. second );
    }
    
    bool insert( const std::string& key, unsigned int val );
    // Returns true if the insertion took place.
    
    unsigned int& operator[] ( const std::string& key );
    unsigned int& at( const std::string& key );
    unsigned int at( const std::string& key ) const;
    
    bool contains_key( const std::string& key ) const;
    
    bool remove( const std::string& key );
    // Returns true if s was present.
    
    size_t size() const
    {
        return map_size;
    }
    
    bool isempty() const
    {
        return map_size == 0;
    }
    
    double loadfactor() const
    {
        return static_cast<double> ( map_size ) / buckets. size( );
        // C-style casts of form (double) map_size are deprecated.
    }
    
    double standarddev() const;
    
    void clear();
    
    void rehash( size_t newbucketsize );
    
    void check_rehash( );
    // Check if the load factor is so high that a rehash is needed.
    // If yes, then call rehash( ).
    
    std::ostream& print( std::ostream& out ) const;
    std::ostream& printstatistics( std::ostream& out ) const;
    
};

inline std::ostream& operator << ( std::ostream& out, const map& m ) 
{
    return m. print( out );
}


#endif


