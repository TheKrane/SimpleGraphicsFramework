#!/bin/bash

cat > include/$1.hpp << EOF
#ifndef ${1^^}_HPP
#define ${1^^}_HPP

class $1
{
public:
    $1();
    ~$1();
};

#endif
EOF

cat > src/$1.cpp << EOF
#include "$1.hpp"

$1::$1()
{

}

$1::~$1()
{

}

EOF