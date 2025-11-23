#pragma once

#include <cstdint>
#include <string>

namespace nodeeditor::common::utility
{

#include <string>

    struct SPort
    {
        enum class Orientation : uint8_t
        {
            Input,     ///< Receives data from other nodes.
            Parameter, ///< Represents a control parameter input.
            Output     ///< Sends data to other nodes.
        };
        std::string name;
        std::string nodeName;
        bool input = true;

        // --- Constructors ---
        SPort() = default;

        SPort(std::string name_, std::string nodeName_, bool input_ = true)
            : name(std::move(name_))
            , nodeName(std::move(nodeName_))
            , input(input_)
        {}

        // Copy constructor
        SPort(const SPort&) = default;

        // Move constructor
        SPort(SPort&&) noexcept = default;

        // --- Assignment operators ---
        // Copy assignment
        SPort& operator=(const SPort&) = default;

        // Move assignment
        SPort& operator=(SPort&&) noexcept = default;

        // --- Comparison operators ---
        bool operator==(const SPort& other) const
        {
            return name == other.name &&
                   nodeName == other.nodeName &&
                   input == other.input;
        }

        bool operator!=(const SPort& other) const
        {
            return !(*this == other);
        }
    };

} // namespace nodeeditor::common::utility
