/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


namespace tbaricault::sockets
{

    /**
     * @brief Initializes the sockets module
     * 
     * This function must be called before any other function in the sockets module.
     */
    void init() noexcept;

    /**
     * @brief Cleans up the sockets module
     * 
     * After calling this function, no other socket function should be used.
     */
    void cleanup() noexcept;

}
