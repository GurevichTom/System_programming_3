cmake_minimum_required(VERSION 2.90)
project(HW2_2025_Solution C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_executable(HW2_2025_Solution
    Customer.c
    Customer.h
    Date.c
    Date.h
    General.c
    General.h
    main.c
    main.h
    Product.c
    Product.h
    ShoppingCart.c
    ShoppingCart.h
    ShoppingItem.c
    ShoppingItem.h
    Supermarket.c
    Supermarket.h
        list.h
        list.c
        ClubMember.h
        ClubMember.c
        CustomerVTable.h
        save_load.h
        save_load.c)
